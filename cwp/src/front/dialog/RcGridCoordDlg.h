#ifndef RCGRID_COORD_DLG_H
#define RCGRID_COORD_DLG_H

#include "InputCoordDlg.h"
#include "RcGridValues.h"

class QSpinBox;
class QCheckBox;
class QDoubleSpinBox;

//! Dialog for input RcGrid coordinate
class RcGridCoordDlg: public InputCoordDlg
{
    Q_OBJECT
public:
    RcGridCoordDlg(QWidget* parent = 0);

    RcGridValues GetRcGridValues() const;
    void SetRcGridValues(const RcGridValues& values);

protected slots:
    virtual void accept();

private:
    int         mRadius;
    QSpinBox*   mSpinBoxRadius;
    int         mStep;
    QSpinBox*   mSpinBoxStep;
    int         mSector;
    QSpinBox*   mSpinBoxSector;

    QCheckBox*  mCheckBoxDeclination;
    QDoubleSpinBox*   mSpinBoxDeclination;
};

#endif // RCGRID_COORD_DLG_H
