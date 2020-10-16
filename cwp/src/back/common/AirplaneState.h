#ifndef AIRPLANESTATE_H
#define AIRPLANESTATE_H

#include "Coordinate.h"
#include <QtCore/QTime>
#include "AirplaneCollision.h"
#include "BearingData.h"

//! Class contains a state airplane
class CAirplaneState
{
    public:
        CAirplaneState() :
            m_SAC(0), m_SIC(0), m_SPI(0), m_TrackNumber(0), m_TrackNumberUvd(0),
            m_Mode3AReply(0), m_bMode3AReply(false), m_bTime(false),
            m_bCoordinate(false), m_FlightLevel(0.), m_bFlightLevel(false), m_bVelocity(false),
            m_ZVelocity(0.), m_bZVelocity(false),
            m_FuelReserve(0), m_bFuelReserve(false),
            m_VFI(Emergency), m_bVFI(false),
            m_FplStatus(0), m_bFplStatus(false),
            m_HeightQNH(0), m_bHeightQNH(false),
            m_TrackStatus(NoStatus), m_bTrackStatus(false),
            m_BPS(0), m_bBPS(false),
            hdg_(0), vrc_(0), spd_(0), spd_mach_(0), gsp_(0.), cfl_(0), sfl_(0), m_isUvd(false), m_bACAS(false)
        {
        }

        //! Vehicle Fleet Identification
        enum VFI
        {
            Emergency   = 7,
            Fuel        = 12
        };

        Q_DECLARE_FLAGS(VFIs, VFI)

        enum TrackStatus
        {
            NoStatus = 0,
            BeginTrack = 1,
            EndTrack = 2
        };

        static const quint16 kClearAllTracks = 0xFFFF;

        quint8 GetSAC() const { return m_SAC; }
        void SetSAC(quint8 V) { m_SAC = V; }

        quint8 GetSIC() const { return m_SIC; }
        void SetSIC(quint8 V) { m_SIC = V; }

        quint8 GetSPI() const { return m_SPI; }
        void SetSPI(quint8 V) { m_SPI = V; }

        quint16 GetTrackNumber() const { return m_TrackNumber; }
        void SetTrackNumber(quint16 V) { m_TrackNumber = V; }

        quint32 GetTrackNumberUvd() const { return m_TrackNumberUvd; }
        void SetTrackNumberUvd(quint32 V) { m_TrackNumberUvd = V; }

        quint32 GetMode3AReply() const { return m_Mode3AReply; }
        void SetMode3AReply(quint32 V) { m_Mode3AReply = V; MarkMode3AReply(true); }
        bool IsMode3AReply() const { return m_bMode3AReply; }
        void MarkMode3AReply(bool bMode3AReply) { m_bMode3AReply = bMode3AReply; }

        const QTime &GetTime() const { return m_Time; }
        void SetTime(const QTime &V) { m_Time = V; MarkTime(true); }
        bool IsTime() const { return m_bTime; }
        void MarkTime(bool bTime) { m_bTime = bTime; }

        const CCoord &GetCoordinate() const { return m_Coordinate; }
        void SetCoordinate(const CCoord &V) { m_Coordinate = V; MarkCoordinate(true); }
        bool IsCoordinate() const { return m_bCoordinate; }
        void MarkCoordinate(bool bCoordinate) { m_bCoordinate = bCoordinate; }

        const double GetFlightLevel() const { return m_FlightLevel; }
        void SetFlightLevel(const double &V) { m_FlightLevel = V; MarkFlightLevel(true); }
        bool IsFlightLevel() const { return m_bFlightLevel; }
        void MarkFlightLevel(bool bFlightLevel) { m_bFlightLevel = bFlightLevel; }

        const CVelocity &GetVelocity() const { return m_Velocity; }
        void SetVelocity(const CVelocity &V) { m_Velocity = V; MarkVelocity(true); }
        bool IsVelocity() const { return m_bVelocity; }
        void MarkVelocity(bool bVelocity) { m_bVelocity = bVelocity; }

        const double &GetZVelocity() const { return m_ZVelocity; }
        void SetZVelocity(const double &V) { m_ZVelocity = V; MarkZVelocity(true); }
        bool IsZVelocity() const { return m_bZVelocity; }
        void MarkZVelocity(bool bZVelocity) { m_bZVelocity = bZVelocity; }

        const quint8 &GetFuelReserve() const { return m_FuelReserve; }
        void SetFuelReserve(const quint8 &V) { m_FuelReserve = V; MarkFuelReserve(true); }
        bool IsFuelReserve() const { return m_bFuelReserve; }
        void MarkFuelReserve(bool bFuelReserve) { m_bFuelReserve = bFuelReserve; }

        const VFIs &GetVFI() const { return m_VFI; }
        void SetVFI(const VFIs &V) { m_VFI = V; MarkVFI(true); }
        bool IsVFI() const { return m_bVFI; }
        void MarkVFI(bool bVFI) { m_bVFI = bVFI; }

        const quint8 &GetFplStatus() const { return m_FplStatus; }
        void SetFplStatus(const quint8 &V) { m_FplStatus = V; MarkFplStatus(true); }
        bool IsFplStatus() const { return m_bFuelReserve; }
        void MarkFplStatus(bool bFplStatus) { m_bFplStatus = bFplStatus; }

        const double &GetHeightQNH() const { return m_HeightQNH; }
        void SetHeightQNH(const double &V) { m_HeightQNH = V; MarkHeightQNH(true); }
        bool IsHeightQNH() const { return m_bHeightQNH; }
        void MarkHeightQNH(bool bHeightQNH) { m_bHeightQNH = bHeightQNH; }

        const int &GetTrackStatus() const { return m_TrackStatus; }
        void SetTrackStatus(const int &V) { m_TrackStatus = V; MarkTrackStatus(true); }
        bool IsTrackStatus() const { return m_bTrackStatus; }
        void MarkTrackStatus(bool bTrackStatus) { m_bTrackStatus = bTrackStatus; }

        const double &GetBPS() const { return m_BPS; }
        void SetBPS(const double &V) { m_BPS = V; MarkBPS(true); }
        bool IsBPS() const { return m_bBPS; }
        void MarkBPS(bool bBPS) { m_bBPS = bBPS; }

        const QString& GetCallsign() const { return m_Callsign; }
        void SetCallsign(const QString& callsign) { m_Callsign = callsign; }

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

        void setEmg() { mLastEmg = QDateTime::currentDateTimeUtc(); }
        const QDateTime lastEmg() const { return mLastEmg; }

        bool IsACAS() const { return m_bACAS; }
        void SetACAS(bool bACAS) { m_bACAS = bACAS; }

    private:
        quint8 m_SAC;
        quint8 m_SIC;
        quint8 m_SPI;
        quint16 m_TrackNumber;
        quint32 m_TrackNumberUvd;
        quint32 m_Mode3AReply;
        bool m_bMode3AReply;
        QTime m_Time;
        bool m_bTime;
        CCoord m_Coordinate;
        bool m_bCoordinate;
        double m_FlightLevel;
        bool m_bFlightLevel;
        CVelocity m_Velocity;
        bool m_bVelocity;
        double m_ZVelocity;
        bool m_bZVelocity;
        quint8 m_FuelReserve;
        bool m_bFuelReserve;
        VFIs m_VFI;
        bool m_bVFI;
        quint8 m_FplStatus;
        bool m_bFplStatus;
        double m_HeightQNH;
        bool m_bHeightQNH;
        int m_TrackStatus;
        bool m_bTrackStatus;
        double m_BPS;
        bool m_bBPS;
        QString m_Callsign;
        //!!! Track status I062/080

        double hdg_;
        double vrc_;
        int spd_;
        double spd_mach_;
        double gsp_;
        int tsp_;
        double cfl_;
        double sfl_;

        bool m_isUvd;

        QDateTime mLastEmg;
        bool m_bACAS;
};

//! Interface for asterix data source
class IAsterixDataSrc
{
    public:
        virtual ~IAsterixDataSrc()
        {
        }
        ;

        //! Start service
        virtual void Start() = 0;
        //! Stop service
        virtual void Stop() = 0;
        //! Block receiving of messages.
        virtual void BlockMessages(bool on) = 0;
};

namespace asterix {
    //! Interface to data handler
    class IParent
    {
        public:
            //! Destructor
            virtual ~IParent()
            {
            }
            ;
            //! Event of asterix data coming
            /*!
             \param Data received data from asterix
             */
            virtual void OnAsterixData(CAirplaneState& Data) = 0;
            //! Event of bearing data coming
            /*!
             \param Data received data from bearing
             */
            virtual void OnBearingData(CBearingData& Data) = 0;
            //! Event of STCA data coming
            /*!
             \param Data received data from STCA
             */
            virtual void OnSTCAData(const QVariant &v) = 0;
    };
}

#endif // AIRPLANESTATE_H
