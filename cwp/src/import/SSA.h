#ifndef SSA_H
#define SSA_H

#include "Route.h"
#include "ItemHandler.h"

#include "RouteFragmentation.h"

#include <QtCore/QMap>

#include <QtGui/QPainterPath>


//! Class describes the SSA point
class CSSAPoint
{
    public:
        enum Type
        {
            enUN = 0, enIF = 1, enTF, enCF, enDF, enCA, enCI
        };
        enum Turn
        {
            enUnknown = ' ', enLeft = 'L', enRight = 'R'
        };

        typedef QSharedPointer<CPoint> TPoint;
        CSSAPoint() :
            m_ID(0), m_SN(0), m_Type(enUN), m_bCourse(false), m_Turn(enUnknown), m_bAlt1(false), m_bSpeedLim(false),
                            m_SSAID(0), m_FlyOver(0)
        {
        }

        const quint32 &GetID() const
        {
            return m_ID;
        }
        void SetID(const quint32 &V)
        {
            m_ID = V;
        }
        const int &GetSN() const
        {
            return m_SN;
        }
        void SetSN(const int &V)
        {
            m_SN = V;
        }
        const Type &GetType() const
        {
            return m_Type;
        }
        void SetType(const Type &V)
        {
            m_Type = V;
        }
        const QString &GetName() const
        {
            return m_sName;
        }
        void SetName(const QString &V)
        {
            m_sName = V;
        }
        const TPoint &GetPoint() const
        {
            return m_Point;
        }
        void SetPoint(const TPoint &V)
        {
            m_Point = V;
        }
        const double &GetCourse() const
        {
            return m_Course;
        }
        void SetCourse(const double &V)
        {
            m_Course = V;           
            MarkCourse(true);
        }
        bool IsCourse() const
        {
            return m_bCourse;
        }
        void MarkCourse(bool bCourse)
        {
            m_bCourse = bCourse;
        }
        const Turn &GetTurn() const
        {
            return m_Turn;
        }
        void SetTurn(const Turn &V)
        {
            m_Turn = V;
        }
        const double &GetAlt1() const
        {
            return m_Alt1;
        }
        void SetAlt1(const double &V)
        {
            m_Alt1 = V;
            MarkAlt1(true);
        }
        bool IsAlt1() const
        {
            return m_bAlt1;
        }
        void MarkAlt1(bool bAlt1)
        {
            m_bAlt1 = bAlt1;
        }
        const double &GetSpeedLim() const
        {
            return m_SpeedLim;
        }
        void SetSpeedLim(const double &V)
        {
            m_SpeedLim = V;
            MarkSpeedLim(true);
        }
        bool IsSpeedLim() const
        {
            return m_bSpeedLim;
        }
        void MarkSpeedLim(bool bSpeedLim)
        {
            m_bSpeedLim = bSpeedLim;
        }
        const quint32 &GetSSAID() const
        {
            return m_SSAID;
        }
        void SetSSAID(const quint32 &V)
        {
            m_SSAID = V;
        }
        void SetType(const QString& sType);
        void SetTurn(const char cTurn);
        const int &GetFlyOver() const
        {
            return m_FlyOver;
        }
        void SetFlyOver(const int &V)
        {
            m_FlyOver = V;
        }

    private:
        quint32 m_ID;
        int m_SN;
        Type m_Type;
        QString m_sName;
        TPoint m_Point;
        double m_Course;
        bool m_bCourse;
        Turn m_Turn;
        double m_Alt1;
        bool m_bAlt1;
        double m_SpeedLim;
        bool m_bSpeedLim;
        quint32 m_SSAID;
        int m_FlyOver;
};

//! Class describes the SSA
class CSSA: public ItemHandler
{
    //static const int TURN_MINRADIUS = 3800;
    static const int TURN_MINRADIUS = 2000;
    public:
        //! Interface to CSSA
        class IParent
        {
            public:
                virtual ~IParent()
                {
                }
                ;
                virtual const CCoord& GetPlace() const = 0;
                virtual double GetDirection() const = 0;
                virtual double GetRunwayAlt() const = 0;
                virtual double GetRunwayLength() const = 0;
                virtual const QString& GetRunwayName() const = 0;
                virtual double GetDeclination() const = 0;
        };
        typedef QSharedPointer<CSSAPoint> TPointPtr;
        typedef QMap<int, TPointPtr> TPoints;
        enum Type
        {
            enUnknown    = 0,
            enSID        = 1,
            enSTAR       = 2,
            enApproach   = 3,
            enVA         = 4,
            enMA         = 5
        };

        CSSA() :
            m_ID(0), m_Type(enUnknown), m_bTransitAlt(false), m_RunwayID(0), m_Enable(true), m_pParent(NULL)
        {
        }
        virtual ~CSSA()
        {
        }

        const quint32 &GetID() const
        {
            return m_ID;
        }
        void SetID(const quint32 &V)
        {
            m_ID = V;
        }
        const Type &GetType() const
        {
            return m_Type;
        }
        void SetType(const Type &V)
        {
            m_Type = V;
        }
        const QString &GetName() const
        {
            return m_Name;
        }
        void SetName(const QString &V)
        {
            m_Name = V;
        }
        const double &GetTransitAlt() const
        {
            return m_TransitAlt;
        }
        void SetTransitAlt(const double &V)
        {
            m_TransitAlt = V;
            MarkTransitAlt(true);
        }
        bool IsTransitAlt() const
        {
            return m_bTransitAlt;
        }
        void MarkTransitAlt(bool bTransitAlt)
        {
            m_bTransitAlt = bTransitAlt;
        }
        const quint32 &GetRunwayID() const
        {
            return m_RunwayID;
        }
        void SetRunwayID(const quint32 &V)
        {
            m_RunwayID = V;
        }
        const TPoints &GetPoints() const
        {
            return m_Points;
        }
        void SetPoints(const TPoints &V)
        {
            m_Points = V;
        }
        const bool &GetEnable() const
        {
            return m_Enable;
        }
        void SetEnable(const bool &V)
        {
            m_Enable = V;
        }
        void SetParent(IParent* pParent)
        {
            m_pParent = pParent;
        }
        IParent* GetParent() const
        {
            return m_pParent;
        }
        void AddPoint(const TPointPtr& pPoint);
        TPoints& GetPoints()
        {
            return m_Points;
        }

        void CalculatePath(qint64 qllXMin, qint64 qllYMax, double dScale, bool bRoute, bool bAddRunwayPoint = true);
        QPainterPath& GetPath()
        {
            return m_Path;
        }
        const QPainterPath& GetPath() const
        {
            return m_Path;
        }

        QSharedPointer<CProcFrag> GetRoute()
        {
            return m_pRoute;
        }

        void _CalcTF(const TPointPtr& pPoint, const QPointF& pnt);
        void _CalcCA(const TPointPtr& pPoint, const QPointF& pnt);
        void _CalcCF(const TPointPtr& pPoint, const QPointF& pnt);
        void _CalcDF(const TPointPtr& pPoint, const QPointF& pnt);
        void _CalcCI(const TPointPtr& pPoint, const TPointPtr& pPointNext, const QPointF& pnt);
        void _CalcAzimuth(const TPointPtr& pPoint);
        void _AddArc(const QRectF& rectangle, qreal startAngle, qreal sweepLength, const TPointPtr& pPoint);
        void _AddLine(const QPointF& pnt, const TPointPtr& pPoint, bool bInclude);
        void _AddPoint(const QPointF& pnt, const TPointPtr& pPoint, bool bInclude);

        QPainterPath m_Path;
        QSharedPointer<CProcFrag> m_pRoute;
        double m_dAlt;
        double m_dScale;
        int m_iSN;
        bool m_bRoute;

    private:
        quint32 m_ID;
        Type m_Type;
        QString m_Name;
        double m_TransitAlt;
        bool m_bTransitAlt;
        quint32 m_RunwayID;
        TPoints m_Points;
        bool m_Enable;
        IParent* m_pParent;
};

//! Class describes the SSA key
class CSSAKey
{
    public:
        CSSAKey(const QString& sName, CSSA::Type Type) :
            m_sName(sName), m_Type(Type)
        {
        }

        bool operator<(const CSSAKey& ref2nd) const
        {
            if (!m_Type || m_sName != ref2nd.m_sName)
                return m_sName < ref2nd.m_sName;
            return m_Type < ref2nd.m_Type;
        }

    private:
        QString m_sName;
        CSSA::Type m_Type;
};
#endif // SSA_H
