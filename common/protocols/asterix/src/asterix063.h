#ifndef ASTERIX063_PROTO_H
#define ASTERIX063_PROTO_H

#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>

class AsterixData;

class ASTERIX_EXPORT Asterix063Proto : public Asterix
{
protected:
    void parse_010(QDataStream &stream, AsterixData &result);
    void parse_015(QDataStream &stream, AsterixData &result);
    void parse_030(QDataStream &stream, AsterixData &result);
    void parse_050(QDataStream &stream, AsterixData &result);
    void parse_060(QDataStream &stream, AsterixData &result);
    void parse_070(QDataStream &stream, AsterixData &result);
    void parse_080(QDataStream &stream, AsterixData &result);
    void parse_081(QDataStream &stream, AsterixData &result);
    void parse_090(QDataStream &stream, AsterixData &result);
    void parse_091(QDataStream &stream, AsterixData &result);
    void parse_092(QDataStream &stream, AsterixData &result);

    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_015(AsterixData &map, QDataStream &stream);
    bool generate_030(AsterixData &map, QDataStream &stream);
    bool generate_050(AsterixData &map, QDataStream &stream);
    bool generate_060(AsterixData &map, QDataStream &stream);
    bool generate_070(AsterixData &map, QDataStream &stream);
    bool generate_080(AsterixData &map, QDataStream &stream);
    bool generate_081(AsterixData &map, QDataStream &stream);
    bool generate_090(AsterixData &map, QDataStream &stream);
    bool generate_091(AsterixData &map, QDataStream &stream);
    bool generate_092(AsterixData &map, QDataStream &stream);

public:
    Asterix063Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Data Item I063/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC;
    static const QString DataSourceIdentifier_SIC;

    // Data Item I063/015, Service Identification
    static const QString ServiceIdentification;

    // Data Item I063/030, Time of Message
    static const QString TimeOfMessage;

    // Data Item I063/050, Sensor Identifier
    static const QString SensorIdentifier_SAC;
    static const QString SensorIdentifier_SIC;

    // Data Item I063/060, Sensor Configuration and Status
    static const QString SensorConfigurationAndStatus_CON;
    static const QString SensorConfigurationAndStatus_PSR;
    static const QString SensorConfigurationAndStatus_SSR;
    static const QString SensorConfigurationAndStatus_MDS;
    static const QString SensorConfigurationAndStatus_ADS;
    static const QString SensorConfigurationAndStatus_MLT;
    static const QString SensorConfigurationAndStatus_FX;

    struct I063_060_FX
    {
        quint8 OPS : 1;
        quint8 ODP : 1;
        quint8 OXT : 1;
        quint8 MSC : 1;
        quint8 TSV : 1;
        quint8 NPW : 1;
    };

    // Data Item I063/070, Time Stamping Bias
    static const QString TimeStampingBias;

    // Data Item I063/080, SSR / Mode S Range Gain and Bias
    static const QString SsrModeSRangeGainAndBias_SRG;
    static const QString SsrModeSRangeGainAndBias_SRB;

    // Data Item I063/081, SSR / Mode S Azimuth Bias
    static const QString SsrModeSAzimuthBias;

    // Data Item I063/090, PSR Range Gain and Bias
    static const QString PsrRangeGainSndBias_PRG;
    static const QString PsrRangeGainSndBias_PRB;

    // Data Item I063/091, PSR Azimuth Bias
    static const QString PsrAzimuthBias;

    // Data Item I063/092, PSR Elevation Bias
    static const QString PsrElevationBias;

    static const QString SpecialPurposeField;
    static const QString ReservedExpansionField;
};

Q_DECLARE_METATYPE(Asterix063Proto::I063_060_FX)

#endif // ASTERIX063_PROTO_H
