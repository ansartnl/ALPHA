#include "conversionfunctions.h"

namespace conversion_detail
{
    static const QString Knot("Kt");
    static const QString Mach("M");
    static const QString Kmh("Km");

    static const QString fmin("fm");
    static const QString msec("ms");

    bool isKnot(const QString &value)
    {
        return value.startsWith(Knot);
    }

    bool isMach(const QString &value)
    {
        return value.startsWith(Mach);
    }

    bool isFeetInMinutes(const QString &value)
    {
        return value.startsWith(fmin);
    }

    bool canConvertToKnot(const QString &value)
    {
        return (value.startsWith(Knot) || value.startsWith(Mach) || value.startsWith(Kmh));
    }

    bool canConvertToFeetInMinutes(const QString &value)
    {
        return (value.startsWith(fmin) || value.startsWith(msec));
    }

    qreal convertToKnot(const QString &value)
    {
        if (value.startsWith(Knot)) {
            bool ok;
            float result = value.right(value.size() - 2).toFloat(&ok);
            if (ok)
                return result;
        } else if (value.startsWith(Mach)) {
            bool ok;
            float result = value.right(value.size() - 1).toFloat(&ok);
            if (ok)
                return result * 573.524;
        } else if (value.startsWith(Kmh)) {
            bool ok;
            float result = value.right(value.size() - 2).toFloat(&ok);
            if (ok)
                return result * 0.5399;
        }

        return 0;
    }

    qreal convertToMach(const QString &value)
    {
        if (value.startsWith(Knot)) {
            bool ok;
            float result = value.right(value.size() - 2).toFloat(&ok);
            if (ok)
                return result / 573.524;
        } else if (value.startsWith(Mach)) {
            bool ok;
            float result = value.right(value.size() - 1).toFloat(&ok);
            if (ok)
                return result;
        } else if (value.startsWith(Kmh)) {
            bool ok;
            float result = value.right(value.size() - 2).toFloat(&ok);
            if (ok)
                return result * 0.5399 / 573.524;
        }

        return 0;
    }

    qreal convertToFeetInMinutes(const QString &value)
    {
        if (value.startsWith(fmin)) {
            bool ok;
            float result = value.right(value.size() - 2).toFloat(&ok);
            if (ok)
                return result;
        } else if (value.startsWith(msec)) {
            bool ok;
            float result = value.right(value.size() - 2).toFloat(&ok);
            if (ok)
                return result * 196.85;
        }

        return 0;
    }

/*bool isMetricUnits(const QString &value)
{
    if (value.isEmpty())
        return true;

    if (value[0] == 'C')
        return true;

    if (value.startsWith("ms"))
        return true;

    if (value.startsWith("Km"))
        return true;

    return false;
}

QString convertFeetMeter(qreal value)
{
    auto convert = [](const QString & prefix, qreal value, int field_width)->QString {
        return prefix + QString("%1").arg(value, field_width, 'f', 0, '0');
    };

    if (settings::HeightMeasure::Meters == height_measure) {
        value /= 10.f;
        return (use_measure_prefixes ? convert("C", value, 4) : convert("", value, 4));
    } else {
        value *= 3.28f;
        value /= 100.f;
        return (use_measure_prefixes ? convert("F", value, 3) : convert("", value, 3));
    }
}

QString convertFeetMeter(qreal value_for_convert, int field_width)
{
    Q_UNUSED(value_for_convert);
    Q_UNUSED(field_width);

    auto convert = [](const QString & prefix, qreal value_for_convert, int field_width)->QString {
        return prefix + QString("%1").arg(value_for_convert, field_width, 'f', 0, '0');
    };

    auto height_measure = (settings::HeightMeasure)settings_storage->value(settings::key_heightMeasure).toInt();
    auto use_measure_prefixes = settings_storage->value(settings::key_measurePrefixes).toBool();

    QString sign_prefix((value_for_convert >= 0. ? "+" : ""));

    if (settings::HeightMeasure::Meters == height_measure) {
        return (use_measure_prefixes ? convert("ms" + sign_prefix, value_for_convert, field_width) : convert(sign_prefix, value_for_convert, field_width - 3));
    } else {
        value_for_convert *= 3.28f;
        return (use_measure_prefixes ? convert("fm" + sign_prefix, value_for_convert, field_width) : convert(sign_prefix, value_for_convert, field_width - 3));
    }
    return QString();
}

qreal convertFeetMeterReal(qreal value)
{
    auto height_measure = (settings::HeightMeasure)settings_storage->value(settings::key_heightMeasure).toInt();

    if (settings::HeightMeasure::Meters == height_measure) {
        value /= 10.f;
        return value;
    } else {
        value *= 3.28f;
        value /= 100.f;
        return value;
    }
}

QString convertGroundSpeed(qreal gsp)
{
    static const qreal gsp_high_limit = 999.;
    static const QString gsp_high_limit_string = "999??";

    auto convert = [](const QString & prefix, qreal value, int precision, int width) {
        return prefix + QString("%1").arg(value, 4, 'f', precision).trimmed().rightJustified(width, ' ');
    };

    if (settings::SpeedMeasure::Kmhs == speed_measure) {
        gsp *= 3.6;
        if (gsp > gsp_high_limit)
            return gsp_high_limit_string;
        if (use_measure_prefixes)
            return convert("Km", gsp, 0, 3);
    } else if (settings::SpeedMeasure::Knots == speed_measure) {
        gsp *= 1.946;
        if (gsp > gsp_high_limit)
            return gsp_high_limit_string;
        if (use_measure_prefixes)
            return convert("Kt", gsp, 0, 3);
    } else {
        gsp *= 0.003;
        if (gsp > gsp_high_limit)
            return gsp_high_limit_string;
        return (use_measure_prefixes ? convert("M", gsp, 1, 4) : convert("", gsp, 1, 4));
    }

    return convert("", gsp, 0, 3);
}

QString valueWithoutUnits(const QString &value)
{
    QString ret_value(value);
    if (ret_value.isEmpty())
        return ret_value;

    if (ret_value[0] == 'C')
        return ret_value.remove(0, 1);

    if (ret_value[0] == 'F')
        return ret_value.remove(0, 1);

    if (ret_value[0] == 'M')
        return ret_value.remove(0, 1);

    if (ret_value.startsWith("fm"))
        return ret_value.remove(0, 2);

    if (ret_value.startsWith("ms"))
        return ret_value.remove(0, 2);

    if (ret_value.startsWith("Kt"))
        return ret_value.remove(0, 2);

    if (ret_value.startsWith("Km"))
        return ret_value.remove(0, 2);

    return ret_value;
}

qreal heightInMeters(const QString &value)
{
    qreal result = -1;

    if (value.isEmpty())
        return result;

    bool isOk = true;
    if (value[0] == 'C')
        result = QString(value).remove(0, 1).toDouble(&isOk) * 10;
    else if (value[0] == 'F')
        result = QString(value).remove(0, 1).toDouble(&isOk) * 3;
    else
        result = -1;

    if (false == isOk)
        result = -1;

    return result;
}

qreal translateSpeedToMach(const qreal &airplaneSpeed, const qreal &windSpeed, const qreal &alphaRad, const qreal &airTempC)
{
    // constants
    qreal x = 1.4f;
    qreal zeroK = 273.15f;
    qreal R = 287.0f;
    // M = v/c
    // process v
    qreal v = airplaneSpeed + cos(alphaRad) * windSpeed;
    // process c
    qreal c = sqrt(x * R * (zeroK + airTempC));
    // process M
    return v / c;
}

qreal translateSpeedToMachSimple(const qreal &airplaneSpeed, const qreal &airTemp)
{
    return flight_detail::translateSpeedToMach(airplaneSpeed, 0.0f, 0.0f, airTemp);
}*/


} // namespace conversion_detail
