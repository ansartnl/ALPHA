#include "AirplaneType.h"

double CAirplaneType::GetMaxIAS(double dHigh) const
{
	double dResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan)
	{
		dResult = (*itScan)->GetIASMax();
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return dResult ? dResult : 150;
}

double CAirplaneType::GetMinIAS(double dHigh) const
{
    double dResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan)
	{
		dResult = (*itScan)->GetIASMin();
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return dResult ? dResult : 50;
}

double CAirplaneType::GetIAS(double dHigh) const
{
    double dResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan)
	{
		dResult = (*itScan)->GetIAS();
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return dResult ? dResult : 100;
}

double CAirplaneType::GetMaxClimb(double dHigh) const
{
    double dResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan)
	{
		dResult = (*itScan)->GetVClimbMax();
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return dResult ? dResult : 40;
}

double CAirplaneType::GetClimb(double dHigh) const
{
    double dResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan)
	{
		dResult = (*itScan)->GetVClimb();
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return dResult ? dResult : 20;
}

double CAirplaneType::GetMaxDescend(double dHigh) const
{
    double dResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan)
	{
		dResult = (*itScan)->GetVDescendMax();
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return dResult ? dResult : 30;
}

double CAirplaneType::GetDescend(double dHigh) const
{
    double dResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan)
	{
		dResult = (*itScan)->GetVDescend();
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return dResult ? dResult : 20;
}

int CAirplaneType::GetLevelN(double dHigh) const
{
	int iResult(0);
	TSpeedList::const_iterator itScan = m_Speeds.begin();
	for(; itScan != m_Speeds.end(); ++itScan, ++iResult)
	{
		if ((*itScan)->GetLevel() > dHigh)
			break;
	}
    return iResult ? iResult : 9000;
}
