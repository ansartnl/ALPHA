#ifndef ASTERIX020_PROTO_H
#define ASTERIX020_PROTO_H

#include "asterixexport.h"
#include "asterixerrors.h"
#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>

class AsterixData;

class ASTERIX_EXPORT Asterix020Proto : public Asterix
{
protected:
    void parse_010(QDataStream &stream, AsterixData &result);
    void parse_020(QDataStream &stream, AsterixData &result);
    void parse_030(QDataStream &stream, AsterixData &result);
    void parse_041(QDataStream &stream, AsterixData &result);
    void parse_042(QDataStream &stream, AsterixData &result);
    void parse_050(QDataStream &stream, AsterixData &result);
    void parse_055(QDataStream &stream, AsterixData &result);
    void parse_070(QDataStream &stream, AsterixData &result);
    void parse_090(QDataStream &stream, AsterixData &result);
    void parse_100(QDataStream &stream, AsterixData &result);
    void parse_105(QDataStream &stream, AsterixData &result);
    void parse_110(QDataStream &stream, AsterixData &result);
    void parse_140(QDataStream &stream, AsterixData &result);
    void parse_161(QDataStream &stream, AsterixData &result);
    void parse_170(QDataStream &stream, AsterixData &result);
    void parse_202(QDataStream &stream, AsterixData &result);
    void parse_210(QDataStream &stream, AsterixData &result);
    void parse_220(QDataStream &stream, AsterixData &result);
    void parse_230(QDataStream &stream, AsterixData &result);
    void parse_245(QDataStream &stream, AsterixData &result);
    void parse_250(QDataStream &stream, AsterixData &result);
    void parse_260(QDataStream &stream, AsterixData &result);
    void parse_300(QDataStream &stream, AsterixData &result);
    void parse_310(QDataStream &stream, AsterixData &result);
    void parse_400(QDataStream &stream, AsterixData &result);
    void parse_500(QDataStream &stream, AsterixData &result);

    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_020(AsterixData &map, QDataStream &stream);
    bool generate_030(AsterixData &map, QDataStream &stream);
    bool generate_041(AsterixData &map, QDataStream &stream);
    bool generate_042(AsterixData &map, QDataStream &stream);
    bool generate_050(AsterixData &map, QDataStream &stream);
    bool generate_055(AsterixData &map, QDataStream &stream);
    bool generate_070(AsterixData &map, QDataStream &stream);
    bool generate_090(AsterixData &map, QDataStream &stream);
    bool generate_100(AsterixData &map, QDataStream &stream);
    bool generate_105(AsterixData &map, QDataStream &stream);
    bool generate_110(AsterixData &map, QDataStream &stream);
    bool generate_140(AsterixData &map, QDataStream &stream);
    bool generate_161(AsterixData &map, QDataStream &stream);
    bool generate_170(AsterixData &map, QDataStream &stream);
    bool generate_202(AsterixData &map, QDataStream &stream);
    bool generate_210(AsterixData &map, QDataStream &stream);
    bool generate_220(AsterixData &map, QDataStream &stream);
    bool generate_230(AsterixData &map, QDataStream &stream);
    bool generate_245(AsterixData &map, QDataStream &stream);
    bool generate_250(AsterixData &map, QDataStream &stream);
    bool generate_260(AsterixData &map, QDataStream &stream);
    bool generate_300(AsterixData &map, QDataStream &stream);
    bool generate_310(AsterixData &map, QDataStream &stream);
    bool generate_400(AsterixData &map, QDataStream &stream);
    bool generate_500(AsterixData &map, QDataStream &stream);

public:
    //! Target Report Descriptor data structure
    struct STargetReportDescriptor
    {
        enum FirstPart {
            SSR     = 0x80,
            MS      = 0x40,
            HF      = 0x20,
            VDL4    = 0x10,
            UAT     = 0x08,
            DME     = 0x04,
            OT      = 0x02
        };

        enum FirstExtent {
            RAB     = 0x80,
            SPI     = 0x40,
            CHN     = 0x20,
            GBS     = 0x10,
            CRT     = 0x08,
            SIM     = 0x04,
            TST     = 0x02
        };

    public:
        quint8 part;                /** First part byte */
        QList<quint8>   extents;    /** Extent bytes */
    };

    //! Warning/Error Conditions enumeration
    /*! When used, it shall be transmitted only if different from zero
    */
    enum EWarningErrorCode
    {
        WEMultipathReply = 0x01,            //!< Multipath Reply (Reflection)
        WESplitPlot = 0x03,                 //!< Split plot
        WEPhantomSSRPlot = 0x0A,            //!< Phantom SSR plot
        WENonMatchingMode3ACode = 0x0B,     //!< Non-Matching Mode-3/A Code
        WEModeCSAbnormalValue = 0x0C,       //!< Mode C code / Mode S altitude code abnormal value compared to the track
        WETransponderAnomaly = 0x0F,        //!< Transponder anomaly detected
        WEModeSAircraftAddress = 0x10,      //!< Duplicated or Illegal Mode S Aircraft Address
        WEModeSCorrectionApplied = 0x11,    //!< Mode S error correction applied
        WEUndecodableModeCSCode = 0x12      //!< Undecodable Mode C code / Mode S altitude code
    };

    //! Mode-2 or Mode-3/A Code in Octal Representation data structure
    struct SMode23ACodeInOctalRepresentation
    {
        SMode23ACodeInOctalRepresentation() : code(0) {}

        enum {
            V   = 0x8000,
            G   = 0x4000,
            L   = 0x2000
        };

        quint16 A4() const { return ( code & 0x0800 ) >> 9; }
        quint16 A2() const { return ( code & 0x0400 ) >> 9; }
        quint16 A1() const { return ( code & 0x0200 ) >> 9; }
        quint16 B4() const { return ( code & 0x0100 ) >> 6; }
        quint16 B2() const { return ( code & 0x0080 ) >> 6; }
        quint16 B1() const { return ( code & 0x0040 ) >> 6; }
        quint16 C4() const { return ( code & 0x0020 ) >> 3; }
        quint16 C2() const { return ( code & 0x0010 ) >> 3; }
        quint16 C1() const { return ( code & 0x0008 ) >> 3; }
        quint16 D4() const { return ( code & 0x0004 ); }
        quint16 D2() const { return ( code & 0x0002 ); }
        quint16 D1() const { return ( code & 0x0001 ); }

        void setA4() { code |= 0x0800; }
        void setA2() { code |= 0x0400; }
        void setA1() { code |= 0x0200; }
        void setB4() { code |= 0x0100; }
        void setB2() { code |= 0x0080; }
        void setB1() { code |= 0x0040; }
        void setC4() { code |= 0x0020; }
        void setC2() { code |= 0x0010; }
        void setC1() { code |= 0x0008; }
        void setD4() { code |= 0x0004; }
        void setD2() { code |= 0x0002; }
        void setD1() { code |= 0x0001; }

    public:
        quint16     code;   /** Mode-2 or Mode-3/A code converted into octal representation */
    };

    //! Mode-1 Code in Octal Representation data structure
    struct SMode1CodeInOctalRepresentation
    {
        SMode1CodeInOctalRepresentation() : code(0) {}

        enum {
            V   = 0x80,
            G   = 0x40,
            L   = 0x20
        };

        quint8 A4() const { return ( code & 0x10 ) >> 2; }
        quint8 A2() const { return ( code & 0x08 ) >> 2; }
        quint8 A1() const { return ( code & 0x04 ) >> 2; }
        quint8 B2() const { return ( code & 0x02 ); }
        quint8 B1() const { return ( code & 0x01 ); }

        void setA4() { code |= 0x10; }
        void setA2() { code |= 0x08; }
        void setA1() { code |= 0x04; }
        void setB2() { code |= 0x02; }
        void setB1() { code |= 0x01; }

    public:
        quint8      code;   /** Mode-1 code converted into octal representation */
    };

    //! Flight Level in Binary Representation data structure
    struct SFlightLevel
    {
        SFlightLevel() : code(0) {}

        enum {
            V   = 0x8000,
            G   = 0x4000
        };

        double altitude() const { return 25.*(code & 0x3FFF); }     // feet
        void setAltitude(double a) { code |= quint16(qRound(a / 25.)); }    // feet

    public:
        quint16     code;   /** Flight Level (Mode S Altitude) converted into binary two's complement representation */
    };

    //! Mode-C Code data structure
    struct SModeCCode
    {
        SModeCCode() : code(0) {}

        enum {
            V   = 0x80000000,
            G   = 0x40000000
        };

        quint32 A4() const { return ( code & 0x00400000 ) >> 22 << 2; }
        quint32 A2() const { return ( code & 0x01000000 ) >> 24 << 1; }
        quint32 A1() const { return ( code & 0x04000000 ) >> 26; }
        quint32 B4() const { return ( code & 0x00020000 ) >> 17 << 2; }
        quint32 B2() const { return ( code & 0x00080000 ) >> 19 << 1; }
        quint32 B1() const { return ( code & 0x00200000 ) >> 21; }
        quint32 C4() const { return ( code & 0x00800000 ) >> 23 << 2; }
        quint32 C2() const { return ( code & 0x02000000 ) >> 25 << 1; }
        quint32 C1() const { return ( code & 0x08000000 ) >> 27; }
        quint32 D4() const { return ( code & 0x00010000 ) >> 16 << 2; }
        quint32 D2() const { return ( code & 0x00040000 ) >> 18 << 1; }
        quint32 D1() const { return ( code & 0x00100000 ) >> 20; }

        quint32 QA4() const { return ( code & 0x00400000 ) >> 6 << 2; }
        quint32 QA2() const { return ( code & 0x01000000 ) >> 8 << 1; }
        quint32 QA1() const { return ( code & 0x04000000 ) >> 10; }
        quint32 QB4() const { return ( code & 0x00020000 ) >> 1 << 2; }
        quint32 QB2() const { return ( code & 0x00080000 ) >> 3 << 1; }
        quint32 QB1() const { return ( code & 0x00200000 ) >> 5; }
        quint32 QC4() const { return ( code & 0x00800000 ) >> 7 << 2; }
        quint32 QC2() const { return ( code & 0x02000000 ) >> 9 << 1; }
        quint32 QC1() const { return ( code & 0x08000000 ) >> 11; }
        quint32 QD4() const { return ( code & 0x00010000 ) << 2; }
        quint32 QD2() const { return ( code & 0x00040000 ) >> 2 << 1; }
        quint32 QD1() const { return ( code & 0x00100000 ) >> 4; }

        void setA4() { code |= 0x00400000; }
        void setA2() { code |= 0x01000000; }
        void setA1() { code |= 0x04000000; }
        void setB4() { code |= 0x00020000; }
        void setB2() { code |= 0x00080000; }
        void setB1() { code |= 0x00200000; }
        void setC4() { code |= 0x00800000; }
        void setC2() { code |= 0x02000000; }
        void setC1() { code |= 0x08000000; }
        void setD4() { code |= 0x00010000; }
        void setD2() { code |= 0x00040000; }
        void setD1() { code |= 0x00100000; }

        void setQA4() { code |= 0x00000040; }
        void setQA2() { code |= 0x00000100; }
        void setQA1() { code |= 0x00000400; }
        void setQB4() { code |= 0x00000002; }
        void setQB2() { code |= 0x00000008; }
        void setQB1() { code |= 0x00000020; }
        void setQC4() { code |= 0x00000080; }
        void setQC2() { code |= 0x00000200; }
        void setQC1() { code |= 0x00000800; }
        void setQD4() { code |= 0x00000001; }
        void setQD2() { code |= 0x00000004; }
        void setQD1() { code |= 0x00000010; }

    public:
        quint32     code;   /** Mode-C height in Gray notation as received from the
                                transponder together with the confidence level for each reply
                                bit as provided by a MSSR/Mode-S station */
    };

    //! Track Status data structure
    struct STrackStatus
    {
        STrackStatus() : part(0) {}

        enum
        {
            CNF = 0x80, //!< 0 - Confirmed track; 1 - Track in initiation phase
            TRE = 0x40, //!< 0 - Default; 1 - Last report for a track
            CST = 0x20, //!< 0 - Not extrapolated; 1 - Extrapolated
            MAH = 0x04, //!< 0 - Default; 1 - Horizontal manoeuvre
            STH = 0x02  //!< 0 - Measured position; 1 - Smoothed position
        };

        enum CDM
        {
            Maintaining = 0x00,
            Climbing    = 0x01,
            Descending  = 0x02,
            Invalid     = 0x03
        };

        // Enumeration for first extent
        enum
        {
            GHO = 0x80  //!< 0 - Default; 1 - Ghost track
        };

        CDM cdm() const { return CDM(( part >> 3 ) & 0x03); }
        void setCdm(CDM v) { part |= (quint8(v) & 0x03) << 3; }

    public:
        quint8  part;               /** First part */
        QList<quint8>   extents;    /** Extent bytes */
    };

    //! Communications/ACAS Capability and Flight Status data structure
    struct SCommunicationsACASCapabilityAndFlightStatus
    {
        SCommunicationsACASCapabilityAndFlightStatus() : item(0) {}

        enum
        {
            MSSC = 0x80,    //!< Mode-S Specific Service Capability: 0 - No; 1 - Yes
            ARC  = 0x40,    //!< Altitude reporting capability: 0 - 100, 1- 25 ft resolution
            AIC  = 0x20,    //!< Aircraft identification capability: 0 - No; 1 - Yes
            B1A  = 0x10     //!< BDS 1,0 bit 16
        };

        //! BDS 1,0 bits 37/40
        quint16 B1B() const { return item & 0x000F; }
        void setB1B(quint16 v) { item |= v & 0x000F; }

        //! Communications capability of the transponder
        /*!
            0 - No communications capability (surveillance only)
            1 - Comm. A and Comm. B capability
            2 - Comm. A, Comm. B and Uplink ELM
            3 - Comm. A, Comm. B, Uplink ELM and Downlink ELM
            4 - Level 5 Transponder capability
        */
        quint16 COM() const { return (item >> 13) & 0x0007; }
        void setCOM(quint16 v) { item |= (v & 0x0007) << 13; }

        //! Flight Status
        /*!
            0 - No alert, no SPI, aircraft airborne
            1 - No alert, no SPI, aircraft on ground
            2 - Alert, no SPI, aircraft airborne
            3 - Alert, no SPI, aircraft on ground
            4 - Alert, SPI, aircraft airborne or on ground
            5 - No alert, SPI, aircraft airborne or on ground
            6 – Not assigned
            7 - Information not yet extracted
        */
        quint16 STAT() const { return (item >> 10) & 0x0007; }
        void setSTAT(quint16 v) { item |= (v & 0x0007) << 10; }

    public:
        quint16     item;
    };

    //! Target (aircraft or vehicle) identification in 8 characters data structure
    struct STargetIdentification
    {
        STargetIdentification() : sti(NotDefined)
        { memset(static_cast<void *>(characters), 0, 8); }

        enum STI
        {
            NotDownlinked,  //!< Callsign or registration not downlinked from transponder
            Registration,   //!< Registration downlinked from transponder
            Callsign,       //!< Callsign downlinked from transponder
            NotDefined      //!< Not defined
        };

        STargetIdentification& operator = (const STargetIdentification &v)
        {
            sti = v.sti;
            memcpy((void *)characters, (const void *)v.characters, 8);
            return *this;
        }

        STI     sti;
        /** Characters 1-8 (coded on 6 bits each) defining target identification */
        char    characters[8];
    };

    //! Mode S MB Data data structure of one item from the group
    struct SModeSMBData
    {
        SModeSMBData() : BDS1(0), BDS2(0)
        { memset(static_cast<void *>(data), 0, 56); }

        SModeSMBData& operator = (const SModeSMBData &v)
        {
            BDS1 = v.BDS1; BDS2 = v.BDS2;
            memcpy((void *)data, (const void *)v.data, 7);
            return *this;
        }

        char        data[7];    /** 56-bit message conveying Mode S Comm B message data */
        quint8      BDS1;       /** Comm B Data Buffer Store 1 Address */
        quint8      BDS2;       /** Comm B Data Buffer Store 2 Address */
    };
    typedef QVariantList    TModeSMBDataList;

    //! Vehicle fleet identification number
    enum EVehicleFleetIdentification
    {
        VFIUnknown,
        VFIATC,         //!< ATC equipment maintenance
        VFIAirport,     //!< Airport maintenance
        VFIFire,        //!< Fire
        VFIBird,        //!< Bird scarer
        VFISnow,        //!< Snow plough
        VFIRunway,      //!< Runway sweeper
        VFIEmergency,   //!< Emergency
        VFIPolice,      //!< Police
        VFIBus,         //!< Bus
        VFITug,         //!< Tug (push/tow)
        VFIGrass,       //!< Grass cutter
        VFIFuel,        //!< Fuel
        VFIBaggage,     //!< Baggage
        VFICatering,    //!< Catering
        VFIAircraft,    //!< Aircraft maintenance
        VFIFlyco        //!< Flyco (follow me)
    };

    //! Number related to a pre-programmed message that can be transmitted by a vehicle
    struct SPreProgrammedMessage
    {
        SPreProgrammedMessage() : item(0) {}

        enum {
            TRB = 0x80  //!< 0 - Default; 1 - In Trouble
        };

        /*!
            1 - Towing aircraft
            2 - “Follow me” operation
            3 - Runway check
            4 - Emergency operation (fire, medical…)
            5 - Work in progress (maintenance, birds scarer, sweepers…)
        */
        quint8 MSG() const { return ( item & 0x7F ); }
        void setMSG(quint8 v) { item |= v & 0x7F; }

    public:
        quint8  item;
    };

    //! Deviation of position data structure
    struct SDeviationPosition
    {
        qreal   x;      /** Deviation or Standard Deviation of X component */
        qreal   y;      /** Deviation or Standard Deviation of Y component */
        qreal   rho;    /** Correlation coefficient */
    };

public:
    Asterix020Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Data Item I020/010, Data Source Identifier
    static const QString DataSourceIdentification_SAC;
    static const QString DataSourceIdentification_SIC;

    // Data Item I020/020, Target Report Descriptor
    static const QString TargetReportDescriptor;        // STargetReportDescriptor

    // Data Item I020/030, Warning/Error Conditions
    static const QString WarningErrorConditions;        // int - EWarningErrorCode

    // Data Item I020/041, Position in WGS-84 Coordinates
    static const QString PositionInWGS84Coordinates_Latitude;   // degree
    static const QString PositionInWGS84Coordinates_Longitude;  // degree

    // Data Item I020/042, Position in Cartesian Coordinates
    static const QString PositionInCartesianCoordinates_X;      // meters
    static const QString PositionInCartesianCoordinates_Y;      // meters

    // Data Item I020/050, Mode-2 Code in Octal Representation
    static const QString Mode2CodeInOctalRepresentation;    // SMode23ACodeInOctalRepresentation

    // Data Item I020/055, Mode-1 Code in Octal Representation
    static const QString Mode1CodeInOctalRepresentation;    // SMode1CodeInOctalRepresentation

    // Data Item I020/070, Mode-3/A Code in Octal Representation
    static const QString Mode3ACodeInOctalRepresentation;   // SMode23ACodeInOctalRepresentation

    // Data Item I020/090, Flight Level in Binary Representation
    static const QString FlightLevel;                       // SFlightLevel

    // Data Item I020/100, Mode-C Code
    static const QString ModeCCode;                         // SModeCCode

    // Data Item I020/105, Geometric Height (WGS-84)
    static const QString GeometricHeight;                   // feet

    // Data Item I020/110, Measured Height (Local Cartesian Coordinates)
    static const QString MeasuredHeight;                    // feet

    // Data Item I020/140, Time of Day
    static const QString TimeOfDay;                         // QTime

    // Data Item I020/161, Track Number
    static const QString TrackNumber;                       // quint16

    // Data Item I020/170, Track Status
    static const QString TrackStatus;                       // STrackStatus

    // Data Item I020/202, Calculated Track Velocity in Cartesian Coordinates
    static const QString CalculatedTrackVelocity_Vx;        // double - meters/sec
    static const QString CalculatedTrackVelocity_Vy;        // double - meters/sec

    // Data Item I020/210, Calculated Acceleration
    static const QString CalculatedAcceleration_Ax;         // double - meters/sec2
    static const QString CalculatedAcceleration_Ay;         // double - meters/sec2

    // Data Item I020/220, Target Address
    static const QString TargetAddress;                     // 24-bit - quint32

    // Data Item I020/230, Communications/ACAS Capability and Flight Status
    static const QString CommunicationsACASCapabilityAndFlightStatus;   // SCommunicationsACASCapabilityAndFlightStatus

    // Data Item I020/245, Target Identification
    static const QString TargetIdentification;              // STargetIdentification

    // Data Item I020/250, Mode S MB Data
    static const QString ModeSMBData;                       // TModeSMBDataList

    // Data Item I020/260, ACAS Resolution Advisory Report
    static const QString ACASResolutionAdvisoryReport;      // 56-bit QByteArray

    // Data Item I020/300, Vehicle Fleet Identification
    static const QString VehicleFleetIdentification;        // int - EVehicleFleetIdentification

    // Data Item I020/310, Pre-programmed Message
    static const QString PreProgrammedMessage;              // SPreProgrammedMessage

    // Data Item I020/400, Contributing Receivers
    static const QString ContributingReceivers;             // QByteArray

    // Data Item I020/500, Position Accuracy
    // Deviation of Position
    static const QString PositionAccuracy_DOP;              // SDeviationPosition
    // Standard Deviation of Position
    static const QString PositionAccuracy_SDP;              // SDeviationPosition
    // Standard Deviation of Geometric Height
    static const QString PositionAccuracy_SDH;              // double - meters

};

Q_DECLARE_METATYPE(Asterix020Proto::STargetReportDescriptor)
Q_DECLARE_METATYPE(Asterix020Proto::SMode23ACodeInOctalRepresentation)
Q_DECLARE_METATYPE(Asterix020Proto::SMode1CodeInOctalRepresentation)
Q_DECLARE_METATYPE(Asterix020Proto::SFlightLevel)
Q_DECLARE_METATYPE(Asterix020Proto::SModeCCode)
Q_DECLARE_METATYPE(Asterix020Proto::STrackStatus)
Q_DECLARE_METATYPE(Asterix020Proto::SCommunicationsACASCapabilityAndFlightStatus)
Q_DECLARE_METATYPE(Asterix020Proto::STargetIdentification)
Q_DECLARE_METATYPE(Asterix020Proto::SModeSMBData)
Q_DECLARE_METATYPE(Asterix020Proto::SPreProgrammedMessage)
Q_DECLARE_METATYPE(Asterix020Proto::SDeviationPosition)

#endif // ASTERIX020_PROTO_H
