#include "MeteoRunwayDialog.h"

#include <QtDebug>
#include <QLineEdit>
#include <QtCore/QDateTime>
#include <QMessageBox>

MeteoRunwayDialog::MeteoRunwayDialog(QWidget *parent)
    :QDialog(parent)
{
    setupUi(this);
}

MeteoRunwayDialog::~MeteoRunwayDialog()
{

}

void MeteoRunwayDialog::setMeteo(const MeteoRunway &meteo)
{
    airportName = meteo.GetAirportName();
    runwayName->setText(meteo.GetRunwayName());

    QString result = meteo.contains(MeteoRunway::ControlTime)?
        meteo.value(MeteoRunway::ControlTime).toDateTime().toString("hhmm"):
                "----";
    controlTime->setText(result);

    result = meteo.contains(MeteoRunway::MagneticWind)?
        QString::number(meteo.value(MeteoRunway::MagneticWind).toUInt()):
                "----";
    magneticWind->setText(result);

    result = meteo.contains(MeteoRunway::AverageSpeed) ?
                QString::number(meteo.value(MeteoRunway::AverageSpeed).toUInt()) : "--";
    averageSpeed->setText(result);

    result = meteo.contains(MeteoRunway::MaxSpeed) ?
                QString::number(meteo.value(MeteoRunway::MaxSpeed).toUInt()) : "--";
    maxSpeed->setText(result);

    result = meteo.contains(MeteoRunway::SideWind) ?
                QString::number(meteo.value(MeteoRunway::SideWind).toDouble(), 'f', 1) : "--.-";

    sideWind->setText(result);

    QString longWindSign = meteo.contains(MeteoRunway::LongitudialWind) ?
                (meteo.value(MeteoRunway::LongitudialWind).toDouble() >= 0. ? "+" : "-") : "-";

    result = longWindSign + (meteo.contains(MeteoRunway::LongitudialWind) ?
                                 QString::number(meteo.value(MeteoRunway::LongitudialWind).toDouble(), 'f', 1) : "--");
    longitudialWind->setText(result);


    if ( !meteo.contains(MeteoRunway::SightDistance) )
        result = "----";
    else
    {
        quint32 dist = meteo.value(MeteoRunway::SightDistance).toUInt();
        if ( dist > 9999 )
            dist = 9999;
        result = QString("%1").arg(dist, 4, 10, QChar('0'));
    }
    sightDistance->setText(result);

    result = meteo.contains(MeteoRunway::SDistanceStart) ?
                QString::number(meteo.value(MeteoRunway::SDistanceStart).toUInt()) : "----";
    sDistanceStart->setText(result);

    result = meteo.contains(MeteoRunway::SDistanceMiddle) ?
                QString::number(meteo.value(MeteoRunway::SDistanceMiddle).toUInt()) : "----";
    sDistanceMiddle->setText(result);
    result = meteo.contains(MeteoRunway::SDistanceEnd) ?
                QString::number(meteo.value(MeteoRunway::SDistanceEnd).toUInt()) : "----";
    sDistanceEnd->setText(result);

    result = ( meteo.contains(MeteoRunway::LowerCloudiness) ?
                QString::number(meteo.value(MeteoRunway::LowerCloudiness).toUInt()) : "----" );
    lowerCloudiness->setText(result);

    // 1013,25 гПа = 760 мм.рт.ст.
    const qreal mmToGPaCoef = 1.33322368421;

    result = meteo.contains(MeteoRunway::Pressure) ?
                QString::number(meteo.value(MeteoRunway::Pressure).toUInt()) : "---";
    pressureMM->setText(result);
    result = meteo.contains(MeteoRunway::PressureSea) ?
                QString::number(meteo.value(MeteoRunway::PressureSea).toUInt()) : "---";
    pressureSea->setText(result);

    result = meteo.contains(MeteoRunway::Pressure) ?
                QString::number(quint32(meteo.value(MeteoRunway::Pressure).toDouble()*mmToGPaCoef)) : "---";
    pressgPa->setText(result);

    QString temperatureSign = meteo.contains(MeteoRunway::Temperature) ?
                (meteo.value(MeteoRunway::Temperature).toDouble() >= 0. ? "+" : "-") : "-";
    QString condPointSign = meteo.contains(MeteoRunway::CondensationPoint) ?
                (meteo.value(MeteoRunway::CondensationPoint).toDouble() >= 0. ? "+" : "-") : "-";

    result = temperatureSign + (meteo.contains(MeteoRunway::Temperature) ?
                QString::number(meteo.value(MeteoRunway::Temperature).toDouble(), 'f', 1) : "--.-");
    temperature->setText(result);

    result = condPointSign + (meteo.contains(MeteoRunway::CondensationPoint) ?
                QString::number(meteo.value(MeteoRunway::CondensationPoint).toDouble(), 'f', 1) : "--.-");
    condensationPoint->setText(result);
}

MeteoRunway MeteoRunwayDialog::meteo() const
{
    MeteoRunway meteo;

    meteo.SetAirportName(airportName);
    meteo.SetRunwayName(runwayName->text());

    if (!controlTime->text().isEmpty())
    {
        if (QTime::fromString(controlTime->text(), "hhmm").isValid())
        {
            QDateTime dateTime = QDateTime::currentDateTimeUtc();
            dateTime.setTime(QTime::fromString(controlTime->text(), "hhmm"));
            meteo.insert(MeteoRunway::ControlTime, dateTime);
        }
    }

    if (!magneticWind->text().isEmpty())
        meteo.insert(MeteoRunway::MagneticWind, magneticWind->text().toUInt());

    if (!averageSpeed->text().isEmpty())
        meteo.insert(MeteoRunway::AverageSpeed, averageSpeed->text().toUInt());

    if (!maxSpeed->text().isEmpty())
        meteo.insert(MeteoRunway::MaxSpeed, maxSpeed->text().toUInt());

    if (!sideWind->text().isEmpty())
    {
        bool ok = false;
        double val = sideWind->text().toDouble(&ok);
        if (ok)
            meteo.insert(MeteoRunway::SideWind, val);
    }

    if (!longitudialWind->text().isEmpty())
    {
        bool ok = false;
        double val = longitudialWind->text().toDouble(&ok);
        if (ok)
            meteo.insert(MeteoRunway::LongitudialWind, val);
    }

    if (!sightDistance->text().isEmpty())
        meteo.insert(MeteoRunway::SightDistance, sightDistance->text().toUInt());

    if (!sDistanceStart->text().isEmpty())
        meteo.insert(MeteoRunway::SDistanceStart, sDistanceStart->text().toUInt());

    if (!sDistanceMiddle->text().isEmpty())
        meteo.insert(MeteoRunway::SDistanceMiddle, sDistanceMiddle->text().toUInt());

    if (!sDistanceEnd->text().isEmpty())
        meteo.insert(MeteoRunway::SDistanceEnd, sDistanceEnd->text().toUInt());

    if (!lowerCloudiness->text().isEmpty())
        meteo.insert(MeteoRunway::LowerCloudiness, lowerCloudiness->text().toUInt());

    if (!pressureMM->text().isEmpty())
        meteo.insert(MeteoRunway::Pressure, pressureMM->text().toUInt());

    if (!pressureSea->text().isEmpty())
        meteo.insert(MeteoRunway::PressureSea, pressureSea->text().toUInt());

    if (!temperature->text().isEmpty())
    {
        bool ok = false;
        double val = temperature->text().toDouble(&ok);
        if (ok)
            meteo.insert(MeteoRunway::Temperature, val);
    }

    if (!condensationPoint->text().isEmpty())
    {
        bool ok = false;
        double val = condensationPoint->text().toDouble(&ok);
        if (ok)
            meteo.insert(MeteoRunway::CondensationPoint, val);
    }

    return meteo;
}

void MeteoRunwayDialog::accept()
{
    if(validateMeteo())
        QDialog::accept();
}

bool MeteoRunwayDialog::validateMeteo()
{
    bool bRes = true;

    if (bRes && !pressureMM->text().isEmpty())
    {
        int iVal = pressureMM->text().toUInt();
        if(iVal > 0)
        {
            if(iVal < 500 || iVal > 1200)
            {
                bRes = false;
                QMessageBox::warning(this, tr("Error"), tr("Pressure from 500 to 1200"));
            }
        }
    }

    if (bRes && !pressureSea->text().isEmpty())
    {
        int iVal = pressureSea->text().toUInt();
        if(iVal > 0)
        {
            if(iVal < 500 || iVal > 1200)
            {
                bRes = false;
                QMessageBox::warning(this, tr("Error"), tr("Pressure from 500 to 1200"));
            }
        }
    }

    return bRes;
}
