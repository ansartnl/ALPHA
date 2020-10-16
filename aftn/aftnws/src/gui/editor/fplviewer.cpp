#include "fplviewer.h"
#include "fpl.h"

#include "printutils.h"
#include "AFTNDao.h"

#include <QtGui/QPushButton>

#include <QtSql/QSqlField>

FplViewer::FplViewer(QWidget* pParent) :
  AbstractDialog(pParent)
{
  setupUi(this);

  // Print engine
  printButton = new QPushButton(tr("Print..."), this);
  buttonBox->addButton(printButton, QDialogButtonBox::ActionRole);
  printButton->show();

  restoreState(staticMetaObject.className());
}

FplViewer::~FplViewer()
{
  saveState(staticMetaObject.className());
}

/** Copy information from FPL to dialog and over */
void FplViewer::setFPL(const FPL& Fpl)
{
  setWindowTitle(tr("Displaying Flight Plane #%1").arg(Fpl.id()));
  FIREdit->setText(Fpl.firName());
  ACIDEdit->setText(Fpl.aircarftId());
  TurbulenceEdit->setText(Fpl.turbulenceCategory());
  RulesEdit->setText(Fpl.flightRules());
  FlightTypeEdit->setText(Fpl.flightType());
  NumberEdit->setText(QString::number(Fpl.aircraftNumber()));
  AirplaneEdit->setText(Fpl.aircraftType());
  EquipmentEdit->setText(Fpl.equipment());
  AerodromeDepEdit->setText(Fpl.departureAerodrome());
  TimeDepEdit->setTime(Fpl.departureTime());
  AerodromeDestEdit->setText(Fpl.destinationAerodrome());
  EET->setText(Fpl.EET());
  entryFIREET->setText(Fpl.entryFIREET());
  RouteEdit->setText(Fpl.route());
  OtherEdit->setText(Fpl.otherInformation());
  ALTNEdit->setText(Fpl.altDestinationAerodrome());
  ALTN2Edit->setText(Fpl.altDestinationAero2());
  Speed1Edit->setText(QString(Fpl.speedUnit()));
  Speed2Edit->setText(Fpl.speedValue());
  Level1Edit->setText(QString(Fpl.levelUnit()));
  Level2Edit->setText(Fpl.levelValue());
  status->setText(Fpl.exFPL());
}

void FplViewer::changeEvent(QEvent* event)
{
  QDialog::changeEvent(event);

  if (event->type() == QEvent::LanguageChange) {
    retranslateUi(this);
    printButton->setText(tr("Print..."));
  }
}

void FplViewer::on_buttonBox_clicked(QAbstractButton* button)
{
  if (button == printButton)
    printWidget(frame, QPrinter::Landscape);
}
