#include "RunwayWidgetData.h"
#include <QUuid>
#include <QMetaType>

int metatype_id_RunwayWidgetDataFIR = qRegisterMetaType<RunwayWidgetDataFIR>("RunwayWidgetDataFIR");
int metatype_id_RunwayWidgetDataVPP = qRegisterMetaType<RunwayWidgetDataVPP>("RunwayWidgetDataVPP");

// RunwayWidgetData
RunwayWidgetData::RunwayWidgetData()
    : mState(0), mILS(0), mNDB(0)
{

}

QString RunwayWidgetData::name() const
{
    return mName;
}
void RunwayWidgetData::setName(const QString& s)
{
    mName = s;
}

int RunwayWidgetData::state() const
{
    return mState;
}
void RunwayWidgetData::setState(int s)
{
    mState = s;
}

int RunwayWidgetData::ILS() const
{
    return mILS;
}
void RunwayWidgetData::setILS(int s)
{
    mILS = s;
}

int RunwayWidgetData::NDB() const
{
    return mNDB;
}
void RunwayWidgetData::setNDB(int s)
{
    mNDB = s;
}

// RunwayWidgetDataFIR
RunwayWidgetDataFIR::RunwayWidgetDataFIR()
    : mFIR("???")
{

}

RunwayWidgetDataFIR::RunwayWidgetDataFIR(const QString& sFIR)
    : mFIR(sFIR)
{

}

QString RunwayWidgetDataFIR::FIR() const
{
    return mFIR;
}
void RunwayWidgetDataFIR::setFIR(const QString& s)
{
    mFIR = s;
}

QString RunwayWidgetDataFIR::currentRunway() const
{
    return mCurrentRunway;
}
void RunwayWidgetDataFIR::setCurrentRunway(const QString& s)
{
    mCurrentRunway = s;
}

bool RunwayWidgetDataFIR::isValidRunway() const
{
    return mRunways.contains(mCurrentRunway);
}
RunwayWidgetData& RunwayWidgetDataFIR::runway(const QString& runwayName)
{
    if(mRunways.contains(runwayName))
        return mRunways[runwayName];
    else
        return mEmptyRunway;
}

void RunwayWidgetDataFIR::addRunway(const RunwayWidgetData& s)
{
    mRunways.insert(s.name(), s);
}

RunwayWidgetDataFIR::TRunways& RunwayWidgetDataFIR::runwaysList()
{
    return mRunways;
}

bool RunwayWidgetDataFIR::Serv1() const
{
    return mServ1;
}
void RunwayWidgetDataFIR::setServ1(bool ch)
{
    mServ1 = ch;
}
bool RunwayWidgetDataFIR::Vehicle() const
{
    return mVehicle;
}
void RunwayWidgetDataFIR::setVehicle(bool ch)
{
    mVehicle = ch;
}
bool RunwayWidgetDataFIR::Serv2() const
{
    return mServ2;
}
void RunwayWidgetDataFIR::setServ2(bool ch)
{
    mServ2 = ch;
}
bool RunwayWidgetDataFIR::Other() const
{
    return mOther;
}
void RunwayWidgetDataFIR::setOther(bool ch)
{
    mOther = ch;
}

QVariantMap RunwayWidgetData::toVariantMap() const
{
    QVariantMap map;
    map.insert("NAME", mName);
    map.insert("STATE", mState);
    map.insert("ILS", mILS);
    map.insert("NDB", mNDB);
    return map;
}

void RunwayWidgetData::fromVariantMap(const QVariantMap& map)
{
    mName = map.value("NAME", mName).toString();
    mState = map.value("STATE", mState).toInt();
    mILS = map.value("ILS", mILS).toInt();
    mNDB = map.value("NDB", mNDB).toInt();
}

QVariantMap RunwayWidgetDataFIR::toVariantMap() const
{
    QVariantMap map;
    map.insert("FIR", mFIR);
    map.insert("CUR_RUNWAY", mCurrentRunway);
    map.insert("RUNWAYS_COUNT", mRunways.size());
    int i = 0;
    for(TRunways::const_iterator itRunway = mRunways.constBegin(); itRunway != mRunways.constEnd(); ++itRunway, ++i)
    {
        QString sKey = QString("RUNWAY%1").arg(i);
        map.insert(sKey, itRunway->toVariantMap());
    }
    map.insert("SERV1", mServ1);
    map.insert("VEHICLE", mVehicle);
    map.insert("SERV2", mServ2);
    map.insert("OTHER", mOther);
    return map;
}

void RunwayWidgetDataFIR::fromVariantMap(const QVariantMap& map)
{
    mFIR = map.value("FIR", mFIR).toString();
    mCurrentRunway = map.value("CUR_RUNWAY", mCurrentRunway).toString();
    mRunways.clear();
    int iCount = map.value("RUNWAYS_COUNT", 0).toInt();
    for(int i = 0; i < iCount; ++i)
    {
        QString sKey = QString("RUNWAY%1").arg(i);
        QVariantMap rwMap = map.value(sKey).toMap();
        RunwayWidgetData rw;
        rw.fromVariantMap(rwMap);
        addRunway(rw);
    }
    mServ1 = map.value("SERV1", mServ1).toBool();
    mVehicle = map.value("VEHICLE", mVehicle).toBool();
    mServ2 = map.value("SERV2", mServ2).toBool();
    mOther = map.value("OTHER", mOther).toBool();
}

RunwayWidgetDataVPP::RunwayWidgetDataVPP()
    : mFIR("???"), mQNH(0), mTL(0), mUseMeteo(false)
{

}

RunwayWidgetDataVPP::RunwayWidgetDataVPP(const QString& sFIR)
    : mFIR(sFIR), mQNH(0), mTL(0), mUseMeteo(false)
{

}

QString RunwayWidgetDataVPP::FIR() const
{
    return mFIR;
}
void RunwayWidgetDataVPP::setFIR(const QString& s)
{
    mFIR = s;
}

int RunwayWidgetDataVPP::QNH() const
{
    return mQNH;
}
void RunwayWidgetDataVPP::setQNH(int s)
{
    mQNH = s;
}

int RunwayWidgetDataVPP::TL() const
{
    return mTL;
}
void RunwayWidgetDataVPP::setTL(int s)
{
    mTL = s;
}

QString RunwayWidgetDataVPP::ATIS() const
{
    return mATIS;
}
void RunwayWidgetDataVPP::setATIS(const QString& s)
{
    mATIS = s;
}

bool RunwayWidgetDataVPP::UseMeteo() const
{
    return mUseMeteo;
}
void RunwayWidgetDataVPP::setUseMeteo(bool ch)
{
    mUseMeteo = ch;
}

QVariantMap RunwayWidgetDataVPP::toVariantMap() const
{
    QVariantMap map;
    map.insert("FIR", mFIR);
    map.insert("QNH", mQNH);
    map.insert("TL", mTL);
    map.insert("ATIS", mATIS);
    map.insert("USE_METEO", mUseMeteo);
    return map;
}

void RunwayWidgetDataVPP::fromVariantMap(const QVariantMap& map)
{
    mFIR = map.value("FIR", mFIR).toString();
    mQNH = map.value("QNH", mQNH).toInt();
    mTL = map.value("TL", mTL).toInt();
    mATIS = map.value("ATIS", mATIS).toString();
    mUseMeteo = map.value("USE_METEO", mUseMeteo).toBool();
}

