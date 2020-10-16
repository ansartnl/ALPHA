#include "RcGridCoordDlg.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QCheckBox>

#include "main.h"

RcGridCoordDlg::RcGridCoordDlg(QWidget *parent)
    : InputCoordDlg(parent, tr("RcGrid center coordinate")),
      mRadius(0), mSpinBoxRadius(0),
      mStep(0), mSpinBoxStep(0),
      mSector(0), mSpinBoxSector(0),
      mCheckBoxDeclination(0), mSpinBoxDeclination(0)
{
     QVBoxLayout* pLayout = dynamic_cast<QVBoxLayout*>(layout());
     if(pLayout)
     {
         QHBoxLayout* pHorLayout = new QHBoxLayout;

         pHorLayout->addWidget(new QLabel(tr("Radius(km)"), this));
         mSpinBoxRadius = new QSpinBox(this);
         mSpinBoxRadius->setRange(1, 9999);
         mSpinBoxRadius->setSingleStep(1);
         pHorLayout->addWidget(mSpinBoxRadius);
         pHorLayout->addStretch();

         pHorLayout->addWidget(new QLabel(tr("Step(km)"), this));
         mSpinBoxStep = new QSpinBox(this);
         mSpinBoxStep->setRange(1, 9999);
         mSpinBoxStep->setSingleStep(1);
         pHorLayout->addWidget(mSpinBoxStep);
         pHorLayout->addStretch();

         pHorLayout->addWidget(new QLabel(tr("Sector(degree)"), this));
         mSpinBoxSector = new QSpinBox(this);
         mSpinBoxSector->setRange(1, 360);
         mSpinBoxSector->setSingleStep(1);
         pHorLayout->addWidget(mSpinBoxSector);

         QLayoutItem* itemLayout = pLayout->itemAt(0);
         if(itemLayout)
         {
             QHBoxLayout* pHorLayout1 = dynamic_cast<QHBoxLayout*>(itemLayout->layout());
             if(pHorLayout1)
             {
                 QGroupBox* pBox = new QGroupBox(tr("Declination"), this);
                 QGridLayout* pGrid = new QGridLayout(pBox);
                 mCheckBoxDeclination = new QCheckBox(pBox);
                 mCheckBoxDeclination->setCheckState("yes" == networkSettings()->value("users/declination_enable", "no").toString() ? Qt::Checked : Qt::Unchecked);
                 pGrid->addWidget(mCheckBoxDeclination, 0, 0);
                 pGrid->addWidget(new QLabel(tr("Enable"), pBox), 0, 1);

                 mSpinBoxDeclination = new QDoubleSpinBox(pBox);
                 mSpinBoxDeclination->setDecimals(1);
                 mSpinBoxDeclination->setRange(-180.0, 180.0);
                 mSpinBoxDeclination->setSingleStep(1.0);
                 mSpinBoxDeclination->setValue(networkSettings()->value("users/declination_value", 0).toDouble());
                 pGrid->addWidget(mSpinBoxDeclination, 1, 0);
                 pGrid->addWidget(new QLabel(tr("Value"), pBox), 1, 1);

                 //pGrid->setColumnStretch(2, 1);
                 pHorLayout1->addWidget(pBox);
             }
         }

         pLayout->insertLayout(0, pHorLayout);

         adjustSize();
     }
}

void RcGridCoordDlg::accept()
{
    if(mSpinBoxRadius)
        mRadius = mSpinBoxRadius->value() * 1000;
    if(mSpinBoxStep)
        mStep = mSpinBoxStep->value() * 1000;
    if(mSpinBoxSector)
        mSector = mSpinBoxSector->value();
    if(mCheckBoxDeclination)
        networkSettings()->setValue("users/declination_enable", mCheckBoxDeclination->isChecked() ? "yes" : "no");
    if(mSpinBoxDeclination)
        networkSettings()->setValue("users/declination_value", mSpinBoxDeclination->value());
    InputCoordDlg::accept();
}

RcGridValues RcGridCoordDlg::GetRcGridValues() const
{
    RcGridValues val;
    val.mCenter = GetCoord();
    val.mRadius = mRadius;
    val.mStep = mStep;
    val.mSector = mSector;
    return val;
}

void RcGridCoordDlg::SetRcGridValues(const RcGridValues& values)
{
    SetCoord(values.mCenter);
    mRadius = values.mRadius;
    if(mRadius <= 0) mRadius = 400000;
    if(mSpinBoxRadius)
        mSpinBoxRadius->setValue(mRadius / 1000);
    mStep = values.mStep;
    if(mStep <= 0) mStep = 50000;
    if(mSpinBoxStep)
        mSpinBoxStep->setValue(mStep / 1000);
    mSector = values.mSector % 360;
    if(mSector <= 0)
        mSector = 30;
    if(mSpinBoxSector)
        mSpinBoxSector->setValue(mSector);
}
