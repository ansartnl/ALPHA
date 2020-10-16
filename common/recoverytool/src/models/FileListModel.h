#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractItemModel>
#include <QFile>

class CFileListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CFileListModel(QObject *parent = 0);
    
    struct FileInfo{
        QString fileName;
        QString shortName;
    };

    typedef QList<FileInfo> TFileList;

    //! Returns the data stored under the given role for the item referred to by the index.
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //! Returns the data for the given role and section in the header with the specified orientation.
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    //! Returns the item flags for the given index.
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Returns the index of the item in the model specified by the given row, column and parent index.
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    //! Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned.
    virtual QModelIndex parent(const QModelIndex &child) const;

    /*! Returns the number of rows under the given parent. When the parent is valid it
        means that rowCount is returning the number of children of parent.
    */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //! Returns the number of columns for the children of the given parent.
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Redraw view.
    void update();

    void refreshModel();

    void setParams(QString &fileDir, QString &flashDir, QString &mask, QString &script);

    void run(int fileIdx);
    void rename(int fileIdx);
    void del(int fileIdx);
    void copy(int fileIdx);

signals:
    void fileDirectory(bool exists);
    void flashDirectory(bool exists);
    void scriptPath(bool exists);

public slots:

private:
    TFileList   mData;

    QString mFileDir;
    QString mFlashDir;
    QString mScript;
    QString mMask;
    
};

#endif // FILELISTMODEL_H
