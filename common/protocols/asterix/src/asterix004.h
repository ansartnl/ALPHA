#ifndef ASTERIX004_PROTO_H
#define ASTERIX004_PROTO_H

#include "asterixexport.h"
#include "asterixerrors.h"
#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>

class AsterixData;

class ASTERIX_EXPORT Asterix004Proto : public Asterix
{
protected:
    virtual void parse_000(QDataStream &stream, AsterixData &result);
    virtual void parse_010(QDataStream &stream, AsterixData &result);
    virtual void parse_015(QDataStream &stream, AsterixData &result);
    virtual void parse_020(QDataStream &stream, AsterixData &result);
    virtual void parse_030(QDataStream &stream, AsterixData &result);
    virtual void parse_035(QDataStream &stream, AsterixData &result);
    virtual void parse_040(QDataStream &stream, AsterixData &result);
    virtual void parse_045(QDataStream &stream, AsterixData &result);
    virtual void parse_070(QDataStream &stream, AsterixData &result);
    virtual void parse_110(QDataStream &stream, AsterixData &result);
    virtual void parse_120(QDataStream &stream, AsterixData &result);
    virtual void parse_170(QDataStream &stream, AsterixData &result);
    virtual void parse_171(QDataStream &stream, AsterixData &result);

public:
    Asterix004Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;


    struct SDPS
    {
        quint8 sac;
        quint8 sic;
    };

    struct FDPS
    {
        quint8 centre;
        quint8 position;
    };

    static const int mCategory;
    static const QString mEdition;

    // Data Item I001/000, Message Type
    static const QString MessageType;

    // Data Item I001/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC; // System Area Code
    static const QString DataSourceIdentifier_SIC; // System Identification Code

    // Data Item I001/015, SDPS Identifier
    static const QString SDPSIdentifier;

    // Data Item I002/020, Time of Message
    static const QString TimeOfMessage;

    // Data Item I002/030, Track Number 1
    static const QString TrackNumber1;

    // Data Item I002/035, Track Number 2
    static const QString TrackNumber2;

    // Data Item I001/040, Alert Identifier
    static const QString AlertIdentifier;

    // Data Item I001/045, Alert Status
    static const QString AlertStatus_STAT;

    // Data Item I001/060, Safety Net Function & System Status
    static const QString SafetyNetFunctionAndSystemStatus;

    // Data Item I001/070, Conflict Timing and Separation
    static const QString ConflictTimingAndSeparation_TC;  // Time to Conflict
    static const QString ConflictTimingAndSeparation_TCA; // Time to Closest Approach
    static const QString ConflictTimingAndSeparation_CHS; // Current Horizontal Separation, meters
    static const QString ConflictTimingAndSeparation_MHS; // Estimated Minimum Horizontal Separation, meters
    static const QString ConflictTimingAndSeparation_CVS; // Current Vertical Separation, feet
    static const QString ConflictTimingAndSeparation_MVS; // Estimated Minimum Vertical Separation, feet

    // Data Item I001/074, Longitudinal Deviation
    static const QString LongitudinalDeviation;

    // Data Item I001/075, Transversal Distance Deviation
    static const QString TransversalDistanceDeviation;

    // Data Item I001/076, Vertical Deviation
    static const QString VerticalDeviation;

    // Data Item I001/100, Area Definitions
    static const QString AreaDefinitions;

    // Data Item I001/110, FDPS Sector Control Positions
    static const QString FDPSSectorControlPositions;

    // Data Item I001/120, Conflict Characteristics
    // Conflict Nature
    static const QString ConflictCharacteristics_MAS;
    static const QString ConflictCharacteristics_CAS;
    static const QString ConflictCharacteristics_FLD;
    static const QString ConflictCharacteristics_FVD;
    static const QString ConflictCharacteristics_Type;
    static const QString ConflictCharacteristics_Cross;
    static const QString ConflictCharacteristics_Div;
    static const QString ConflictCharacteristics_RRC;
    static const QString ConflictCharacteristics_RTC;
    static const QString ConflictCharacteristics_MRVA;
    static const QString ConflictCharacteristics_VRAM_CRM;
    static const QString ConflictCharacteristics_VRAM_VRM;
    static const QString ConflictCharacteristics_VRAM_VTM;
    static const QString ConflictCharacteristics_HAM_HD;
    static const QString ConflictCharacteristics_HAM_RD;
    static const QString ConflictCharacteristics_HAM_VD;
    static const QString ConflictCharacteristics_DBPSM_ARR;
    static const QString ConflictCharacteristics_DBPSM_DEP;
    static const QString ConflictCharacteristics_DBPSM_TL;
    static const QString ConflictCharacteristics_AIW;
    // Conflict Classification
    static const QString ConflictCharacteristics_TableId;
    static const QString ConflictCharacteristics_ConflictProperties;
    static const QString ConflictCharacteristics_CS;
    // Conflict Probability
    static const QString ConflictCharacteristics_Probability;
    // Conflict Duration
    static const QString ConflictCharacteristics_Duration;

    // Data Item I001/170, Aircraft Identification & Characteristics 1
    static const QString AircraftIdentificationAndCharacteristics1;

    // Data Item I001/171, Aircraft Identification & Characteristics 2
    static const QString AircraftIdentificationAndCharacteristics2;

    static const QString AircraftIdentificationAndCharacteristics_AI;
    static const QString AircraftIdentificationAndCharacteristics_M3;
    static const QString AircraftIdentificationAndCharacteristics_CPW_Latitude;
    static const QString AircraftIdentificationAndCharacteristics_CPW_Longitude;
    static const QString AircraftIdentificationAndCharacteristics_CPW_Altitude; // feet
    static const QString AircraftIdentificationAndCharacteristics_CPC_X;
    static const QString AircraftIdentificationAndCharacteristics_CPC_Y;
    static const QString AircraftIdentificationAndCharacteristics_CPC_Z; // feet
    static const QString AircraftIdentificationAndCharacteristics_TT;
    static const QString AircraftIdentificationAndCharacteristics_DT;
    static const QString AircraftIdentificationAndCharacteristics_AC_GAT_OAT;
    static const QString AircraftIdentificationAndCharacteristics_AC_FR1_FR2;
    static const QString AircraftIdentificationAndCharacteristics_AC_RVSM;
    static const QString AircraftIdentificationAndCharacteristics_AC_HPR;
    static const QString AircraftIdentificationAndCharacteristics_AC_CDM;
    static const QString AircraftIdentificationAndCharacteristics_AC_PRI;
    static const QString AircraftIdentificationAndCharacteristics_AC_GV;
    static const QString AircraftIdentificationAndCharacteristics_MS;
    static const QString AircraftIdentificationAndCharacteristics_FP;
    static const QString AircraftIdentificationAndCharacteristics_CF; // FL

private:
    QVariantMap parseAircraftIdentificationAndCharacteristics(QDataStream &stream);
};

Q_DECLARE_METATYPE(QList<Asterix004Proto::SDPS>)
Q_DECLARE_METATYPE(QList<Asterix004Proto::FDPS>)

#endif // ASTERIX004_PROTO_H
