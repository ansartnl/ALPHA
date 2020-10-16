#include "sqltablebrowser.h"

#include "ocitablemodel.h"

#include "translations.h"

SqlTableBrowser::SqlTableBrowser(const QString& tableName, QWidget* parent)
    : AbstractBrowser(parent), mTableName(tableName), model(new OciTableModel(this))
{
    setupUi(this);

    model->setTable(tableName);

    model->refresh();

    table->setModel(model);

    setWindowTitle(tr("Table: %1").arg(Translations::tableNameTr(tableName)));

    Translations::translateOciSqlModel(model);

    restoreState(staticMetaObject.className() + mTableName);
}

void SqlTableBrowser::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        setWindowTitle(tr("Table: %1").arg(Translations::tableNameTr(mTableName)));
        Translations::translateOciSqlModel(model);
        break;
    default:
        break;
    }
}

void SqlTableBrowser::closeEvent(QCloseEvent* event)
{
    saveState(staticMetaObject.className() + mTableName);
    table->setModel(0);

    AbstractBrowser::closeEvent(event);
}
