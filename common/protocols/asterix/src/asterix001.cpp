#include "asterix001.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>

#include <math.h>


const int Asterix001Proto::mCategory = 1;
const QString Asterix001Proto::mEdition = "1.1";

const QString Asterix001Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix001Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix001Proto::TargetReportDescriptor_TYP = "a001.TargetReportDescriptor.TYP";
const QString Asterix001Proto::TargetReportDescriptor_SIM = "a001.TargetReportDescriptor.SIM";
const QString Asterix001Proto::TargetReportDescriptor_SSRPSR = "a001.TargetReportDescriptor.SSRPSR";
const QString Asterix001Proto::TargetReportDescriptor_ANT = "a001.TargetReportDescriptor.ANT";
const QString Asterix001Proto::TargetReportDescriptor_SPI = "a001.TargetReportDescriptor.SPI";
const QString Asterix001Proto::TargetReportDescriptor_RAB = "a001.TargetReportDescriptor.RAB";
const QString Asterix001Proto::TargetReportDescriptor_FX = "a001.TargetReportDescriptor.FX";

const QString Asterix001Proto::WarningErrorConditions = "a001.WarningErrorConditions";

const QString Asterix001Proto::MeasuredPositionInPolarCoordinates_RHO = "a001.MeasuredPositionInPolarCoordinates.RHO";
const QString Asterix001Proto::MeasuredPositionInPolarCoordinates_THETA = "a001.MeasuredPositionInPolarCoordinates.THETA";

const QString Asterix001Proto::CalculatedPositionInCartesianCoordinates_X = "a001.CalculatedPositionInCartesianCoordinates.X";
const QString Asterix001Proto::CalculatedPositionInCartesianCoordinates_Y = "a001.CalculatedPositionInCartesianCoordinates.Y";

const QString Asterix001Proto::Mode2CodeInOctalRepresentation_V = "a001.Mode2CodeInOctalRepresentation.V";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_G = "a001.Mode2CodeInOctalRepresentation.G";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_L = "a001.Mode2CodeInOctalRepresentation.L";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_A4 = "a001.Mode2CodeInOctalRepresentation.A4";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_A2 = "a001.Mode2CodeInOctalRepresentation.A2";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_A1 = "a001.Mode2CodeInOctalRepresentation.A1";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_B4 = "a001.Mode2CodeInOctalRepresentation.B4";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_B2 = "a001.Mode2CodeInOctalRepresentation.B2";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_B1 = "a001.Mode2CodeInOctalRepresentation.B1";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_C4 = "a001.Mode2CodeInOctalRepresentation.C4";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_C2 = "a001.Mode2CodeInOctalRepresentation.C2";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_C1 = "a001.Mode2CodeInOctalRepresentation.C1";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_D4 = "a001.Mode2CodeInOctalRepresentation.D4";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_D2 = "a001.Mode2CodeInOctalRepresentation.D2";
const QString Asterix001Proto::Mode2CodeInOctalRepresentation_D1 = "a001.Mode2CodeInOctalRepresentation.D1";

const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QA4 = "a001.Mode2CodeConfidenceIndicator.QA4";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QA2 = "a001.Mode2CodeConfidenceIndicator.QA2";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QA1 = "a001.Mode2CodeConfidenceIndicator.QA1";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QB4 = "a001.Mode2CodeConfidenceIndicator.QB4";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QB2 = "a001.Mode2CodeConfidenceIndicator.QB2";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QB1 = "a001.Mode2CodeConfidenceIndicator.QB1";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QC4 = "a001.Mode2CodeConfidenceIndicator.QC4";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QC2 = "a001.Mode2CodeConfidenceIndicator.QC2";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QC1 = "a001.Mode2CodeConfidenceIndicator.QC1";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QD4 = "a001.Mode2CodeConfidenceIndicator.QD4";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QD2 = "a001.Mode2CodeConfidenceIndicator.QD2";
const QString Asterix001Proto::Mode2CodeConfidenceIndicator_QD1 = "a001.Mode2CodeConfidenceIndicator.QD1";

const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_V = "a001.Mode3ACodeInOctalRepresentation.V";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_G = "a001.Mode3ACodeInOctalRepresentation.G";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_L = "a001.Mode3ACodeInOctalRepresentation.L";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_A4 = "a001.Mode3ACodeInOctalRepresentation.A4";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_A2 = "a001.Mode3ACodeInOctalRepresentation.A2";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_A1 = "a001.Mode3ACodeInOctalRepresentation.A1";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_B4 = "a001.Mode3ACodeInOctalRepresentation.B4";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_B2 = "a001.Mode3ACodeInOctalRepresentation.B2";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_B1 = "a001.Mode3ACodeInOctalRepresentation.B1";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_C4 = "a001.Mode3ACodeInOctalRepresentation.C4";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_C2 = "a001.Mode3ACodeInOctalRepresentation.C2";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_C1 = "a001.Mode3ACodeInOctalRepresentation.C1";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_D4 = "a001.Mode3ACodeInOctalRepresentation.D4";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_D2 = "a001.Mode3ACodeInOctalRepresentation.D2";
const QString Asterix001Proto::Mode3ACodeInOctalRepresentation_D1 = "a001.Mode3ACodeInOctalRepresentation.D1";

const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QA4 = "a001.Mode3ACodeConfidenceIndicator.QA4";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QA2 = "a001.Mode3ACodeConfidenceIndicator.QA2";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QA1 = "a001.Mode3ACodeConfidenceIndicator.QA1";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QB4 = "a001.Mode3ACodeConfidenceIndicator.QB4";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QB2 = "a001.Mode3ACodeConfidenceIndicator.QB2";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QB1 = "a001.Mode3ACodeConfidenceIndicator.QB1";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QC4 = "a001.Mode3ACodeConfidenceIndicator.QC4";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QC2 = "a001.Mode3ACodeConfidenceIndicator.QC2";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QC1 = "a001.Mode3ACodeConfidenceIndicator.QC1";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QD4 = "a001.Mode3ACodeConfidenceIndicator.QD4";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QD2 = "a001.Mode3ACodeConfidenceIndicator.QD2";
const QString Asterix001Proto::Mode3ACodeConfidenceIndicator_QD1 = "a001.Mode3ACodeConfidenceIndicator.QD1";

const QString Asterix001Proto::ModeCCodeInBinaryRepresentation_V = "a001.ModeCCodeInBinaryRepresentation.V";
const QString Asterix001Proto::ModeCCodeInBinaryRepresentation_G = "a001.ModeCCodeInBinaryRepresentation.G";
const QString Asterix001Proto::ModeCCodeInBinaryRepresentation_HEIGHT = "a001.ModeCCodeInBinaryRepresentation.HEIGHT";

const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_V = "a001.ModeCCodeAndCodeConfidenceIndicator.V";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_G = "a001.ModeCCodeAndCodeConfidenceIndicator.G";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_C1 = "a001.ModeCCodeAndCodeConfidenceIndicator.C1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_A1 = "a001.ModeCCodeAndCodeConfidenceIndicator.A1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_C2 = "a001.ModeCCodeAndCodeConfidenceIndicator.C2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_A2 = "a001.ModeCCodeAndCodeConfidenceIndicator.A2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_C4 = "a001.ModeCCodeAndCodeConfidenceIndicator.C4";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_A4 = "a001.ModeCCodeAndCodeConfidenceIndicator.A4";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_B1 = "a001.ModeCCodeAndCodeConfidenceIndicator.B1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_D1 = "a001.ModeCCodeAndCodeConfidenceIndicator.D1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_B2 = "a001.ModeCCodeAndCodeConfidenceIndicator.B2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_D2 = "a001.ModeCCodeAndCodeConfidenceIndicator.D2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_B4 = "a001.ModeCCodeAndCodeConfidenceIndicator.B4";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_D4 = "a001.ModeCCodeAndCodeConfidenceIndicator.D4";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QC1 = "a001.ModeCCodeAndCodeConfidenceIndicator.QC1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QA1 = "a001.ModeCCodeAndCodeConfidenceIndicator.QA1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QC2 = "a001.ModeCCodeAndCodeConfidenceIndicator.QC2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QA2 = "a001.ModeCCodeAndCodeConfidenceIndicator.QA2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QC4 = "a001.ModeCCodeAndCodeConfidenceIndicator.QC4";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QA4 = "a001.ModeCCodeAndCodeConfidenceIndicator.QA4";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QB1 = "a001.ModeCCodeAndCodeConfidenceIndicator.QB1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QD1 = "a001.ModeCCodeAndCodeConfidenceIndicator.QD1";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QB2 = "a001.ModeCCodeAndCodeConfidenceIndicator.QB2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QD2 = "a001.ModeCCodeAndCodeConfidenceIndicator.QD2";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QB4 = "a001.ModeCCodeAndCodeConfidenceIndicator.QB4";
const QString Asterix001Proto::ModeCCodeAndCodeConfidenceIndicator_QD4 = "a001.ModeCCodeAndCodeConfidenceIndicator.QD4";

const QString Asterix001Proto::MeasuredRadialDopplerSpeed = "a001.MeasuredRadialDopplerSpeed";

const QString Asterix001Proto::RadarPlotCharacteristics = "a001.RadarPlotCharacteristics";

const QString Asterix001Proto::ReceivedPower = "a001.ReceivedPower";

const QString Asterix001Proto::TruncatedTimeOfDay = "a001.TruncatedTimeOfDay";

const QString Asterix001Proto::PresenceOfXPulse_XA = "a001.PresenceOfXPulse.XA";
const QString Asterix001Proto::PresenceOfXPulse_XC = "a001.PresenceOfXPulse.XC";
const QString Asterix001Proto::PresenceOfXPulse_X2 = "a001.PresenceOfXPulse.X2";

const QString Asterix001Proto::TrackPlotNumber = "a001.TrackPlotNumber";

const QString Asterix001Proto::TrackStatus_CON = "a001.TrackStatus.CON";
const QString Asterix001Proto::TrackStatus_RAD = "a001.TrackStatus.RAD";
const QString Asterix001Proto::TrackStatus_MAN = "a001.TrackStatus.MAN";
const QString Asterix001Proto::TrackStatus_DOU = "a001.TrackStatus.DOU";
const QString Asterix001Proto::TrackStatus_RDPC = "a001.TrackStatus.RDPC";
const QString Asterix001Proto::TrackStatus_GHO = "a001.TrackStatus.GHO";
const QString Asterix001Proto::TrackStatus_TRE = "a001.TrackStatus.TRE";

const QString Asterix001Proto::CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED = "a001.CalculatedTrackVelocityInPolarCoordinates.GROUNDSPEED";
const QString Asterix001Proto::CalculatedTrackVelocityInPolarCoordinates_HEADING = "a001.CalculatedTrackVelocityInPolarCoordinates.HEADING";

const QString Asterix001Proto::TrackQuality = "a001.TrackQuality";

Asterix001Proto::Asterix001Proto()
{}

int Asterix001Proto::category() const
{
    return mCategory;
}

QString Asterix001Proto::edition() const
{
    return mEdition;
}

void Asterix001Proto::parse(QDataStream &stream, AsterixData &asterix)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_010(stream, asterix);
        else
            asterix.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_020(stream, asterix);
        else
            asterix.appendError(mandatory("Target Report Descriptor"));

        if (asterix.value(TargetReportDescriptor_TYP).toBool())
            parseAsTrack(mainMask, stream, asterix);
        else
            parseAsPlot(mainMask, stream, asterix);

    }
    catch (AsterixErrors::ErrorCode error)
    {
        asterix.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        asterix.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            asterix.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
}

void Asterix001Proto::parseAsTrack(const PresentData &mainMask, QDataStream &stream, AsterixData &asterix)
{
    if (mainMask.isSet(3))
        parse_161(stream, asterix);
    else
        asterix.appendError(mandatory("Track Plot Number"));

    if (mainMask.isSet(4))
        parse_040(stream, asterix);

    if (mainMask.isSet(5))
        parse_042(stream, asterix);

    if (mainMask.isSet(6))
        parse_200(stream, asterix);

    if (mainMask.isSet(7))
        parse_070(stream, asterix);
    else
        asterix.appendError(mandatory("Mode-3/A Code in Octal Representation"));

    if (mainMask.isSet(8))
        parse_090(stream, asterix);
    else
        asterix.appendError(mandatory("Mode-C Code in Binary Representation"));

    if (mainMask.isSet(9))
        parse_141(stream, asterix);

    if (mainMask.isSet(10))
        parse_130(stream, asterix);

    if (mainMask.isSet(11))
        parse_131(stream, asterix);

    if (mainMask.isSet(12))
        parse_120(stream, asterix);

    if (mainMask.isSet(13))
        parse_170(stream, asterix);
    else
        asterix.appendError(mandatory("Track Status"));

    if (mainMask.isSet(14))
        parse_210(stream, asterix);

    if (mainMask.isSet(15))
        parse_050(stream, asterix);

    if (mainMask.isSet(16))
        parse_080(stream, asterix);

    if (mainMask.isSet(17))
        parse_100(stream, asterix);

    if (mainMask.isSet(18))
        parse_060(stream, asterix);

    if (mainMask.isSet(19))
        parse_030(stream, asterix);

    if (mainMask.isSet(20))
        parse_sp(stream, asterix);

    if (mainMask.isSet(21))
        parse_re(stream, asterix);

    if (mainMask.isSet(22))
        parse_150(stream, asterix);
}

void Asterix001Proto::parseAsPlot(const PresentData &mainMask, QDataStream &stream, AsterixData &asterix)
{
    if (mainMask.isSet(3))
        parse_040(stream, asterix);
    else
        asterix.appendError(mandatory("Measured Position in Polar Coordinates"));

    if (mainMask.isSet(4))
        parse_070(stream, asterix);
    else if (asterix.value(TargetReportDescriptor_SSRPSR).toUInt() & M2)
        asterix.appendError(mandatory("Mode-3/A Code in Octal Representation"));

    if (mainMask.isSet(5))
        parse_090(stream, asterix);
    else if (asterix.value(TargetReportDescriptor_SSRPSR).toUInt() & M2)
        asterix.appendError(mandatory("Mode-C Code in Binary Representation"));

    if (mainMask.isSet(6))
        parse_130(stream, asterix);

    if (mainMask.isSet(7))
        parse_141(stream, asterix);

    if (mainMask.isSet(8))
        parse_050(stream, asterix);

    if (mainMask.isSet(9))
        parse_120(stream, asterix);

    if (mainMask.isSet(10))
        parse_131(stream, asterix);

    if (mainMask.isSet(11))
        parse_080(stream, asterix);

    if (mainMask.isSet(12))
        parse_100(stream, asterix);

    if (mainMask.isSet(13))
        parse_060(stream, asterix);

    if (mainMask.isSet(14))
        parse_030(stream, asterix);

    if (mainMask.isSet(15))
        parse_150(stream, asterix);

    if (mainMask.isSet(20))
        parse_sp(stream, asterix);

    if (mainMask.isSet(21))
        parse_re(stream, asterix);
}

QByteArray Asterix001Proto::generate(AsterixData &asterix)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        if (generate_010(asterix, stream))
            mainMask.set(1);
        else
            asterix.appendError(mandatory("Data Source Identifier"));

        if (generate_020(asterix, stream))
            mainMask.set(2);
        else
            asterix.appendError(mandatory("Target Report Descriptor"));

        if (asterix.value(TargetReportDescriptor_TYP).toBool())
            generateAsTrack(mainMask, asterix, stream);
        else
            generateAsPlot(mainMask, asterix, stream);
    }
    catch (AsterixErrors::ErrorCode error)
    {
        asterix.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        asterix.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            asterix.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
    return result.prepend(mainMask.data());
}

void Asterix001Proto::generateAsTrack(PresentData &mainMask, AsterixData &asterix, QDataStream &stream)
{
    if (generate_161(asterix, stream))
        mainMask.set(3);
    else
        asterix.appendError(mandatory("Track Plot Number"));

    if (generate_040(asterix, stream))
        mainMask.set(4);

    if (generate_042(asterix, stream))
        mainMask.set(5);

    if (generate_200(asterix, stream))
        mainMask.set(6);

    if (generate_070(asterix, stream))
        mainMask.set(7);
    else
        asterix.appendError(mandatory("Mode-3/A Code in Octal Representation"));

    if (generate_090(asterix, stream))
        mainMask.set(8);
    else
        asterix.appendError(mandatory("Mode-C Code in Binary Representation"));

    if (generate_141(asterix, stream))
        mainMask.set(9);

    if (generate_130(asterix, stream))
        mainMask.set(10);

    if (generate_131(asterix, stream))
        mainMask.set(11);

    if (generate_120(asterix, stream))
        mainMask.set(12);

    if (generate_170(asterix, stream))
        mainMask.set(13);
    else
        asterix.appendError(mandatory("Track Status"));

    if (generate_210(asterix, stream))
        mainMask.set(14);

    if (generate_050(asterix, stream))
        mainMask.set(15);

    if (generate_080(asterix, stream))
        mainMask.set(16);

    if (generate_100(asterix, stream))
        mainMask.set(17);

    if (generate_060(asterix, stream))
        mainMask.set(18);

    if (generate_030(asterix, stream))
        mainMask.set(19);

    if (generate_sp(asterix, stream))
        mainMask.set(20);

    if (generate_re(asterix, stream))
        mainMask.set(20);

    if (generate_150(asterix, stream))
        mainMask.set(22);
}

void Asterix001Proto::generateAsPlot(PresentData &mainMask, AsterixData &asterix, QDataStream &stream)
{
    if (generate_040(asterix, stream))
        mainMask.set(3);
    else
        asterix.appendError(mandatory("Measured Position in Polar Coordinates"));

    if (generate_070(asterix, stream))
        mainMask.set(4);
    else if (asterix.value(TargetReportDescriptor_SSRPSR).toUInt() & M2)
        asterix.appendError(mandatory("Mode-3/A Code in Octal Representation"));

    if (generate_090(asterix, stream))
        mainMask.set(5);
    else if (asterix.value(TargetReportDescriptor_SSRPSR).toUInt() & M2)
        asterix.appendError(mandatory("Mode-C Code in Binary Representation"));

    if (generate_130(asterix, stream))
        mainMask.set(6);

    if (generate_141(asterix, stream))
        mainMask.set(7);

    if (generate_050(asterix, stream))
        mainMask.set(8);

    if (generate_120(asterix, stream))
        mainMask.set(9);

    if (generate_131(asterix, stream))
        mainMask.set(10);

    if (generate_080(asterix, stream))
        mainMask.set(11);

    if (generate_100(asterix, stream))
        mainMask.set(12);

    if (generate_060(asterix, stream))
        mainMask.set(13);

    if (generate_030(asterix, stream))
        mainMask.set(14);

    if (generate_150(asterix, stream))
        mainMask.set(15);

    if (generate_sp(asterix, stream))
        mainMask.set(20);

    if (generate_re(asterix, stream))
        mainMask.set(20);
}

QStringList Asterix001Proto::fields() const
{
    static QStringList result = QStringList()

        << DataSourceIdentifier_SAC
        << DataSourceIdentifier_SIC

        << TargetReportDescriptor_TYP
        << TargetReportDescriptor_SIM
        << TargetReportDescriptor_SSRPSR
        << TargetReportDescriptor_ANT
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

        << ModeCCodeInBinaryRepresentation_V
        << ModeCCodeInBinaryRepresentation_G
        << ModeCCodeInBinaryRepresentation_HEIGHT

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

        << MeasuredRadialDopplerSpeed

        << RadarPlotCharacteristics

        << ReceivedPower

        << TruncatedTimeOfDay

        << PresenceOfXPulse_XA
        << PresenceOfXPulse_XC
        << PresenceOfXPulse_X2

        << TrackPlotNumber

        << TrackStatus_CON
        << TrackStatus_RAD
        << TrackStatus_MAN
        << TrackStatus_DOU
        << TrackStatus_RDPC
        << TrackStatus_GHO
        << TrackStatus_TRE

        << CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED
        << CalculatedTrackVelocityInPolarCoordinates_HEADING

        << TrackQuality;

    return result;
}

void Asterix001Proto::parse_010(QDataStream &stream, AsterixData &map) // Data Item I001/010, Data Source Identifier
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Asterix001Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(TargetReportDescriptor_TYP, data >> 7 & M1);
    map.insert(TargetReportDescriptor_SIM, data >> 6 & M1);
    map.insert(TargetReportDescriptor_SSRPSR, data >> 4 & M2);
    map.insert(TargetReportDescriptor_ANT, data >> 3 & M1);
    map.insert(TargetReportDescriptor_SPI, data >> 2 & M1);
    map.insert(TargetReportDescriptor_RAB, data >> 1 & M1);

    QList<I001_020_FX> extension;
    while (data & M1)
    {
        data = read<quint8>(stream);
        I001_020_FX ext;
        ext.TST = data >> 7 & M1;
        ext.DS1DS2 = data >> 5 & M2;
        ext.ME = data >> 4 & M1;
        ext.MI = data >> 3 & M1;
        extension.push_back(ext);
    }

    if (!extension.isEmpty())
        map.insert(TargetReportDescriptor_FX, QVariant::fromValue(extension));
}

void Asterix001Proto::parse_030(QDataStream &stream, AsterixData &map)
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

void Asterix001Proto::parse_040(QDataStream &stream, AsterixData &map)
{
    map.insert(MeasuredPositionInPolarCoordinates_RHO, double(read<quint16>(stream)) / 128); // <= 512
    map.insert(MeasuredPositionInPolarCoordinates_THETA, double(read<quint16>(stream)) * 360 / 65536 );
}

void Asterix001Proto::parse_042(QDataStream &stream, AsterixData &map)
{
    map.insert(CalculatedPositionInCartesianCoordinates_X, double(read<qint16>(stream)) / 64);
    map.insert(CalculatedPositionInCartesianCoordinates_Y, double(read<qint16>(stream)) / 64);
}

void Asterix001Proto::parse_050(QDataStream &stream, AsterixData &map)
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

void Asterix001Proto::parse_060(QDataStream &stream, AsterixData &map)
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

void Asterix001Proto::parse_070(QDataStream &stream, AsterixData &map)
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

void Asterix001Proto::parse_080(QDataStream &stream, AsterixData &map)
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

void Asterix001Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    qint16 data = read<qint16>(stream);
    map.insert(ModeCCodeInBinaryRepresentation_V, data >> 15 & M1);
    map.insert(ModeCCodeInBinaryRepresentation_G, data >> 14 & M1);
    map.insert(ModeCCodeInBinaryRepresentation_HEIGHT, double(restoreSign(data, 14)) * 25);
}

void Asterix001Proto::parse_100(QDataStream &stream, AsterixData &map)
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

void Asterix001Proto::parse_120(QDataStream &stream, AsterixData &map)
{
    map.insert(MeasuredRadialDopplerSpeed, double(read<qint8>(stream)) / 16384);
}

void Asterix001Proto::parse_130(QDataStream &stream, AsterixData &map)
{
    quint8 data;
    QList<quint8> result;
    do
    {
        data = read<quint8>(stream);
        result.push_back(data >> 1 & M7);
    } while(data & M1);
    map.insert(RadarPlotCharacteristics, QVariant::fromValue(result));
}

void Asterix001Proto::parse_131(QDataStream &stream, AsterixData &map)
{
    map.insert(ReceivedPower, read<qint8>(stream));
}

void Asterix001Proto::parse_141(QDataStream &stream, AsterixData &map)
{
    map.insert(TruncatedTimeOfDay, double(read<quint16>(stream)) / 128);
}

void Asterix001Proto::parse_150(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(PresenceOfXPulse_XA, data >> 7 & M1);
    map.insert(PresenceOfXPulse_XC, data >> 5 & M1);
    map.insert(PresenceOfXPulse_X2, data >> 2 & M1);
}

void Asterix001Proto::parse_161(QDataStream &stream, AsterixData &map)
{
    map.insert(TrackPlotNumber, read<quint16>(stream));
}

void Asterix001Proto::parse_170(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(TrackStatus_CON, data >> 7 & M1);
    map.insert(TrackStatus_RAD, data >> 6 & M1);
    map.insert(TrackStatus_MAN, data >> 5 & M1);
    map.insert(TrackStatus_DOU, data >> 4 & M1);
    map.insert(TrackStatus_RDPC, data >> 3 & M1);
    map.insert(TrackStatus_GHO, data >> 1 & M1);
    if (!(data & M1))
        return;

    data = read<quint8>(stream);
    map.insert(TrackStatus_TRE, data >> 7 & M1);

    while (data & M1)
        data = read<quint8>(stream);
}

void Asterix001Proto::parse_200(QDataStream &stream, AsterixData &map)
{
    map.insert(CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED, double(read<quint16>(stream)) / 16384);
    map.insert(CalculatedTrackVelocityInPolarCoordinates_HEADING, double(read<quint16>(stream)) * 360 / 65536);
}

void Asterix001Proto::parse_210(QDataStream &stream, AsterixData &map)
{
    quint8 data;
    QList<quint8> result;
    do
    {
        data = read<quint8>(stream);
        result.push_back(data >> 1 & M7);
    } while(data & M1);
    map.insert(TrackQuality, QVariant::fromValue(result));
}

bool Asterix001Proto::generate_010(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_020(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TargetReportDescriptor_TYP)
        && map.contains(TargetReportDescriptor_SIM)
        && map.contains(TargetReportDescriptor_SSRPSR)
        && map.contains(TargetReportDescriptor_ANT)
        && map.contains(TargetReportDescriptor_SPI)
        && map.contains(TargetReportDescriptor_RAB))
    {
        quint8 data = (map.value(TargetReportDescriptor_TYP).toUInt() & M1) << 7;
        data |= (map.value(TargetReportDescriptor_SIM).toUInt() & M1) << 6;
        data |= (map.value(TargetReportDescriptor_SSRPSR).toUInt() & M2) << 5;
        data |= (map.value(TargetReportDescriptor_ANT).toUInt() & M1) << 3;
        data |= (map.value(TargetReportDescriptor_SPI).toUInt() & M1) << 2;
        data |= (map.value(TargetReportDescriptor_RAB).toUInt() & M1) << 1;

        if (map.contains(TargetReportDescriptor_FX))
        {
            QList<I001_020_FX> extension = map.value(TargetReportDescriptor_FX).value< QList<I001_020_FX> >();
            foreach (const I001_020_FX &ext, extension)
            {
                data |= M1;
                write8(stream, data);
                data = (ext.TST & M1) << 7;
                data |= (ext.DS1DS2 & M2) << 5;
                data |= (ext.ME & M1) << 4;
                data |= (ext.MI & M1) << 3;
            }
        }
        write8(stream, data);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_030(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_040(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MeasuredPositionInPolarCoordinates_RHO) && map.contains(MeasuredPositionInPolarCoordinates_THETA))
    {
        write16(stream, map.value(MeasuredPositionInPolarCoordinates_RHO).toDouble() * 128);
        write16(stream, map.value(MeasuredPositionInPolarCoordinates_THETA).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_042(AsterixData &map, QDataStream &stream)
{
    if (map.contains(CalculatedPositionInCartesianCoordinates_X) && map.contains(CalculatedPositionInCartesianCoordinates_Y))
    {
        write16(stream, map.value(CalculatedPositionInCartesianCoordinates_X).toDouble() * 64);
        write16(stream, map.value(CalculatedPositionInCartesianCoordinates_Y).toDouble() * 64);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_050(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_060(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_070(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_080(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ModeCCodeInBinaryRepresentation_V)
        && map.contains(ModeCCodeInBinaryRepresentation_G)
        && map.contains(ModeCCodeInBinaryRepresentation_HEIGHT))
        {
        qint16 data = (map.value(ModeCCodeInBinaryRepresentation_V).toUInt() & M1) << 15;
        data |= (map.value(ModeCCodeInBinaryRepresentation_G).toUInt() & M1) << 14;
        data |= qint16(map.value(ModeCCodeInBinaryRepresentation_HEIGHT).toDouble() / 25) & M14;
        write16(stream, data);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_100(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_120(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MeasuredRadialDopplerSpeed))
    {
        write8(stream, map.value(MeasuredRadialDopplerSpeed).toDouble() * 16384);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_130(AsterixData &map, QDataStream &stream)
{
    if (map.contains(RadarPlotCharacteristics))
    {
        const QList<quint8> vector = map.value(RadarPlotCharacteristics).value< QList<quint8> >();
        if (vector.isEmpty())
            return false;

        quint8 data = (vector.first() & M7) << 1;
        foreach (quint8 val, vector)
        {
            write8(stream, data | M1);
            data = (val & M7) << 1;
        }
        write8(stream, data);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_131(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ReceivedPower))
    {
        write8(stream, map.value(ReceivedPower).toInt());
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_141(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TruncatedTimeOfDay))
    {
        write16(stream, map.value(TruncatedTimeOfDay).toDouble() * 128);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_150(AsterixData &map, QDataStream &stream)
{
    if (map.contains(PresenceOfXPulse_XA)
        && map.contains(PresenceOfXPulse_XC)
        && map.contains(PresenceOfXPulse_X2))
        {
        quint8 data;
        data = (map.value(PresenceOfXPulse_XA).toUInt() & M1) << 7;
        data |= (map.value(PresenceOfXPulse_XC).toUInt() & M1) << 5;
        data |= (map.value(PresenceOfXPulse_X2).toUInt() & M1) << 2;
        write8(stream, data);
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_161(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TrackPlotNumber))
    {
        write16(stream, map.value(TrackPlotNumber).toUInt());
        return true;
    }
    return false;
}

bool Asterix001Proto::generate_170(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TrackStatus_CON)
        && map.contains(TrackStatus_RAD)
        && map.contains(TrackStatus_MAN)
        && map.contains(TrackStatus_DOU)
        && map.contains(TrackStatus_RDPC)
        && map.contains(TrackStatus_GHO))
    {
        quint8 data;
        data = (map.value(TrackStatus_CON).toUInt() & M1) << 7;
        data |= (map.value(TrackStatus_RAD).toUInt() & M1) << 6;
        data |= (map.value(TrackStatus_MAN).toUInt() & M1) << 5;
        data |= (map.value(TrackStatus_DOU).toUInt() & M1) << 4;
        data |= (map.value(TrackStatus_RDPC).toUInt() & M1) << 3;
        data |= (map.value(TrackStatus_GHO).toUInt() & M1) << 1;

        if (map.contains(TrackStatus_TRE))
        {
            write8(stream, data | M1);
            data = (map.value(TrackStatus_TRE).toUInt() & M1) << 7;
            write8(stream, data);
        }
        else
            write8(stream, data);

        return true;
    }
    return false;
}

bool Asterix001Proto::generate_200(AsterixData &map, QDataStream &stream)
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

bool Asterix001Proto::generate_210(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TrackQuality))
    {
        const QList<quint8> vector = map.value(TrackQuality).value< QList<quint8> >();
        if (vector.isEmpty())
            return false;

        quint8 data = (vector.first() & M7) << 1;
        foreach (quint8 val, vector)
        {
            write8(stream, data | M1);
            data = (val & M7) << 1;
        }
        write8(stream, data);
        return true;
    }
    return false;
}
