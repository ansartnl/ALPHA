#ifndef ASTERIX001_PROTO_H
#define ASTERIX001_PROTO_H

#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QPair>

class PresentData;

class ASTERIX_EXPORT Asterix001Proto : public Asterix
{
protected:
    void parse_010(QDataStream &stream, AsterixData &result);
    void parse_020(QDataStream &stream, AsterixData &result);
    void parse_030(QDataStream &stream, AsterixData &result);
    void parse_040(QDataStream &stream, AsterixData &result);
    void parse_042(QDataStream &stream, AsterixData &result);
    void parse_050(QDataStream &stream, AsterixData &result);
    void parse_060(QDataStream &stream, AsterixData &result);
    void parse_070(QDataStream &stream, AsterixData &result);
    void parse_080(QDataStream &stream, AsterixData &result);
    void parse_090(QDataStream &stream, AsterixData &result);
    void parse_100(QDataStream &stream, AsterixData &result);
    void parse_120(QDataStream &stream, AsterixData &result);
    void parse_130(QDataStream &stream, AsterixData &result);
    void parse_131(QDataStream &stream, AsterixData &result);
    void parse_141(QDataStream &stream, AsterixData &result);
    void parse_150(QDataStream &stream, AsterixData &result);
    void parse_161(QDataStream &stream, AsterixData &result);
    void parse_170(QDataStream &stream, AsterixData &result);
    void parse_200(QDataStream &stream, AsterixData &result);
    void parse_210(QDataStream &stream, AsterixData &result);

    void parseAsTrack(const PresentData &mainMask, QDataStream &stream, AsterixData &result);
    void parseAsPlot(const PresentData &mainMask, QDataStream &stream, AsterixData &result);

    bool generate_010(AsterixData &asterix, QDataStream &stream);
    bool generate_020(AsterixData &asterix, QDataStream &stream);
    bool generate_030(AsterixData &asterix, QDataStream &stream);
    bool generate_040(AsterixData &asterix, QDataStream &stream);
    bool generate_042(AsterixData &asterix, QDataStream &stream);
    bool generate_050(AsterixData &asterix, QDataStream &stream);
    bool generate_060(AsterixData &asterix, QDataStream &stream);
    bool generate_070(AsterixData &asterix, QDataStream &stream);
    bool generate_080(AsterixData &asterix, QDataStream &stream);
    bool generate_090(AsterixData &asterix, QDataStream &stream);
    bool generate_100(AsterixData &asterix, QDataStream &stream);
    bool generate_120(AsterixData &asterix, QDataStream &stream);
    bool generate_130(AsterixData &asterix, QDataStream &stream);
    bool generate_131(AsterixData &asterix, QDataStream &stream);
    bool generate_141(AsterixData &asterix, QDataStream &stream);
    bool generate_150(AsterixData &asterix, QDataStream &stream);
    bool generate_161(AsterixData &asterix, QDataStream &stream);
    bool generate_170(AsterixData &asterix, QDataStream &stream);
    bool generate_200(AsterixData &asterix, QDataStream &stream);
    bool generate_210(AsterixData &asterix, QDataStream &stream);

    void generateAsTrack(PresentData &mainMask, AsterixData &asterix, QDataStream &stream);
    void generateAsPlot(PresentData &mainMask, AsterixData &asterix, QDataStream &stream);

public:
    Asterix001Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Data Item I001/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC; // System Area Code
    static const QString DataSourceIdentifier_SIC; // System Identification Code

    // Data Item I001/020, Target Report Descriptor
    static const QString TargetReportDescriptor_TYP;
    static const QString TargetReportDescriptor_SIM;
    static const QString TargetReportDescriptor_SSRPSR;
    static const QString TargetReportDescriptor_ANT;
    static const QString TargetReportDescriptor_SPI;
    static const QString TargetReportDescriptor_RAB;
    static const QString TargetReportDescriptor_FX; // QList<Asterix001::I001_020_FX>

    struct I001_020_FX
    {
        I001_020_FX() : TST(0), DS1DS2(0), ME(0), MI(0) {}
        quint8 TST : 1;
        quint8 DS1DS2 : 2;
        quint8 ME : 1;
        quint8 MI : 1;
    };

    // Data Item I001/030, Warning/Error Conditions
    static const QString WarningErrorConditions; // QVariantList

    // Data Item I001/040, Measured Position in Polar Coordinates
    static const QString MeasuredPositionInPolarCoordinates_RHO;
    static const QString MeasuredPositionInPolarCoordinates_THETA;

    // Data Item I001/042, Calculated Position in Cartesian Coordinates
    static const QString CalculatedPositionInCartesianCoordinates_X;
    static const QString CalculatedPositionInCartesianCoordinates_Y;

    // Data Item I001/050, Mode-2 Code in Octal Representation
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

    // Data Item I001/060, Mode-2 Code Confidence Indicator
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

    // Data Item I001/070, Mode-3/A Code in Octal Representation
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

    // Data Item I001/080, Mode-3/A Code Confidence Indicator
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

    // Data Item I001/090, Mode-C Code in Binary Representation
    static const QString ModeCCodeInBinaryRepresentation_V;
    static const QString ModeCCodeInBinaryRepresentation_G;
    static const QString ModeCCodeInBinaryRepresentation_HEIGHT;

    // Data Item I001/100, Mode-C Code and Code Confidence Indicator
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

    // Data Item I001/120, Measured Radial Doppler Speed
    static const QString MeasuredRadialDopplerSpeed;

    // Data Item I001/130, Radar Plot Characteristics
    static const QString RadarPlotCharacteristics; // QVariantList

    // Data Item I001/131, Received Power
    static const QString ReceivedPower;

    // Data Item I001/141, Truncated Time of Day
    static const QString TruncatedTimeOfDay;

    // Data Item I001/150, Presence of X-Pulse
    static const QString PresenceOfXPulse_XA;
    static const QString PresenceOfXPulse_XC;
    static const QString PresenceOfXPulse_X2;

    // Data Item I001/161, Track Plot Number
    static const QString TrackPlotNumber;

    // Data Item I001/170, Track Status
    static const QString TrackStatus_CON;
    static const QString TrackStatus_RAD;
    static const QString TrackStatus_MAN;
    static const QString TrackStatus_DOU;
    static const QString TrackStatus_RDPC;
    static const QString TrackStatus_GHO;
    static const QString TrackStatus_TRE;

    // Data Item I001/200, Calculated Track Velocity in Polar Coordinates
    static const QString CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED;
    static const QString CalculatedTrackVelocityInPolarCoordinates_HEADING;

    // Data Item I001/210, Track Quality
    static const QString TrackQuality; // QVariantList
};

Q_DECLARE_METATYPE(QList<Asterix001Proto::I001_020_FX>)

#endif // ASTERIX001_PROTO_H
