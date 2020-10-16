#ifndef CONVERSION_FUNCTIONS_H_
#define CONVERSION_FUNCTIONS_H_

#include <QString>

namespace conversion_detail
{

    bool isKnot(const QString &value);

    bool isMach(const QString &value);

    bool isFeetInMinutes(const QString &value);

    bool canConvertToKnot(const QString &value);

    bool canConvertToFeetInMinutes(const QString &value);

    qreal convertToKnot(const QString &value);

    qreal convertToMach(const QString &value);

    qreal convertToFeetInMinutes(const QString &value);

/*bool isMetricUnits(const QString &value);

QString convertFeetMeter(qreal value);

QString convertFeetMeter(qreal value_for_convert, int field_width);

qreal convertFeetMeterReal(qreal value);

QString convertGroundSpeed(qreal gsp);

QString valueWithoutUnits(const QString &value);

qreal heightInMeters(const QString &value);

qreal translateSpeedToMach(const qreal &airplaneSpeed, const qreal &windSpeed, const qreal &alpha, const qreal &airTemp);

qreal translateSpeedToMachSimple(const qreal &airplaneSpeed, const qreal &airTemp = 0.0f);*/

} // namespace conversion_detail


#endif // CONVERSION_FUNCTIONS_H_
