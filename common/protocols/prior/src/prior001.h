#ifndef PRIOR001_H
#define PRIOR001_H

#include "prior.h"

#include <QtCore/QMap>
#include <QtCore/QPair>


//! Сообщение "Приор" категории 1.
/*!
    Сообщение  координатное  плота  ATC  RBS.
    Сообщение  координатное  плота  ВРЛ УВД.
    Сообщение  координатное  плота  «Только  первичный».
*/
class PRIOR_EXPORT Prior001 : public Prior
{
protected:
    void parse_DataSourceIdentifier(QDataStream &stream, PriorData &map);
    void parse_TargetReportDescriptor(QDataStream &stream, PriorData &map);
    void parse_Coords(QDataStream &stream, PriorData &map);
    void parse_TimeOfDay(QDataStream &stream, PriorData &map);
    void parse_Mode3ACode(QDataStream &stream, PriorData &map);
    void parse_ModeCCode(QDataStream &stream, PriorData &map);
    void parse_BoardNumber(QDataStream &stream, PriorData &map);
    void parse_FuelReserve(QDataStream &stream, PriorData &map);
    void parse_FlightLevel(QDataStream &stream, PriorData &map);

    bool generate_DataSourceIdentifier(PriorData &map, QDataStream &stream);
    bool generate_TargetReportDescriptor(PriorData &map, QDataStream &stream);
    bool generate_Coords(PriorData &map, QDataStream &stream);
    bool generate_TimeOfDay(PriorData &map, QDataStream &stream);
    bool generate_Mode3ACode(PriorData &map, QDataStream &stream);
    bool generate_ModeCCode(PriorData &map, QDataStream &stream);
    bool generate_BoardNumber(PriorData &map, QDataStream &stream);
    bool generate_FuelReserve(PriorData &map, QDataStream &stream);
    bool generate_FlightLevel(PriorData &map, QDataStream &stream);

public:
    //! Target Report Descriptor data structure
    struct STargetReportDescriptor
    {
        STargetReportDescriptor() : part(0) {}

        enum SSRPSR
        {
            NoDetection,    //!< No detection
            PSR,            //!< Sole primary detection
            SSR,            //!< Sole secondary detection
            PSR_SSR         //!< Combined primary and secondary detection
        };

        enum
        {
            RAB = 0x02,
            SPI = 0x04,
            ANT = 0x08,
            SIM = 0x40,
            TYP = 0x80
        };

        enum
        {
            TST = 0x80
        };

        enum DS
        {
            Default,
            UlfInterference,    //!< Unlawful interference (code 7500)
            RCFailure,          //!< Radio-communication failure (code 7600)
            Emergency           //!< Emergency (code 7700)
        };

        SSRPSR ssrPsr() const { return SSRPSR((part >> 4) & 0x03); }
        DS ds() const { Q_ASSERT(!extents.empty()); return DS((extents[0] >> 5) & 0x03); }

        void setSsrPsr(SSRPSR v) { part |= (quint8(v) & 0x03) << 4; }
        void setDs(DS v) { if (extents.empty()) extents.append(0); extents[0] |= (quint8(v) & 0x03) << 5; }

    public:
        quint8 part;                /** First part byte */
        QList<quint8>   extents;    /** Extent bytes */
    };

    //! Mode-2 or Mode-3/A Code in Octal Representation data structure
    struct SMode3ACode
    {
        SMode3ACode() : code(0) {}

        enum {
            V   = 0x8000,   //!< 0 - Code validated; 1 - Code not validated
            G   = 0x4000    //!< 0 - Default; 1 - Garbled code
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
        quint16     code;   /** Mode-3/A code converted into octal representation */
    };

    //! Mode-C Code data structure
    struct PRIOR_EXPORT SModeCCode
    {
        SModeCCode() : code(0) {}

        enum {
            V   = 0x8000,   //!< 0 - Code validated; 1 - Code not validated
            G   = 0x4000    //!< 0 - Default; 1 - Garbled code
        };

        qint32 height() const;      // meters
        void setHeight(qint32 v);   // meters

    public:
        quint16     code;
    };

    //! Flight Level data structure
    struct PRIOR_EXPORT SFlightLevel
    {
        SFlightLevel() : code(0) {}

        enum {
            H   = 0x8000,   //!< 0 - relative, 1 - absolute
            DS  = 0x4000    //!< Disaster
        };

        qint32 altitude() const;        // meters
        void setAltitude(qint32 v);     // meters

    public:
        quint16     code;
    };

    //! Board number data structure
    struct PRIOR_EXPORT SBoardNumber
    {
        SBoardNumber() : code(0) {}

        enum {
            V   = 0x00800000,   //!< 0 - Code validated; 1 - Code not validated
            G   = 0x00400000    //!< 0 - Default; 1 - Garbled code
        };

        quint32 number() const;
        void setNumber(quint32 v);

    public:
        quint32     code;
    };

public:
    Prior001();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, PriorData &map);
    virtual QByteArray generate(PriorData &map);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Data Source Identifier
    static const QString DataSourceIdentifier_SAC;  // System Area Code
    static const QString DataSourceIdentifier_SIC;  // System Identification Code

    // Target Report Descriptor
    static const QString TargetReportDescriptor;    // STargetReportDescriptor

    // Calculated Position in Cartesian Coordinates
    static const QString CalculatedPositionInCartesianCoordinates_X;    // meters
    static const QString CalculatedPositionInCartesianCoordinates_Y;    // meters

    // Truncated Time of Day
    static const QString TruncatedTimeOfDay;        // QTime

    // Mode-3/A Code in Octal Representation
    static const QString Mode3ACode;                // SMode3ACode

    // Mode-C Code in Binary Representation
    static const QString ModeCCode;                 // SModeCCode

    // Board number
    static const QString BoardNumber;               // SBoardNumber

    // Flight Level
    static const QString FlightLevel;               // SFlightLevel

    // Fuel reserve
    static const QString FuelReserve;               // [0 - 100] %
};

Q_DECLARE_METATYPE(Prior001::STargetReportDescriptor)
Q_DECLARE_METATYPE(Prior001::SMode3ACode)
Q_DECLARE_METATYPE(Prior001::SModeCCode)
Q_DECLARE_METATYPE(Prior001::SFlightLevel)
Q_DECLARE_METATYPE(Prior001::SBoardNumber)

#endif // PRIOR001_H
