#ifndef BEARING_DLG_H
#define BEARING_DLG_H

#include <QDialog>
#include "ui_BearingDlg.h"
#include "BearingPoint.h"

class CBearingDlg: public QDialog, public Ui::BearingDlg
{
    Q_OBJECT
public:
    CBearingDlg(const QString& sFIR, QList<CBearingPoint>& list, QWidget* parent = 0);
private slots:
    virtual void accept();
private:
    QList<CBearingPoint>&   m_listBearing;
    QList<CBearingPoint>    m_listAllBearing;
};
#endif // BEARING_DLG_H
