#ifndef ASTERIX048_PROTO_H
#define ASTERIX048_PROTO_H

#include "asterix.h"

#include <qglobal.h>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>

class AsterixData;

class ASTERIX_EXPORT Asterix048Proto : public Asterix
{
    void parse_010(QDataStream &stream, AsterixData &result);
    void parse_020(QDataStream &stream, AsterixData &result);
    void parse_030(QDataStream &stream, AsterixData &result);
    void parse_040(QDataStream &stream, AsterixData &result);
    void parse_042(QDataStream &stream, AsterixData &result);
    void parse_050(QDataStream &stream, AsterixData &result);
    void parse_055(QDataStream &stream, AsterixData &result);
    void parse_060(QDataStream &stream, AsterixData &result);
    void parse_065(QDataStream &stream, AsterixData &result);
    void parse_070(QDataStream &stream, AsterixData &result);
    void parse_080(QDataStream &stream, AsterixData &result);
    void parse_090(QDataStream &stream, AsterixData &result);
    void parse_100(QDataStream &stream, AsterixData &result);
    void parse_110(QDataStream &stream, AsterixData &result);
    void parse_120(QDataStream &stream, AsterixData &result);
    void parse_130(QDataStream &stream, AsterixData &result);
    void parse_140(QDataStream &stream, AsterixData &result);
    void parse_161(QDataStream &stream, AsterixData &result);
    void parse_170(QDataStream &stream, AsterixData &result);
    void parse_200(QDataStream &stream, AsterixData &result);
    void parse_210(QDataStream &stream, AsterixData &result);
    void parse_220(QDataStream &stream, AsterixData &result);
    void parse_230(QDataStream &stream, AsterixData &result);
    void parse_240(QDataStream &stream, AsterixData &result);
    void parse_250(QDataStream &stream, AsterixData &result);
    void parse_260(QDataStream &stream, AsterixData &result);

    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_020(AsterixData &map, QDataStream &stream);
    bool generate_030(AsterixData &map, QDataStream &stream);
    bool generate_040(AsterixData &map, QDataStream &stream);
    bool generate_042(AsterixData &map, QDataStream &stream);
    bool generate_050(AsterixData &map, QDataStream &stream);
    bool generate_055(AsterixData &map, QDataStream &stream);
    bool generate_060(AsterixData &map, QDataStream &stream);
    bool generate_065(AsterixData &map, QDataStream &stream);
    bool generate_070(AsterixData &map, QDataStream &stream);
    bool generate_080(AsterixData &map, QDataStream &stream);
    bool generate_090(AsterixData &map, QDataStream &stream);
    bool generate_100(AsterixData &map, QDataStream &stream);
    bool generate_110(AsterixData &map, QDataStream &stream);
    bool generate_120(AsterixData &map, QDataStream &stream);
    bool generate_130(AsterixData &map, QDataStream &stream);
    bool generate_140(AsterixData &map, QDataStream &stream);
    bool generate_161(AsterixData &map, QDataStream &stream);
    bool generate_170(AsterixData &map, QDataStream &stream);
    bool generate_200(AsterixData &map, QDataStream &stream);
    bool generate_210(AsterixData &map, QDataStream &stream);
    bool generate_220(AsterixData &map, QDataStream &stream);
    bool generate_230(AsterixData &map, QDataStream &stream);
    bool generate_240(AsterixData &map, QDataStream &stream);
    bool generate_250(AsterixData &map, QDataStream &stream);
    bool generate_260(AsterixData &map, QDataStream &stream);

public:
    Asterix048Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Data Item I048/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC;
    static const QString DataSourceIdentifier_SIC;

    // Data Item I048/020, Target Report Descriptor
    static const QString TargetReportDescriptor_TYP;
    static const QString TargetReportDescriptor_SIM;
    static const QString TargetReportDescriptor_RDP;
    static const QString TargetReportDescriptor_SPI;
    static const QString TargetReportDescriptor_RAB;
    static const QString TargetReportDescriptor_FX; // QList<I048_020_FX>

    struct I048_020_FX
    {
        I048_020_FX() : TST(0), ME(0), MI(0), FOEFRI(0) {}
        quint8 TST : 1;
        quint8 ME : 1;
        quint8 MI : 1;
        quint8 FOEFRI : 2;
    };

    // Data Item I048/030, Warning/Error Conditions
    static const QString WarningErrorConditions; // QList<quint8>

    // Data Item I048/040, Measured Position in Polar Co-ordinates
    static const QString MeasuredPositionInPolarCoordinates_RHO;
    static const QString MeasuredPositionInPolarCoordinates_THETA;

    // Data Item I048/042, Calculated Position in Cartesian Co-ordinates
    static const QString CalculatedPositionInCartesianCoordinates_X;
    static const QString CalculatedPositionInCartesianCoordinates_Y;

    // Data Item I048/050, Mode-2 Code in Octal Representation
    static const QString Mode2CodeInOctalRepresentation_V;
    static const QString Mode2CodeInOctalRepresentation_G;
    static const QString Mode2CodeInOctalRepresentation_L;
    static const QString Mode2CodeInOctalRepresentation_A4;
    static const QString Mode2CodeInOctalRepresentation_A2;
    static const QString Mode2CodeInOctalRepresentation_A1;
    static const QString Mode2CodeInOctalRepresentation_B4;
    static const QString Mode2CodeInOctalRepresentation_B2;
    static const QString Mode2CodeInOctalRepresentation_B1;
    static const QString Mode2CodeInOctalRepresentation_C4;
    static const QString Mode2CodeInOctalRepresentation_C2;
    static const QString Mode2CodeInOctalRepresentation_C1;
    static const QString Mode2CodeInOctalRepresentation_D4;
    static const QString Mode2CodeInOctalRepresentation_D2;
    static const QString Mode2CodeInOctalRepresentation_D1;

    // Data Item I048/055, Mode-1 Code in Octal Representation
    static const QString Mode1CodeInOctalRepresentation_V;
    static const QString Mode1CodeInOctalRepresentation_G;
    static const QString Mode1CodeInOctalRepresentation_L;
    static const QString Mode1CodeInOctalRepresentation_A4;
    static const QString Mode1CodeInOctalRepresentation_A2;
    static const QString Mode1CodeInOctalRepresentation_A1;
    static const QString Mode1CodeInOctalRepresentation_B2;
    static const QString Mode1CodeInOctalRepresentation_B1;

    // Data Item I048/060, Mode-2 Code Confidence Indicator
    static const QString Mode2CodeConfidenceIndicator_QA4;
    static const QString Mode2CodeConfidenceIndicator_QA2;
    static const QString Mode2CodeConfidenceIndicator_QA1;
    static const QString Mode2CodeConfidenceIndicator_QB4;
    static const QString Mode2CodeConfidenceIndicator_QB2;
    static const QString Mode2CodeConfidenceIndicator_QB1;
    static const QString Mode2CodeConfidenceIndicator_QC4;
    static const QString Mode2CodeConfidenceIndicator_QC2;
    static const QString Mode2CodeConfidenceIndicator_QC1;
    static const QString Mode2CodeConfidenceIndicator_QD4;
    static const QString Mode2CodeConfidenceIndicator_QD2;
    static const QString Mode2CodeConfidenceIndicator_QD1;

    // Data Item I048/065, Mode-1 Code Confidence Indicator
    static const QString Mode1CodeConfidenceIndicator_QA4;
    static const QString Mode1CodeConfidenceIndicator_QA2;
    static const QString Mode1CodeConfidenceIndicator_QA1;
    static const QString Mode1CodeConfidenceIndicator_QB2;
    static const QString Mode1CodeConfidenceIndicator_QB1;

    // Data Item I048/070, Mode-3/A Code in Octal Representation
    static const QString Mode3ACodeInOctalRepresentation_V;
    static const QString Mode3ACodeInOctalRepresentation_G;
    static const QString Mode3ACodeInOctalRepresentation_L;
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

    // Data Item I048/080, Mode-3/A Code Confidence Indicator
    static const QString Mode3ACodeConfidenceIndicator_QA4;
    static const QString Mode3ACodeConfidenceIndicator_QA2;
    static const QString Mode3ACodeConfidenceIndicator_QA1;
    static const QString Mode3ACodeConfidenceIndicator_QB4;
    static const QString Mode3ACodeConfidenceIndicator_QB2;
    static const QString Mode3ACodeConfidenceIndicator_QB1;
    static const QString Mode3ACodeConfidenceIndicator_QC4;
    static const QString Mode3ACodeConfidenceIndicator_QC2;
    static const QString Mode3ACodeConfidenceIndicator_QC1;
    static const QString Mode3ACodeConfidenceIndicator_QD4;
    static const QString Mode3ACodeConfidenceIndicator_QD2;
    static const QString Mode3ACodeConfidenceIndicator_QD1;

    // Data Item I048/090, Flight Level in Binary Representation
    static const QString FlightLevelInBinaryRepresentation_V;
    static const QString FlightLevelInBinaryRepresentation_G;
    static const QString FlightLevelInBinaryRepresentation_FlightLevel;

    // Data Item I048/100, Mode-C Code and Code Confidence Indicator
    static const QString ModeCCodeAndCodeConfidenceIndicator_V;
    static const QString ModeCCodeAndCodeConfidenceIndicator_G;
    static const QString ModeCCodeAndCodeConfidenceIndicator_C1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_A1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_C2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_A2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_C4;
    static const QString ModeCCodeAndCodeConfidenceIndicator_A4;
    static const QString ModeCCodeAndCodeConfidenceIndicator_B1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_D1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_B2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_D2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_B4;
    static const QString ModeCCodeAndCodeConfidenceIndicator_D4;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QC1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QA1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QC2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QA2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QC4;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QA4;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QB1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QD1;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QB2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QD2;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QB4;
    static const QString ModeCCodeAndCodeConfidenceIndicator_QD4;

    // Data Item I048/110, Height Measured by a 3D Radar
    static const QString HeightMeasuredBy3DRadar;

    // Data Item I048/120, Radial Doppler Speed
    static const QString RadialDopplerSpeed_CalculatedDopplerSpeed_D;
    static const QString RadialDopplerSpeed_CalculatedDopplerSpeed_CAL;
    static const QString RadialDopplerSpeed_RawDopplerSpeed; // QList<I048_120_2>

    struct I048_120_2
    {
        I048_120_2() : DopplerSpeed(0), AmbiguityRange(0), TransmitterFrequency(0) {}
        quint16 DopplerSpeed;
        quint16 AmbiguityRange;
        quint16 TransmitterFrequency;
    };

    // Data Item I048/130, Radar Plot Characteristics
    static const QString RadarPlotCharacteristics_SsrPlotRunlength;
    static const QString RadarPlotCharacteristics_NumberOfReceivedRepliesForMSsr;
    static const QString RadarPlotCharacteristics_AmplitudeOfMSSRReply;
    static const QString RadarPlotCharacteristics_PrimaryPlotRunlength;
    static const QString RadarPlotCharacteristics_AmplitudeOfPrimaryPlot;
    static const QString RadarPlotCharacteristics_DifferenceInRangeBetweenPsrAndSsrPlot;
    static const QString RadarPlotCharacteristics_DifferenceInAzimuthBetweenPsrSndSsrPlot;

    // Data Item I048/140, Time of Day
    static const QString TimeOfDay;

    // Data Item I048/161, Track Number
    static const QString TrackNumber;

    // Data Item I048/170, Track Status
    static const QString TrackStatus_CNF;
    static const QString TrackStatus_RAD;
    static const QString TrackStatus_DOU;
    static const QString TrackStatus_MAH;
    static const QString TrackStatus_CDM;
    static const QString TrackStatus_FX; // QList<I048_170_FX>

    struct I048_170_FX
    {
        I048_170_FX() : TRE(false), GHO(false), SUP(false), TCC(false) {}
        bool TRE;
        bool GHO;
        bool SUP;
        bool TCC;
    };

    // Data Item I048/200, Calculated Track Velocity in Polar Co-ordinates
    static const QString CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED;
    static const QString CalculatedTrackVelocityInPolarCoordinates_HEADING;

    // Data Item I048/210, Track Quality
    static const QString TrackQuality_X;
    static const QString TrackQuality_Y;
    static const QString TrackQuality_V;
    static const QString TrackQuality_H;

    // Data Item I048/220, Aircraft Address
    static const QString AircraftAddress;

    // Data Item I048/230, Communications/ACAS Capability and Flight Status
    static const QString CommunicationsAcasCapabilityAndFlightStatus_COM;
    static const QString CommunicationsAcasCapabilityAndFlightStatus_STAT;
    static const QString CommunicationsAcasCapabilityAndFlightStatus_SI;
    static const QString CommunicationsAcasCapabilityAndFlightStatus_MSSC;
    static const QString CommunicationsAcasCapabilityAndFlightStatus_ARC;
    static const QString CommunicationsAcasCapabilityAndFlightStatus_AIC;
    static const QString CommunicationsAcasCapabilityAndFlightStatus_B1A;
    static const QString CommunicationsAcasCapabilityAndFlightStatus_B1B;

    // Data Item I048/240, Aircraft Identification
    static const QString AircraftIdentification;

    // Data Item I048/250, Mode S MB Data
    static const QString ModeSMbData; // QList<I048_250>

    struct I048_250
    {
        I048_250() : BDS1(0), BDS2(0) { for (int i = 0; i < 7; ++i) MbData[i] = 0; }
        quint8 MbData[7];
        quint8 BDS1 : 4;
        quint8 BDS2 : 4;
    };

    // Data Item I048/260, ACAS Resolution Advisory Report
    static const QString AcasResolutionAdvisoryReport;
};

Q_DECLARE_METATYPE(QList<Asterix048Proto::I048_020_FX>)
Q_DECLARE_METATYPE(QList<Asterix048Proto::I048_120_2>)
Q_DECLARE_METATYPE(QList<Asterix048Proto::I048_170_FX>)
Q_DECLARE_METATYPE(QList<Asterix048Proto::I048_250>)

#endif // ASTERIX048_PROTO_H
