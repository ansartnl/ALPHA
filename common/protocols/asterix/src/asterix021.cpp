#include "asterix021.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QTime>
#include <math.h>


const int Asterix021Proto::mCategory = 21;
const QString Asterix021Proto::mEdition = "1.8";

// Data Item I021/008, Aircraft Operational Status
const QString Asterix021Proto::AircraftOperationalStatus_RA = "a021.AircraftOperationalStatus.RA";
const QString Asterix021Proto::AircraftOperationalStatus_TC = "a021.AircraftOperationalStatus.TC";
const QString Asterix021Proto::AircraftOperationalStatus_TS = "a021.AircraftOperationalStatus.TS";
const QString Asterix021Proto::AircraftOperationalStatus_ARV = "a021.AircraftOperationalStatus.ARV";
const QString Asterix021Proto::AircraftOperationalStatus_CDTIA = "a021.AircraftOperationalStatus.CDTIA";
const QString Asterix021Proto::AircraftOperationalStatus_NotTCAS = "a021.AircraftOperationalStatus.NotTCAS";

// Data Item I021/010, Data Source Identification
const QString Asterix021Proto::DataSourceIdentification_SAC = ASTERIX_SAC;
const QString Asterix021Proto::DataSourceIdentification_SIC = ASTERIX_SIC;

// Data Item I021/015, Service Identification
const QString Asterix021Proto::ServiceIdentification = "a021.ServiceIdentification";

// Data Item I021/016, Service Management
const QString Asterix021Proto::ServiceManagement = "a021.ServiceManagement";

// Data Item I021/020, Emitter Category
const QString Asterix021Proto::EmitterCategory = "a021.EmitterCategory";

// Data Item I021/040, Target Report Descriptor
const QString Asterix021Proto::TargetReportDescriptor_ATP = "a021.TargetReportDescriptor_ATP";
const QString Asterix021Proto::TargetReportDescriptor_ARC = "a021.TargetReportDescriptor_ARC";
const QString Asterix021Proto::TargetReportDescriptor_RC = "a021.TargetReportDescriptor_RC";
const QString Asterix021Proto::TargetReportDescriptor_RAB = "a021.TargetReportDescriptor_RAB";

const QString Asterix021Proto::TargetReportDescriptor_DCR = "a021.TargetReportDescriptor_DCR";
const QString Asterix021Proto::TargetReportDescriptor_GBS = "a021.TargetReportDescriptor_GBS";
const QString Asterix021Proto::TargetReportDescriptor_SIM = "a021.TargetReportDescriptor_SIM";
const QString Asterix021Proto::TargetReportDescriptor_TST = "a021.TargetReportDescriptor_TST";
const QString Asterix021Proto::TargetReportDescriptor_SAA = "a021.TargetReportDescriptor_SAA";
const QString Asterix021Proto::TargetReportDescriptor_CL = "a021.TargetReportDescriptor_CL";

const QString Asterix021Proto::TargetReportDescriptor_EC7 = "a021.TargetReportDescriptor_EC7";
const QString Asterix021Proto::TargetReportDescriptor_EC6 = "a021.TargetReportDescriptor_EC6";
const QString Asterix021Proto::TargetReportDescriptor_IPC = "a021.TargetReportDescriptor_IPC";
const QString Asterix021Proto::TargetReportDescriptor_NOGO = "a021.TargetReportDescriptor_NOGO";
const QString Asterix021Proto::TargetReportDescriptor_CPR = "a021.TargetReportDescriptor_CPR";
const QString Asterix021Proto::TargetReportDescriptor_LDPJ = "a021.TargetReportDescriptor_LDPJ";
const QString Asterix021Proto::TargetReportDescriptor_RCF = "a021.TargetReportDescriptor_RCF";

// Data Item I021/070, Mode 3/A Code in Octal Representation
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_A4 = "a021.Mode3ACodeInOctalRepresentation.A4";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_A2 = "a021.Mode3ACodeInOctalRepresentation.A2";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_A1 = "a021.Mode3ACodeInOctalRepresentation.A1";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_B4 = "a021.Mode3ACodeInOctalRepresentation.B4";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_B2 = "a021.Mode3ACodeInOctalRepresentation.B2";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_B1 = "a021.Mode3ACodeInOctalRepresentation.B1";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_C4 = "a021.Mode3ACodeInOctalRepresentation.C4";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_C2 = "a021.Mode3ACodeInOctalRepresentation.C2";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_C1 = "a021.Mode3ACodeInOctalRepresentation.C1";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_D4 = "a021.Mode3ACodeInOctalRepresentation.D4";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_D2 = "a021.Mode3ACodeInOctalRepresentation.D2";
const QString Asterix021Proto::Mode3ACodeInOctalRepresentation_D1 = "a021.Mode3ACodeInOctalRepresentation.D1";

// Data Item I021/071, Time of Applicability for Position
const QString Asterix021Proto::TimeOfApplicabilityForPosition = "a021.TimeOfApplicabilityForPosition";

// Data Item I021/072, Time of Applicability for Velocity
const QString Asterix021Proto::TimeOfApplicabilityForVelocity = "a021.TimeOfApplicabilityForVelocity";

// Data Item I021/073, Time of Message Reception for Position
const QString Asterix021Proto::TimeOfMessageReceptionForPosition = "a021.TimeOfMessageReceptionForPosition";

// Data Item I021/074, Time of Message Reception of Position–High Precision
const QString Asterix021Proto::TimeOfMessageReceptionOfPositionHighPrecision = "a021.TimeOfMessageReceptionOfPositionHighPrecision";
const QString Asterix021Proto::TimeOfMessageReceptionOfPositionHighPrecision_FSI = "a021.TimeOfMessageReceptionOfPositionHighPrecision.FSI";

// Data Item I021/075, Time of Message Reception for Velocity
const QString Asterix021Proto::TimeOfMessageReceptionForVelocity = "a021.TimeOfMessageReceptionForVelocity";

// Data Item I021/076, Time of Message Reception of Velocity–High Precision
const QString Asterix021Proto::TimeOfMessageReceptionOfVelocityHighPrecision = "a021.TimeOfMessageReceptionOfVelocityHighPrecision";
const QString Asterix021Proto::TimeOfMessageReceptionOfVelocityHighPrecision_FSI = "a021.TimeOfMessageReceptionOfVelocityHighPrecision.FSI";

// Data Item I021/077, Time of ASTERIX Report Transmission
const QString Asterix021Proto::TimeOfAsterixReportTransmission = "a021.TimeOfAsterixReportTransmission";

// Data Item I021/080, Target Address
const QString Asterix021Proto::TargetAddress = "a021.TargetAddress";

// Data Item I021/090, Quality Indicators
const QString Asterix021Proto::QualityIndicators_NUCrNACv = "a021.QualityIndicators.NUCrNACv";
const QString Asterix021Proto::QualityIndicators_NUCpNIC = "a021.QualityIndicators.NUCpNIC";
const QString Asterix021Proto::QualityIndicators_FX = "a021.QualityIndicators.FX";

// Data Item I021/110, Trajectory Intent
const QString Asterix021Proto::TrajectoryIntent_TIS = "a021.TrajectoryIntent.TIS";
const QString Asterix021Proto::TrajectoryIntent_TID = "a021.TrajectoryIntent.TID";

// Data Item I021/130, Position in WGS-84 Co-ordinates
const QString Asterix021Proto::PositionInWGS84Coordinates_Latitude = "a021.PositionInWGS84Coordinates.Latitude";
const QString Asterix021Proto::PositionInWGS84Coordinates_Longitude = "a021.PositionInWGS84Coordinates.Longitude";

// Data Item I021/131, High-Resolution Position in WGS-84 Co-ordinates
const QString Asterix021Proto::HighResolutionPositionInWGS84Coordinates_Latitude = "a021.HighResolutionPositionInWGS84Coordinates.Latitude";
const QString Asterix021Proto::HighResolutionPositionInWGS84Coordinates_Longitude = "a021.HighResolutionPositionInWGS84Coordinates.Longitude";

// Data Item I021/132, Message Amplitude
const QString Asterix021Proto::MessageAmplitude = "a021.MessageAmplitude";

// Data Item I021/140, Geometric Height
const QString Asterix021Proto::GeometricHeight = "a021.GeometricHeight";

// Data Item I021/145, Flight Level
const QString Asterix021Proto::FlightLevel = "a021.FlightLevel";

// Data Item I021/146, Intermediate State Selected Altitude
const QString Asterix021Proto::IntermediateStateSelectedAltitude_SAS = "a021.IntermediateStateSelectedAltitude.SAS";
const QString Asterix021Proto::IntermediateStateSelectedAltitude_Source = "a021.IntermediateStateSelectedAltitude.Source";
const QString Asterix021Proto::IntermediateStateSelectedAltitude_Altitude = "a021.IntermediateStateSelectedAltitude.Altitude";

// Data Item I021/148, Final State Selected Altitude
const QString Asterix021Proto::FinalStateSelectedAltitude_MV = "a021.FinalStateSelectedAltitude.MV";
const QString Asterix021Proto::FinalStateSelectedAltitude_AH = "a021.FinalStateSelectedAltitude.AH";
const QString Asterix021Proto::FinalStateSelectedAltitude_AM = "a021.FinalStateSelectedAltitude.AM";
const QString Asterix021Proto::FinalStateSelectedAltitude_Altitude = "a021.FinalStateSelectedAltitude.Altitude";

// Data Item I021/150, Air Speed
const QString Asterix021Proto::AirSpeed = "a021.AirSpeed";
const QString Asterix021Proto::AirSpeed_IM = "a021.AirSpeed.IM";

// Data Item I021/151 True Airspeed
const QString Asterix021Proto::TrueAirspeed = "a021.TrueAirspeed";
const QString Asterix021Proto::TrueAirspeed_RE = "a021.TrueAirspeed.RE";

// Data Item I021/152, Magnetic Heading
const QString Asterix021Proto::MagneticHeading = "a021.MagneticHeading";

// Data Item I021/155, Barometric Vertical Rate
const QString Asterix021Proto::BarometricVerticalRate = "a021.BarometricVerticalRate";
const QString Asterix021Proto::BarometricVerticalRate_RE = "a021.BarometricVerticalRate.RE";

// Data Item I021/157, Geometric Vertical Rate
const QString Asterix021Proto::GeometricVerticalRate = "a021.GeometricVerticalRate";
const QString Asterix021Proto::GeometricVerticalRate_RE = "a021.GeometricVerticalRate.RE";

// Data Item I021/160, Ground Vector
const QString Asterix021Proto::GroundVector_RE = "a021.GroundVector.RE";
const QString Asterix021Proto::GroundVector_Speed = "a021.GroundVector.Speed";
const QString Asterix021Proto::GroundVector_Angle = "a021.GroundVector.Angle";

// Data Item I021/161, Track Number
const QString Asterix021Proto::TrackNumber = "a021.TrackNumber";

// Data Item I021/165, Track Angle Rate
const QString Asterix021Proto::TrackAngleRate = "a021.TrackAngleRate";

// Data Item I021/170, Target Identification
const QString Asterix021Proto::TargetIdentification = "a021.TargetIdentification";

// Data Item I021/200, Target Status
const QString Asterix021Proto::TargetStatus_PS = "a021.TargetStatus.PS";
const QString Asterix021Proto::TargetStatus_SS = "a021.TargetStatus.SS";

// Data Item I021/210, MOPS Version
const QString Asterix021Proto::MopsVersion_VNS = "a021.MopsVersion.VNS";
const QString Asterix021Proto::MopsVersion_VN = "a021.MopsVersion.VN";
const QString Asterix021Proto::MopsVersion_LTT = "a021.MopsVersion.LTT";

// Data Item I021/220, Met Information
const QString Asterix021Proto::MetInformation_WindSpeed = "a021.MetInformation.WindSpeed";
const QString Asterix021Proto::MetInformation_WindDirection = "a021.MetInformation.WindDirection";
const QString Asterix021Proto::MetInformation_Temperature = "a021.MetInformation.Temperature";
const QString Asterix021Proto::MetInformation_Turbulence = "a021.MetInformation.Turbulence";

// Data Item I021/230, Roll Angle
const QString Asterix021Proto::RollAngle = "a021.RollAngle";

// Data Item I021/250, Mode S MB Data
const QString Asterix021Proto::ModeSMbData = "a021.ModeSMbData";

// Data Item I021/260, ACAS Resolution Advisory Report
const QString Asterix021Proto::AcasResolutionAdvisoryReport = "a021.AcasResolutionAdvisoryReport";

// Data Item I021/271, Surface Capabilities and Characteristics
const QString Asterix021Proto::SurfaceCapabilitiesAndCharacteristics_POA = "a021.SurfaceCapabilitiesAndCharacteristics.POA";
const QString Asterix021Proto::SurfaceCapabilitiesAndCharacteristics_CDTIS = "a021.SurfaceCapabilitiesAndCharacteristics.CDTIS";
const QString Asterix021Proto::SurfaceCapabilitiesAndCharacteristics_B2low = "a021.SurfaceCapabilitiesAndCharacteristics.B2low";
const QString Asterix021Proto::SurfaceCapabilitiesAndCharacteristics_RAS = "a021.SurfaceCapabilitiesAndCharacteristics.RAS";
const QString Asterix021Proto::SurfaceCapabilitiesAndCharacteristics_IDENT = "a021.SurfaceCapabilitiesAndCharacteristics.IDENT";
const QString Asterix021Proto::SurfaceCapabilitiesAndCharacteristics_LengthAndWidth = "a021.SurfaceCapabilitiesAndCharacteristics.LengthAndWidth";

// Data Item I021/295, Data Ages
const QString Asterix021Proto::DataAges_AircraftOperationalStatusAge = "a021.DataAges.AircraftOperationalStatusAge";
const QString Asterix021Proto::DataAges_TargetReportDescriptorAge = "a021.DataAges.TargetReportDescriptorAge";
const QString Asterix021Proto::DataAges_Mode3AAge = "a021.DataAges.Mode3AAge";
const QString Asterix021Proto::DataAges_QualityIndicatorsAge = "a021.DataAges.QualityIndicatorsAge";
const QString Asterix021Proto::DataAges_TrajectoryIntentAge = "a021.DataAges.TrajectoryIntentAge";
const QString Asterix021Proto::DataAges_MessageAmplitudeAge = "a021.DataAges.MessageAmplitudeAge";
const QString Asterix021Proto::DataAges_GeometricHeightAge = "a021.DataAges.GeometricHeightAge";
const QString Asterix021Proto::DataAges_FlightLevelAge = "a021.DataAges.FlightLevelAge";
const QString Asterix021Proto::DataAges_IntermediateStateSelectedAltitudeAge = "a021.DataAges.IntermediateStateSelectedAltitudeAge";
const QString Asterix021Proto::DataAges_FinalStateSelectedAltitudeAge = "a021.DataAges.FinalStateSelectedAltitudeAge";
const QString Asterix021Proto::DataAges_AirSpeedAge = "a021.DataAges.AirSpeedAge";
const QString Asterix021Proto::DataAges_TrueAirSpeedAge = "a021.DataAges.TrueAirSpeedAge";
const QString Asterix021Proto::DataAges_MagneticHeadingAge = "a021.DataAges.MagneticHeadingAge";
const QString Asterix021Proto::DataAges_BarometricVerticalRateAge = "a021.DataAges.BarometricVerticalRateAge";
const QString Asterix021Proto::DataAges_GeometricVerticalRateAge = "a021.DataAges.GeometricVerticalRateAge";
const QString Asterix021Proto::DataAges_GroundVectorAge = "a021.DataAges.GroundVectorAge";
const QString Asterix021Proto::DataAges_TrackRateAge = "a021.DataAges.TrackRateAge";
const QString Asterix021Proto::DataAges_TargetIdentificationAge = "a021.DataAges.TargetIdentificationAge";
const QString Asterix021Proto::DataAges_TargetStatusAge = "a021.DataAges.TargetStatusAge";
const QString Asterix021Proto::DataAges_MetInformationAge = "a021.DataAges.MetInformationAge";
const QString Asterix021Proto::DataAges_RollAngleAge = "a021.DataAges.RollAngleAge";
const QString Asterix021Proto::DataAges_AcasResolutionAdvisoryAge = "a021.DataAges.AcasResolutionAdvisoryAge";
const QString Asterix021Proto::DataAges_SurfaceCapabilitiesAndCharacteristicsAge = "a021.DataAges.SurfaceCapabilitiesAndCharacteristicsAge";

// Data Item I021/400, Receiver ID
const QString Asterix021Proto::ReceiverId = "a021.ReceiverId";

Asterix021Proto::Asterix021Proto()
{}

int Asterix021Proto::category() const
{
    return mCategory;
}

QString Asterix021Proto::edition() const
{
    return mEdition;
}

void Asterix021Proto::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.size() > 7)
            map.appendError(AsterixErrors::Error_FSPEC);

        if (mainMask.isSet(1))
            parse_010(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_040(stream, map);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (mainMask.isSet(3))
            parse_161(stream, map);

        if (mainMask.isSet(4))
            parse_015(stream, map);

        if (mainMask.isSet(5))
            parse_071(stream, map);

        if (mainMask.isSet(6))
            parse_130(stream, map);

        if (mainMask.isSet(7))
            parse_131(stream, map);

        if (mainMask.isSet(8))
            parse_072(stream, map);

        if (mainMask.isSet(9))
            parse_150(stream, map);

        if (mainMask.isSet(10))
            parse_151(stream, map);

        if (mainMask.isSet(11))
            parse_080(stream, map);
        else
            map.appendError(mandatory("Target Address"));

        if (mainMask.isSet(12))
            parse_073(stream, map);

        if (mainMask.isSet(13))
            parse_074(stream, map);

        if (mainMask.isSet(14))
            parse_075(stream, map);

        if (mainMask.isSet(15))
            parse_076(stream, map);

        if (mainMask.isSet(16))
            parse_140(stream, map);

        if (mainMask.isSet(17))
            parse_090(stream, map);
        else
            map.appendError(mandatory("Quality Indicators"));

        if (mainMask.isSet(18))
            parse_210(stream, map);

        if (mainMask.isSet(19))
            parse_070(stream, map);

        if (mainMask.isSet(20))
            parse_230(stream, map);

        if (mainMask.isSet(21))
            parse_145(stream, map);

        if (mainMask.isSet(22))
            parse_152(stream, map);

        if (mainMask.isSet(23))
            parse_200(stream, map);

        if (mainMask.isSet(24))
            parse_155(stream, map);

        if (mainMask.isSet(25))
            parse_157(stream, map);

        if (mainMask.isSet(26))
            parse_160(stream, map);

        if (mainMask.isSet(27))
            parse_165(stream, map);

        if (mainMask.isSet(28))
            parse_077(stream, map);

        if (mainMask.isSet(29))
            parse_170(stream, map);

        if (mainMask.isSet(30))
            parse_020(stream, map);

        if (mainMask.isSet(31))
            parse_220(stream, map);

        if (mainMask.isSet(32))
            parse_146(stream, map);

        if (mainMask.isSet(33))
            parse_148(stream, map);

        if (mainMask.isSet(34))
            parse_110(stream, map);

        if (mainMask.isSet(35))
            parse_016(stream, map);

        if (mainMask.isSet(36))
            parse_008(stream, map);

        if (mainMask.isSet(37))
            parse_271(stream, map);

        if (mainMask.isSet(38))
            parse_132(stream, map);

        if (mainMask.isSet(39))
            parse_250(stream, map);

        if (mainMask.isSet(40))
            parse_260(stream, map);

        if (mainMask.isSet(41))
            parse_400(stream, map);

        if (mainMask.isSet(42))
            parse_295(stream, map);

        if (mainMask.isSet(43))
            map.appendError(AsterixErrors::Error_FSPEC);

        if (mainMask.isSet(44))
            map.appendError(AsterixErrors::Error_FSPEC);

        if (mainMask.isSet(45))
            map.appendError(AsterixErrors::Error_FSPEC);

        if (mainMask.isSet(46))
            map.appendError(AsterixErrors::Error_FSPEC);

        if (mainMask.isSet(47))
            map.appendError(AsterixErrors::Error_FSPEC);

        if (mainMask.isSet(48))
            parse_re(stream, map);

        if (mainMask.isSet(49))
            parse_sp(stream, map);
    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
}

QByteArray Asterix021Proto::generate(AsterixData &map)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        if (generate_010(map, stream))
            mainMask.set(1);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (generate_040(map, stream))
            mainMask.set(2);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (generate_161(map, stream))
            mainMask.set(3);

        if (generate_015(map, stream))
            mainMask.set(4);

        if (generate_071(map, stream))
            mainMask.set(5);

        if (generate_130(map, stream))
            mainMask.set(6);

        if (generate_131(map, stream))
            mainMask.set(7);

        if (generate_072(map, stream))
            mainMask.set(8);

        if (generate_150(map, stream))
            mainMask.set(9);

        if (generate_151(map, stream))
            mainMask.set(10);

        if (generate_080(map, stream))
            mainMask.set(11);
        else
            map.appendError(mandatory("Target Address"));

        if (generate_073(map, stream))
            mainMask.set(12);

        if (generate_074(map, stream))
            mainMask.set(13);

        if (generate_075(map, stream))
            mainMask.set(14);

        if (generate_076(map, stream))
            mainMask.set(15);

        if (generate_140(map, stream))
            mainMask.set(16);

        if (generate_090(map, stream))
            mainMask.set(17);
        else
            map.appendError(mandatory("Quality Indicators"));

        if (generate_210(map, stream))
            mainMask.set(18);

        if (generate_070(map, stream))
            mainMask.set(19);

        if (generate_230(map, stream))
            mainMask.set(20);

        if (generate_145(map, stream))
            mainMask.set(21);

        if (generate_152(map, stream))
            mainMask.set(22);

        if (generate_200(map, stream))
            mainMask.set(23);

        if (generate_155(map, stream))
            mainMask.set(24);

        if (generate_157(map, stream))
            mainMask.set(25);

        if (generate_160(map, stream))
            mainMask.set(26);

        if (generate_165(map, stream))
            mainMask.set(27);

        if (generate_077(map, stream))
            mainMask.set(28);

        if (generate_170(map, stream))
            mainMask.set(29);

        if (generate_020(map, stream))
            mainMask.set(30);

        if (generate_220(map, stream))
            mainMask.set(31);

        if (generate_146(map, stream))
            mainMask.set(32);

        if (generate_148(map, stream))
            mainMask.set(33);

        if (generate_110(map, stream))
            mainMask.set(34);

        if (generate_016(map, stream))
            mainMask.set(35);

        if (generate_008(map, stream))
            mainMask.set(36);

        if (generate_271(map, stream))
            mainMask.set(37);

        if (generate_132(map, stream))
            mainMask.set(38);

        if (generate_250(map, stream))
            mainMask.set(39);
        else
            map.appendError(mandatory("Mode S MB Data"));

        if (generate_260(map, stream))
            mainMask.set(40);

        if (generate_400(map, stream))
            mainMask.set(41);

        if (generate_295(map, stream))
            mainMask.set(42);

        if (generate_re(map, stream))
            mainMask.set(48);

        if (generate_sp(map, stream))
            mainMask.set(49);
    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
    return result.prepend(mainMask.data());
}

void Asterix021Proto::parse_008(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(AircraftOperationalStatus_RA, data >> 7 & M1);
    map.insert(AircraftOperationalStatus_TC, data >> 5 & M2);
    map.insert(AircraftOperationalStatus_TS, data >> 4 & M1);
    map.insert(AircraftOperationalStatus_ARV, data >> 3 & M1);
    map.insert(AircraftOperationalStatus_CDTIA, data >> 2 & M1);
    map.insert(AircraftOperationalStatus_NotTCAS, data >> 1 & M1);
}

bool Asterix021Proto::generate_008(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, AircraftOperationalStatus_RA
        , AircraftOperationalStatus_TC
        , AircraftOperationalStatus_TS
        , AircraftOperationalStatus_ARV
        , AircraftOperationalStatus_CDTIA
        , AircraftOperationalStatus_NotTCAS))
    {
        quint8 data = (map.value(AircraftOperationalStatus_RA).toUInt() & M1) << 7;
        data |= (map.value(AircraftOperationalStatus_TC).toUInt() & M2) << 5;
        data |= (map.value(AircraftOperationalStatus_TS).toUInt() & M1) << 4;
        data |= (map.value(AircraftOperationalStatus_ARV).toUInt() & M1) << 3;
        data |= (map.value(AircraftOperationalStatus_CDTIA).toUInt() & M1) << 2;
        data |= (map.value(AircraftOperationalStatus_NotTCAS).toUInt() & M1) << 1;
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentification_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentification_SIC, read<quint8>(stream));
}

bool Asterix021Proto::generate_010(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, DataSourceIdentification_SAC, DataSourceIdentification_SIC))
    {
        write8(stream, map.value(DataSourceIdentification_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentification_SIC).toUInt());
        return true;
    }
    return false;
}

void Asterix021Proto::parse_015(QDataStream &stream, AsterixData &map)
{
    map.insert(ServiceIdentification, read<quint8>(stream));
}

bool Asterix021Proto::generate_015(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ServiceIdentification))
    {
        write8(stream, map.value(ServiceIdentification).toUInt());
        return true;
    }
    return false;
}

void Asterix021Proto::parse_016(QDataStream &stream, AsterixData &map)
{
    map.insert(ServiceManagement, double(read<quint8>(stream)) / 2);
}

bool Asterix021Proto::generate_016(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ServiceManagement))
    {
        write8(stream, map.value(ServiceManagement).toDouble() * 2);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    map.insert(EmitterCategory, read<quint8>(stream));
}

bool Asterix021Proto::generate_020(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, EmitterCategory))
    {
        write8(stream, map.value(EmitterCategory).toUInt());
        return true;
    }
    return false;
}

void Asterix021Proto::parse_040(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(TargetReportDescriptor_ATP, data >> 5 & M3);
    map.insert(TargetReportDescriptor_ARC, data >> 3 & M2);
    map.insert(TargetReportDescriptor_RC, data >> 2 & M1);
    map.insert(TargetReportDescriptor_RAB, data >> 1 & M1);

    if (data & M1)
    {
        data = read<quint8>(stream);
        map.insert(TargetReportDescriptor_DCR, data >> 7 & M1);
        map.insert(TargetReportDescriptor_GBS, data >> 6 & M1);
        map.insert(TargetReportDescriptor_SIM, data >> 5 & M1);
        map.insert(TargetReportDescriptor_TST, data >> 4 & M1);
        map.insert(TargetReportDescriptor_SAA, data >> 3 & M1);
        map.insert(TargetReportDescriptor_CL, data >> 1 & M2);

        if (data & M1)
        {
            data = read<quint8>(stream);
            map.insert(TargetReportDescriptor_EC7, data >> 7 & M1);
            map.insert(TargetReportDescriptor_EC6, data >> 6 & M1);
            map.insert(TargetReportDescriptor_IPC, data >> 5 & M1);
            map.insert(TargetReportDescriptor_NOGO, data >> 4 & M1);
            map.insert(TargetReportDescriptor_CPR, data >> 3 & M1);
            map.insert(TargetReportDescriptor_LDPJ, data >> 2 & M1);
            map.insert(TargetReportDescriptor_RCF, data >> 1 & M1);

            while (data & M1)
                data = read<quint8>(stream);
        }
    }
}

bool Asterix021Proto::generate_040(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TargetReportDescriptor_ATP, TargetReportDescriptor_ARC
                     , TargetReportDescriptor_RC, TargetReportDescriptor_RAB))
    {
        quint8 data = (map.value(TargetReportDescriptor_ATP).toUInt() & M3) << 5;
        data |= (map.value(TargetReportDescriptor_ARC).toUInt() & M2) << 3;
        data |= (map.value(TargetReportDescriptor_RC).toUInt() & M1) << 2;
        data |= (map.value(TargetReportDescriptor_RAB).toUInt() & M1) << 1;

        if (isContain(map, TargetReportDescriptor_DCR, TargetReportDescriptor_GBS, TargetReportDescriptor_SIM, TargetReportDescriptor_TST, TargetReportDescriptor_SAA, TargetReportDescriptor_CL))
        {
            write8(stream, data | M1);
            data = (map.value(TargetReportDescriptor_DCR).toUInt() & M1) << 7;
            data |= (map.value(TargetReportDescriptor_GBS).toUInt() & M1) << 6;
            data |= (map.value(TargetReportDescriptor_SIM).toUInt() & M1) << 5;
            data |= (map.value(TargetReportDescriptor_TST).toUInt() & M1) << 4;
            data |= (map.value(TargetReportDescriptor_SAA).toUInt() & M1) << 3;
            data |= (map.value(TargetReportDescriptor_CL).toUInt() & M2) << 1;

            if (isContain(map, TargetReportDescriptor_EC7, TargetReportDescriptor_EC6, TargetReportDescriptor_IPC, TargetReportDescriptor_NOGO, TargetReportDescriptor_CPR, TargetReportDescriptor_LDPJ, TargetReportDescriptor_RCF))
            {
                write8(stream, data | M1);
                data = (map.value(TargetReportDescriptor_EC7).toUInt() & M1) << 7;
                data |= (map.value(TargetReportDescriptor_EC6).toUInt() & M1) << 6;
                data |= (map.value(TargetReportDescriptor_IPC).toUInt() & M1) << 5;
                data |= (map.value(TargetReportDescriptor_NOGO).toUInt() & M1) << 4;
                data |= (map.value(TargetReportDescriptor_CPR).toUInt() & M1) << 3;
                data |= (map.value(TargetReportDescriptor_LDPJ).toUInt() & M1) << 2;
                data |= (map.value(TargetReportDescriptor_RCF).toUInt() & M1) << 1;
            }
        }

        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_070(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(Mode3ACodeInOctalRepresentation_A4, data >> 11 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_A2, data >> 10 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_A1, data >> 9 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_B4, data >> 8 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_B2, data >> 7 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_B1, data >> 6 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_C4, data >> 5 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_C2, data >> 4 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_C1, data >> 3 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_D4, data >> 2 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_D2, data >> 1 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_D1, data & M1);
}

bool Asterix021Proto::generate_070(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, Mode3ACodeInOctalRepresentation_A4
                     , Mode3ACodeInOctalRepresentation_A2
                     , Mode3ACodeInOctalRepresentation_A1
                     , Mode3ACodeInOctalRepresentation_B4
                     , Mode3ACodeInOctalRepresentation_B2
                     , Mode3ACodeInOctalRepresentation_B1
                     , Mode3ACodeInOctalRepresentation_C4
                     , Mode3ACodeInOctalRepresentation_C2
                     , Mode3ACodeInOctalRepresentation_C1
                     , Mode3ACodeInOctalRepresentation_D4
                     , Mode3ACodeInOctalRepresentation_D2
                     , Mode3ACodeInOctalRepresentation_D1))
    {
        quint16 data = (map.value(Mode3ACodeInOctalRepresentation_A4).toUInt() & M1) << 11;
        data |= (map.value(Mode3ACodeInOctalRepresentation_A2).toUInt() & M1) << 10;
        data |= (map.value(Mode3ACodeInOctalRepresentation_A1).toUInt() & M1) << 9;
        data |= (map.value(Mode3ACodeInOctalRepresentation_B4).toUInt() & M1) << 8;
        data |= (map.value(Mode3ACodeInOctalRepresentation_B2).toUInt() & M1) << 7;
        data |= (map.value(Mode3ACodeInOctalRepresentation_B1).toUInt() & M1) << 6;
        data |= (map.value(Mode3ACodeInOctalRepresentation_C4).toUInt() & M1) << 5;
        data |= (map.value(Mode3ACodeInOctalRepresentation_C2).toUInt() & M1) << 4;
        data |= (map.value(Mode3ACodeInOctalRepresentation_C1).toUInt() & M1) << 3;
        data |= (map.value(Mode3ACodeInOctalRepresentation_D4).toUInt() & M1) << 2;
        data |= (map.value(Mode3ACodeInOctalRepresentation_D2).toUInt() & M1) << 1;
        data |= map.value(Mode3ACodeInOctalRepresentation_D1).toUInt() & M1;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_071(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfApplicabilityForPosition, QTime().addSecs(ceil(double(read24(stream)) * 128)));
}

bool Asterix021Proto::generate_071(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfApplicabilityForPosition))
    {
        write24(stream, double(QTime(0,0).secsTo(map.value(TimeOfApplicabilityForPosition).toTime())) / 128);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_072(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfApplicabilityForVelocity, QTime().addSecs(ceil(double(read24(stream)) * 128)));
}

bool Asterix021Proto::generate_072(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfApplicabilityForVelocity))
    {
        write24(stream, double(QTime(0,0).msecsTo(map.value(TimeOfApplicabilityForVelocity).toTime())) / 128);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_073(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfMessageReceptionForPosition, QTime().addSecs(ceil(double(read24(stream)) * 128)));
}

bool Asterix021Proto::generate_073(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfMessageReceptionForPosition))
    {
        write24(stream, double(QTime(0,0).msecsTo(map.value(TimeOfMessageReceptionForPosition).toTime())) / 128);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_074(QDataStream &stream, AsterixData &map)
{
    quint32 data = read<quint32>(stream);
    map.insert(TimeOfMessageReceptionOfPositionHighPrecision_FSI, data >> 30 & M2);
    map.insert(TimeOfMessageReceptionOfPositionHighPrecision, double(data & M30) / 1073741824);
}

bool Asterix021Proto::generate_074(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfMessageReceptionOfPositionHighPrecision_FSI) && map.contains(TimeOfMessageReceptionOfPositionHighPrecision))
    {
        quint32 data = (map.value(TimeOfMessageReceptionOfPositionHighPrecision_FSI).toUInt() & M2) << 30;
        data |= quint32(map.value(TimeOfMessageReceptionOfPositionHighPrecision).toDouble() * 1073741824) & M30;
        write32(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_075(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfMessageReceptionForVelocity, QTime().addSecs(ceil(double(read24(stream)) * 128)));
}

bool Asterix021Proto::generate_075(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfMessageReceptionForVelocity))
    {
        write24(stream, double(QTime(0,0).secsTo(map.value(TimeOfMessageReceptionForVelocity).toTime())) / 128);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_076(QDataStream &stream, AsterixData &map)
{
    quint32 data = read<quint32>(stream);
    map.insert(TimeOfMessageReceptionOfVelocityHighPrecision_FSI, data >> 30 & M2);
    map.insert(TimeOfMessageReceptionOfVelocityHighPrecision, double(data & M30) / 1073741824);
}

bool Asterix021Proto::generate_076(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfMessageReceptionOfVelocityHighPrecision_FSI, TimeOfMessageReceptionOfVelocityHighPrecision))
    {
        quint32 data = (map.value(TimeOfMessageReceptionOfVelocityHighPrecision_FSI).toUInt() & M2) << 30;
        data |= quint32(map.value(TimeOfMessageReceptionOfVelocityHighPrecision).toDouble() * 1073741824) & M30;
        write32(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_077(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfAsterixReportTransmission, QTime().addSecs(ceil(double(read24(stream)) * 128)));
}

bool Asterix021Proto::generate_077(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfAsterixReportTransmission))
    {
        write24(stream, double(QTime(0,0).secsTo(map.value(TimeOfAsterixReportTransmission).toTime())) / 128);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_080(QDataStream &stream, AsterixData &map)
{
    map.insert(TargetAddress, read24(stream));
}

bool Asterix021Proto::generate_080(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TargetAddress))
    {
        write24(stream, map.value(TargetAddress).toUInt());
        return true;
    }
    return false;
}

void Asterix021Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(QualityIndicators_NUCrNACv, data >> 5 & M3);
    map.insert(QualityIndicators_NUCpNIC, data >> 1 & M4);

    QList<I021_090_FX> extension;
    while (data & M1)
    {
        data = read<quint8>(stream);
        I021_090_FX val = {0};
        val.NICbaro = data >> 7 & M1;
        val.SIL = data >> 5 & M2;
        val.NACp = data >> 1 & M4;
        extension.push_back(val);
    }

    if (!extension.isEmpty())
        map.insert(QualityIndicators_FX, QVariant::fromValue(extension));
}

bool Asterix021Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, QualityIndicators_NUCrNACv, QualityIndicators_NUCpNIC))
    {
        quint8 data = (map.value(QualityIndicators_NUCrNACv).toUInt() & M3) << 5;
        data |= (map.value(QualityIndicators_NUCpNIC).toUInt() & M4) << 1;

        if (map.contains(QualityIndicators_FX))
        {
            QList<I021_090_FX> extension = map.value(QualityIndicators_FX).value< QList<I021_090_FX> >();
            foreach (const I021_090_FX &val, extension)
            {
                write8(stream, data | M1);
                data = (val.NICbaro & M1) << 7;
                data |= (val.SIL & M2) << 5;
                data |= (val.NACp & M4) << 1;
            }
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_110(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.isSet(1))
    {
        quint8 data;
        QList<I021_110_1> extension;
        do
        {
            data = read<quint8>(stream);
            I021_110_1 val = {0};
            val.NAV = data >> 7 & M1;
            val.NVB = data >> 6 & M1;
            extension.push_back(val);
        } while (data & M1);
        map.insert(TrajectoryIntent_TIS, QVariant::fromValue(extension));
    }

    if (mask.isSet(2))
    {
        QList<I021_110_2> extension;
        for (quint8 rep = read<quint8>(stream); 0 != rep; --rep)
        {
            I021_110_2 val = {0};
            quint8 data = read<quint8>(stream);
            val.TCA = data >> 7 & M1;
            val.NC = data >> 6 & M1;
            val.TCPnumber = data & M6;
            val.altitude = double(read<qint16>(stream)) * 10;
            val.latitude = double(read24s(stream)) * 180 / 8388608;
            val.longitude = double(read24s(stream)) * 180 / 8388608;
            data = read<quint8>(stream);
            val.pointType = data >> 4 & M4;
            val.TD = data >> 2 & M2;
            val.TRA = data >> 1 & M1;
            val.TOA = data & M1;
            val.TOV = read24(stream);
            val.TTR = double(read<quint16>(stream)) / 100;
            extension.push_back(val);
        }
        map.insert(TrajectoryIntent_TID, QVariant::fromValue(extension));
    }
}

bool Asterix021Proto::generate_110(AsterixData &map, QDataStream &stream)
{
    PresentData subMask;
    QByteArray subArray;
    QDataStream subStream(&subArray, QIODevice::WriteOnly);

    if (isContain(map, TrajectoryIntent_TIS))
    {
        QList<I021_110_1> extension = map.value(TrajectoryIntent_TIS).value< QList<I021_110_1> >();
        if (!extension.isEmpty())
        {
            subMask.set(1);
            I021_110_1 val = extension.first();
            quint8 data = (val.NAV & M1) << 7;
            data |= (val.NVB & M1) << 6;
            for (int i = 1; i < extension.size(); ++i)
            {
                write8(subStream, data);
                val = extension.at(i);
                data = (val.NAV & M1) << 7;
                data |= (val.NVB & M1) << 6;
            }
            write8(subStream, data | M1);
        }
    }

    if (isContain(map, TrajectoryIntent_TID))
    {
        subMask.set(2);
        QList<I021_110_2> extension = map.value(TrajectoryIntent_TID).value< QList<I021_110_2> >();
        quint8 rep = calculateRepetition(extension);
        if (0 != rep)
        {
            write8(subStream, rep);
            foreach (const I021_110_2 &val, extension)
            {
                quint8 data = (val.TCA & M1) << 7;
                data |= (val.NC & M1) << 6;
                data |= val.TCPnumber & M6;
                write8(subStream, data);
                write16(subStream, val.altitude / 10);
                write24(subStream, val.latitude * 8388608 / 180);
                write24(subStream, val.longitude * 8388608 / 180);
                data = (val.pointType & M4) << 4;
                data |= (val.TD & M2) << 2;
                data |= (val.TRA & M1) << 1;
                data |= val.TOA & M1;
                write8(subStream, data);
                write24(subStream, val.TOV);
                write16(subStream, val.TTR * 100);
            }
        }
        else
            map.appendError(AsterixErrors::Error_REP);
    }

    if (!subMask.isEmpty())
    {
        writeArray(stream, subMask.data());
        writeArray(stream, subArray);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_130(QDataStream &stream, AsterixData &map)
{
    map.insert(PositionInWGS84Coordinates_Latitude, double(read24s(stream)) * 180 / 8388608);
    map.insert(PositionInWGS84Coordinates_Longitude, double(read24s(stream)) * 180 / 8388608);
}

bool Asterix021Proto::generate_130(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PositionInWGS84Coordinates_Latitude, PositionInWGS84Coordinates_Longitude))
    {
        write24(stream, map.value(PositionInWGS84Coordinates_Latitude).toDouble() * 8388608 / 180);
        write24(stream, map.value(PositionInWGS84Coordinates_Longitude).toDouble() * 8388608 / 180);
        return true;
    }
    return false;
}


void Asterix021Proto::parse_131(QDataStream &stream, AsterixData &map)
{
    map.insert(HighResolutionPositionInWGS84Coordinates_Latitude, double(read<qint32>(stream)) * 180 / 1073741824);
    map.insert(HighResolutionPositionInWGS84Coordinates_Longitude, double(read<qint32>(stream)) * 180 / 1073741824);
}

bool Asterix021Proto::generate_131(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, HighResolutionPositionInWGS84Coordinates_Latitude, HighResolutionPositionInWGS84Coordinates_Longitude))
    {
        write32(stream, map.value(HighResolutionPositionInWGS84Coordinates_Latitude).toDouble() * 1073741824 / 180);
        write32(stream, map.value(HighResolutionPositionInWGS84Coordinates_Longitude).toDouble() * 1073741824 / 180);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_132(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageAmplitude, read<qint8>(stream));
}

bool Asterix021Proto::generate_132(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, MessageAmplitude))
    {
        write8(stream, map.value(MessageAmplitude).toInt());
        return true;
    }
    return false;
}

void Asterix021Proto::parse_140(QDataStream &stream, AsterixData &map)
{
    map.insert(GeometricHeight, double(read<qint16>(stream)) * 25 / 4);
}

bool Asterix021Proto::generate_140(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, GeometricHeight))
    {
        write16(stream, map.value(GeometricHeight).toDouble() * 4 / 25);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_145(QDataStream &stream, AsterixData &map)
{
    // LSB = 1/4 FL = 25 feet
    map.insert(FlightLevel, double(read<qint16>(stream)) * 25.);
}

bool Asterix021Proto::generate_145(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, FlightLevel))
    {
        // LSB = 1/4 FL = 25 feet
        write16(stream, map.value(FlightLevel).toDouble() / 25.);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_146(QDataStream &stream, AsterixData &map)
{
    qint16 data = read<quint16>(stream);
    map.insert(IntermediateStateSelectedAltitude_SAS, data >> 15 & M1);
    map.insert(IntermediateStateSelectedAltitude_Source, data >> 13 & M2);
    map.insert(IntermediateStateSelectedAltitude_Altitude, double(restoreSign(data, 13)) * 25);
}

bool Asterix021Proto::generate_146(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, IntermediateStateSelectedAltitude_SAS, IntermediateStateSelectedAltitude_Source, IntermediateStateSelectedAltitude_Altitude))
    {
        quint16 data = (map.value(IntermediateStateSelectedAltitude_SAS).toUInt() & M1) << 15;
        data |= (map.value(IntermediateStateSelectedAltitude_Source).toUInt() & M2) << 13;
        data |= quint16(map.value(IntermediateStateSelectedAltitude_Altitude).toDouble() / 25) & M13;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_148(QDataStream &stream, AsterixData &map)
{
    qint16 data = read<quint16>(stream);
    map.insert(FinalStateSelectedAltitude_MV, data >> 15 & M1);
    map.insert(FinalStateSelectedAltitude_AH, data >> 14 & M1);
    map.insert(FinalStateSelectedAltitude_AM, data >> 13 & M1);
    map.insert(FinalStateSelectedAltitude_Altitude, double(restoreSign(data, 13)) * 25);
}

bool Asterix021Proto::generate_148(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, FinalStateSelectedAltitude_MV, FinalStateSelectedAltitude_AH, FinalStateSelectedAltitude_AM, FinalStateSelectedAltitude_Altitude))
    {
        quint16 data = (map.value(FinalStateSelectedAltitude_MV).toUInt() & M1) << 15;
        data |= (map.value(FinalStateSelectedAltitude_AH).toUInt() & M1) << 14;
        data |= (map.value(FinalStateSelectedAltitude_AM).toUInt() & M2) << 13;
        data |= quint16(map.value(FinalStateSelectedAltitude_Altitude).toDouble() / 25) & M13;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_150(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    bool isMach = data >> 15 & M1;
    map.insert(AirSpeed_IM, isMach);
    map.insert(AirSpeed, double(data & M15) / (isMach ? 1000 : 16384));
}

bool Asterix021Proto::generate_150(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, AirSpeed_IM, AirSpeed))
    {
        bool isMach = map.value(AirSpeed_IM).toBool();
        quint16 data = (isMach & M1) << 15;
        data |= qRound(map.value(AirSpeed).toDouble() * (isMach ? 1000 : 16384)) & M15;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_151(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(TrueAirspeed_RE, data >> 15 & M1);
    map.insert(TrueAirspeed, data & M15);
}

bool Asterix021Proto::generate_151(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TrueAirspeed_RE, TrueAirspeed))
    {
        quint16 data = (map.value(TrueAirspeed_RE).toUInt() & M1) << 15;
        data |= map.value(TrueAirspeed).toUInt() & M15;
        write16(stream, data);
        return true;
    }
    return false;
}


void Asterix021Proto::parse_152(QDataStream &stream, AsterixData &map)
{
    map.insert(MagneticHeading, double(read<quint16>(stream)) * 360 / 65536);
}

bool Asterix021Proto::generate_152(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, MagneticHeading))
    {
        write16(stream, map.value(MagneticHeading).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_155(QDataStream &stream, AsterixData &map)
{
    qint16 data = read<qint16>(stream);
    map.insert(BarometricVerticalRate_RE, data >> 15 & M1);
    map.insert(BarometricVerticalRate, double(restoreSign(data, 15)) * 25 / 4);
}

bool Asterix021Proto::generate_155(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, BarometricVerticalRate_RE, BarometricVerticalRate))
    {
        quint16 data = (map.value(BarometricVerticalRate_RE).toUInt() & M1) << 15;
        data |= qint16(map.value(BarometricVerticalRate).toDouble() * 4 / 25) & M15;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_157(QDataStream &stream, AsterixData &map)
{
    qint16 data = read<quint16>(stream);
    map.insert(GeometricVerticalRate_RE, data >> 15 & M1);
    map.insert(GeometricVerticalRate, double(restoreSign(data, 15)) * 25 / 4);
}

bool Asterix021Proto::generate_157(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, GeometricVerticalRate_RE, GeometricVerticalRate))
    {
        quint16 data = (map.value(GeometricVerticalRate_RE).toUInt() & M1) << 15;
        data |= qint16(map.value(GeometricVerticalRate).toDouble() * 4 / 25) & M15;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_160(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(GroundVector_RE, data >> 15 & M1);
    map.insert(GroundVector_Speed, double(data & M15) / 16384);
    map.insert(GroundVector_Angle, double(read<quint16>(stream)) * 360 / 65536);
}

bool Asterix021Proto::generate_160(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, GroundVector_RE, GroundVector_Speed, GroundVector_Angle))
    {
        quint16 data = (map.value(GroundVector_RE).toUInt() & M1) << 15;
        data |= quint16(map.value(GroundVector_Speed).toDouble() * 16384) & M15;
        write16(stream, data);
        write16(stream, map.value(GroundVector_Angle).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_161(QDataStream &stream, AsterixData &map)
{
    map.insert(TrackNumber, read<quint16>(stream) & M12);
}

bool Asterix021Proto::generate_161(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TrackNumber))
    {
        write16(stream, map.value(TrackNumber).toUInt() & M12);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_165(QDataStream &stream, AsterixData &map)
{
    map.insert(TrackAngleRate, double(read<qint16>(stream) & M10) / 32);
}

bool Asterix021Proto::generate_165(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TrackAngleRate))
    {
        write16(stream, qint16(map.value(TrackAngleRate).toDouble() * 32) & M10);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_170(QDataStream &stream, AsterixData &map)
{
    map.insert(TargetIdentification, readCharacters(stream, 2));
}

bool Asterix021Proto::generate_170(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TargetIdentification))
    {
        QByteArray data = map.value(TargetIdentification).toByteArray();
        if (8 == data.size())
        {
            writeCharacters(stream, data);
            return true;
        }
    }
    return false;
}

void Asterix021Proto::parse_200(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(TargetStatus_PS, data >> 2 & M3);
    map.insert(TargetStatus_SS, data & M2);
}

bool Asterix021Proto::generate_200(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TargetStatus_PS, TargetStatus_SS))
    {
        quint16 data = (map.value(TargetStatus_PS).toUInt() & M3) << 2;
        data |= map.value(TargetStatus_SS).toUInt() & M2;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_210(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(MopsVersion_VNS, data >> 6 & M1);
    map.insert(MopsVersion_VN, data >> 3 & M3);
    map.insert(MopsVersion_LTT, data & M3);
}

bool Asterix021Proto::generate_210(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, MopsVersion_VNS, MopsVersion_VN, MopsVersion_LTT))
    {
        quint8 data = (map.value(MopsVersion_VNS).toUInt() & M1) << 6;
        data |= (map.value(MopsVersion_VN).toUInt() & M3) << 3;
        data |= map.value(MopsVersion_LTT).toUInt() & M3;
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_220(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.isSet(1))
        map.insert(MetInformation_WindSpeed, read<quint16>(stream));

    if (mask.isSet(2))
        map.insert(MetInformation_WindDirection, read<quint16>(stream));

    if (mask.isSet(3))
        map.insert(MetInformation_Temperature, double(read<qint16>(stream)) / 4);

    if (mask.isSet(4))
        map.insert(MetInformation_Turbulence, read<quint8>(stream));
}

bool Asterix021Proto::generate_220(AsterixData &map, QDataStream &stream)
{
    PresentData subMask;
    QByteArray subArray;
    QDataStream subStream(&subArray, QIODevice::WriteOnly);

    if (isContain(map, MetInformation_WindSpeed))
    {
        subMask.set(1);
        write16(subStream, map.value(MetInformation_WindSpeed).toUInt());
    }

    if (isContain(map, MetInformation_WindDirection))
    {
        subMask.set(2);
        write16(subStream, map.value(MetInformation_WindDirection).toUInt());
    }

    if (isContain(map, MetInformation_Temperature))
    {
        subMask.set(3);
        write16(subStream, map.value(MetInformation_Temperature).toDouble() * 4);
    }

    if (isContain(map, MetInformation_Turbulence))
    {
        subMask.set(4);
        write8(subStream, map.value(MetInformation_Turbulence).toUInt());
    }

    if (!subMask.isEmpty())
    {
        writeArray(stream, subMask.data());
        writeArray(stream, subArray);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_230(QDataStream &stream, AsterixData &map)
{
    map.insert(RollAngle, double(read<qint16>(stream)) / 100);
}

bool Asterix021Proto::generate_230(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, RollAngle))
    {
        write16(stream, map.value(RollAngle).toDouble() * 100);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_250(QDataStream &stream, AsterixData &map)
{
    QList<I021_250> repData;
    for (quint8 rep = read<quint8>(stream); 0 != rep; --rep)
    {
        I021_250 data = {{0}};
        for (unsigned int i = 0; i < sizeof(data.MbData); ++i)
            data.MbData[i] = read<quint8>(stream);
        quint8 dbs = read<quint8>(stream);
        data.BDS1 = dbs >> 4 & M4;
        data.BDS2 = dbs & M4;
        repData.push_back(data);
    }
    if (!repData.isEmpty())
        map.insert(ModeSMbData, QVariant::fromValue(repData));
}

bool Asterix021Proto::generate_250(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ModeSMbData))
    {
        QList<I021_250> repData = map.value(ModeSMbData).value< QList<I021_250> >();
        quint8 rep = calculateRepetition(repData);
        if (0 != rep)
        {
            write8(stream, calculateRepetition(repData));
            foreach (const I021_250 &val, repData)
            {
                for (unsigned int i = 0; i < sizeof(val.MbData); ++i)
                    write8(stream, val.MbData[i]);
                quint8 dbs = (val.BDS1 & M4) << 4;
                dbs |= val.BDS2 & M4;
                write8(stream, dbs);
            }
            return true;
        }
        map.appendError(AsterixErrors::Error_REP);
    }
    return false;
}

void Asterix021Proto::parse_260(QDataStream &stream, AsterixData &map)
{
    map.insert(AcasResolutionAdvisoryReport, readArray(stream, 7));
}

bool Asterix021Proto::generate_260(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, AcasResolutionAdvisoryReport))
    {
        QByteArray data = map.value(AcasResolutionAdvisoryReport).toByteArray();
        if (7 == data.size())
        {
            writeArray(stream, data);
            return true;
        }
    }
    return false;
}

void Asterix021Proto::parse_271(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(SurfaceCapabilitiesAndCharacteristics_POA, data >> 5 & M1);
    map.insert(SurfaceCapabilitiesAndCharacteristics_CDTIS, data >> 4 & M1);
    map.insert(SurfaceCapabilitiesAndCharacteristics_B2low, data >> 3 & M1);
    map.insert(SurfaceCapabilitiesAndCharacteristics_RAS, data >> 2 & M1);
    map.insert(SurfaceCapabilitiesAndCharacteristics_IDENT, data >> 1 & M1);

    if (data & M1)
        map.insert(SurfaceCapabilitiesAndCharacteristics_LengthAndWidth, read<quint8>(stream) & M4);
}

bool Asterix021Proto::generate_271(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, SurfaceCapabilitiesAndCharacteristics_POA, SurfaceCapabilitiesAndCharacteristics_CDTIS, SurfaceCapabilitiesAndCharacteristics_B2low, SurfaceCapabilitiesAndCharacteristics_RAS, SurfaceCapabilitiesAndCharacteristics_IDENT))
    {
        quint8 data = (map.value(SurfaceCapabilitiesAndCharacteristics_POA).toUInt() & M1) << 5;
        data |= (map.value(SurfaceCapabilitiesAndCharacteristics_CDTIS).toUInt() & M1) << 4;
        data |= (map.value(SurfaceCapabilitiesAndCharacteristics_B2low).toUInt() & M1) << 3;
        data |= (map.value(SurfaceCapabilitiesAndCharacteristics_RAS).toUInt() & M1) << 2;
        data |= (map.value(SurfaceCapabilitiesAndCharacteristics_IDENT).toUInt() & M1) << 1;
        if (isContain(map, SurfaceCapabilitiesAndCharacteristics_LengthAndWidth))
        {
            write8(stream, data | M1);
            data = map.value(SurfaceCapabilitiesAndCharacteristics_LengthAndWidth).toUInt() & M4;
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_295(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.isSet(1))
        map.insert(DataAges_AircraftOperationalStatusAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(2))
        map.insert(DataAges_TargetReportDescriptorAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(3))
        map.insert(DataAges_Mode3AAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(4))
        map.insert(DataAges_QualityIndicatorsAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(5))
        map.insert(DataAges_TrajectoryIntentAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(6))
        map.insert(DataAges_MessageAmplitudeAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(7))
        map.insert(DataAges_GeometricHeightAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(8))
        map.insert(DataAges_FlightLevelAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(9))
        map.insert(DataAges_IntermediateStateSelectedAltitudeAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(10))
        map.insert(DataAges_FinalStateSelectedAltitudeAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(11))
        map.insert(DataAges_AirSpeedAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(12))
        map.insert(DataAges_TrueAirSpeedAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(13))
        map.insert(DataAges_MagneticHeadingAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(14))
        map.insert(DataAges_BarometricVerticalRateAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(15))
        map.insert(DataAges_GeometricVerticalRateAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(16))
        map.insert(DataAges_GroundVectorAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(17))
        map.insert(DataAges_TrackRateAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(18))
        map.insert(DataAges_TargetIdentificationAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(19))
        map.insert(DataAges_TargetStatusAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(20))
        map.insert(DataAges_MetInformationAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(21))
        map.insert(DataAges_RollAngleAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(22))
        map.insert(DataAges_AcasResolutionAdvisoryAge, double(read<quint8>(stream)) / 10);

    if (mask.isSet(23))
        map.insert(DataAges_SurfaceCapabilitiesAndCharacteristicsAge, double(read<quint8>(stream)) / 10);
}

bool Asterix021Proto::generate_295(AsterixData &map, QDataStream &stream)
{
    PresentData subMask;
    QByteArray subArray;
    QDataStream subStream(&subArray, QIODevice::WriteOnly);

    if (isContain(map, DataAges_AircraftOperationalStatusAge))
    {
        subMask.set(1);
        write8(subStream, map.value(DataAges_AircraftOperationalStatusAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_TargetReportDescriptorAge))
    {
        subMask.set(2);
        write8(subStream, map.value(DataAges_TargetReportDescriptorAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_Mode3AAge))
    {
        subMask.set(3);
        write8(subStream, map.value(DataAges_Mode3AAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_QualityIndicatorsAge))
    {
        subMask.set(4);
        write8(subStream, map.value(DataAges_QualityIndicatorsAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_TrajectoryIntentAge))
    {
        subMask.set(5);
        write8(subStream, map.value(DataAges_TrajectoryIntentAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_MessageAmplitudeAge))
    {
        subMask.set(6);
        write8(subStream, map.value(DataAges_MessageAmplitudeAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_GeometricHeightAge))
    {
        subMask.set(7);
        write8(subStream, map.value(DataAges_GeometricHeightAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_FlightLevelAge))
    {
        subMask.set(8);
        write8(subStream, map.value(DataAges_FlightLevelAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_IntermediateStateSelectedAltitudeAge))
    {
        subMask.set(9);
        write8(subStream, map.value(DataAges_IntermediateStateSelectedAltitudeAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_FinalStateSelectedAltitudeAge))
    {
        subMask.set(10);
        write8(subStream, map.value(DataAges_FinalStateSelectedAltitudeAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_AirSpeedAge))
    {
        subMask.set(11);
        write8(subStream, map.value(DataAges_AirSpeedAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_TrueAirSpeedAge))
    {
        subMask.set(12);
        write8(subStream, map.value(DataAges_TrueAirSpeedAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_MagneticHeadingAge))
    {
        subMask.set(13);
        write8(subStream, map.value(DataAges_MagneticHeadingAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_BarometricVerticalRateAge))
    {
        subMask.set(14);
        write8(subStream, map.value(DataAges_BarometricVerticalRateAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_GeometricVerticalRateAge))
    {
        subMask.set(15);
        write8(subStream, map.value(DataAges_GeometricVerticalRateAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_GroundVectorAge))
    {
        subMask.set(16);
        write8(subStream, map.value(DataAges_GroundVectorAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_TrackRateAge))
    {
        subMask.set(17);
        write8(subStream, map.value(DataAges_TrackRateAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_TargetIdentificationAge))
    {
        subMask.set(18);
        write8(subStream, map.value(DataAges_TargetIdentificationAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_TargetStatusAge))
    {
        subMask.set(19);
        write8(subStream, map.value(DataAges_TargetStatusAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_MetInformationAge))
    {
        subMask.set(20);
        write8(subStream, map.value(DataAges_MetInformationAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_RollAngleAge))
    {
        subMask.set(21);
        write8(subStream, map.value(DataAges_RollAngleAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_AcasResolutionAdvisoryAge))
    {
        subMask.set(22);
        write8(subStream, map.value(DataAges_AcasResolutionAdvisoryAge).toDouble() * 10);
    }

    if (isContain(map, DataAges_SurfaceCapabilitiesAndCharacteristicsAge))
    {
        subMask.set(23);
        write8(subStream, map.value(DataAges_SurfaceCapabilitiesAndCharacteristicsAge).toDouble() * 10);
    }

    if (!subMask.isEmpty())
    {
        writeArray(stream, subMask.data());

        writeArray(stream, subArray);
        return true;
    }
    return false;
}

void Asterix021Proto::parse_400(QDataStream &stream, AsterixData &map)
{
    map.insert(ReceiverId, read<quint8>(stream));
}

bool Asterix021Proto::generate_400(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ReceiverId))
    {
        write8(stream, map.value(ReceiverId).toUInt());
        return true;
    }
    return false;
}

QStringList Asterix021Proto::fields() const
{
    static QStringList result = QStringList()
            << AircraftOperationalStatus_RA
            << AircraftOperationalStatus_TC
            << AircraftOperationalStatus_TS
            << AircraftOperationalStatus_ARV
            << AircraftOperationalStatus_CDTIA
            << AircraftOperationalStatus_NotTCAS

            // Data Item I021/010, Data Source Identification
            << DataSourceIdentification_SAC
            << DataSourceIdentification_SIC

            // Data Item I021/015, Service Identification
            << ServiceIdentification

            // Data Item I021/016, Service Management
            << ServiceManagement

            // Data Item I021/020, Emitter Category
            << EmitterCategory

            // Data Item I021/040, Target Report Descriptor
            << TargetReportDescriptor_ATP
            << TargetReportDescriptor_ARC
            << TargetReportDescriptor_RC
            << TargetReportDescriptor_RAB

            << TargetReportDescriptor_DCR
            << TargetReportDescriptor_GBS
            << TargetReportDescriptor_SIM
            << TargetReportDescriptor_TST
            << TargetReportDescriptor_SAA
            << TargetReportDescriptor_CL

            << TargetReportDescriptor_EC7
            << TargetReportDescriptor_EC6
            << TargetReportDescriptor_IPC
            << TargetReportDescriptor_NOGO
            << TargetReportDescriptor_CPR
            << TargetReportDescriptor_LDPJ
            << TargetReportDescriptor_RCF

            // Data Item I021/070, Mode 3/A Code in Octal Representation
            << Mode3ACodeInOctalRepresentation_A4
            << Mode3ACodeInOctalRepresentation_A2
            << Mode3ACodeInOctalRepresentation_A1
            << Mode3ACodeInOctalRepresentation_B4
            << Mode3ACodeInOctalRepresentation_B2
            << Mode3ACodeInOctalRepresentation_B1
            << Mode3ACodeInOctalRepresentation_C4
            << Mode3ACodeInOctalRepresentation_C2
            << Mode3ACodeInOctalRepresentation_C1
            << Mode3ACodeInOctalRepresentation_D4
            << Mode3ACodeInOctalRepresentation_D2
            << Mode3ACodeInOctalRepresentation_D1

            // Data Item I021/071, Time of Applicability for Position
            << TimeOfApplicabilityForPosition

            // Data Item I021/072, Time of Applicability for Velocity
            << TimeOfApplicabilityForVelocity

            // Data Item I021/073, Time of Message Reception for Position
            << TimeOfMessageReceptionForPosition

            // Data Item I021/074, Time of Message Reception of Position–High Precision
            << TimeOfMessageReceptionOfPositionHighPrecision
            << TimeOfMessageReceptionOfPositionHighPrecision_FSI

            // Data Item I021/075, Time of Message Reception for Velocity
            << TimeOfMessageReceptionForVelocity

            // Data Item I021/076, Time of Message Reception of Velocity–High Precision
            << TimeOfMessageReceptionOfVelocityHighPrecision
            << TimeOfMessageReceptionOfVelocityHighPrecision_FSI

            // Data Item I021/077, Time of ASTERIX Report Transmission
            << TimeOfAsterixReportTransmission

            // Data Item I021/080, Target Address
            << TargetAddress

            // Data Item I021/090, Quality Indicators
            << QualityIndicators_NUCrNACv
            << QualityIndicators_NUCpNIC
            << QualityIndicators_FX

            // Data Item I021/110, Trajectory Intent
            << TrajectoryIntent_TIS
            << TrajectoryIntent_TID

            // Data Item I021/130, Position in WGS-84 Co-ordinates
            << PositionInWGS84Coordinates_Latitude
            << PositionInWGS84Coordinates_Longitude

            // Data Item I021/131, High-Resolution Position in WGS-84 Co-ordinates
            << HighResolutionPositionInWGS84Coordinates_Latitude
            << HighResolutionPositionInWGS84Coordinates_Longitude

            // Data Item I021/132, Message Amplitude
            << MessageAmplitude

            // Data Item I021/140, Geometric Height
            << GeometricHeight

            // Data Item I021/145, Flight Level
            << FlightLevel

            // Data Item I021/146, Intermediate State Selected Altitude
            << IntermediateStateSelectedAltitude_SAS
            << IntermediateStateSelectedAltitude_Source
            << IntermediateStateSelectedAltitude_Altitude

            // Data Item I021/148, Final State Selected Altitude
            << FinalStateSelectedAltitude_MV
            << FinalStateSelectedAltitude_AH
            << FinalStateSelectedAltitude_AM
            << FinalStateSelectedAltitude_Altitude

            // Data Item I021/150, Air Speed
            << AirSpeed
            << AirSpeed_IM

            // Data Item I021/151 True Airspeed
            << TrueAirspeed
            << TrueAirspeed_RE

            // Data Item I021/152, Magnetic Heading
            << MagneticHeading

            // Data Item I021/155, Barometric Vertical Rate
            << BarometricVerticalRate
            << BarometricVerticalRate_RE

            // Data Item I021/157, Geometric Vertical Rate
            << GeometricVerticalRate
            << GeometricVerticalRate_RE

            // Data Item I021/160, Ground Vector
            << GroundVector_RE
            << GroundVector_Speed
            << GroundVector_Angle

            // Data Item I021/161, Track Number
            << TrackNumber

            // Data Item I021/165, Track Angle Rate
            << TrackAngleRate

            // Data Item I021/170, Target Identification
            << TargetIdentification

            // Data Item I021/200, Target Status
            << TargetStatus_PS
            << TargetStatus_SS

            // Data Item I021/210, MOPS Version
            << MopsVersion_VNS
            << MopsVersion_VN
            << MopsVersion_LTT

            // Data Item I021/220, Met Information
            << MetInformation_WindSpeed
            << MetInformation_WindDirection
            << MetInformation_Temperature
            << MetInformation_Turbulence

            // Data Item I021/230, Roll Angle
            << RollAngle

            // Data Item I021/250, Mode S MB Data
            << ModeSMbData

            // Data Item I021/260, ACAS Resolution Advisory Report
            << AcasResolutionAdvisoryReport

            // Data Item I021/271, Surface Capabilities and Characteristics
            << SurfaceCapabilitiesAndCharacteristics_POA
            << SurfaceCapabilitiesAndCharacteristics_CDTIS
            << SurfaceCapabilitiesAndCharacteristics_B2low
            << SurfaceCapabilitiesAndCharacteristics_RAS
            << SurfaceCapabilitiesAndCharacteristics_IDENT
            << SurfaceCapabilitiesAndCharacteristics_LengthAndWidth

            // Data Item I021/295, Data Ages
            << DataAges_AircraftOperationalStatusAge
            << DataAges_TargetReportDescriptorAge
            << DataAges_Mode3AAge
            << DataAges_QualityIndicatorsAge
            << DataAges_TrajectoryIntentAge
            << DataAges_MessageAmplitudeAge
            << DataAges_GeometricHeightAge
            << DataAges_FlightLevelAge
            << DataAges_IntermediateStateSelectedAltitudeAge
            << DataAges_FinalStateSelectedAltitudeAge
            << DataAges_AirSpeedAge
            << DataAges_TrueAirSpeedAge
            << DataAges_MagneticHeadingAge
            << DataAges_BarometricVerticalRateAge
            << DataAges_GeometricVerticalRateAge
            << DataAges_GroundVectorAge
            << DataAges_TrackRateAge
            << DataAges_TargetIdentificationAge
            << DataAges_TargetStatusAge
            << DataAges_MetInformationAge
            << DataAges_RollAngleAge
            << DataAges_AcasResolutionAdvisoryAge
            << DataAges_SurfaceCapabilitiesAndCharacteristicsAge

            // Data Item I021/400, Receiver ID
            << ReceiverId

            << SpecialPurposeField
            << ReservedExpansionField;
    return result;
}
