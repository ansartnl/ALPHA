#include "asterix048.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


const int Asterix048Proto::mCategory = 48;
const QString Asterix048Proto::mEdition = "1.19";

const QString Asterix048Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix048Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix048Proto::TargetReportDescriptor_TYP = "a048.TargetReportDescriptor.TYP";
const QString Asterix048Proto::TargetReportDescriptor_SIM = "a048.TargetReportDescriptor.SIM";
const QString Asterix048Proto::TargetReportDescriptor_RDP = "a048.TargetReportDescriptor.RDP";
const QString Asterix048Proto::TargetReportDescriptor_SPI = "a048.TargetReportDescriptor.SPI";
const QString Asterix048Proto::TargetReportDescriptor_RAB = "a048.TargetReportDescriptor.RAB";
const QString Asterix048Proto::TargetReportDescriptor_FX = "a048.TargetReportDescriptor.FX";

const QString Asterix048Proto::WarningErrorConditions = "a048.WarningErrorConditions";

const QString Asterix048Proto::MeasuredPositionInPolarCoordinates_RHO = "a048.MeasuredPositionInPolarCoordinates.RHO";
const QString Asterix048Proto::MeasuredPositionInPolarCoordinates_THETA = "a048.MeasuredPositionInPolarCoordinates.THETA";

const QString Asterix048Proto::CalculatedPositionInCartesianCoordinates_X = "a048.CalculatedPositionInCartesianCoordinates.X";
const QString Asterix048Proto::CalculatedPositionInCartesianCoordinates_Y = "a048.CalculatedPositionInCartesianCoordinates.Y";

const QString Asterix048Proto::Mode2CodeInOctalRepresentation_V = "a048.Mode2CodeInOctalRepresentation.V";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_G = "a048.Mode2CodeInOctalRepresentation.G";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_L = "a048.Mode2CodeInOctalRepresentation.L";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_A4 = "a048.Mode2CodeInOctalRepresentation.A4";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_A2 = "a048.Mode2CodeInOctalRepresentation.A2";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_A1 = "a048.Mode2CodeInOctalRepresentation.A1";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_B4 = "a048.Mode2CodeInOctalRepresentation.B4";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_B2 = "a048.Mode2CodeInOctalRepresentation.B2";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_B1 = "a048.Mode2CodeInOctalRepresentation.B1";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_C4 = "a048.Mode2CodeInOctalRepresentation.C4";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_C2 = "a048.Mode2CodeInOctalRepresentation.C2";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_C1 = "a048.Mode2CodeInOctalRepresentation.C1";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_D4 = "a048.Mode2CodeInOctalRepresentation.D4";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_D2 = "a048.Mode2CodeInOctalRepresentation.D2";
const QString Asterix048Proto::Mode2CodeInOctalRepresentation_D1 = "a048.Mode2CodeInOctalRepresentation.D1";

const QString Asterix048Proto::Mode1CodeInOctalRepresentation_V = "a048.Mode1CodeInOctalRepresentation.V";
const QString Asterix048Proto::Mode1CodeInOctalRepresentation_G = "a048.Mode1CodeInOctalRepresentation.G";
const QString Asterix048Proto::Mode1CodeInOctalRepresentation_L = "a048.Mode1CodeInOctalRepresentation.L";
const QString Asterix048Proto::Mode1CodeInOctalRepresentation_A4 = "a048.Mode1CodeInOctalRepresentation.A4";
const QString Asterix048Proto::Mode1CodeInOctalRepresentation_A2 = "a048.Mode1CodeInOctalRepresentation.A2";
const QString Asterix048Proto::Mode1CodeInOctalRepresentation_A1 = "a048.Mode1CodeInOctalRepresentation.A1";
const QString Asterix048Proto::Mode1CodeInOctalRepresentation_B2 = "a048.Mode1CodeInOctalRepresentation.B2";
const QString Asterix048Proto::Mode1CodeInOctalRepresentation_B1 = "a048.Mode1CodeInOctalRepresentation.B1";

const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QA4 = "a048.Mode2CodeConfidenceIndicator.QA4";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QA2 = "a048.Mode2CodeConfidenceIndicator.QA2";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QA1 = "a048.Mode2CodeConfidenceIndicator.QA1";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QB4 = "a048.Mode2CodeConfidenceIndicator.QB4";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QB2 = "a048.Mode2CodeConfidenceIndicator.QB2";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QB1 = "a048.Mode2CodeConfidenceIndicator.QB1";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QC4 = "a048.Mode2CodeConfidenceIndicator.QC4";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QC2 = "a048.Mode2CodeConfidenceIndicator.QC2";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QC1 = "a048.Mode2CodeConfidenceIndicator.QC1";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QD4 = "a048.Mode2CodeConfidenceIndicator.QD4";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QD2 = "a048.Mode2CodeConfidenceIndicator.QD2";
const QString Asterix048Proto::Mode2CodeConfidenceIndicator_QD1 = "a048.Mode2CodeConfidenceIndicator.QD1";

const QString Asterix048Proto::Mode1CodeConfidenceIndicator_QA4 = "a048.Mode1CodeConfidenceIndicator.QA4";
const QString Asterix048Proto::Mode1CodeConfidenceIndicator_QA2 = "a048.Mode1CodeConfidenceIndicator.QA2";
const QString Asterix048Proto::Mode1CodeConfidenceIndicator_QA1 = "a048.Mode1CodeConfidenceIndicator.QA1";
const QString Asterix048Proto::Mode1CodeConfidenceIndicator_QB2 = "a048.Mode1CodeConfidenceIndicator.QB2";
const QString Asterix048Proto::Mode1CodeConfidenceIndicator_QB1 = "a048.Mode1CodeConfidenceIndicator.QB1";

const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_V = "a048.Mode3ACodeInOctalRepresentation.V";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_G = "a048.Mode3ACodeInOctalRepresentation.G";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_L = "a048.Mode3ACodeInOctalRepresentation.L";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_A4 = "a048.Mode3ACodeInOctalRepresentation.A4";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_A2 = "a048.Mode3ACodeInOctalRepresentation.A2";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_A1 = "a048.Mode3ACodeInOctalRepresentation.A1";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_B4 = "a048.Mode3ACodeInOctalRepresentation.B4";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_B2 = "a048.Mode3ACodeInOctalRepresentation.B2";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_B1 = "a048.Mode3ACodeInOctalRepresentation.B1";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_C4 = "a048.Mode3ACodeInOctalRepresentation.C4";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_C2 = "a048.Mode3ACodeInOctalRepresentation.C2";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_C1 = "a048.Mode3ACodeInOctalRepresentation.C1";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_D4 = "a048.Mode3ACodeInOctalRepresentation.D4";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_D2 = "a048.Mode3ACodeInOctalRepresentation.D2";
const QString Asterix048Proto::Mode3ACodeInOctalRepresentation_D1 = "a048.Mode3ACodeInOctalRepresentation.D1";

const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QA4 = "a048.Mode3ACodeConfidenceIndicator.QA4";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QA2 = "a048.Mode3ACodeConfidenceIndicator.QA2";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QA1 = "a048.Mode3ACodeConfidenceIndicator.QA1";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QB4 = "a048.Mode3ACodeConfidenceIndicator.QB4";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QB2 = "a048.Mode3ACodeConfidenceIndicator.QB2";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QB1 = "a048.Mode3ACodeConfidenceIndicator.QB1";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QC4 = "a048.Mode3ACodeConfidenceIndicator.QC4";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QC2 = "a048.Mode3ACodeConfidenceIndicator.QC2";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QC1 = "a048.Mode3ACodeConfidenceIndicator.QC1";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QD4 = "a048.Mode3ACodeConfidenceIndicator.QD4";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QD2 = "a048.Mode3ACodeConfidenceIndicator.QD2";
const QString Asterix048Proto::Mode3ACodeConfidenceIndicator_QD1 = "a048.Mode3ACodeConfidenceIndicator.QD1";

const QString Asterix048Proto::FlightLevelInBinaryRepresentation_V = "a048.FlightLevelInBinaryRepresentation.V";
const QString Asterix048Proto::FlightLevelInBinaryRepresentation_G = "a048.FlightLevelInBinaryRepresentation.G";
const QString Asterix048Proto::FlightLevelInBinaryRepresentation_FlightLevel = "a048.FlightLevelInBinaryRepresentation.FlightLevel";

const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_V = "a048.ModeCCodeAndCodeConfidenceIndicator.V";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_G = "a048.ModeCCodeAndCodeConfidenceIndicator.G";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_C1 = "a048.ModeCCodeAndCodeConfidenceIndicator.C1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_A1 = "a048.ModeCCodeAndCodeConfidenceIndicator.A1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_C2 = "a048.ModeCCodeAndCodeConfidenceIndicator.C2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_A2 = "a048.ModeCCodeAndCodeConfidenceIndicator.A2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_C4 = "a048.ModeCCodeAndCodeConfidenceIndicator.C4";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_A4 = "a048.ModeCCodeAndCodeConfidenceIndicator.A4";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_B1 = "a048.ModeCCodeAndCodeConfidenceIndicator.B1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_D1 = "a048.ModeCCodeAndCodeConfidenceIndicator.D1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_B2 = "a048.ModeCCodeAndCodeConfidenceIndicator.B2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_D2 = "a048.ModeCCodeAndCodeConfidenceIndicator.D2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_B4 = "a048.ModeCCodeAndCodeConfidenceIndicator.B4";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_D4 = "a048.ModeCCodeAndCodeConfidenceIndicator.D4";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QC1 = "a048.ModeCCodeAndCodeConfidenceIndicator.QC1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QA1 = "a048.ModeCCodeAndCodeConfidenceIndicator.QA1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QC2 = "a048.ModeCCodeAndCodeConfidenceIndicator.QC2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QA2 = "a048.ModeCCodeAndCodeConfidenceIndicator.QA2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QC4 = "a048.ModeCCodeAndCodeConfidenceIndicator.QC4";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QA4 = "a048.ModeCCodeAndCodeConfidenceIndicator.QA4";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QB1 = "a048.ModeCCodeAndCodeConfidenceIndicator.QB1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QD1 = "a048.ModeCCodeAndCodeConfidenceIndicator.QD1";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QB2 = "a048.ModeCCodeAndCodeConfidenceIndicator.QB2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QD2 = "a048.ModeCCodeAndCodeConfidenceIndicator.QD2";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QB4 = "a048.ModeCCodeAndCodeConfidenceIndicator.QB4";
const QString Asterix048Proto::ModeCCodeAndCodeConfidenceIndicator_QD4 = "a048.ModeCCodeAndCodeConfidenceIndicator.QD4";

const QString Asterix048Proto::HeightMeasuredBy3DRadar = "a048.HeightMeasuredBy3DRadar";

const QString Asterix048Proto::RadialDopplerSpeed_CalculatedDopplerSpeed_D = "a048.RadialDopplerSpeed.CalculatedDopplerSpeed.D";
const QString Asterix048Proto::RadialDopplerSpeed_CalculatedDopplerSpeed_CAL = "a048.RadialDopplerSpeed.CalculatedDopplerSpeed.CAL";
const QString Asterix048Proto::RadialDopplerSpeed_RawDopplerSpeed = "a048.RadialDopplerSpeed.RawDopplerSpeed";

const QString Asterix048Proto::RadarPlotCharacteristics_SsrPlotRunlength = "a048.RadarPlotCharacteristics.SsrPlotRunlength";
const QString Asterix048Proto::RadarPlotCharacteristics_NumberOfReceivedRepliesForMSsr = "a048.RadarPlotCharacteristics.NumberOfReceivedRepliesForMSsr";
const QString Asterix048Proto::RadarPlotCharacteristics_AmplitudeOfMSSRReply = "a048.RadarPlotCharacteristics.AmplitudeOfMSSRReply";
const QString Asterix048Proto::RadarPlotCharacteristics_PrimaryPlotRunlength = "a048.RadarPlotCharacteristics.PrimaryPlotRunlength";
const QString Asterix048Proto::RadarPlotCharacteristics_AmplitudeOfPrimaryPlot = "a048.RadarPlotCharacteristics.AmplitudeOfPrimaryPlot";
const QString Asterix048Proto::RadarPlotCharacteristics_DifferenceInRangeBetweenPsrAndSsrPlot = "a048.RadarPlotCharacteristics.DifferenceInRangeBetweenPsrAndSsrPlot";
const QString Asterix048Proto::RadarPlotCharacteristics_DifferenceInAzimuthBetweenPsrSndSsrPlot = "a048.RadarPlotCharacteristics.DifferenceInAzimuthBetweenPsrSndSsrPlot";

const QString Asterix048Proto::TimeOfDay = "a048.TimeOfDay";

const QString Asterix048Proto::TrackNumber = "a048.TrackNumber";

const QString Asterix048Proto::TrackStatus_CNF = "a048.TrackStatus.CNF";
const QString Asterix048Proto::TrackStatus_RAD = "a048.TrackStatus.RAD";
const QString Asterix048Proto::TrackStatus_DOU = "a048.TrackStatus.DOU";
const QString Asterix048Proto::TrackStatus_MAH = "a048.TrackStatus.MAH";
const QString Asterix048Proto::TrackStatus_CDM = "a048.TrackStatus.CDM";
const QString Asterix048Proto::TrackStatus_FX = "a048.TrackStatus.FX";

const QString Asterix048Proto::Asterix048Proto::CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED = "a048.CalculatedTrackVelocityInPolarCoordinates.GROUNDSPEED";
const QString Asterix048Proto::Asterix048Proto::CalculatedTrackVelocityInPolarCoordinates_HEADING = "a048.CalculatedTrackVelocityInPolarCoordinates.HEADING";

const QString Asterix048Proto::TrackQuality_X = "a048.TrackQuality.X";
const QString Asterix048Proto::TrackQuality_Y = "a048.TrackQuality.Y";
const QString Asterix048Proto::TrackQuality_V = "a048.TrackQuality.V";
const QString Asterix048Proto::TrackQuality_H = "a048.TrackQuality.H";

const QString Asterix048Proto::AircraftAddress = "a048.AircraftAddress";

const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_COM = "a048.CommunicationsAcasCapabilityAndFlightStatus.COM";
const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_STAT = "a048.CommunicationsAcasCapabilityAndFlightStatus.STAT";
const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_SI = "a048.CommunicationsAcasCapabilityAndFlightStatus.SI";
const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_MSSC = "a048.CommunicationsAcasCapabilityAndFlightStatus.MSSC";
const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_ARC = "a048.CommunicationsAcasCapabilityAndFlightStatus.ARC";
const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_AIC = "a048.CommunicationsAcasCapabilityAndFlightStatus.AIC";
const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_B1A = "a048.CommunicationsAcasCapabilityAndFlightStatus.B1A";
const QString Asterix048Proto::CommunicationsAcasCapabilityAndFlightStatus_B1B = "a048.CommunicationsAcasCapabilityAndFlightStatus.B1B";

const QString Asterix048Proto::AircraftIdentification = "a048.AircraftIdentification";

const QString Asterix048Proto::ModeSMbData = "a048.ModeSMbData";

const QString Asterix048Proto::AcasResolutionAdvisoryReport = "a048.AcasResolutionAdvisoryReport";


Asterix048Proto::Asterix048Proto()
{}

int Asterix048Proto::category() const
{
    return mCategory;
}

QString Asterix048Proto::edition() const
{
    return mEdition;
}

void Asterix048Proto::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_010(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_140(stream, map);

        if (mainMask.isSet(3))
            parse_020(stream, map);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (mainMask.isSet(4))
            parse_040(stream, map);

        if (mainMask.isSet(5))
            parse_070(stream, map);

        if (mainMask.isSet(6))
            parse_090(stream, map);

        if (mainMask.isSet(7))
            parse_130(stream, map);

        if (mainMask.isSet(8))
            parse_220(stream, map);

        if (mainMask.isSet(9))
            parse_240(stream, map);

        if (mainMask.isSet(10))
            parse_250(stream, map);

        if (mainMask.isSet(11))
            parse_161(stream, map);

        if (mainMask.isSet(12))
            parse_042(stream, map);

        if (mainMask.isSet(13))
            parse_200(stream, map);

        if (mainMask.isSet(14))
            parse_170(stream, map);

        if (mainMask.isSet(15))
            parse_210(stream, map);

        if (mainMask.isSet(16))
            parse_030(stream, map);

        if (mainMask.isSet(17))
            parse_080(stream, map);

        if (mainMask.isSet(18))
            parse_100(stream, map);

        if (mainMask.isSet(19))
            parse_110(stream, map);

        if (mainMask.isSet(20))
            parse_120(stream, map);

        if (mainMask.isSet(21))
            parse_230(stream, map);

        if (mainMask.isSet(22))
            parse_260(stream, map);

        if (mainMask.isSet(23))
            parse_055(stream, map);

        if (mainMask.isSet(24))
            parse_050(stream, map);

        if (mainMask.isSet(25))
            parse_065(stream, map);

        if (mainMask.isSet(26))
            parse_060(stream, map);

        if (mainMask.isSet(27))
            parse_sp(stream, map);

        if (mainMask.isSet(28))
            parse_re(stream, map);
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

QByteArray Asterix048Proto::generate(AsterixData &map)
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

        if (generate_140(map, stream))
            mainMask.set(2);

        if (generate_020(map, stream))
            mainMask.set(3);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (generate_040(map, stream))
            mainMask.set(4);

        if (generate_070(map, stream))
            mainMask.set(5);

        if (generate_090(map, stream))
            mainMask.set(6);

        if (generate_130(map, stream))
            mainMask.set(7);

        if (generate_220(map, stream))
            mainMask.set(8);

        if (generate_240(map, stream))
            mainMask.set(9);

        if (generate_250(map, stream))
            mainMask.set(10);

        if (generate_161(map, stream))
            mainMask.set(11);

        if (generate_042(map, stream))
            mainMask.set(12);

        if (generate_200(map, stream))
            mainMask.set(13);

        if (generate_170(map, stream))
            mainMask.set(14);

        if (generate_210(map, stream))
            mainMask.set(15);

        if (generate_030(map, stream))
            mainMask.set(16);

        if (generate_080(map, stream))
            mainMask.set(17);

        if (generate_100(map, stream))
            mainMask.set(18);

        if (generate_110(map, stream))
            mainMask.set(19);

        if (generate_120(map, stream))
            mainMask.set(20);

        if (generate_230(map, stream))
            mainMask.set(21);

        if (generate_260(map, stream))
            mainMask.set(22);

        if (generate_055(map, stream))
            mainMask.set(23);

        if (generate_050(map, stream))
            mainMask.set(24);

        if (generate_065(map, stream))
            mainMask.set(25);

        if (generate_060(map, stream))
            mainMask.set(26);

        if (generate_sp(map, stream))
            mainMask.set(27);

        if (generate_re(map, stream))
            mainMask.set(28);
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

QStringList Asterix048Proto::fields() const
{
    static QStringList result = QStringList()
        << DataSourceIdentifier_SAC
        << DataSourceIdentifier_SIC

        << TargetReportDescriptor_TYP
        << TargetReportDescriptor_SIM
        << TargetReportDescriptor_RDP
        << TargetReportDescriptor_SPI
        << TargetReportDescriptor_RAB
        << TargetReportDescriptor_FX

        << WarningErrorConditions

        << MeasuredPositionInPolarCoordinates_RHO
        << MeasuredPositionInPolarCoordinates_THETA

        << CalculatedPositionInCartesianCoordinates_X
        << CalculatedPositionInCartesianCoordinates_Y

        << Mode2CodeInOctalRepresentation_V
        << Mode2CodeInOctalRepresentation_G
        << Mode2CodeInOctalRepresentation_L
        << Mode2CodeInOctalRepresentation_A4
        << Mode2CodeInOctalRepresentation_A2
        << Mode2CodeInOctalRepresentation_A1
        << Mode2CodeInOctalRepresentation_B4
        << Mode2CodeInOctalRepresentation_B2
        << Mode2CodeInOctalRepresentation_B1
        << Mode2CodeInOctalRepresentation_C4
        << Mode2CodeInOctalRepresentation_C2
        << Mode2CodeInOctalRepresentation_C1
        << Mode2CodeInOctalRepresentation_D4
        << Mode2CodeInOctalRepresentation_D2
        << Mode2CodeInOctalRepresentation_D1

        << Mode1CodeInOctalRepresentation_V
        << Mode1CodeInOctalRepresentation_G
        << Mode1CodeInOctalRepresentation_L
        << Mode1CodeInOctalRepresentation_A4
        << Mode1CodeInOctalRepresentation_A2
        << Mode1CodeInOctalRepresentation_A1
        << Mode1CodeInOctalRepresentation_B2
        << Mode1CodeInOctalRepresentation_B1

        << Mode2CodeConfidenceIndicator_QA4
        << Mode2CodeConfidenceIndicator_QA2
        << Mode2CodeConfidenceIndicator_QA1
        << Mode2CodeConfidenceIndicator_QB4
        << Mode2CodeConfidenceIndicator_QB2
        << Mode2CodeConfidenceIndicator_QB1
        << Mode2CodeConfidenceIndicator_QC4
        << Mode2CodeConfidenceIndicator_QC2
        << Mode2CodeConfidenceIndicator_QC1
        << Mode2CodeConfidenceIndicator_QD4
        << Mode2CodeConfidenceIndicator_QD2
        << Mode2CodeConfidenceIndicator_QD1

        << Mode1CodeConfidenceIndicator_QA4
        << Mode1CodeConfidenceIndicator_QA2
        << Mode1CodeConfidenceIndicator_QA1
        << Mode1CodeConfidenceIndicator_QB2
        << Mode1CodeConfidenceIndicator_QB1

        << Mode3ACodeInOctalRepresentation_V
        << Mode3ACodeInOctalRepresentation_G
        << Mode3ACodeInOctalRepresentation_L
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

        << Mode3ACodeConfidenceIndicator_QA4
        << Mode3ACodeConfidenceIndicator_QA2
        << Mode3ACodeConfidenceIndicator_QA1
        << Mode3ACodeConfidenceIndicator_QB4
        << Mode3ACodeConfidenceIndicator_QB2
        << Mode3ACodeConfidenceIndicator_QB1
        << Mode3ACodeConfidenceIndicator_QC4
        << Mode3ACodeConfidenceIndicator_QC2
        << Mode3ACodeConfidenceIndicator_QC1
        << Mode3ACodeConfidenceIndicator_QD4
        << Mode3ACodeConfidenceIndicator_QD2
        << Mode3ACodeConfidenceIndicator_QD1

        << FlightLevelInBinaryRepresentation_V
        << FlightLevelInBinaryRepresentation_G
        << FlightLevelInBinaryRepresentation_FlightLevel

        << ModeCCodeAndCodeConfidenceIndicator_V
        << ModeCCodeAndCodeConfidenceIndicator_G
        << ModeCCodeAndCodeConfidenceIndicator_C1
        << ModeCCodeAndCodeConfidenceIndicator_A1
        << ModeCCodeAndCodeConfidenceIndicator_C2
        << ModeCCodeAndCodeConfidenceIndicator_A2
        << ModeCCodeAndCodeConfidenceIndicator_C4
        << ModeCCodeAndCodeConfidenceIndicator_A4
        << ModeCCodeAndCodeConfidenceIndicator_B1
        << ModeCCodeAndCodeConfidenceIndicator_D1
        << ModeCCodeAndCodeConfidenceIndicator_B2
        << ModeCCodeAndCodeConfidenceIndicator_D2
        << ModeCCodeAndCodeConfidenceIndicator_B4
        << ModeCCodeAndCodeConfidenceIndicator_D4
        << ModeCCodeAndCodeConfidenceIndicator_QC1
        << ModeCCodeAndCodeConfidenceIndicator_QA1
        << ModeCCodeAndCodeConfidenceIndicator_QC2
        << ModeCCodeAndCodeConfidenceIndicator_QA2
        << ModeCCodeAndCodeConfidenceIndicator_QC4
        << ModeCCodeAndCodeConfidenceIndicator_QA4
        << ModeCCodeAndCodeConfidenceIndicator_QB1
        << ModeCCodeAndCodeConfidenceIndicator_QD1
        << ModeCCodeAndCodeConfidenceIndicator_QB2
        << ModeCCodeAndCodeConfidenceIndicator_QD2
        << ModeCCodeAndCodeConfidenceIndicator_QB4
        << ModeCCodeAndCodeConfidenceIndicator_QD4

        << HeightMeasuredBy3DRadar

        << RadialDopplerSpeed_CalculatedDopplerSpeed_D
        << RadialDopplerSpeed_CalculatedDopplerSpeed_CAL
        << RadialDopplerSpeed_RawDopplerSpeed

        << RadarPlotCharacteristics_SsrPlotRunlength
        << RadarPlotCharacteristics_NumberOfReceivedRepliesForMSsr
        << RadarPlotCharacteristics_AmplitudeOfMSSRReply
        << RadarPlotCharacteristics_PrimaryPlotRunlength
        << RadarPlotCharacteristics_AmplitudeOfPrimaryPlot
        << RadarPlotCharacteristics_DifferenceInRangeBetweenPsrAndSsrPlot
        << RadarPlotCharacteristics_DifferenceInAzimuthBetweenPsrSndSsrPlot

        << TimeOfDay

        << TrackNumber

        << TrackStatus_CNF
        << TrackStatus_RAD
        << TrackStatus_DOU
        << TrackStatus_MAH
        << TrackStatus_CDM
        << TrackStatus_FX

        << CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED
        << CalculatedTrackVelocityInPolarCoordinates_HEADING

        << TrackQuality_X
        << TrackQuality_Y
        << TrackQuality_V
        << TrackQuality_H

        << AircraftAddress

        << CommunicationsAcasCapabilityAndFlightStatus_COM
        << CommunicationsAcasCapabilityAndFlightStatus_STAT
        << CommunicationsAcasCapabilityAndFlightStatus_SI
        << CommunicationsAcasCapabilityAndFlightStatus_MSSC
        << CommunicationsAcasCapabilityAndFlightStatus_ARC
        << CommunicationsAcasCapabilityAndFlightStatus_AIC
        << CommunicationsAcasCapabilityAndFlightStatus_B1A
        << CommunicationsAcasCapabilityAndFlightStatus_B1B

        << AircraftIdentification

        << ModeSMbData

        << AcasResolutionAdvisoryReport

        << SpecialPurposeField
        << ReservedExpansionField;
    return result;
}

void Asterix048Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

bool Asterix048Proto::generate_010(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DataSourceIdentifier_SAC)
        && map.contains(DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

void Asterix048Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(TargetReportDescriptor_TYP, data >> 5 & M3);
    map.insert(TargetReportDescriptor_SIM, data >> 4 & M1);
    map.insert(TargetReportDescriptor_RDP, data >> 3 & M1);
    map.insert(TargetReportDescriptor_SPI, data >> 2 & M1);
    map.insert(TargetReportDescriptor_RAB, data >> 1 & M1);

    QList<I048_020_FX> extension;
    while (data & M1)
    {
        data = read<quint8>(stream);
        I048_020_FX val;
        val.TST = data >> 7 & M1;
        val.ME = data >> 4 & M1;
        val.MI = data >> 3 & M1;
        val.FOEFRI = data >> 1 & M2;
        extension.push_back(val);
    }

    if (!extension.isEmpty())
        map.insert(TargetReportDescriptor_FX, QVariant::fromValue(extension));
}

bool Asterix048Proto::generate_020(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TargetReportDescriptor_TYP)
        && map.contains(TargetReportDescriptor_SIM)
        && map.contains(TargetReportDescriptor_RDP)
        && map.contains(TargetReportDescriptor_SPI)
        && map.contains(TargetReportDescriptor_RAB))
    {
        quint8 data = (map.value(TargetReportDescriptor_TYP).toUInt() & M3) << 5;
        data |= (map.value(TargetReportDescriptor_SIM).toUInt() & M1) << 4;
        data |= (map.value(TargetReportDescriptor_RDP).toUInt() & M1) << 3;
        data |= (map.value(TargetReportDescriptor_SPI).toUInt() & M1) << 2;
        data |= (map.value(TargetReportDescriptor_RAB).toUInt() & M1) << 1;

        if (map.contains(TargetReportDescriptor_FX))
        {
            QList<I048_020_FX> extension = map.value(TargetReportDescriptor_FX).value< QList<I048_020_FX> >();
            foreach (const I048_020_FX &val, extension)
            {
                write8(stream, data | M1);
                data = (val.TST & M1) << 7;
                data |= (val.ME & M1) << 4;
                data |= (val.MI & M1) << 3;
                data |= (val.FOEFRI & M2) << 1;
            }
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_030(QDataStream &stream, AsterixData &map)
{
    quint8 data;
    QList<quint8> result;
    do
    {
        data = read<quint8>(stream);
        result.push_back(data >> 1 & M7);
    } while(data & M1);
    map.insert(WarningErrorConditions, QVariant::fromValue(result));
}

bool Asterix048Proto::generate_030(AsterixData &map, QDataStream &stream)
{
    if (map.contains(WarningErrorConditions))
    {
        const QList<quint8> extension = map.value(WarningErrorConditions).value< QList<quint8> >();
        if (extension.isEmpty())
            return false;

        quint8 data = (extension.first() & M7) << 1;
        foreach (quint8 val, extension)
        {
            write8(stream, data | M1);
            data = (val & M7) << 1;
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_040(QDataStream &stream, AsterixData &map)
{
    map.insert(MeasuredPositionInPolarCoordinates_RHO, double(read<quint16>(stream)) / 256); // <= 256
    map.insert(MeasuredPositionInPolarCoordinates_THETA, double(read<quint16>(stream)) * 360 / 65536 );
}

bool Asterix048Proto::generate_040(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MeasuredPositionInPolarCoordinates_RHO) && map.contains(MeasuredPositionInPolarCoordinates_THETA))
    {
        write16(stream, map.value(MeasuredPositionInPolarCoordinates_RHO).toDouble() * 256);
        write16(stream, map.value(MeasuredPositionInPolarCoordinates_THETA).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_042(QDataStream &stream, AsterixData &map)
{
    map.insert(CalculatedPositionInCartesianCoordinates_X, double(read<qint16>(stream)) / 128); // -256 < val < 256
    map.insert(CalculatedPositionInCartesianCoordinates_Y, double(read<qint16>(stream)) / 128); // -256 < val < 256
}

bool Asterix048Proto::generate_042(AsterixData &map, QDataStream &stream)
{
    if (map.contains(CalculatedPositionInCartesianCoordinates_X) && map.contains(CalculatedPositionInCartesianCoordinates_Y))
    {
        write16(stream, map.value(CalculatedPositionInCartesianCoordinates_X).toDouble() * 128);
        write16(stream, map.value(CalculatedPositionInCartesianCoordinates_Y).toDouble() * 128);
        return true;
    }
    return false;

}

void Asterix048Proto::parse_050(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(Mode2CodeInOctalRepresentation_V, data >> 15 & M1);
    map.insert(Mode2CodeInOctalRepresentation_G, data >> 14 & M1);
    map.insert(Mode2CodeInOctalRepresentation_L, data >> 13 & M1);
    map.insert(Mode2CodeInOctalRepresentation_A4, data >> 11 & M1);
    map.insert(Mode2CodeInOctalRepresentation_A2, data >> 10 & M1);
    map.insert(Mode2CodeInOctalRepresentation_A1, data >> 9 & M1);
    map.insert(Mode2CodeInOctalRepresentation_B4, data >> 8 & M1);
    map.insert(Mode2CodeInOctalRepresentation_B2, data >> 7 & M1);
    map.insert(Mode2CodeInOctalRepresentation_B1, data >> 6 & M1);
    map.insert(Mode2CodeInOctalRepresentation_C4, data >> 5 & M1);
    map.insert(Mode2CodeInOctalRepresentation_C2, data >> 4 & M1);
    map.insert(Mode2CodeInOctalRepresentation_C1, data >> 3 & M1);
    map.insert(Mode2CodeInOctalRepresentation_D4, data >> 2 & M1);
    map.insert(Mode2CodeInOctalRepresentation_D2, data >> 1 & M1);
    map.insert(Mode2CodeInOctalRepresentation_D1, data & M1);
}

bool Asterix048Proto::generate_050(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Mode2CodeInOctalRepresentation_V)
        && map.contains(Mode2CodeInOctalRepresentation_G)
        && map.contains(Mode2CodeInOctalRepresentation_L)
        && map.contains(Mode2CodeInOctalRepresentation_A4)
        && map.contains(Mode2CodeInOctalRepresentation_A2)
        && map.contains(Mode2CodeInOctalRepresentation_A1)
        && map.contains(Mode2CodeInOctalRepresentation_B4)
        && map.contains(Mode2CodeInOctalRepresentation_B2)
        && map.contains(Mode2CodeInOctalRepresentation_B1)
        && map.contains(Mode2CodeInOctalRepresentation_C4)
        && map.contains(Mode2CodeInOctalRepresentation_C2)
        && map.contains(Mode2CodeInOctalRepresentation_C1)
        && map.contains(Mode2CodeInOctalRepresentation_D4)
        && map.contains(Mode2CodeInOctalRepresentation_D2)
        && map.contains(Mode2CodeInOctalRepresentation_D1))
    {
        quint16 data = (map.value(Mode2CodeInOctalRepresentation_V).toUInt() & M1) << 15;
        data |= (map.value(Mode2CodeInOctalRepresentation_G).toUInt() & M1) << 14;
        data |= (map.value(Mode2CodeInOctalRepresentation_L).toUInt() & M1) << 13;
        data |= (map.value(Mode2CodeInOctalRepresentation_A4).toUInt() & M1) << 11;
        data |= (map.value(Mode2CodeInOctalRepresentation_A2).toUInt() & M1) << 10;
        data |= (map.value(Mode2CodeInOctalRepresentation_A1).toUInt() & M1) << 9;
        data |= (map.value(Mode2CodeInOctalRepresentation_B4).toUInt() & M1) << 8;
        data |= (map.value(Mode2CodeInOctalRepresentation_B2).toUInt() & M1) << 7;
        data |= (map.value(Mode2CodeInOctalRepresentation_B1).toUInt() & M1) << 6;
        data |= (map.value(Mode2CodeInOctalRepresentation_C4).toUInt() & M1) << 5;
        data |= (map.value(Mode2CodeInOctalRepresentation_C2).toUInt() & M1) << 4;
        data |= (map.value(Mode2CodeInOctalRepresentation_C1).toUInt() & M1) << 3;
        data |= (map.value(Mode2CodeInOctalRepresentation_D4).toUInt() & M1) << 2;
        data |= (map.value(Mode2CodeInOctalRepresentation_D2).toUInt() & M1) << 1;
        data |= map.value(Mode2CodeInOctalRepresentation_D1).toUInt() & M1;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_055(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(Mode1CodeInOctalRepresentation_V, data >> 7 & M1);
    map.insert(Mode1CodeInOctalRepresentation_G, data >> 6 & M1);
    map.insert(Mode1CodeInOctalRepresentation_L, data >> 5 & M1);
    map.insert(Mode1CodeInOctalRepresentation_A4, data >> 4 & M1);
    map.insert(Mode1CodeInOctalRepresentation_A2, data >> 3 & M1);
    map.insert(Mode1CodeInOctalRepresentation_A1, data >> 2 & M1);
    map.insert(Mode1CodeInOctalRepresentation_B2, data >> 1 & M1);
    map.insert(Mode1CodeInOctalRepresentation_B1, data & M1);
}

bool Asterix048Proto::generate_055(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Mode1CodeInOctalRepresentation_V)
        && map.contains(Mode1CodeInOctalRepresentation_G)
        && map.contains(Mode1CodeInOctalRepresentation_L)
        && map.contains(Mode1CodeInOctalRepresentation_A4)
        && map.contains(Mode1CodeInOctalRepresentation_A2)
        && map.contains(Mode1CodeInOctalRepresentation_A1)
        && map.contains(Mode1CodeInOctalRepresentation_B2)
        && map.contains(Mode1CodeInOctalRepresentation_B1))
    {
        quint8 data = (map.value(Mode1CodeInOctalRepresentation_V).toUInt() & M1) << 7;
        data |= (map.value(Mode1CodeInOctalRepresentation_G).toUInt() & M1) << 6;
        data |= (map.value(Mode1CodeInOctalRepresentation_L).toUInt() & M1) << 5;
        data |= (map.value(Mode1CodeInOctalRepresentation_A4).toUInt() & M1) << 4;
        data |= (map.value(Mode1CodeInOctalRepresentation_A2).toUInt() & M1) << 3;
        data |= (map.value(Mode1CodeInOctalRepresentation_A1).toUInt() & M1) << 2;
        data |= (map.value(Mode1CodeInOctalRepresentation_B2).toUInt() & M1) << 1;
        data |= map.value(Mode1CodeInOctalRepresentation_B1).toUInt() & M1;
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_060(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(Mode2CodeConfidenceIndicator_QA4, data >> 11 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QA2, data >> 10 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QA1, data >> 9 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QB4, data >> 8 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QB2, data >> 7 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QB1, data >> 6 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QC4, data >> 5 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QC2, data >> 4 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QC1, data >> 3 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QD4, data >> 2 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QD2, data >> 1 & M1);
    map.insert(Mode2CodeConfidenceIndicator_QD1, data & M1);
}

bool Asterix048Proto::generate_060(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Mode2CodeConfidenceIndicator_QA4)
        && map.contains(Mode2CodeConfidenceIndicator_QA2)
        && map.contains(Mode2CodeConfidenceIndicator_QA1)
        && map.contains(Mode2CodeConfidenceIndicator_QB4)
        && map.contains(Mode2CodeConfidenceIndicator_QB2)
        && map.contains(Mode2CodeConfidenceIndicator_QB1)
        && map.contains(Mode2CodeConfidenceIndicator_QC4)
        && map.contains(Mode2CodeConfidenceIndicator_QC2)
        && map.contains(Mode2CodeConfidenceIndicator_QC1)
        && map.contains(Mode2CodeConfidenceIndicator_QD4)
        && map.contains(Mode2CodeConfidenceIndicator_QD2)
        && map.contains(Mode2CodeConfidenceIndicator_QD1))
    {
        quint16 data = (map.value(Mode2CodeConfidenceIndicator_QA4).toUInt() & M1) << 11;
        data |= (map.value(Mode2CodeConfidenceIndicator_QA2).toUInt() & M1) << 10;
        data |= (map.value(Mode2CodeConfidenceIndicator_QA1).toUInt() & M1) << 9;
        data |= (map.value(Mode2CodeConfidenceIndicator_QB4).toUInt() & M1) << 8;
        data |= (map.value(Mode2CodeConfidenceIndicator_QB2).toUInt() & M1) << 7;
        data |= (map.value(Mode2CodeConfidenceIndicator_QB1).toUInt() & M1) << 6;
        data |= (map.value(Mode2CodeConfidenceIndicator_QC4).toUInt() & M1) << 5;
        data |= (map.value(Mode2CodeConfidenceIndicator_QC2).toUInt() & M1) << 4;
        data |= (map.value(Mode2CodeConfidenceIndicator_QC1).toUInt() & M1) << 3;
        data |= (map.value(Mode2CodeConfidenceIndicator_QD4).toUInt() & M1) << 2;
        data |= (map.value(Mode2CodeConfidenceIndicator_QD2).toUInt() & M1) << 1;
        data |= map.value(Mode2CodeConfidenceIndicator_QD1).toUInt() & M1;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_065(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(Mode1CodeConfidenceIndicator_QA4, data >> 4 & M1);
    map.insert(Mode1CodeConfidenceIndicator_QA2, data >> 3 & M1);
    map.insert(Mode1CodeConfidenceIndicator_QA1, data >> 2 & M1);
    map.insert(Mode1CodeConfidenceIndicator_QB2, data >> 1 & M1);
    map.insert(Mode1CodeConfidenceIndicator_QB1, data & M1);
}

bool Asterix048Proto::generate_065(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Mode1CodeConfidenceIndicator_QA4)
        && map.contains(Mode1CodeConfidenceIndicator_QA2)
        && map.contains(Mode1CodeConfidenceIndicator_QA1)
        && map.contains(Mode1CodeConfidenceIndicator_QB2)
        && map.contains(Mode1CodeConfidenceIndicator_QB1))
    {
        quint8 data = (map.value(Mode1CodeConfidenceIndicator_QA4).toUInt() & M1) << 4;
        data |= (map.value(Mode1CodeConfidenceIndicator_QA2).toUInt() & M1) << 3;
        data |= (map.value(Mode1CodeConfidenceIndicator_QA1).toUInt() & M1) << 2;
        data |= (map.value(Mode1CodeConfidenceIndicator_QB2).toUInt() & M1) << 1;
        data |= map.value(Mode1CodeConfidenceIndicator_QB1).toUInt() & M1;
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_070(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(Mode3ACodeInOctalRepresentation_V, data >> 15 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_G, data >> 14 & M1);
    map.insert(Mode3ACodeInOctalRepresentation_L, data >> 13 & M1);
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

bool Asterix048Proto::generate_070(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Mode3ACodeInOctalRepresentation_V)
        && map.contains(Mode3ACodeInOctalRepresentation_G)
        && map.contains(Mode3ACodeInOctalRepresentation_L)
        && map.contains(Mode3ACodeInOctalRepresentation_A4)
        && map.contains(Mode3ACodeInOctalRepresentation_A2)
        && map.contains(Mode3ACodeInOctalRepresentation_A1)
        && map.contains(Mode3ACodeInOctalRepresentation_B4)
        && map.contains(Mode3ACodeInOctalRepresentation_B2)
        && map.contains(Mode3ACodeInOctalRepresentation_B1)
        && map.contains(Mode3ACodeInOctalRepresentation_C4)
        && map.contains(Mode3ACodeInOctalRepresentation_C2)
        && map.contains(Mode3ACodeInOctalRepresentation_C1)
        && map.contains(Mode3ACodeInOctalRepresentation_D4)
        && map.contains(Mode3ACodeInOctalRepresentation_D2)
        && map.contains(Mode3ACodeInOctalRepresentation_D1))
    {
        quint16 data = (map.value(Mode3ACodeInOctalRepresentation_V).toUInt() & M1) << 15;
        data |= (map.value(Mode3ACodeInOctalRepresentation_G).toUInt() & M1) << 14;
        data |= (map.value(Mode3ACodeInOctalRepresentation_L).toUInt() & M1) << 13;
        data |= (map.value(Mode3ACodeInOctalRepresentation_A4).toUInt() & M1) << 11;
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

void Asterix048Proto::parse_080(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(Mode3ACodeConfidenceIndicator_QA4, data >> 11 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QA2, data >> 10 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QA1, data >> 9 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QB4, data >> 8 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QB2, data >> 7 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QB1, data >> 6 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QC4, data >> 5 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QC2, data >> 4 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QC1, data >> 3 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QD4, data >> 2 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QD2, data >> 1 & M1);
    map.insert(Mode3ACodeConfidenceIndicator_QD1, data & M1);
}

bool Asterix048Proto::generate_080(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Mode3ACodeConfidenceIndicator_QA4)
        && map.contains(Mode3ACodeConfidenceIndicator_QA2)
        && map.contains(Mode3ACodeConfidenceIndicator_QA1)
        && map.contains(Mode3ACodeConfidenceIndicator_QB4)
        && map.contains(Mode3ACodeConfidenceIndicator_QB2)
        && map.contains(Mode3ACodeConfidenceIndicator_QB1)
        && map.contains(Mode3ACodeConfidenceIndicator_QC4)
        && map.contains(Mode3ACodeConfidenceIndicator_QC2)
        && map.contains(Mode3ACodeConfidenceIndicator_QC1)
        && map.contains(Mode3ACodeConfidenceIndicator_QD4)
        && map.contains(Mode3ACodeConfidenceIndicator_QD2)
        && map.contains(Mode3ACodeConfidenceIndicator_QD1))
        {
        quint16 data = (map.value(Mode3ACodeConfidenceIndicator_QA4).toUInt() & M1) << 11;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QA2).toUInt() & M1) << 10;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QA1).toUInt() & M1) << 9;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QB4).toUInt() & M1) << 8;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QB2).toUInt() & M1) << 7;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QB1).toUInt() & M1) << 6;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QC4).toUInt() & M1) << 5;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QC2).toUInt() & M1) << 4;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QC1).toUInt() & M1) << 3;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QD4).toUInt() & M1) << 2;
        data |= (map.value(Mode3ACodeConfidenceIndicator_QD2).toUInt() & M1) << 1;
        data |= map.value(Mode3ACodeConfidenceIndicator_QD1).toUInt() & M1;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(FlightLevelInBinaryRepresentation_V, data >> 15 & M1);
    map.insert(FlightLevelInBinaryRepresentation_G, data >> 14 & M1);
    map.insert(FlightLevelInBinaryRepresentation_FlightLevel, data & M14);
}

bool Asterix048Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (map.contains(FlightLevelInBinaryRepresentation_V)
        && map.contains(FlightLevelInBinaryRepresentation_G)
        && map.contains(FlightLevelInBinaryRepresentation_FlightLevel))
    {
        quint16 data = (map.value(FlightLevelInBinaryRepresentation_V).toUInt() & M1) << 15;
        data |= (map.value(FlightLevelInBinaryRepresentation_G).toUInt() & M1) << 14;
        data |= map.value(FlightLevelInBinaryRepresentation_FlightLevel).toUInt() & M14;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_100(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_V, data >> 15 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_G, data >> 14 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_C1, data >> 11 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_A1, data >> 10 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_C2, data >> 9 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_A2, data >> 8 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_C4, data >> 7 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_A4, data >> 6 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_B1, data >> 5 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_D1, data >> 4 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_B2, data >> 3 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_D2, data >> 2 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_B4, data >> 1 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_D4, data & M1);

    data = read<quint16>(stream);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QC1, data >> 11 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QA1, data >> 10 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QC2, data >> 9 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QA2, data >> 8 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QC4, data >> 7 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QA4, data >> 6 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QB1, data >> 5 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QD1, data >> 4 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QB2, data >> 3 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QD2, data >> 2 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QB4, data >> 1 & M1);
    map.insert(ModeCCodeAndCodeConfidenceIndicator_QD4, data & M1);
}

bool Asterix048Proto::generate_100(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ModeCCodeAndCodeConfidenceIndicator_V)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_G)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_C1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_A1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_C2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_A2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_C4)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_A4)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_B1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_D1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_B2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_D2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_B4)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_D4)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QC1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QA1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QC2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QA2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QC4)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QA4)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QB1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QD1)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QB2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QD2)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QB4)
        && map.contains(ModeCCodeAndCodeConfidenceIndicator_QD4))
        {
        quint16 data = (map.value(ModeCCodeAndCodeConfidenceIndicator_V).toUInt() & M1) << 15;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_G).toUInt() & M1) << 14;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_C1).toUInt() & M1) << 11;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_A1).toUInt() & M1) << 10;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_C2).toUInt() & M1) << 9;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_A2).toUInt() & M1) << 8;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_C4).toUInt() & M1) << 7;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_A4).toUInt() & M1) << 6;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_B1).toUInt() & M1) << 5;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_D1).toUInt() & M1) << 4;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_B2).toUInt() & M1) << 3;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_D2).toUInt() & M1) << 2;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_B4).toUInt() & M1) << 1;
        data |= map.value(ModeCCodeAndCodeConfidenceIndicator_D4).toUInt() & M1;
        write16(stream, data);

        data = (map.value(ModeCCodeAndCodeConfidenceIndicator_QC1).toUInt() & M1) << 11;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QA1).toUInt() & M1) << 10;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QC2).toUInt() & M1) << 9;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QA2).toUInt() & M1) << 8;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QC4).toUInt() & M1) << 7;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QA4).toUInt() & M1) << 6;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QB1).toUInt() & M1) << 5;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QD1).toUInt() & M1) << 4;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QB2).toUInt() & M1) << 3;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QD2).toUInt() & M1) << 2;
        data |= (map.value(ModeCCodeAndCodeConfidenceIndicator_QB4).toUInt() & M1) << 1;
        data |= map.value(ModeCCodeAndCodeConfidenceIndicator_QD4).toUInt() & M1;
        write16(stream, data);

        return true;
    }
    return false;
}

void Asterix048Proto::parse_110(QDataStream &stream, AsterixData &map)
{
    map.insert(HeightMeasuredBy3DRadar, double(read<quint16>(stream) & M14) * 25);
}

bool Asterix048Proto::generate_110(AsterixData &map, QDataStream &stream)
{
    if (map.contains(HeightMeasuredBy3DRadar))
    {
        write16(stream, map.value(HeightMeasuredBy3DRadar).toDouble() / 25);
        return true;
    }
    return false;
}


void Asterix048Proto::parse_120(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);
    if (mask.isSet(1))
    {
        quint16 data = read<quint16>(stream);
        map.insert(RadialDopplerSpeed_CalculatedDopplerSpeed_D, data >> 15 & M1);
        map.insert(RadialDopplerSpeed_CalculatedDopplerSpeed_CAL, data & M10);
    }

    if (mask.isSet(2))
    {
        QList<I048_120_2> extension;
        for (quint8 rep = read<quint8>(stream); 0 != rep; --rep)
        {
            I048_120_2 val;
            val.DopplerSpeed = read<quint16>(stream);
            val.AmbiguityRange = read<quint16>(stream);
            val.TransmitterFrequency = read<quint16>(stream);
            extension.push_back(val);
        }
        if (!extension.isEmpty())
            map.insert(RadialDopplerSpeed_RawDopplerSpeed, QVariant::fromValue(extension));
    }
    else
        map.appendError(mandatory("Radial Doppler Speed subfield 2 (Raw Doppler Speed)"));
}


bool Asterix048Proto::generate_120(AsterixData &map, QDataStream &stream)
{
    PresentData subMask;
    QByteArray subArray;
    QDataStream subStream(&subArray, QIODevice::WriteOnly);
    if (map.contains(RadialDopplerSpeed_CalculatedDopplerSpeed_D)
        && map.contains(RadialDopplerSpeed_CalculatedDopplerSpeed_CAL))
    {
        subMask.set(1);
        quint16 data = (map.value(RadialDopplerSpeed_CalculatedDopplerSpeed_D).toUInt() & M1) << 15;
        data |= (map.value(RadialDopplerSpeed_CalculatedDopplerSpeed_CAL).toUInt() & M10);
        write16(subStream, data);
    }

    bool is2 = true;
    if (map.contains(RadialDopplerSpeed_RawDopplerSpeed))
    {
        QList<I048_120_2> data = map.value(RadialDopplerSpeed_RawDopplerSpeed).value< QList<I048_120_2> >();
        quint8 rep =  calculateRepetition(data);
        if (0 != rep)
        {
            subMask.set(2);
            write8(subStream, rep);
            foreach (const I048_120_2 &data, data)
            {
                write16(subStream, data.DopplerSpeed);
                write16(subStream, data.AmbiguityRange);
                write16(subStream, data.TransmitterFrequency);
            }
            is2 = false;
        }
    }

    if (!subMask.isEmpty())
    {
        if (is2)
            map.appendError(mandatory("Radial Doppler Speed subfield 2 (Raw Doppler Speed)"));
        writeArray(stream, subMask.data());
        writeArray(stream, subArray);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_130(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.isSet(1))
        map.insert(RadarPlotCharacteristics_SsrPlotRunlength, double(read<quint8>(stream)) * 360 / 8192);

    if (mask.isSet(2))
        map.insert(RadarPlotCharacteristics_NumberOfReceivedRepliesForMSsr, read<quint8>(stream));

    if (mask.isSet(3))
        map.insert(RadarPlotCharacteristics_AmplitudeOfMSSRReply, read<quint8>(stream));

    if (mask.isSet(4))
        map.insert(RadarPlotCharacteristics_PrimaryPlotRunlength, double(read<quint8>(stream)) * 360 / 8192);

    if (mask.isSet(5))
        map.insert(RadarPlotCharacteristics_AmplitudeOfPrimaryPlot, read<quint8>(stream));

    if (mask.isSet(6))
        map.insert(RadarPlotCharacteristics_DifferenceInRangeBetweenPsrAndSsrPlot, double(read<qint8>(stream)) / 256);

    if (mask.isSet(7))
        map.insert(RadarPlotCharacteristics_DifferenceInAzimuthBetweenPsrSndSsrPlot, double(read<qint8>(stream)) * 360 / 16384);

}

bool Asterix048Proto::generate_130(AsterixData &map, QDataStream &stream)
{
    PresentData subMask;
    QByteArray subArray;
    QDataStream subStream(&subArray, QIODevice::WriteOnly);

    if (map.contains(RadarPlotCharacteristics_SsrPlotRunlength))
    {
        subMask.set(1);
        write8(subStream, map.value(RadarPlotCharacteristics_SsrPlotRunlength).toDouble() * 8192 / 360);
    }

    if (map.contains(RadarPlotCharacteristics_NumberOfReceivedRepliesForMSsr))
    {
        subMask.set(2);
        write8(subStream, map.value(RadarPlotCharacteristics_NumberOfReceivedRepliesForMSsr).toUInt());
    }

    if (map.contains(RadarPlotCharacteristics_AmplitudeOfMSSRReply))
    {
        subMask.set(3);
        write8(subStream, map.value(RadarPlotCharacteristics_AmplitudeOfMSSRReply).toUInt());
    }

    if (map.contains(RadarPlotCharacteristics_PrimaryPlotRunlength))
    {
        subMask.set(4);
        write8(subStream, map.value(RadarPlotCharacteristics_PrimaryPlotRunlength).toDouble() * 8192 / 360);
    }

    if (map.contains(RadarPlotCharacteristics_AmplitudeOfPrimaryPlot))
    {
        subMask.set(5);
        write8(subStream, map.value(RadarPlotCharacteristics_AmplitudeOfPrimaryPlot).toUInt());
    }

    if (map.contains(RadarPlotCharacteristics_DifferenceInRangeBetweenPsrAndSsrPlot))
    {
        subMask.set(6);
        write8(subStream, map.value(RadarPlotCharacteristics_DifferenceInRangeBetweenPsrAndSsrPlot).toDouble() * 256);
    }

    if (map.contains(RadarPlotCharacteristics_DifferenceInAzimuthBetweenPsrSndSsrPlot))
    {
        subMask.set(7);
        write8(subStream, map.value(RadarPlotCharacteristics_DifferenceInAzimuthBetweenPsrSndSsrPlot).toDouble() * 16384 / 360);
    }

    if (!subMask.isEmpty())
    {
        writeArray(stream, subMask.data());
        writeArray(stream, subArray);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_140(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfDay, QTime().addSecs(ceil(double(read24(stream)) / 128)));
}

bool Asterix048Proto::generate_140(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TimeOfDay))
    {
        write24(stream, double(map.value(TimeOfDay).toTime().secsTo(QTime(0,0,0,0))) * -128);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_161(QDataStream &stream, AsterixData &map)
{
    map.insert(TrackNumber, read<quint16>(stream) & M12);
}

bool Asterix048Proto::generate_161(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TrackNumber))
    {
        write16(stream, map.value(TrackNumber).toUInt() & M12);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_170(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(TrackStatus_CNF, data >> 7 & M1);
    map.insert(TrackStatus_RAD, data >> 5 & M2);
    map.insert(TrackStatus_DOU, data >> 4 & M1);
    map.insert(TrackStatus_MAH, data >> 3 & M1);
    map.insert(TrackStatus_CDM, data >> 1 & M2);

    QList<I048_170_FX> extension;
    while (data & M1)
    {
        data = read<quint8>(stream);
        I048_170_FX subData;
        subData.TRE = data >> 7 & M1;
        subData.GHO = data >> 6 & M1;
        subData.SUP = data >> 5 & M1;
        subData.TCC = data >> 4 & M1;
        extension.push_back(subData);
    }

    if (!extension.isEmpty())
        map.insert(TrackStatus_FX, QVariant::fromValue(extension));
}

bool Asterix048Proto::generate_170(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TrackStatus_CNF)
        && map.contains(TrackStatus_RAD)
        && map.contains(TrackStatus_DOU)
        && map.contains(TrackStatus_MAH)
        && map.contains(TrackStatus_CDM))
    {
        quint8 data = (map.value(TrackStatus_CNF).toUInt() & M1) << 7;
        data |= (map.value(TrackStatus_RAD).toUInt() & M2) << 5;
        data |= (map.value(TrackStatus_DOU).toUInt() & M1) << 4;
        data |= (map.value(TrackStatus_MAH).toUInt() & M1) << 3;
        data |= (map.value(TrackStatus_CDM).toUInt() & M2) << 1;

        if (map.contains(TrackStatus_FX))
        {
            QList<I048_170_FX> extension = map.value(TrackStatus_FX).value< QList<I048_170_FX> >();
            foreach (const I048_170_FX &val, extension)
            {
                data |= M1;
                write8(stream, data);
                data = (val.TRE & M1) << 7;
                data |= (val.GHO & M1) << 6;
                data |= (val.SUP & M1) << 5;
                data |= (val.TCC & M1) << 4;
            }
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_200(QDataStream &stream, AsterixData &map)
{
    map.insert(CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED, double(read<quint16>(stream)) / 16384);
    map.insert(CalculatedTrackVelocityInPolarCoordinates_HEADING, double(read<quint16>(stream)) * 360 / 65536);
}

bool Asterix048Proto::generate_200(AsterixData &map, QDataStream &stream)
{
    if (map.contains(CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED)
        && map.contains(CalculatedTrackVelocityInPolarCoordinates_HEADING))
    {
        write16(stream, map.value(CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED).toDouble() * 16384);
        write16(stream, map.value(CalculatedTrackVelocityInPolarCoordinates_HEADING).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_210(QDataStream &stream, AsterixData &map)
{
    map.insert(TrackQuality_X, double(read<quint8>(stream)) / 128);
    map.insert(TrackQuality_Y, double(read<quint8>(stream)) / 128);
    map.insert(TrackQuality_V, double(read<quint8>(stream)) / 16384);
    map.insert(TrackQuality_H, double(read<quint8>(stream)) * 360 / 4096);
}

bool Asterix048Proto::generate_210(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TrackQuality_X)
        && map.contains(TrackQuality_Y)
        && map.contains(TrackQuality_V)
        && map.contains(TrackQuality_H))
    {
        write8(stream, map.value(TrackQuality_X).toDouble() * 128);
        write8(stream, map.value(TrackQuality_Y).toDouble() * 128);
        write8(stream, map.value(TrackQuality_V).toDouble() * 16384);
        write8(stream, map.value(TrackQuality_H).toDouble() * 4096 / 360);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_220(QDataStream &stream, AsterixData &map)
{
    map.insert(AircraftAddress, read24(stream));
}

bool Asterix048Proto::generate_220(AsterixData &map, QDataStream &stream)
{
    if (map.contains(AircraftAddress))
    {
        write24(stream, map.value(AircraftAddress).toUInt());
        return true;
    }
    return false;
}

void Asterix048Proto::parse_230(QDataStream &stream, AsterixData &map)
{
    quint16 data = read<quint16>(stream);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_COM, data >> 13 & M3);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_STAT, data >> 10 & M3);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_SI, data >> 9 & M1);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_MSSC, data >> 7 & M1);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_ARC, data >> 6 & M1);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_AIC, data >> 5 & M1);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_B1A, data >> 4 & M1);
    map.insert(CommunicationsAcasCapabilityAndFlightStatus_B1B, data & M4);
}

bool Asterix048Proto::generate_230(AsterixData &map, QDataStream &stream)
{
    if (map.contains(CommunicationsAcasCapabilityAndFlightStatus_COM)
        && map.contains(CommunicationsAcasCapabilityAndFlightStatus_STAT)
        && map.contains(CommunicationsAcasCapabilityAndFlightStatus_SI)
        && map.contains(CommunicationsAcasCapabilityAndFlightStatus_MSSC)
        && map.contains(CommunicationsAcasCapabilityAndFlightStatus_ARC)
        && map.contains(CommunicationsAcasCapabilityAndFlightStatus_AIC)
        && map.contains(CommunicationsAcasCapabilityAndFlightStatus_B1A)
        && map.contains(CommunicationsAcasCapabilityAndFlightStatus_B1B))
    {
        quint16 data = (map.value(CommunicationsAcasCapabilityAndFlightStatus_COM).toUInt() & M3) << 13;
        data |= (map.value(CommunicationsAcasCapabilityAndFlightStatus_STAT).toUInt() & M3) << 10;
        data |= (map.value(CommunicationsAcasCapabilityAndFlightStatus_SI).toUInt() & M1) << 9;
        data |= (map.value(CommunicationsAcasCapabilityAndFlightStatus_MSSC).toUInt() & M1) << 7;
        data |= (map.value(CommunicationsAcasCapabilityAndFlightStatus_ARC).toUInt() & M1) << 6;
        data |= (map.value(CommunicationsAcasCapabilityAndFlightStatus_AIC).toUInt() & M1) << 5;
        data |= (map.value(CommunicationsAcasCapabilityAndFlightStatus_B1A).toUInt() & M1) << 4;
        data |= map.value(CommunicationsAcasCapabilityAndFlightStatus_B1B).toUInt() & M4;
        write16(stream, data);
        return true;
    }
    return false;
}

void Asterix048Proto::parse_240(QDataStream &stream, AsterixData &map)
{
    map.insert(AircraftIdentification, readCharacters(stream, 2));
}

bool Asterix048Proto::generate_240(AsterixData &map, QDataStream &stream)
{
    if (map.contains(AircraftIdentification))
    {
        QByteArray data = map.value(AircraftIdentification).toByteArray();
        if (8 == data.size())
        {
            writeCharacters(stream, data);
            return true;
        }
    }
    return false;
}

void Asterix048Proto::parse_250(QDataStream &stream, AsterixData &map)
{
    QList<I048_250> repData;
    for (quint8 rep = read<quint8>(stream); 0 != rep; --rep)
    {
        I048_250 data;
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

bool Asterix048Proto::generate_250(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ModeSMbData))
    {
        QList<I048_250> repData = map.value(ModeSMbData).value< QList<I048_250> >();
        quint8 rep = calculateRepetition(repData);
        if (0 != rep)
        {
            write8(stream, rep);
            foreach (const I048_250 &val, repData)
            {
                for (unsigned int i = 0; i < sizeof(val.MbData); ++i)
                    write8(stream, val.MbData[i]);
                quint8 dbs = (val.BDS1 & M4) << 4;
                dbs |= val.BDS2 & M4;
                write8(stream, dbs);
            }
            return true;
        }
    }
    return false;
}

void Asterix048Proto::parse_260(QDataStream &stream, AsterixData &map)
{
    map.insert(AcasResolutionAdvisoryReport, readArray(stream, 7));
}

bool Asterix048Proto::generate_260(AsterixData &map, QDataStream &stream)
{
    if (map.contains(AcasResolutionAdvisoryReport))
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
