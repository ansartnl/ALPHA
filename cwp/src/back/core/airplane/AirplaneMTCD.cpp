#include "AirplaneMTCD.h"
#include "Calculation.h"
#include <QLineF>
#include <main.h>

extern QString currentUserName;

namespace mtcd
{

MTCD::MTCD(int idPlane, double dSpeedGS, const CCoord& centerCoord)
    : mPlaneID(idPlane), mSpeedGS(dSpeedGS), mCenterCoord(centerCoord),
      mEpsilonSecs(30), mCheckHeight(true)//, mInit(false)
{
    QSettings &settings = *globalSettings();
    settings.beginGroup(currentUserName);
    settings.beginGroup("MTCD");
    mEpsilonSecs = settings.value("EpsilonSecs", 30).toInt();
    settings.setValue("EpsilonSecs", mEpsilonSecs);
    mCheckHeight = settings.value("CheckHeight", true).toBool();
    settings.setValue("CheckHeight", mCheckHeight);
    settings.endGroup();
    settings.endGroup();
}

void MTCD::AddPoint(const MTCDin& point)
{
    MTCDin newPoint(point);
    if(!newPoint.coord1.IsNull() && !newPoint.coord1.IsCenter())
        newPoint.coord1.Center(mCenterCoord);
    if(!newPoint.coord2.IsNull() && !newPoint.coord2.IsCenter())
        newPoint.coord2.Center(mCenterCoord);
    if(newPoint.coord1.IsCenter() && newPoint.coord2.IsCenter())
    {
        int secs(0);
        if(mPointList.size())
        {
            const MTCDin& prev = mPointList.last();
            secs = prev.secsCoord2;
            secs += GetDistance(prev.coord2, newPoint.coord1) / mSpeedGS;
        }
        newPoint.secsCoord1 = secs;
        secs += GetDistance(newPoint.coord1, newPoint.coord2) / mSpeedGS;
        newPoint.secsCoord2 = secs;
        mPointList.append(newPoint);
    }
//    if(point.coord1.IsCenter() && point.coord2.IsCenter())
//    {
//        if(mInit)
//        {
//            mMutex.lock();
//            mInit = false;
//            mMutex.unlock();
//        }
//        mPointList.append(point);
//    }
}

//void MTCD::Init()
//{
//    mMutex.lock();
//    if(!mInit)
//    {
//        mInit = true;
//    }
//    mMutex.unlock();
//}

bool MTCD::IsEmpty() const
{
    return mPointList.isEmpty();
}

int MTCD::GetPlaneID() const
{
    return mPlaneID;
}

double MTCD::GetSpeedGS() const
{
    return mSpeedGS;
}

const MTCD::TListPoint& MTCD::GetListPoint() const
{
    return mPointList;
//    if(mInit)
//        return mPointList;
//    else
//        return MTCD::TListPoint();
}

TListMTCDPtr MTCD::CalculateMTCD(const MTCD& other)
{
    if(GetPlaneID() == other.GetPlaneID() || mSpeedGS < 1 || other.GetSpeedGS() < 1)
        return TListMTCDPtr();

//    if(!mInit)
//        Init();

    TListMTCDPtr pListRes(new TListMTCD);
    for(TListPoint::const_iterator itMy = mPointList.constBegin(); itMy != mPointList.constEnd(); ++itMy)
    {
        for(TListPoint::const_iterator itOther = other.GetListPoint().constBegin(); itOther != other.GetListPoint().constEnd(); ++itOther)
        {
            if(CheckHeight(*itMy, *itOther))
            {
                TMTCDPtr pMTCD = GetCross(*itMy, *itOther);
                if(pMTCD && pMTCD->crossType != None)
                {
                    pMTCD->planeID1 = GetPlaneID();
                    pMTCD->planeID2 = other.GetPlaneID();
                    pMTCD->coord1 = itMy->coord1;
                    pMTCD->coord2 = itMy->coord2;
                    if(pMTCD->coordCross1.GetX() && pMTCD->coordCross1.GetY() && !pMTCD->coordCross1.IsCenter())
                    {
                        pMTCD->coordCross1.DeCenter(mCenterCoord);
                        pMTCD->coordCross1.Center(mCenterCoord);
                    }
                    if(pMTCD->coordCross2.GetX() && pMTCD->coordCross2.GetY() && !pMTCD->coordCross2.IsCenter())
                    {
                        pMTCD->coordCross2.DeCenter(mCenterCoord);
                        pMTCD->coordCross2.Center(mCenterCoord);
                    }

                    if(pMTCD->crossType == Crossing)
                    {
                        int secs1 = itMy->secsCoord1 + GetDistance(itMy->coord1, pMTCD->coordCross1) / mSpeedGS;
                        int secs2 = itOther->secsCoord1 + GetDistance(itOther->coord1, pMTCD->coordCross1) / other.GetSpeedGS();
                        if(qAbs(secs1-secs2) < mEpsilonSecs)
                        {
                            pMTCD->coordRendezvous = pMTCD->coordCross1;
                        }
                    }
                    else if(pMTCD->crossType == Following)
                    {
                        int secs11 = itMy->secsCoord1 + GetDistance(itMy->coord1, pMTCD->coordCross1) / mSpeedGS;
                        int secs12 = itMy->secsCoord1 + GetDistance(itMy->coord1, pMTCD->coordCross2) / mSpeedGS;
                        int secs21 = itOther->secsCoord1 + GetDistance(itOther->coord1, pMTCD->coordCross1) / other.GetSpeedGS();
                        int secs22 = itOther->secsCoord1 + GetDistance(itOther->coord1, pMTCD->coordCross2) / other.GetSpeedGS();
                        double v1(0), v2(0);
                        int t1(0), t2(0);
                        if(mSpeedGS < other.GetSpeedGS())
                        {
                            if((secs11 < secs21) && (secs12 > secs22))
                            {
                                v1 = mSpeedGS;
                                v2 = other.GetSpeedGS();
                                t1 = secs11;
                                t2 = secs21;
                            }
                        }
                        else if(mSpeedGS > other.GetSpeedGS())
                        {
                            if((secs21 < secs11) && (secs22 > secs12))
                            {
                                v1 = other.GetSpeedGS();
                                v2 = mSpeedGS;
                                t1 = secs21;
                                t2 = secs11;
                            }
                        }
                        if((v1 < v2) && (t1 < t2))
                        {
                            // v1 < v2, t0 = t2 - t1 > 0, S1 = v1*(t0+t) = v2*t; S1 = ( (t2-t1)*v1*v2 ) / (v2-v1)

                            double S = GetDistance(pMTCD->coordCross1, pMTCD->coordCross2);
                            double S1 = ( (t2-t1)*v1*v2 ) / (v2-v1);
                            QLineF line(pMTCD->coordCross1.GetX(), pMTCD->coordCross1.GetY(), pMTCD->coordCross2.GetX(), pMTCD->coordCross2.GetY());
                            if(S1 > S)
                            {
                                LOG_STR(WARNING, "MTCD::CalculateMTCD  S1 > S (Following)");
                            }
                            else
                            {
                                line.setLength(S1);
                                pMTCD->coordRendezvous.SetX(line.p2().x());
                                pMTCD->coordRendezvous.SetY(line.p2().y());
                            }
                        }
                    }
                    else if(pMTCD->crossType == Counter)
                    {
                        int secs11 = itMy->secsCoord1 + GetDistance(itMy->coord1, pMTCD->coordCross1) / mSpeedGS;
                        int secs12 = itMy->secsCoord1 + GetDistance(itMy->coord1, pMTCD->coordCross2) / mSpeedGS;
                        int secs21 = itOther->secsCoord1 + GetDistance(itOther->coord1, pMTCD->coordCross2) / other.GetSpeedGS();
                        int secs22 = itOther->secsCoord1 + GetDistance(itOther->coord1, pMTCD->coordCross1) / other.GetSpeedGS();
                        if(secs12 < secs21 || secs22 < secs11)
                        {

                        }
                        else
                        {
                            // S = S1 + S2, t = t1 + S1/v1 = t2 + S2/v2; S1 = ( S*v1 - (t1-t2)*v1*v2 ) / (v1+v2)

                            double S = GetDistance(pMTCD->coordCross1, pMTCD->coordCross2);
                            double S1 = (S*mSpeedGS - (secs11-secs21)*mSpeedGS*other.GetSpeedGS()) / (mSpeedGS + other.GetSpeedGS());
                            QLineF line(pMTCD->coordCross1.GetX(), pMTCD->coordCross1.GetY(), pMTCD->coordCross2.GetX(), pMTCD->coordCross2.GetY());
                            if(S1 > S)
                            {
                                LOG_STR(WARNING, "MTCD::CalculateMTCD  S1 > S (Counter)");
                            }
                            else
                            {
                                line.setLength(S1);
                                pMTCD->coordRendezvous.SetX(line.p2().x());
                                pMTCD->coordRendezvous.SetY(line.p2().y());
                            }
                        }
                    }
                    if(pMTCD->coordRendezvous.GetX() && pMTCD->coordRendezvous.GetY() && !pMTCD->coordRendezvous.IsCenter())
                    {
                        pMTCD->coordRendezvous.DeCenter(mCenterCoord);
                        pMTCD->coordRendezvous.Center(mCenterCoord);
                    }
                    if(pMTCD->coordRendezvous.IsCenter())
                        pListRes->append(pMTCD);
                }
            }
        }
    }
    return pListRes;
}

TMTCDPtr MTCD::GetCross(const MTCDin& m1, const MTCDin& m2) const
{
    TMTCDPtr pMTCD;
    QLineF l1(m1.coord1.GetX(), m1.coord1.GetY(), m1.coord2.GetX(), m1.coord2.GetY());
    QLineF l2(m2.coord1.GetX(), m2.coord1.GetY(), m2.coord2.GetX(), m2.coord2.GetY());
    int angle = qRound(l1.angleTo(l2)) % 360;
    bool bOneOrParallel = ((angle == 180) || (angle == 0)) ? true : false;
    if(bOneOrParallel)
    {
        bool bOne1 = ((m1.coord1.GetX()-m2.coord1.GetX())*(m1.coord2.GetY()-m2.coord1.GetY()) == (m1.coord2.GetX()-m2.coord1.GetX())*(m1.coord1.GetY()-m2.coord1.GetY()));
        bool bOne2 = ((m1.coord1.GetX()-m2.coord2.GetX())*(m1.coord2.GetY()-m2.coord2.GetY()) == (m1.coord2.GetX()-m2.coord2.GetX())*(m1.coord1.GetY()-m2.coord2.GetY()));
        if(bOne1 || bOne2)
        {
            if(angle == 0)
            {
                // Following
                CCoord c1, c2;
                if(((m1.coord1.GetX() == m2.coord1.GetX()) && (m1.coord1.GetY() == m2.coord1.GetY())) || CbetweenAandB(m1.coord1, m2.coord1, m2.coord2))
                    c1 = m1.coord1;
                else if(CbetweenAandB(m2.coord1, m1.coord1, m1.coord2))
                    c1 = m2.coord1;

                if(((m1.coord2.GetX() == m2.coord2.GetX()) && (m1.coord2.GetY() == m2.coord2.GetY())) || CbetweenAandB(m1.coord2, m2.coord1, m2.coord2))
                    c2 = m1.coord2;
                else if(CbetweenAandB(m2.coord2, m1.coord1, m1.coord2))
                    c2 = m2.coord2;

                if(!c1.IsNull() && !c2.IsNull())
                {
                    pMTCD = TMTCDPtr(new MTCDout);
                    pMTCD->crossType = Following;
                    pMTCD->coordCross1 = c1;
                    pMTCD->coordCross2 = c2;
                }
            }
            else if(angle == 180)
            {
                // Counter
                CCoord c1, c2;
                if(((m1.coord1.GetX() == m2.coord2.GetX()) && (m1.coord1.GetY() == m2.coord2.GetY())) || CbetweenAandB(m1.coord1, m2.coord1, m2.coord2))
                    c1 = m1.coord1;
                else if(CbetweenAandB(m2.coord2, m1.coord1, m1.coord2))
                    c1 = m2.coord2;

                if(((m1.coord2.GetX() == m2.coord1.GetX()) && (m1.coord2.GetY() == m2.coord1.GetY())) || CbetweenAandB(m1.coord2, m2.coord1, m2.coord2))
                    c2 = m1.coord2;
                else if(CbetweenAandB(m2.coord1, m1.coord1, m1.coord2))
                    c2 = m2.coord1;

                if(!c1.IsNull() && !c2.IsNull())
                {
                    pMTCD = TMTCDPtr(new MTCDout);
                    pMTCD->crossType = Counter;
                    pMTCD->coordCross1 = c1;
                    pMTCD->coordCross2 = c2;
                }
            }
        }
        else
        {
            // parallel
        }
    }
    else
    {
        // Crossing
        QPointF intersectionPoint;
        QLineF::IntersectType intersectType = l1.intersect(l2, &intersectionPoint);
        if(intersectType == QLineF::BoundedIntersection)
        {
            pMTCD = TMTCDPtr(new MTCDout);
            pMTCD->crossType = Crossing;
            pMTCD->coordCross1.SetX(intersectionPoint.x());
            pMTCD->coordCross1.SetY(intersectionPoint.y());
        }
    }
    return pMTCD;
}

bool MTCD::CheckHeight(const MTCDin& m1, const MTCDin& m2) const
{
    if(mCheckHeight)
    {
        if(m1.Hmax && m2.Hmin && (m1.Hmax < m2.Hmin-50))
            return false;
        if(m1.Hmin && m2.Hmax && (m1.Hmin > m2.Hmax+50))
            return false;
    }
    return true;
}

}
