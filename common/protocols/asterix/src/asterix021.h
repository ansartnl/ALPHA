#ifndef ASTERIX021_PROTO_H
#define ASTERIX021_PROTO_H

#include "asterixexport.h"
#include "asterixerrors.h"
#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>

class AsterixData;

class ASTERIX_EXPORT Asterix021Proto : public Asterix
{
protected:
    void parse_008(QDataStream &stream, AsterixData &result);
    void parse_010(QDataStream &stream, AsterixData &result);
    void parse_015(QDataStream &stream, AsterixData &result);
    void parse_016(QDataStream &stream, AsterixData &result);
    void parse_020(QDataStream &stream, AsterixData &result);
    void parse_040(QDataStream &stream, AsterixData &result);
    void parse_070(QDataStream &stream, AsterixData &result);
    void parse_071(QDataStream &stream, AsterixData &result);
    void parse_072(QDataStream &stream, AsterixData &result);
    void parse_073(QDataStream &stream, AsterixData &result);
    void parse_074(QDataStream &stream, AsterixData &result);
    void parse_075(QDataStream &stream, AsterixData &result);
    void parse_076(QDataStream &stream, AsterixData &result);
    void parse_077(QDataStream &stream, AsterixData &result);
    void parse_080(QDataStream &stream, AsterixData &result);
    void parse_090(QDataStream &stream, AsterixData &result);
    void parse_110(QDataStream &stream, AsterixData &result);
    void parse_130(QDataStream &stream, AsterixData &result);
    void parse_131(QDataStream &stream, AsterixData &result);
    void parse_132(QDataStream &stream, AsterixData &result);
    void parse_140(QDataStream &stream, AsterixData &result);
    void parse_145(QDataStream &stream, AsterixData &result);
    void parse_146(QDataStream &stream, AsterixData &result);
    void parse_148(QDataStream &stream, AsterixData &result);
    void parse_150(QDataStream &stream, AsterixData &result);
    void parse_151(QDataStream &stream, AsterixData &result);
    void parse_152(QDataStream &stream, AsterixData &result);
    void parse_155(QDataStream &stream, AsterixData &result);
    void parse_157(QDataStream &stream, AsterixData &result);
    void parse_160(QDataStream &stream, AsterixData &result);
    void parse_161(QDataStream &stream, AsterixData &result);
    void parse_165(QDataStream &stream, AsterixData &result);
    void parse_170(QDataStream &stream, AsterixData &result);
    void parse_200(QDataStream &stream, AsterixData &result);
    void parse_210(QDataStream &stream, AsterixData &result);
    void parse_220(QDataStream &stream, AsterixData &result);
    void parse_230(QDataStream &stream, AsterixData &result);
    void parse_250(QDataStream &stream, AsterixData &result);
    void parse_260(QDataStream &stream, AsterixData &result);
    void parse_271(QDataStream &stream, AsterixData &result);
    void parse_295(QDataStream &stream, AsterixData &result);
    void parse_400(QDataStream &stream, AsterixData &result);

    bool generate_008(AsterixData &map, QDataStream &stream);
    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_015(AsterixData &map, QDataStream &stream);
    bool generate_016(AsterixData &map, QDataStream &stream);
    bool generate_020(AsterixData &map, QDataStream &stream);
    bool generate_040(AsterixData &map, QDataStream &stream);
    bool generate_070(AsterixData &map, QDataStream &stream);
    bool generate_071(AsterixData &map, QDataStream &stream);
    bool generate_072(AsterixData &map, QDataStream &stream);
    bool generate_073(AsterixData &map, QDataStream &stream);
    bool generate_074(AsterixData &map, QDataStream &stream);
    bool generate_075(AsterixData &map, QDataStream &stream);
    bool generate_076(AsterixData &map, QDataStream &stream);
    bool generate_077(AsterixData &map, QDataStream &stream);
    bool generate_080(AsterixData &map, QDataStream &stream);
    bool generate_090(AsterixData &map, QDataStream &stream);
    bool generate_110(AsterixData &map, QDataStream &stream);
    bool generate_130(AsterixData &map, QDataStream &stream);
    bool generate_131(AsterixData &map, QDataStream &stream);
    bool generate_132(AsterixData &map, QDataStream &stream);
    bool generate_140(AsterixData &map, QDataStream &stream);
    bool generate_145(AsterixData &map, QDataStream &stream);
    bool generate_146(AsterixData &map, QDataStream &stream);
    bool generate_148(AsterixData &map, QDataStream &stream);
    bool generate_150(AsterixData &map, QDataStream &stream);
    bool generate_151(AsterixData &map, QDataStream &stream);
    bool generate_152(AsterixData &map, QDataStream &stream);
    bool generate_155(AsterixData &map, QDataStream &stream);
    bool generate_157(AsterixData &map, QDataStream &stream);
    bool generate_160(AsterixData &map, QDataStream &stream);
    bool generate_161(AsterixData &map, QDataStream &stream);
    bool generate_165(AsterixData &map, QDataStream &stream);
    bool generate_170(AsterixData &map, QDataStream &stream);
    bool generate_200(AsterixData &map, QDataStream &stream);
    bool generate_210(AsterixData &map, QDataStream &stream);
    bool generate_220(AsterixData &map, QDataStream &stream);
    bool generate_230(AsterixData &map, QDataStream &stream);
    bool generate_250(AsterixData &map, QDataStream &stream);
    bool generate_260(AsterixData &map, QDataStream &stream);
    bool generate_271(AsterixData &map, QDataStream &stream);
    bool generate_295(AsterixData &map, QDataStream &stream);
    bool generate_400(AsterixData &map, QDataStream &stream);

public:
    Asterix021Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Data Item I021/008, Aircraft Operational Status
    static const QString AircraftOperationalStatus_RA;
    static const QString AircraftOperationalStatus_TC;
    static const QString AircraftOperationalStatus_TS;
    static const QString AircraftOperationalStatus_ARV;
    static const QString AircraftOperationalStatus_CDTIA;
    static const QString AircraftOperationalStatus_NotTCAS;

    // Data Item I021/010, Data Source Identification
    static const QString DataSourceIdentification_SAC;
    static const QString DataSourceIdentification_SIC;

    // Data Item I021/015, Service Identification
    static const QString ServiceIdentification;

    // Data Item I021/016, Service Management
    static const QString ServiceManagement;

    // Data Item I021/020, Emitter Category
    static const QString EmitterCategory;

    // Data Item I021/040, Target Report Descriptor
    static const QString TargetReportDescriptor_ATP;
    static const QString TargetReportDescriptor_ARC;
    static const QString TargetReportDescriptor_RC;
    static const QString TargetReportDescriptor_RAB;

    static const QString TargetReportDescriptor_DCR;
    static const QString TargetReportDescriptor_GBS;
    static const QString TargetReportDescriptor_SIM;
    static const QString TargetReportDescriptor_TST;
    static const QString TargetReportDescriptor_SAA;
    static const QString TargetReportDescriptor_CL;

    static const QString TargetReportDescriptor_EC7;
    static const QString TargetReportDescriptor_EC6;
    static const QString TargetReportDescriptor_IPC;
    static const QString TargetReportDescriptor_NOGO;
    static const QString TargetReportDescriptor_CPR;
    static const QString TargetReportDescriptor_LDPJ;
    static const QString TargetReportDescriptor_RCF;

    // Data Item I021/070, Mode 3/A Code in Octal Representation
    static const QString Mode3ACodeInOctalRepresentation_A4;
    static const QString Mode3ACodeInOctalRepresentation_A2;
    static const QString Mode3ACodeInOctalRepresentation_A1;
    static const QString Mode3ACodeInOctalRepresentation_B4;
    static const QString Mode3ACodeInOctalRepresentation_B2;
    static const QString Mode3ACodeInOctalRepresentation_B1;
    static const QString Mode3ACodeInOctalRepresentation_C4;
    static const QString Mode3ACodeInOctalRepresentation_C2;
    static const QString Mode3ACodeInOctalRepresentation_C1;
    static const QString Mode3ACodeInOctalRepresentation_D4;
    static const QString Mode3ACodeInOctalRepresentation_D2;
    static const QString Mode3ACodeInOctalRepresentation_D1;

    // Data Item I021/071, Time of Applicability for Position
    static const QString TimeOfApplicabilityForPosition;

    // Data Item I021/072, Time of Applicability for Velocity
    static const QString TimeOfApplicabilityForVelocity;

    // Data Item I021/073, Time of Message Reception for Position
    static const QString TimeOfMessageReceptionForPosition;

    // Data Item I021/074, Time of Message Reception of Position–High Precision
    static const QString TimeOfMessageReceptionOfPositionHighPrecision;
    static const QString TimeOfMessageReceptionOfPositionHighPrecision_FSI;

    // Data Item I021/075, Time of Message Reception for Velocity
    static const QString TimeOfMessageReceptionForVelocity;

    // Data Item I021/076, Time of Message Reception of Velocity–High Precision
    static const QString TimeOfMessageReceptionOfVelocityHighPrecision;
    static const QString TimeOfMessageReceptionOfVelocityHighPrecision_FSI;

    // Data Item I021/077, Time of ASTERIX Report Transmission
    static const QString TimeOfAsterixReportTransmission;

    // Data Item I021/080, Target Address
    static const QString TargetAddress;

    // Data Item I021/090, Quality Indicators
    static const QString QualityIndicators_NUCrNACv;
    static const QString QualityIndicators_NUCpNIC;
    static const QString QualityIndicators_FX; // QList<I021_090_FX>

    struct I021_090_FX
    {
        quint8 NICbaro : 1 ;
        quint8 SIL : 2;
        quint8 NACp : 4;
    };

    // Data Item I021/110, Trajectory Intent
    static const QString TrajectoryIntent_TIS; // QList<I021_110_1>
    static const QString TrajectoryIntent_TID; // QList<I021_110_2>

    struct I021_110_1
    {
        quint8 NAV : 1;
        quint8 NVB : 1;
    };

    struct I021_110_2
    {
        quint8 TCA : 1;
        quint8 NC : 1;
        quint8 TCPnumber : 6;
        double altitude;
        double latitude;
        double longitude;
        quint8 pointType : 4;
        quint8 TD : 2;
        quint8 TRA : 1;
        quint8 TOA : 1;
        double TOV;
        double TTR;
    };

    // Data Item I021/130, Position in WGS-84 Co-ordinates
    static const QString PositionInWGS84Coordinates_Latitude;
    static const QString PositionInWGS84Coordinates_Longitude;

    // Data Item I021/131, High-Resolution Position in WGS-84 Co-ordinates
    static const QString HighResolutionPositionInWGS84Coordinates_Latitude;
    static const QString HighResolutionPositionInWGS84Coordinates_Longitude;

    // Data Item I021/132, Message Amplitude
    static const QString MessageAmplitude;

    // Data Item I021/140, Geometric Height
    static const QString GeometricHeight;

    // Data Item I021/145, Flight Level
    static const QString FlightLevel;

    // Data Item I021/146, Intermediate State Selected Altitude
    static const QString IntermediateStateSelectedAltitude_SAS;
    static const QString IntermediateStateSelectedAltitude_Source;
    static const QString IntermediateStateSelectedAltitude_Altitude;

    // Data Item I021/148, Final State Selected Altitude
    static const QString FinalStateSelectedAltitude_MV;
    static const QString FinalStateSelectedAltitude_AH;
    static const QString FinalStateSelectedAltitude_AM;
    static const QString FinalStateSelectedAltitude_Altitude;

    // Data Item I021/150, Air Speed
    static const QString AirSpeed_IM;
    static const QString AirSpeed;

    // Data Item I021/151 True Airspeed
    static const QString TrueAirspeed_RE;
    static const QString TrueAirspeed;

    // Data Item I021/152, Magnetic Heading
    static const QString MagneticHeading;

    // Data Item I021/155, Barometric Vertical Rate
    static const QString BarometricVerticalRate_RE;
    static const QString BarometricVerticalRate;

    // Data Item I021/157, Geometric Vertical Rate
    static const QString GeometricVerticalRate_RE;
    static const QString GeometricVerticalRate;

    // Data Item I021/160, Ground Vector
    static const QString GroundVector_RE;
    static const QString GroundVector_Speed;
    static const QString GroundVector_Angle;

    // Data Item I021/161, Track Number
    static const QString TrackNumber;

    // Data Item I021/165, Track Angle Rate
    static const QString TrackAngleRate;

    // Data Item I021/170, Target Identification
    static const QString TargetIdentification;

    // Data Item I021/200, Target Status
    static const QString TargetStatus_PS;
    static const QString TargetStatus_SS;

    // Data Item I021/210, MOPS Version
    static const QString MopsVersion_VNS;
    static const QString MopsVersion_VN;
    static const QString MopsVersion_LTT;

    // Data Item I021/220, Met Information
    static const QString MetInformation_WindSpeed;
    static const QString MetInformation_WindDirection;
    static const QString MetInformation_Temperature;
    static const QString MetInformation_Turbulence;

    // Data Item I021/230, Roll Angle
    static const QString RollAngle;

    // Data Item I021/250, Mode S MB Data
    static const QString ModeSMbData; // QList<I021_250>

    struct I021_250
    {
        quint8 MbData[7];
        quint8 BDS1 : 4;
        quint8 BDS2 : 4;
    };

    // Data Item I021/260, ACAS Resolution Advisory Report
    static const QString AcasResolutionAdvisoryReport;

    // Data Item I021/271, Surface Capabilities and Characteristics
    static const QString SurfaceCapabilitiesAndCharacteristics_POA;
    static const QString SurfaceCapabilitiesAndCharacteristics_CDTIS;
    static const QString SurfaceCapabilitiesAndCharacteristics_B2low;
    static const QString SurfaceCapabilitiesAndCharacteristics_RAS;
    static const QString SurfaceCapabilitiesAndCharacteristics_IDENT;
    static const QString SurfaceCapabilitiesAndCharacteristics_LengthAndWidth;

    // Data Item I021/295, Data Ages
    static const QString DataAges_AircraftOperationalStatusAge;
    static const QString DataAges_TargetReportDescriptorAge;
    static const QString DataAges_Mode3AAge;
    static const QString DataAges_QualityIndicatorsAge;
    static const QString DataAges_TrajectoryIntentAge;
    static const QString DataAges_MessageAmplitudeAge;
    static const QString DataAges_GeometricHeightAge;
    static const QString DataAges_FlightLevelAge;
    static const QString DataAges_IntermediateStateSelectedAltitudeAge;
    static const QString DataAges_FinalStateSelectedAltitudeAge;
    static const QString DataAges_AirSpeedAge;
    static const QString DataAges_TrueAirSpeedAge;
    static const QString DataAges_MagneticHeadingAge;
    static const QString DataAges_BarometricVerticalRateAge;
    static const QString DataAges_GeometricVerticalRateAge;
    static const QString DataAges_GroundVectorAge;
    static const QString DataAges_TrackRateAge;
    static const QString DataAges_TargetIdentificationAge;
    static const QString DataAges_TargetStatusAge;
    static const QString DataAges_MetInformationAge;
    static const QString DataAges_RollAngleAge;
    static const QString DataAges_AcasResolutionAdvisoryAge;
    static const QString DataAges_SurfaceCapabilitiesAndCharacteristicsAge;

    // Data Item I021/400, Receiver ID
    static const QString ReceiverId;
};

Q_DECLARE_METATYPE(QList<Asterix021Proto::I021_090_FX>)
Q_DECLARE_METATYPE(QList<Asterix021Proto::I021_110_1>)
Q_DECLARE_METATYPE(QList<Asterix021Proto::I021_110_2>)
Q_DECLARE_METATYPE(QList<Asterix021Proto::I021_250>)

#endif // Asterix021_PROTO_H
