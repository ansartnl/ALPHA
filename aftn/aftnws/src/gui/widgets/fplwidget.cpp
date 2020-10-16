#include "fplwidget.h"
#include "fpl.h"

FplWidget::FplWidget(QWidget* pParent)
    : QWidget(pParent)
{
    setupUi(this);
}

/** Copy information from FPL to dialog and over */
void FplWidget::setFPL(const FPL& Fpl)
{
    setWindowTitle(tr("Displaying Flight Plane #%1").arg(Fpl.id()));
    FIREdit->setText(Fpl.firName());
    ACIDEdit->setText(Fpl.aircarftId());
    TurbulenceEdit->setText(Fpl.turbulenceCategory());
    RulesEdit->setText(Fpl.flightRules());
    FlightTypeEdit->setText(Fpl.flightType());
    NumberEdit->setText(QString(Fpl.aircraftNumber()));
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
    Speed1Edit->setText(Fpl.speedUnit());
    Speed2Edit->setText(Fpl.speedValue());
    Level1Edit->setText(Fpl.levelUnit());
    Level2Edit->setText(Fpl.levelValue());
    status->setText(Fpl.exFPL());
}

void FplWidget::clear()
{
    foreach(QLineEdit * child, findChildren<QLineEdit*>())
        child->clear();

    TimeDepEdit->clear();
    RouteEdit->clear();
    OtherEdit->clear();
}

void FplWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
    }
    QWidget::changeEvent(event);
}
