#include "coordedit.h"

#include "QVariant"
#include "math.h"

#ifdef _MSC_VER
#define UTF8_QSTRING(str) QString::fromWCharArray(L##str)
#else
#define UTF8_QSTRING(str) QString::fromUtf8(str)
#endif

namespace coordcommon
{
    QString getDegree()
    {
        return UTF8_QSTRING("\u00B0");
    }
}

CoordValidator::CoordValidator(const CoordEdit::CoordType type, QWidget *parent)
    : QRegExpValidator(parent), m_CoordType(type)
{
    switch(type)
    {
    case CoordEdit::Latitude:
        {
            QRegExp rx(QString("([-+ _]?)([0-9_]{2,2})%1([0-9_]{2,2})'([0-9_]{2,2})''").arg(coordcommon::getDegree()));
            setRegExp(rx);
        }
        break;
    case CoordEdit::Longitude:
        {
            QRegExp rx(QString("([-+ _]?)([0-9_]{3,3})%1([0-9_]{2,2})'([0-9_]{2,2})''").arg(coordcommon::getDegree()));
            setRegExp(rx);
        }
        break;
    case CoordEdit::LatitudeLongitude:
        {
            QRegExp rx(QString("([-+ _]?)([0-9_]{2,2})%1([0-9_]{2,2})'([0-9_]{2,2})'' ([-+ _]?)([0-9_]{3,3})%1([0-9_]{2,2})'([0-9_]{2,2})''").arg(coordcommon::getDegree()));
            setRegExp(rx);
        }
        break;
    case CoordEdit::Azimuth:
        Q_ASSERT(false);
        break;
    }
}

QValidator::State CoordValidator::validate(QString& input, int& pos) const
{
    QValidator::State state = QRegExpValidator::validate(input, pos);
    if(QValidator::Acceptable == state)
    {
        if(regExp().exactMatch(input))
        {
            bool bNull = true;
            if(CoordEdit::Latitude == m_CoordType)
            {
                if(regExp().cap(2) != "__" || regExp().cap(3) != "__" || regExp().cap(4) != "__")
                    bNull = false;
            }
            if(CoordEdit::Longitude == m_CoordType)
            {
                if(regExp().cap(2) != "___" || regExp().cap(3) != "__" || regExp().cap(4) != "__")
                    bNull = false;
            }
            if(!bNull)
            {
                int iVal(0);
                bool bOk(false);
                if(CoordEdit::Latitude == m_CoordType)
                {
                    if(!regExp().cap(2).isEmpty())
                    {
                        QString sVal = regExp().cap(2);
                        sVal.replace(QString("_"), QString("0"));
                        iVal = sVal.toInt(&bOk);
                        if(!bOk || iVal > 89)
                        {
                            pos = regExp().pos(2);
                            return QValidator::Invalid;
                        }
                    }
                    else
                    {
                        pos = 0;
                        return QValidator::Invalid;
                    }
                }
                if(CoordEdit::Longitude == m_CoordType)
                {
                    if(!regExp().cap(2).isEmpty())
                    {
                        QString sVal = regExp().cap(2);
                        sVal.replace(QString("_"), QString("0"));
                        iVal = sVal.toInt(&bOk);
                        if(!bOk || iVal > 179)
                        {
                            pos = regExp().pos(2);
                            return QValidator::Invalid;
                        }
                    }
                    else
                    {
                        pos = 0;
                        return QValidator::Invalid;
                    }
                }
                if(!regExp().cap(3).isEmpty())
                {
                    QString sVal = regExp().cap(3);
                    sVal.replace(QString("_"), QString("0"));
                    iVal = sVal.toInt(&bOk);
                    if(!bOk || iVal > 59)
                    {
                        pos = regExp().pos(3);
                        return QValidator::Invalid;
                    }
                }
                else
                {
                    pos = 0;
                    return QValidator::Invalid;
                }
                if(!regExp().cap(4).isEmpty())
                {
                    QString sVal = regExp().cap(4);
                    sVal.replace(QString("_"), QString("0"));
                    iVal = sVal.toInt(&bOk);
                    if(!bOk || iVal > 59)
                    {
                        pos = regExp().pos(4);
                        return QValidator::Invalid;
                    }
                }
                else
                {
                    pos = 0;
                    return QValidator::Invalid;
                }
            }
        }
    }
    return state;
}

bool CoordValidator::isEmpty(const QString& input) const
{
    if(regExp().exactMatch(input))
    {
        bool bNull = true;
        if(CoordEdit::Latitude == m_CoordType)
        {
            if(regExp().cap(2) != "__" || regExp().cap(3) != "__" || regExp().cap(4) != "__")
                bNull = false;
        }
        if(CoordEdit::Longitude == m_CoordType)
        {
            if(regExp().cap(2) != "___" || regExp().cap(3) != "__" || regExp().cap(4) != "__")
                bNull = false;
        }
        return bNull;
    }
    return false;
}

CoordEdit::CoordEdit(const CoordType type, QWidget *parent)
    : QLineEdit(parent), m_CoordType(type)
{
    switch(type)
    {
    case Latitude:
        {
            CoordValidator* validator = new CoordValidator(type, this);
            setValidator(validator);
            QString sMask = "#00" + coordcommon::getDegree() + "00'00'';_";
            setInputMask(sMask);
        }
        break;
    case Longitude:
        {
            CoordValidator* validator = new CoordValidator(type, this);
            setValidator(validator);
            QString sMask = "#000" + coordcommon::getDegree() + "00'00'';_";
            setInputMask(sMask);
        }
        break;
    case LatitudeLongitude:
    case Azimuth:
        Q_ASSERT(false);
        break;
    }
}

QVariant CoordEdit::fromString(const CoordType type, const QString& text)
{
    if(!text.isEmpty())
    {
        QString sText(text);
        int pos(0);
        CoordValidator rx(type);
        if(QValidator::Acceptable == rx.validate(sText, pos))
        {
            switch(type)
            {
            case Latitude:
            case Longitude:
                {
                    QVariant var(QVariant::Double);
                    /* rem this in future
                    */var.setValue(double(0)); //instead of NULL - 0, for support incorrect presentation coordinates
                    if(!rx.isEmpty(sText))
                    {
                        double dSign(0), dDegree(0), dMin(0), dSec(0);
                        dSign = rx.regExp().cap(1) == "-" ? -1. : 1.;

                        QString sVal = rx.regExp().cap(2);
                        sVal.replace(QString("_"), QString("0"));
                        dDegree = sVal.toDouble();

                        sVal = rx.regExp().cap(3);
                        sVal.replace(QString("_"), QString("0"));
                        dMin = sVal.toDouble();

                        sVal = rx.regExp().cap(4);
                        sVal.replace(QString("_"), QString("0"));
                        dSec = sVal.toDouble();

                        double dVal = dSign * (dDegree + dMin/60. + dSec/3600.);
                        var.setValue(dVal);
                    }
                    return var;
                }
                break;
            case LatitudeLongitude:
            case Azimuth:
                Q_ASSERT(false);
                break;
            }
        }
    }
    return QVariant();
}

QString CoordEdit::toString(const CoordType type, const QVariant& value, const bool bUsePlus/* = false*/)
{
    QString sRes;
    if(!value.isNull())
    {
        switch(type)
        {
        case Latitude:
            {
                double dVal = value.toDouble();
                if(dVal) //00G00'00'' - ignore :(
                {
                    QString sSign;
                    if(dVal < 0)
                        sSign = "-";
                    else
                        bUsePlus ? sSign = "+" : sSign = " ";
                    dVal = qAbs(dVal);
                    double dDegree(0), dMin(0), dSec(0);
                    dVal = modf(dVal, &dDegree);
                    dVal *= 60.;
                    dVal = modf(dVal, &dMin);
                    dVal *= 60.;
                    dSec = qRound(dVal);
                    if(!dDegree && !dMin && !dSec)
                        dSec = 1;
                    if (dSec == 60)
                    {
                        dSec = 0;
                        dMin++;
                    }
                    if (dMin == 60)
                    {
                        dMin = 0;
                        dDegree++;
                    }

                    sRes = QString("%1%2%3%4'%5''").arg(sSign).arg(dDegree, 2, 'f', 0, QChar('0'))
                           .arg(coordcommon::getDegree())
                           .arg(dMin, 2, 'f', 0, QChar('0')).arg(dSec, 2, 'f', 0, QChar('0'));
                }
            }
            break;
        case Longitude:
            {
                double dVal = value.toDouble();
                if(dVal) //000G00'00'' - ignore :(
                {
                    QString sSign;
                    if(dVal < 0)
                        sSign = "-";
                    else
                        bUsePlus ? sSign = "+" : sSign = " ";
                    dVal = qAbs(dVal);
                    double dDegree(0), dMin(0), dSec(0);
                    dVal = modf(dVal, &dDegree);
                    dVal *= 60.;
                    dVal = modf(dVal, &dMin);
                    dVal *= 60.;
                    dSec = qRound(dVal);
                    if(!dDegree && !dMin && !dSec)
                        dSec = 1;
                    if (dSec == 60)
                    {
                        dSec = 0;
                        dMin++;
                    }
                    if (dMin == 60)
                    {
                        dMin = 0;
                        dDegree++;
                    }

                    sRes = QString("%1%2%3%4'%5''").arg(sSign).arg(dDegree, 3, 'f', 0, QChar('0'))
                           .arg(coordcommon::getDegree())
                           .arg(dMin, 2, 'f', 0, QChar('0')).arg(dSec, 2, 'f', 0, QChar('0'));
                }
            }
            break;
        case LatitudeLongitude:
        case Azimuth:
            Q_ASSERT(false);
            break;
        }
    }
    return sRes;
}

QSize CoordEdit::sizeHint() const
{
    //return QLineEdit::sizeHint();
    return minimumSizeHint();
}

QSize CoordEdit::minimumSizeHint() const
{
    //return QLineEdit::minimumSizeHint();
    QSize size = QLineEdit::minimumSizeHint();
    QFontMetrics fm = fontMetrics();
    QString sText;
    switch(m_CoordType)
    {
    case CoordEdit::Latitude:
        sText = "+00"+coordcommon::getDegree()+"00'00''";
        break;
    case CoordEdit::Longitude:
        sText = "+000"+coordcommon::getDegree()+"00'00''";
        break;
    case CoordEdit::LatitudeLongitude:
    case CoordEdit::Azimuth:
        Q_ASSERT(false);
        break;
    }
    int w = fm.width(sText)+8;
    size.setWidth(w);
    return size;
}

QVariant CoordEdit::getValue() const
{
    return fromString(m_CoordType, displayText());
}

void CoordEdit::setValue(const QVariant& value)
{
    setText(toString(m_CoordType, value, true));
}
