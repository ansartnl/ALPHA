#ifndef AIRPLANEMTCD_H
#define AIRPLANEMTCD_H

#include <QtCore/QLinkedList>
#include <QtCore/QSharedPointer>
//#include <QtCore/QMutex>
#include "Coordinate.h"

//! Namespace contains the airplane's MTCD
namespace mtcd
{
    enum CrossType { None = 0, Crossing, Following, Counter };

    struct MTCDin
    {
        CCoord  coord1;
        CCoord  coord2;
        int     Hmin;
        int     Hmax;
        int     secsCoord1;
        int     secsCoord2;

        MTCDin() : Hmin(0), Hmax(0),
            secsCoord1(0), secsCoord2(0) {}
    };

    struct MTCDout
    {
        CCoord      coord1;
        CCoord      coord2;
        CCoord      coordCross1;
        CCoord      coordCross2;
        CrossType   crossType;
        CCoord      coordRendezvous;
        int         planeID1;
        int         planeID2;

        MTCDout() : crossType(None), planeID1(0), planeID2(0) {}
    };
    typedef QSharedPointer<MTCDout> TMTCDPtr;
    typedef QLinkedList<TMTCDPtr> TListMTCD;
    typedef QSharedPointer<TListMTCD> TListMTCDPtr;

    class MTCD
    {
    public:
        typedef QLinkedList<MTCDin> TListPoint;

        MTCD(int idPlane, double dSpeedGS, const CCoord& centerCoord);
//        void Init();

        int GetPlaneID() const;
        double GetSpeedGS() const;
        const TListPoint& GetListPoint() const;

        void AddPoint(const MTCDin& point);
        bool IsEmpty() const;
        TListMTCDPtr CalculateMTCD(const MTCD& other);

    private:
        TMTCDPtr GetCross(const MTCDin& m1, const MTCDin& m2) const;
        bool CheckHeight(const MTCDin& m1, const MTCDin& m2) const;

    private:
        int         mPlaneID;
        double      mSpeedGS;
        CCoord      mCenterCoord;
        TListPoint  mPointList;
        int         mEpsilonSecs;
        bool        mCheckHeight;
//        QMutex      mMutex;
//        bool        mInit;
    };
}

#endif // AIRPLANEMTCD_H
