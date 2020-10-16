#include "InputCoordDlg.h"
#include "coordedit.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "front/common/CoordsDigitizer.h"

InputCoordDlg::InputCoordDlg(QWidget *parent, QString sTitle, QString sLabel)
    : QDialog(parent)
{
    setWindowTitle(sTitle);

    QGroupBox* pBox = new QGroupBox(sLabel, this);

    QGridLayout* pGrid = new QGridLayout(pBox);
    pGrid->addWidget(new QLabel(tr("Latitude"), pBox), 0, 0);
    m_pLatitude = new CoordEdit(CoordEdit::Latitude, pBox);
    pGrid->addWidget(m_pLatitude, 0, 1);

    pGrid->addWidget(new QLabel(tr("Longitude"), pBox), 1, 0);
    m_pLongitude = new CoordEdit(CoordEdit::Longitude, pBox);
    pGrid->addWidget(m_pLongitude, 1, 1);

    pGrid->setColumnStretch(2, 1);
    QHBoxLayout* pHorLayout1 = new QHBoxLayout;
    pHorLayout1->addWidget(pBox);

    QHBoxLayout* pHorLayout = new QHBoxLayout;
    QPushButton* pBut = new QPushButton(tr("Copy"), this);
    connect(pBut, SIGNAL(clicked()), SLOT(OnButCopy()));
    pHorLayout->addWidget(pBut);
    pBut = new QPushButton(tr("Paste"), this);
    connect(pBut, SIGNAL(clicked()), SLOT(OnButPaste()));
    pHorLayout->addWidget(pBut);
    pBut = new QPushButton(tr("OK"), this);
    connect(pBut, SIGNAL(clicked()), SLOT(accept()));
    pBut->setDefault(true);
    pHorLayout->addWidget(pBut);
    pBut = new QPushButton(tr("Cancel"), this);
    connect(pBut, SIGNAL(clicked()), SLOT(reject()));
    pHorLayout->addWidget(pBut);

    QVBoxLayout* pVerLayout = new QVBoxLayout(this);
    pVerLayout->addLayout(pHorLayout1);
    pVerLayout->addLayout(pHorLayout);

    adjustSize();
}

CCoord InputCoordDlg::GetCoord() const
{
    return m_coord;
}

void InputCoordDlg::SetCoord(const CCoord &coord)
{
    m_coord = coord;
    m_pLatitude->setValue(m_coord.GetLatitude().GetAngle());
    m_pLongitude->setValue(m_coord.GetLongitude().GetAngle());
}

void InputCoordDlg::accept()
{
    m_coord.SetLatitude(CAngle(m_pLatitude->getValue().toDouble()));
    m_coord.SetLongitude(CAngle(m_pLongitude->getValue().toDouble()));
    QDialog::accept();
}

void InputCoordDlg::OnButCopy()
{
    CCoord coord;
    coord.SetLatitude(CAngle(m_pLatitude->getValue().toDouble()));
    coord.SetLongitude(CAngle(m_pLongitude->getValue().toDouble()));
    CoordsDigitizer dz;
    dz.addCoord(coord);
    dz.toClipboard();
}

void InputCoordDlg::OnButPaste()
{
    CoordsDigitizer::TCoordList list;
    list = CoordsDigitizer::GetPointsFromClipboard(this);
    if(list.size())
    {
        SetCoord(list.first());
    }
}
