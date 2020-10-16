#include "CFPL.h"
#include <QRegExp>

void CFPL::SetSpeed(const QString& sSpeed)
{
	SetSpeed(ParseSpeed(sSpeed));
	SetSpeedS(sSpeed);
}

void CFPL::SetLevel(const QString& sLevel)
{
	SetLevel(ParseLevel(sLevel));
	SetLevelS(sLevel);
}

CCoord CFPL::ParseCoordinate(const QString& sCoord)
{
	TVA_CHECK(
		sCoord.size() == 7 || sCoord.size() == 11,
		QObject::tr("Wrong size of the custom point field \"%1\"").arg(sCoord),
		E_FAIL);
	bool bOk(false);
	int iLat = sCoord.indexOf(QRegExp("[NS]"));
	TVA_CHECK(
		iLat != -1,
		QObject::tr("No latitude in custom point \"%1\"").arg(sCoord),
		E_FAIL);
	int iLong = sCoord.indexOf(QRegExp("[WE]"), iLat + 1);
	TVA_CHECK(
		iLong != -1,
		QObject::tr("No longitude in custom point \"%1\"").arg(sCoord),
		E_FAIL);
	unsigned int uiLat = sCoord.mid(0, iLat).toUInt(&bOk);
	TVA_CHECK(
		bOk,
		QObject::tr("Wrong structure of latitude in custom point \"%1\"").arg(sCoord),
		E_FAIL);
	unsigned int uiLong = sCoord.mid(iLat+1, iLong - (iLat + 1)).toUInt(&bOk);
	TVA_CHECK(
		bOk,
		QObject::tr("Wrong structure of longitude in custom point \"%1\"").arg(sCoord),
		E_FAIL);
	if (sCoord[iLat] == 'S')
		uiLat *= -1;
	if (sCoord[iLong] == 'W')
		uiLong *= -1;
	if (sCoord.size() == 11)
		return CCoord(
			CAngle(uiLong / 100, uiLong % 100, 0),
			CAngle(uiLat / 100, uiLat % 100, 0));
	return CCoord(
		CAngle(uiLong, 0, 0),
		CAngle(uiLat, 0, 0));
}

quint32 CFPL::ParseLevel(const QString& sLevel)
{
	TVA_CHECK(sLevel.size() > 1, QObject::tr("Level has incorrect format"), E_FAIL);
	bool bOk(false);
	quint32 uiResult = sLevel.right(sLevel.size()-1).toUInt(&bOk);
	TVA_CHECK(bOk, QObject::tr("Level has incorrect format"), E_FAIL);
	if (sLevel[0] == 'F')
		// feets
		uiResult = (double)uiResult * 30.48 + 0.5;
	else if (sLevel[0] == 'S')
		// meters
		uiResult *= 10;
	else
		TVA_THROW(QObject::tr("Level has incorrect format"), E_FAIL);

	return uiResult;
}

double CFPL::ParseSpeed(const QString& sSpeed)
{
	TVA_CHECK(sSpeed.size() > 1, QObject::tr("Speed has incorrect format"), E_FAIL);
	bool bOk(false);
	double dResult = (double)sSpeed.right(sSpeed.size()-1).toUInt(&bOk);
	TVA_CHECK(bOk, QObject::tr("Speed has incorrect format"), E_FAIL);
	if (sSpeed[0] == 'N')
		// knots
        dResult *= 1.852;
	else if (sSpeed[0] == 'M')
		// mach
        dResult *= 11.50;
	else if (sSpeed[0] != 'K')
		TVA_THROW(QObject::tr("Speed has incorrect format"), E_FAIL);

	dResult *= (double)1000/3600;

	return dResult;
}

QVariantMap CFPL::toVariantMap() const
{
    QVariantMap map;
    map.insert("id", m_ID);
    map.insert("Uid", m_Uid.toString());
    map.insert("FIRname", m_FirName);
    map.insert("Airplane", /*m_AirplanePtr*/ 0);
    map.insert("ACID", m_ACID);
    map.insert("DepAerodrome", m_DepAerodrome);
    map.insert("Speed", m_Speed);
    map.insert("SpeedS", m_SpeedS);
    map.insert("Level", m_Level);
    map.insert("Route", m_Route);
    map.insert("DestAerodrome", m_DestAerodrome);
    map.insert("Turbulence", m_Turbulence);
    map.insert("Equipment", m_Equipment);
    map.insert("FRules", m_FRules);
    map.insert("LevelS", m_LevelS);
    map.insert("TypeFlight", m_TypeFlight);
    map.insert("Num", m_Num);
    map.insert("DepTime", m_DepTime);
    map.insert("DestTime", m_DestTime);
    map.insert("Alt1Aerodrome", m_Alt1Aerodrome);
    map.insert("Alt2Aerodrome", m_Alt2Aerodrome);
    map.insert("Other", m_Other);
    map.insert("EET",m_EET);
    map.insert("AddInfo", m_AddInfo);
    map.insert("FType", m_FlightType);
    map.insert("SID", m_SID);
    map.insert("STAR", m_STAR);
    return map;
}
void CFPL::fromVariantMap(const QVariantMap& map)
{
    m_ID            = map.value("Id", m_ID).toUInt();
    m_Uid           = QUuid(map.value("Uid", m_Uid.toString()).toString());
    m_FirName       = map.value("FIRname", m_FirName).toString();
    m_ACID          = map.value("ACID", m_ACID).toString();
    m_DepAerodrome  = map.value("DepAerodrome", m_DepAerodrome).toString();
    m_Speed         = map.value("Speed", m_Speed).toDouble();
    m_SpeedS        = map.value("SpeedS", m_SpeedS).toString();
    m_Level         = map.value("Level", m_Level).toUInt();
    m_LevelS        = map.value("LevelS", m_LevelS).toString();
    m_Route         = map.value("Route", m_Route).toString();
    m_DestAerodrome = map.value("DestAerodrome", m_DestAerodrome).toString();
    m_Turbulence    = map.value("Turbulence", m_Turbulence).toString();
    m_Equipment     = map.value("Equipment", m_Equipment).toString();
    m_FRules        = map.value("FRules", m_FRules).toString();
    m_TypeFlight    = map.value("TypeFlight", m_TypeFlight).toString();
    m_Num           = map.value("Num", m_Num).toUInt();
    m_DepTime       = map.value("DepTime", m_DepTime).toDateTime();
    m_DestTime 		= map.value("DestTime", m_DestTime).toString();
    m_Alt1Aerodrome = map.value("Alt1Aerodrome", m_Alt1Aerodrome).toString();
    m_Alt2Aerodrome = map.value("Alt2Aerodrome", m_Alt2Aerodrome).toString();
    m_Other         = map.value("Other", m_Other).toString();
    m_AddInfo       = map.value("AddInfo", m_AddInfo).toString();
    m_EET           = map.value("EET", m_EET).toString();
    m_FlightType    = map.value("FType", m_FlightType).toString();
    m_SID           = map.value("SID", m_SID).toString();
    m_STAR          = map.value("STAR", m_STAR).toString();

    m_AirplanePtr->SetName(map.value("Airplane").toString());
}

bool CFPL::isRVSM() const
{
    /*
     *old
     *if(m_Equipment.contains("W"))
     *   return true;
     *else
     *   return false;
    */
    if(!m_Equipment.isEmpty())
    {
        if(m_Equipment.contains("w"))
            return false;
    }
    return true;
}
