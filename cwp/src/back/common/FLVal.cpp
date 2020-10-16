#include "FLVal.h"

FLVal::FLVal()
    : m_valF(0), m_valM(0), m_bIsFeet(false)
{
}

FLVal::FLVal(const QString& sLevel)
    : m_valF(0), m_valM(0), m_bIsFeet(false)
{
    if(sLevel.size() > 1)
    {
        bool bOk(false);
        quint32 uiResult = sLevel.right(sLevel.size()-1).toUInt(&bOk);
        if(bOk)
        {
            if (sLevel[0] == 'F')
            {
                // feets
                m_valF = uiResult;
                m_valM = m_valF * 30.48;// + 0.5;
                m_bIsFeet = true;
            }
            else if (sLevel[0] == 'S')
            {
                // meters
                m_valM = uiResult * 10;
                m_valF = m_valM / 30.48;// + 0.5;
                m_bIsFeet = false;
            }
        }
    }
}

FLVal::FLVal(double feet, double meters, bool bFeet)
    : m_valF(feet), m_valM(meters), m_bIsFeet(bFeet)
{
}

FLVal::FLVal(double dVal, bool bFeet)
    : m_valF(0), m_valM(0), m_bIsFeet(false)
{
    if(bFeet)
    {
        // feets
        m_valF = dVal;
        m_valM = dVal * 30.48;
        m_bIsFeet = true;
    }
    else
    {
        // meters
        m_valF = dVal / 30.48;
        m_valM = dVal;
        m_bIsFeet = false;
    }
}

QString FLVal::toString() const
{
    QString sRes;
    if(m_valF)
    {
        if(m_bIsFeet)
        {
            // feets
            sRes = QString("F%1").arg(m_valF, 3, 'f', 0, QLatin1Char('0'));
        }
        else
        {
            // meters
            sRes = QString("S%1").arg(m_valM / 10, 4, 'f', 0, QLatin1Char('0'));
        }
    }
    return sRes;
}
