#include "baseeditor.h"
#include <QTableView>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QKeyEvent>
#include <QSqlTableModel>

BaseEditor::BaseEditor(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{

}

void BaseEditor::onFirChanged(int id, const QString &name)
{
    mCurrentFirName = name;
    mCurrentFirId = id;
    updateFirName();
    setFirId();
}

const QString BaseEditor::firName() const
{
    return mCurrentFirName;
}

int BaseEditor::firId() const
{
    return mCurrentFirId;
}

bool BaseEditor::isFirValid() const
{
    return firId() > 0;
}

void BaseEditor::updateFirName()
{
}

void BaseEditor::setFirId()
{
}

bool BaseEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent)
        {
            if(keyEvent->key() == Qt::Key_F3 && keyEvent->modifiers() == Qt::NoModifier)
                QMetaObject::invokeMethod(this, "onFindNext", Qt::QueuedConnection, Q_ARG(QObject*, obj));
            else if(keyEvent->key() == Qt::Key_F && keyEvent->modifiers() == Qt::ControlModifier)
                QMetaObject::invokeMethod(this, "onFind", Qt::QueuedConnection, Q_ARG(QObject*, obj));
        }
    }
    return QWidget::eventFilter(obj, event);
}

void BaseEditor::onFind(QObject *obj)
{
    QTableView* pView = dynamic_cast<QTableView*>(obj);
    if(pView)
    {
        QSettings settings;
        bool bOk(false);
        QString sText = QInputDialog::getText(this, tr("Search"), tr("Find"), QLineEdit::Normal, settings.value("LastFindString").toString(), &bOk);
        if(bOk)
        {
            settings.setValue("LastFindString", sText);
            if(!sText.isEmpty())
                onFindNext(obj);
        }
    }
}

void BaseEditor::onFindNext(QObject *obj)
{
    QTableView* pView = dynamic_cast<QTableView*>(obj);
    if(pView)
    {
        QSettings settings;
        QString sText = settings.value("LastFindString").toString();
        if(sText.isEmpty())
        {
            onFind(obj);
        }
        else
        {
            //QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(pView->model());
            QAbstractItemModel* model = pView->model();
            if(model)
            {
                int row = -1;
                QModelIndex index = pView->currentIndex();
                if(index.isValid())
                    row = index.row();
                for (int i = row + 1; i < model->rowCount(); ++i)
                {
                    //if (model->index(i, model->fieldIndex("name")).data().toString().contains(sText, Qt::CaseInsensitive))
                    for (int j = 0; j < model->columnCount(); ++j)
                    {
                        if (model->index(i, j).data().toString().contains(sText, Qt::CaseSensitive))
                        {
                            pView->selectRow(i);
                            return;
                        }
                    }
                }
                for (int i = 0; i < row; ++i)
                {
                    //if (model->index(i, model->fieldIndex("name")).data().toString().contains(sText, Qt::CaseInsensitive))
                    for (int j = 0; j < model->columnCount(); ++j)
                    {
                        if (model->index(i, j).data().toString().contains(sText, Qt::CaseSensitive))
                        {
                            pView->selectRow(i);
                            return;
                        }
                    }
                }
                QMessageBox::information(this, tr("Search"), tr("Not found"));
            }
        }
    }
}

bool BaseEditor::selectAll(QSqlTableModel* model)
{
    bool bRes = false;
    if (model)
    {
        bRes = model->select();
        if (bRes)
        {
            while (model->canFetchMore())
                model->fetchMore();
        }
    }
    return bRes;
}
