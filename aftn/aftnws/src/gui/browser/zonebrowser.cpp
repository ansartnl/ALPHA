#include "zonebrowser.h"
#include "qtheaderview.h"

#include "translations.h"
// ------------------------
// Import aring
//#include "TVAUtils_global.h"
//#include "AringToDB_global.h"
//
//#include "utils/TVAError.h"
//
//#include "utils/TVAMacro.h"
//#include "ATDArinc.h"
//#include "importaringdialog.h"
// ------------------------

#include <QtDebug>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

ZoneBrowser::ZoneBrowser(QWidget* parent) :
        AbstractBrowser(parent)
{
    setupUi(this);
    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("ZONE");

    QStringList systemColumns = QStringList() << "LOW_LIMIT" << "UP_LIMIT"
                                << "PARENT_ID" << "TYPE";
//    QStringList systemColumns = QStringList() << "ID" << "LOW_LIMIT" << "UP_LIMIT"
//                                << "PARENT_ID" << "TYPE";

    foreach (const QString& column, systemColumns)
        model->removeColumn(model->fieldIndex(column), QModelIndex());

    table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);
    table->setModel(model);

    connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,
                                                              QModelIndex)), this, SLOT(onTableCurrentChanged(QModelIndex, QModelIndex)));

    updateModel();
    Translations::translateSqlTableModel( model);

    QSqlRecord record = model->record();
    for (int i = 0; i < record.count(); ++i) {
        record.setValue(i, QVariant(record.field(i).typeID()));
    }
    model->insertRecord(-1, record);

    restoreState(staticMetaObject.className());
}

void ZoneBrowser::updateModel()
{
    QStringList filters;

    if (filterButton->isChecked() && !filterEdit->text().isEmpty()) {
        QString textFilter;
        textFilter.prepend("(");

        QString sqlLikePattern;
        QString sqlLikeOrPattern;

        // LIKE '%filter%'
        sqlLikePattern.append(" LIKE '%");
        sqlLikePattern.append(filterEdit->text());
        sqlLikePattern.append("%' ");

        sqlLikeOrPattern = sqlLikePattern;
        sqlLikeOrPattern.append(" OR ");

        textFilter.append("NAME" + sqlLikeOrPattern + "ID_NAME" + sqlLikePattern);
        textFilter.append(")");
        filters.append(textFilter);
    }

    filters.append("(type = 1)");

    model->setFilter(filters.join(" AND "));
    model->select();
}

void ZoneBrowser::on_deleteButton_clicked()
{
    int row = table->currentIndex().row();

    if (row != -1 && QMessageBox::question(this, windowTitle(), tr(
            "Do you really want to delete FIR record?"), QMessageBox::Yes
                                           | QMessageBox::No) == QMessageBox::Yes) {
        model->removeRow(row);
        model->submitAll();
    }
}

void ZoneBrowser::on_filterButton_clicked()
{
    updateModel();
}

void ZoneBrowser::on_filterEdit_textChanged(const QString&)
{
    if (filterButton->isChecked())
        updateModel();
}

void ZoneBrowser::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);

    switch (event->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        Translations::translateSqlTableModel( model);
        break;
    default:
        break;
    }
}

void ZoneBrowser::closeEvent(QCloseEvent* event)
{
    saveState(staticMetaObject.className());
    AbstractBrowser::closeEvent(event);

}

// -------------------------------------------------------
ZoneBrowserHelper::ZoneBrowserHelper(QObject* parent) :
        ObjectsManagerHelper(parent), mAction(0)
{
    mAction = new QAction(icon(), name(), this);
    mAction->setObjectName(id());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}

QString ZoneBrowserHelper::id() const
{
    return ZoneBrowser::staticMetaObject.className();
}

QString ZoneBrowserHelper::path() const
{
    return tr("/View/FIR browser");
}

QString ZoneBrowserHelper::name() const
{
    return tr("FIR browser");
}

QString ZoneBrowserHelper::description() const
{
    return tr("Table with FIRs");
}

QWidget* ZoneBrowserHelper::widget(QWidget* parent) const
{
    return new ZoneBrowser(parent);
}

QAction* ZoneBrowserHelper::action() const
{
    return mAction;
}

void ZoneBrowserHelper::retranslateUi()
{
    mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}

