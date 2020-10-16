#ifndef SQLTABLEBROWSER_H
#define SQLTABLEBROWSER_H

#include "ui_sqltablebrowser.h"

#include "abstractbrowser.h"

#include <QtCore/QString>

class OciTableModel;

class SqlTableBrowser : public AbstractBrowser, private Ui::SqlTableBrowser {
    Q_OBJECT
public:
    SqlTableBrowser(const QString& tableName, QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

    void closeEvent(QCloseEvent *event);

private:
    QString mTableName;
    OciTableModel *model;
};

#endif // SQLTABLEBROWSER_H
