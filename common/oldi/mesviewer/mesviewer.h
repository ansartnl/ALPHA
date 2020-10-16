#ifndef OLDI_MES_VIEWER_H
#define OLDI_MES_VIEWER_H

#include "ui_mesviewer.h"
#include <QFrame>
#include <QSqlQueryModel>

//! OLDI message viewer
class MesViewer : public QFrame, protected Ui::mesviewer
{
    Q_OBJECT
public:
    MesViewer(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    void SetFilter(int in_out, QString sFrom, QString sTo, QString sType, QString sACID, QDateTime dt);

protected:
    void changeEvent(QEvent* event);

private slots:
    void OnApply();

private:
    void Translate();
    void ApplyFilter(int in_out, QString sFrom, QString sTo, QString sType, QString sACID, QDateTime dt);

private:
    QSqlQueryModel  m_model;
};

#endif // OLDI_MES_VIEWER_H
