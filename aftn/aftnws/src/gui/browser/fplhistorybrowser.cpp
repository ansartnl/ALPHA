#include "fplhistorybrowser.h"
#include "aftnmessageprocessor.h"

#include "qtsqltableview.h"
#include "qtheaderview.h"

#include "translations.h"

#include <QtGui/QMessageBox>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRelation>

#include <QtDebug>

FplHistoryBrowser::FplHistoryBrowser(QWidget* parent) :
        AbstractBrowser(parent), fplViewer(NULL)
{
    setupUi(this);

    model = new QSqlRelationalTableModel(this);

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable(TBL_HIST_FPL);

    model->setRelation(model->fieldIndex("modified_by"), QSqlRelation("USERS",
                                                                      "ID", "USER_NAME"));

    QStringList systemColumns = QStringList() << "TRTIME" << "VISIBILITY";
    //        << "ACTUAL";
//    QStringList systemColumns = QStringList() << TBL_HIST_FPL_ID << TBL_HIST_FPL_TRTIME << TBL_HIST_FPL_VISIBILITY;
//    //        << TBL_HIST_FPL_ACTUAL;

    foreach (const QString& column, systemColumns)
        model->removeColumn(model->fieldIndex(column), QModelIndex());
    table->setModel(model);
    table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);

    secondModel = new QSqlRelationalTableModel(this);
    secondModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    secondModel->setTable(TBL_HIST_FPL);
    secondModel->setRelation(model->fieldIndex("modified_by"), QSqlRelation(
            "USERS", "ID", "USER_NAME"));

    secondTable->setModel(secondModel);
    secondTable->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);

    systemColumns = QStringList() << "TRTIME" << "VISIBILITY";
    //        << "ACTUAL";
//    systemColumns = QStringList() << TBL_HIST_FPL_ID << TBL_HIST_FPL_TRTIME << TBL_HIST_FPL_VISIBILITY;
//    //        << TBL_HIST_FPL_ACTUAL;

    foreach (const QString& column, systemColumns)
        secondModel->removeColumn(secondModel->fieldIndex(column), QModelIndex());

    dateFilterButton->setShortcut(QString("Ctrl+T"));

    dateEdit->setDisplayFormat(DATE_FORMAT);
    dateEdit->setDate(QDateTime::currentDateTime().toUTC().date());

    filterButton->setShortcut(QString("Ctrl+F"));

    connect(table->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
            SLOT(onTableCurrentChanged(const QModelIndex&, const QModelIndex&)));

    connect(secondTable->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
            SLOT(onSecondTableCurrentChanged(const QModelIndex&, const QModelIndex&)));

    connect(table, SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(onDisplayItem(const QModelIndex&)));

    connect(secondTable, SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(onDisplayItem(const QModelIndex&)));

    connect(dateFilterButton, SIGNAL(clicked()), this, SLOT(updateModel()));
    connect(dateEdit, SIGNAL(dateChanged(const QDate&)), this,
            SLOT(onDateFilterChanged(const QDate&)));
    connect(filterButton, SIGNAL(clicked()), this, SLOT(updateModel()));
    connect(filterEdit, SIGNAL(textChanged(QString)), this,
            SLOT(onTextFilterChanged(QString)));

    updateModel();
    Translations::translateSqlTableModel( model);
    Translations::translateSqlTableModel( secondModel);

    restoreState(staticMetaObject.className());
}

void FplHistoryBrowser::updateModel()
{
    QStringList filters;
    filters.append("("TBL_HIST_FPL_ACTUAL " = '1'" ")");

    if (dateFilterButton->isChecked()) {
        filters.append(
                QString(
                        "(%1 >= to_timestamp('%2', 'yyyy-MM-dd') and %1 <= to_timestamp('%2', 'yyyy-MM-dd') + interval '1' day" ")").arg(
                                TBL_HIST_FPL_TIME, dateEdit->date().toString("yyyy-MM-dd")));
    }

    if (filterButton->isChecked()) {
        QString textFilter = filterEdit->text();
        filters.append("( " TBL_HIST_FPL_FIR_NAME " LIKE '%" + textFilter
                       + "%' OR "
                       TBL_HIST_FPL_AIRCRAFT_ID " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_TAIRCRAFT " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_FRULES " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_TFLIGHT " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_NUM " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_WTC " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_EQUIPMENT " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_ADEP " LIKE '%" + textFilter + "%' OR "
                       SQL_TIMESTAMP_TO_CHAR(TBL_HIST_FPL_TIME) " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_SPEED " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_FLEVEL " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_ROUTE " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_ADES " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_ALT1 " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_ALT2 " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_OTHER " LIKE '%" + textFilter + "%' OR "
                       TBL_HIST_FPL_MODIFIED_BY " LIKE '%" + textFilter + "%' OR "
                       SQL_TIMESTAMP_TO_CHAR(TBL_HIST_FPL_MODIFIED_AT) " LIKE '%" + textFilter
                       + "%' OR "
                       TBL_HIST_FPL_MODIFICATION_TYPE " LIKE '%" + textFilter + "%')");
    }

    model->setFilter(filters.join(" AND "));
    model->select();
    onTableCurrentChanged(QModelIndex(), QModelIndex());
}

void FplHistoryBrowser::onDateFilterChanged(const QDate&)
{
    if (dateFilterButton->isChecked())
        updateModel();
}

void FplHistoryBrowser::onTableCurrentChanged(const QModelIndex& cur,
                                              const QModelIndex& prew)
{
    if (cur.isValid()) {
        QString curSequenceId = model->data(model->index(cur.row(),
                                                         model->fieldIndex(TBL_HIST_FPL_ID))).toString();
        qDebug() << curSequenceId;
        secondModel->setSort(model->fieldIndex(TBL_HIST_FPL_HIST_ID),
                             Qt::DescendingOrder);
        secondModel->setFilter(TBL_HIST_FPL_ID " = " + curSequenceId);
        secondModel->select();
        onSecondTableCurrentChanged(QModelIndex(), QModelIndex());
        secondTable->resizeColumnsToContents();
    } else {
        secondModel->setFilter("1=0");
        secondModel->select();
        onSecondTableCurrentChanged(QModelIndex(), QModelIndex());
    }
}

void FplHistoryBrowser::onSecondTableCurrentChanged(const QModelIndex& current,
                                                    const QModelIndex& /*previous*/)
{
    // --------------------
    // Not currently worked, need aftnmessageservice
    if (current.isValid()) {
        QString sRecordId = secondModel->record(current.row()).value(
                TBL_HIST_FPL_MESSAGE_ID).toString();

        if (!sRecordId.isEmpty() && sRecordId != "0") {
            QSqlQuery
                    query(
                            "SELECT " TBL_ARC_AFTN_TEXT " FROM " TBL_ARC_AFTN " WHERE " TBL_ARC_AFTN_ID " = "
                            + sRecordId);

            if (query.lastError().isValid())
                QMessageBox::critical(this, windowTitle(), tr("SQL execution error: ")
                                      + query.lastError().text());
            else if (query.next()) {
                description->setText(query.value(0).toString());
                return;
            }
        }
    }

    description->setText("");
}

void FplHistoryBrowser::onTextFilterChanged(const QString&)
{
    if (filterButton->isChecked())
        updateModel();
}

void FplHistoryBrowser::onDisplayItem(const QModelIndex& index)
{
    if (index.isValid()) {
        const QSqlTableModel* currentModel = qobject_cast<const QSqlTableModel*> (
                index.model());

        if (!currentModel)
            return;

        FPL Fpl(currentModel->record(index.row()), TBL_HIST_FPL,
                TBL_HIST_FPL_HIST_ID);
        viewer()->setFPL(Fpl);
        viewer()->exec();
    }
}

FplViewer* FplHistoryBrowser::viewer()
{
    if (!fplViewer)
        fplViewer.reset(new FplViewer(this));

    return fplViewer.data();
}

void FplHistoryBrowser::changeEvent(QEvent *event)
{
    AbstractBrowser::changeEvent(event);

    switch (event->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        Translations::translateSqlTableModel( model);
        Translations::translateSqlTableModel( secondModel);
        break;
    default:
        break;
    }
}
void FplHistoryBrowser::closeEvent(QCloseEvent *event)
{
    saveState(staticMetaObject.className());
    AbstractBrowser::closeEvent(event);
}

// ---------------------------------------------------------
FplHistoryBrowserHelper::FplHistoryBrowserHelper(QObject* parent) :
        ObjectsManagerHelper(parent), mAction(0)
{
    mAction = new QAction(icon(), name(), this);
    mAction->setObjectName(id());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}

QString FplHistoryBrowserHelper::id() const
{
    return FplHistoryBrowser::staticMetaObject.className();
}

QString FplHistoryBrowserHelper::path() const
{
    return tr("/History/FPL history browser");
}

QString FplHistoryBrowserHelper::name() const
{
    return tr("FPL history browser");
}

QString FplHistoryBrowserHelper::description() const
{
    return tr("History of FPL messages");
}

QWidget* FplHistoryBrowserHelper::widget(QWidget* parent) const
{
    return new FplHistoryBrowser(parent);
}

QAction *FplHistoryBrowserHelper::action() const
{
    return mAction;
}

void FplHistoryBrowserHelper::retranslateUi()
{
    mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}

