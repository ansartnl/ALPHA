#ifndef AIRPLANEDATA_H
#define AIRPLANEDATA_H

//#include <QColor>

#include "Reminder.h"
#include "Holding.h"
#include "back/common/FLVal.h"

//! Class contains the data airplane
class CAirplaneData
{
    public:
        typedef QMap<quint32, QSharedPointer<CReminder> > TReminderList;
        typedef QMap<CReminderKey, QSharedPointer<CReminder> > TReminderMap;

        CAirplaneData() :
            m_dFL(0), m_dFLOld(0), m_ID(0), m_SPI(0), m_IAS(0), m_Bank(0)
          , m_TrackNumberUvd(0), m_bCustomPoint(false)
          , m_bHolding(false), m_PlaneType(None), m_PlaneTypeBeforeTransfer(None)
          , m_HeightQNH(0), m_HeightQNHOld(0), m_bHeightQNH(false), m_Azimuth2(361)
          , hdg_(0), vrc_(0), spd_(0), spd_mach_(0), gsp_(0.), tsp_(0), cfl_(0), sfl_(0), dbl_(false), m_isUvd(false)
          , m_BPS(0), m_bBPS(false)

        {
        }
        virtual ~CAirplaneData()
        {
        }

        //! Set airplane ACID
        /*!
         \param ushMode3AReply a ACID
         */
        void SetMode3AReply(quint32 ushMode3AReply) { m_ushMode3AReply = ushMode3AReply; }
        //! Get airplane ACID
        /*!
         \return ACID
         */
        quint32 GetMode3AReply() const { return m_ushMode3AReply; }

        //! Set airplane coordinate
        /*!
         \param Coord a reference on CCoord ( airplane coordinate )
         */
        void SetCoord(const CCoord& Coord) { m_Coord = Coord; }
        //! Get airplane coordinate
        /*!
         \return reference on CCoord ( airplane coordinate )
         */
        const CCoord& GetCoord() const { return m_Coord; }
        //! Get airplane coordinate
        /*!
         \return reference on CCoord ( airplane coordinate )
         */
        CCoord& GetCoord() { return m_Coord; }

        //! Set airplane flight level
        /*!
         \param dFL a flight level in hundreds of feet
         */
        virtual void SetFL(double dFL) { m_dFLOldOld = m_dFLOld; m_dFLOld = m_dFL; m_dFL = dFL; }
        //! Get airplane flight level
        /*!
         \return flight level in hundreds of feet
         */
        double GetFL() const { return m_dFL; }
        double GetFLOld() const { return m_dFLOld; }
        double GetFLOldOld() const { return m_dFLOldOld; }
        //! Set airplane horizontal speed m/s
        /*!
         \param Vel a reference on CVelocity
         */
        void SetVelocity(const CVelocity& Vel) { m_Vel = Vel; }
        //! Get airplane horizontal speed m/s
        /*!
         \return reference on CVelocity
         */
        const CVelocity& GetVelocity() const { return m_Vel; }

        //! Set airplane vertical speed feet/min
        /*!
         \param dVz a speed
         */
        void SetVz(double dVz) { m_dVz = dVz; }
        //! Get airplane vertical speed feet/min
        /*!
         \return speed
         */
        double GetVz() const { return m_dVz; }

        const double &GetHeightQNH() const { return m_HeightQNH; }
        const double &GetHeightQNHOld() const { return m_HeightQNHOld; }
        void SetHeightQNH(const double &V) { m_HeightQNHOld = m_HeightQNH; m_HeightQNH = V; MarkHeightQNH(true); }
        bool IsHeightQNH() const { return m_bHeightQNH; }
        void MarkHeightQNH(bool bHeightQNH) { m_bHeightQNH = bHeightQNH; }

        //! Airplane reminders
        /*! \return a reference on TReminderList ( list of reminders ) */
        TReminderList& GetReminders() { return m_Reminder; }
        TReminderMap& GetTempReminders() { return m_TempReminder; }

        //! Airplane primary key ( in DB )
        const quint16 &GetID() const { return m_ID; }
        void SetID(const quint16 &V) { m_ID = V; }

        quint8 GetSPI() const { return m_SPI; }
        void SetSPI(quint8 spi) { m_SPI = spi; }

        const FLVal &GetCFL() const { return m_CFL; }
        void SetCFL(const FLVal &V) { m_CFL = V; }

        const FLVal &GetTFL() const { return m_TFL; }
        void SetTFL(const FLVal &V) { m_TFL = V; }

        const FLVal &GetSFL() const { return m_SFL; }
        void SetSFL(const FLVal &V) { m_SFL = V; }

        const double &GetIAS() const { return m_IAS; }
        void SetIAS(const double &V) { m_IAS = V; }

        const double &GetBank() const { return m_Bank; }
        void SetBank(const double &V) { m_Bank = V; }

        const QString &GetOperName() const { return m_OperName; }
        void SetOperName(const QString &V) { m_OperName = V; }

        const QString &GetACID() const { return m_ACID; }
        void SetACID(const QString &V) { m_ACID = V; }

        const int &GetPlaneAzimuth2() const { return m_Azimuth2; }
        void SetPlaneAzimuth2(int V) { m_Azimuth2 = V; }


        double heading() const { return hdg_; }
        void setHeading(double hdg) { hdg_ = hdg; }

        double verticalRate() const { return vrc_; }
        void setVerticalRate(double vrc) { vrc_ = vrc; }

        int trueAirspeed() const { return tsp_; }
        void setTrueAirspeed(int tsp) { tsp_ = tsp; }

        double groundAirspeed() const { return gsp_; }
        void setGroundAirspeed(double gsp) { gsp_ = gsp; }

        int indicatedAirspeed() const { return spd_; }
        void setIndicatedAirspeed(int spd) { spd_ = spd; }

        double indicatedAirspeedMach() const { return spd_mach_; }
        void setIndicatedAirspeedMach(double spd) { spd_mach_ = spd; }

        double cfl() const { return cfl_; }
        void setCfl(double cfl) { cfl_ = cfl; }

        double sfl() const { return sfl_; }
        void setSfl(double sfl) { sfl_ = sfl; }

        bool isUvd() const { return m_isUvd; }
        void setIsUvd(bool is) { m_isUvd = is; }

        void setLastEmg(const QDateTime &emg) { mLastEmg = emg; }
        const QDateTime lastEmg() const { return mLastEmg; }

        bool dbl() const { return dbl_; }
        void setDbl(bool dbl) { dbl_ = dbl; }

        //QColor dblColor() const { return dbl_color_; }
        //void setDblColor(const QColor &dbl_color) { dbl_color_ = dbl_color; }


        const QSharedPointer<CPoint> &GetCustomPoint() const { return m_CustomPoint; }
        void SetCustomPoint(const QSharedPointer<CPoint> &V) { m_CustomPoint = V; MarkCustomPoint(true); }
        bool IsCustomPoint() const { return m_bCustomPoint; }
        void MarkCustomPoint(bool bCustomPoint) { m_bCustomPoint = bCustomPoint; }

        const QSharedPointer<CHolding> &GetHolding() const { return m_Holding; }
        void SetHolding(const QSharedPointer<CHolding> &V) { m_Holding = V; MarkHolding(true); }
        bool IsHolding() const { return m_bHolding; }
        void MarkHolding(bool bHolding) { m_bHolding = bHolding; }

        const QString &GetTransition() const { return m_Transition; }
        void SetTransition(const QString &V) { m_Transition = V; }

        const quint32 &GetTrackNumberUvd() const { return m_TrackNumberUvd; }
        void SetTrackNumberUvd(const quint32 V) { m_TrackNumberUvd = V; }

        enum PlaneType
        {
            None = 0        /*!< without SFPL */
            , ACT           /*!< in */
            , ASSUME        /*!< our */
            , Transfer      /*!< transfer in process */
            , Transfered    /*!< transfered */
            , ActOutInFir   /*!< transfered out, currently in fir*/
            , ActOut        /*!< transfered out*/
        };
        PlaneType GetPlaneType() const { return m_PlaneType; }
        void SetPlaneType(PlaneType plane_type)
        {
            m_PlaneType = plane_type;
            if (plane_type != Transfer)
                m_PlaneTypeBeforeTransfer = plane_type;
        }
        const PlaneType &GetPlaneTypeBeforeTransfer() const { return m_PlaneTypeBeforeTransfer; }

        const QDateTime &GetACAS() const { return m_ACAS; }
        void SetACAS(const QDateTime &dt = QDateTime::currentDateTime()) { if (m_ACAS.isNull()) m_ACAS = dt; }
        bool IsACAS() const { return !m_ACAS.isNull(); }

        const double &GetBPS() const { return m_BPS; }
        void SetBPS(const double &V) { m_BPS = V; MarkBPS(true); }
        bool IsBPS() const { return m_bBPS; }
        void MarkBPS(bool bBPS) { m_bBPS = bBPS; }

    protected:
        /*! ACID */
        quint32 m_ushMode3AReply;
        /*! Airplane coordinate */
        CCoord m_Coord;
        /*! Flight level, hundreds of feet */
        double m_dFL;
        double m_dFLOld;
        double m_dFLOldOld;
        /*! X & Y Velocity, meter/sec */
        CVelocity m_Vel;
        /*! Z Climb/Descend, feet/min. */
        double m_dVz;
        /*! List of airplane reminders */
        TReminderList m_Reminder;
        TReminderMap m_TempReminder;

    private:
        quint16     m_ID;
        quint8      m_SPI;
        FLVal       m_CFL;
        FLVal       m_SFL; //height from airpalane
        FLVal       m_TFL;
        double      m_IAS;
        double      m_Bank;
        quint32     m_TrackNumberUvd;
        QString     m_ACID;
        QString     m_OperName;
        QSharedPointer<CPoint> m_CustomPoint;
        bool        m_bCustomPoint;
        QSharedPointer<CHolding> m_Holding;
        bool        m_bHolding;
        QString     m_Transition;
        PlaneType   m_PlaneType;
        PlaneType   m_PlaneTypeBeforeTransfer;
        double      m_HeightQNH;
        double      m_HeightQNHOld;
        bool        m_bHeightQNH;

        int         m_Azimuth2;

        double hdg_;
        double vrc_;
        int spd_;
        double spd_mach_;
        int tsp_;//true air speed
        double gsp_;
        double cfl_;
        double sfl_;

        bool dbl_;
        //QColor dbl_color_;

        bool m_isUvd;

        QDateTime mLastEmg;

        QDateTime m_ACAS;
        double    m_BPS;
        bool      m_bBPS;
};

#endif // AIRPLANEDATA_H
