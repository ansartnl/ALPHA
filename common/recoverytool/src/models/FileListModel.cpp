#include "FileListModel.h"
#include <QDir>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>

#include "src/CopyDialog.h"

CFileListModel::CFileListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QVariant CFileListModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || mData.isEmpty() )
        return QVariant();

    if ( role == Qt::DisplayRole )
    {
        return QVariant(mData.at(index.row()).shortName);
    }
    else if ( role == Qt::TextAlignmentRole )
    {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }

    return QVariant();
}

QVariant CFileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return QVariant(tr("Name"));
    }
    return QVariant();
}

Qt::ItemFlags CFileListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex CFileListModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    if ( !parent.isValid() && !mData.isEmpty())
    {
        if ( row >= 0 && row < mData.count() )
            return createIndex(row, column, 0);
    }

    return QModelIndex();
}

QModelIndex CFileListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int CFileListModel::rowCount(const QModelIndex &parent) const
{
    if ( !parent.isValid() && !mData.isEmpty())
        return mData.count();
    return 0;
}

int CFileListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

void CFileListModel::update()
{
    emit layoutChanged();
}

void CFileListModel::refreshModel()
{
    emit layoutAboutToBeChanged();
    mData.clear();
    QDir fileDir = QDir(mFileDir);
    if(fileDir.exists())
    {
        foreach(QFileInfo info, fileDir.entryInfoList(QStringList() << mMask, QDir::Files))
        {
            FileInfo fi;
            fi.fileName = info.absoluteFilePath();
            fi.shortName = info.fileName();
            mData.append(fi);
        }
    }
    emit layoutChanged();
    emit fileDirectory(fileDir.exists());
    emit flashDirectory(QDir(mFlashDir).exists());
    emit scriptPath(QFile(mScript).exists());
}

void CFileListModel::setParams(QString &fileDir, QString &flashDir, QString &mask, QString &script)
{
    mFileDir = fileDir;
    mFlashDir = flashDir;
    mScript = script;
    mMask = mask;
}

void CFileListModel::run(int fileIdx)
{
    if(fileIdx < 0)
        return;

    if(QFile(mScript).exists())
    {
        QProcess::startDetached(mScript, QStringList() << mData.at(fileIdx).fileName);
    }
}

void CFileListModel::rename(int fileIdx)
{
    if(fileIdx < 0)
        return;

    bool ok;
         QString text = QInputDialog::getText(0, tr("Rename"),
                                              tr("File name:"), QLineEdit::Normal,
                                              mData.at(fileIdx).shortName, &ok);
     if (ok && !text.isEmpty())
     {
        QFile::rename(mData.at(fileIdx).fileName, mFileDir + tr("/") + text);
        refreshModel();
     }
}

void CFileListModel::del(int fileIdx)
{
    if(fileIdx < 0)
        return;

    int ret = QMessageBox::warning(0, tr("Delete file"),
                                   tr("Are you sure?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    if(ret == QMessageBox::Ok)
    {
        QFile::remove(mData.at(fileIdx).fileName);
        refreshModel();
    }
}

void CFileListModel::copy(int fileIdx)
{
    if(fileIdx < 0)
        return;

    if(QDir(mFlashDir).exists())
    {
        CCopyDialog dlg;
        dlg.copy(mData.at(fileIdx).fileName, mFlashDir + tr("/") + mData.at(fileIdx).shortName);
        dlg.exec();
    }
}
