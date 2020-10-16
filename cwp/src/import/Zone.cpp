#include "Zone.h"

#include "BoundaryPoint.h"
#include "Coordinate.h"

Zone::Zone() :
        m_iType(0), m_uiId(0), m_iLowLimit(0), m_iUpLimit(0), m_uiParentID(0)
{
}

Zone::~Zone()
{
}

void Zone::SetName(const QString& qsName)
{
    m_qsName = qsName;
}

const QString& Zone::GetName() const
{
    return m_qsName;
}

void Zone::SetIDName(const QString& qsIDName)
{
    m_qsIDName = qsIDName;
}

const QString& Zone::GetIDName() const
{
    return m_qsIDName;
}

void Zone::SetType(int iType)
{
    m_iType = iType;
}

int Zone::GetType() const
{
    return m_iType;
}

void Zone::SetID(unsigned int uiID)
{
    m_uiId = uiID;
}

unsigned int Zone::GetID() const
{
    return m_uiId;
}

void Zone::SetLowLimit(int iLowLimit)
{
    m_iLowLimit = iLowLimit;
}

int Zone::GetLowLimit() const
{
    return m_iLowLimit;
}

void Zone::SetUpLimit(int iUpLimit)
{
    m_iUpLimit = iUpLimit;
}

int Zone::GetUpLimit() const
{
    return m_iUpLimit;
}

void Zone::SetParentID(uint uiID)
{
    m_uiParentID = uiID;
}

uint Zone::GetParentID() const
{
    return m_uiParentID;
}

void Zone::AddBoundaryPoint(const QSharedPointer<CBoundaryPoint>& pPoint)
{
    m_Boundary.append(pPoint);
}

void Zone::SetShortName(const QString &sName)
{
    m_qsShortName = sName;
}

QString Zone::GetShortName() const
{
    return m_qsShortName;
}

void Zone::SetLabelName(QString sName)
{
    m_qsLabelName = sName;
}

QString Zone::GetLabelName() const
{
    return m_qsLabelName;
}

void Zone::SetFrequency(double dFrequency)
{
    m_dFrequency = dFrequency;
}

double Zone::GetFrequency() const
{
    return m_dFrequency;
}
