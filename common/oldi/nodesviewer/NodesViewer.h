#ifndef NODESVIEWER_H
#define NODESVIEWER_H

#include "ui_NodesViewer.h"
#include <QtSql/QSqlTableModel>

class SqlFMTPModel : public QSqlTableModel
{
    Q_OBJECT
public:
    SqlFMTPModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    virtual ~SqlFMTPModel() {}

    virtual bool select();
};

class NodesViewer : public QWidget, private Ui::NodesViewer
{
    Q_OBJECT
public:
    explicit NodesViewer(QWidget* parent = 0);

public slots:
    void on_buttonRefresh_clicked();

protected:
    void changeEvent(QEvent *e);
};

#endif
