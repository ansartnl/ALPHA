#ifndef EUROCONTROL_ASTERIX020_H
#define EUROCONTROL_ASTERIX020_H

#include "asterix020.h"


//! Eurocontrol standard of Asterix protocol
namespace Eurocontrol {

class ASTERIX_EXPORT Asterix020 : public Asterix020Proto
{
public:
    //! Reserved Expansion field: Ground speed data structure
    struct SGroundSpeed
    {
        SGroundSpeed() : isValueExceedsRange(false), speed(0.) {}

        bool    isValueExceedsRange;    /** “Range Exceeded” Indicator. True when value exceeds defined range */
        qreal   speed;                  /** NM/s, 0 < Ground Speed < 2 NM/s */
    };

    //! Reserved Expansion field: Ground Velocity Accuracy data structure
    struct SGroundVelocityAccuracy
    {
        qreal groundSpeed;  //!< Standard deviation of the Ground Speed; (LSB) ~ 0.22 kt; 0 < x < 56.25 Kt
        qreal trackAngle;   //!< Standard deviation of the Track Angle; 0 < x < 22.5 degrees
    };

    //! Reserved Expansion field: Mode S MB Data Age data structure of one item from the group
    struct SModeSMBDataAge
    {
        SModeSMBDataAge() : BDS1(0), BDS2(0), MBA(0.) {}

        quint8      BDS1;       /** Comm B Data Buffer Store 1 Address */
        quint8      BDS2;       /** Comm B Data Buffer Store 2 Address */
        qreal       MBA;        /** Age of the information in the BDS report indicated. Precision = 0.1s */
    };
    typedef QVariantList    TModeSMBDataAgeList;

protected:
    virtual void parse_re(QDataStream &stream, AsterixData &map);
    virtual bool generate_re(AsterixData &map, QDataStream &stream);

public:
    Asterix020();

    virtual QStringList fields() const;


    // Reserved Expansion Fields:

    // Deviation of Position
    static const QString RE_PositionAccuracy_DOP;           // SDeviationPosition
    // Standard Deviation of Position (Cartesian)
    static const QString RE_PositionAccuracy_SDC;           // SDeviationPosition
    // Standard Deviation of Position (WGS-84)
    static const QString RE_PositionAccuracy_SDW;           // SDeviationPosition
    // Standard Deviation of Geometric Height (WGS-84)
    static const QString RE_PositionAccuracy_SDH;           // double - feet

    // Ground Velocity Vector
    // Ground Speed referenced to WGS-84
    static const QString RE_GroundSpeed;                    // SGroundSpeed
    // Track Angle clockwise reference to “True North”
    static const QString RE_TrackAngle;                     // double - degree

    // Track Angle clockwise reference to “True North”
    static const QString RE_GroundVelocityAccuracy;         // SGroundVelocityAccuracy

    // Time of ASTERIX Report Transmission
    static const QString RE_TimeReportTransmission;         // QTime

    // Special Position Identification Age
    static const QString RE_SPI;                            // ms - LSB = 0.1s
    // Target Identification age
    static const QString RE_TI;                             // ms - LSB = 0.1s
    // Mode S MB age
    static const QString RE_MBD;                            // SModeSMBDataAge
    // Mode 3/A Code age
    static const QString RE_M3A;                            // ms - LSB = 0.1s
    // Flight Level age
    static const QString RE_FL;                             // ms - LSB = 0.1s
    // Flight Status age
    static const QString RE_FS;                             // ms - LSB = 0.1s
    // Geometric / Measured Height age
    static const QString RE_GH;                             // ms - LSB = 0.1s
    // Target Address age
    static const QString RE_TA;                             // ms - LSB = 0.1s
    // Mode C code age
    static const QString RE_MC;                             // ms - LSB = 0.1s
    // Mode-S Specific Service Capability age
    static const QString RE_MSS;                            // ms - LSB = 0.1s
    // Altitude reporting capability age
    static const QString RE_ARC;                            // ms - LSB = 0.1s
    // Aircraft identification capability age
    static const QString RE_AIC;                            // ms - LSB = 0.1s
    // Mode-2 Code age
    static const QString RE_M2;                             // ms - LSB = 0.1s
    // Mode-1 Code age
    static const QString RE_M1;                             // ms - LSB = 0.1s
    // ACAS Resolution Advisory age
    static const QString RE_ARA;                            // ms - LSB = 0.1s
    // Vehicle Fleet Identification age
    static const QString RE_VI;                             // ms - LSB = 0.1s
    // Pre-programmed message age
    static const QString RE_MSG;                            // ms - LSB = 0.1s
};

}   // Eurocontrol

Q_DECLARE_METATYPE(Eurocontrol::Asterix020::SGroundSpeed)
Q_DECLARE_METATYPE(Eurocontrol::Asterix020::SGroundVelocityAccuracy)
Q_DECLARE_METATYPE(Eurocontrol::Asterix020::SModeSMBDataAge)

#endif // EUROCONTROL_ASTERIX020_H
