#ifndef ASTERIX002_PROTO_H
#define ASTERIX002_PROTO_H

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

class ASTERIX_EXPORT Asterix002Proto : public Asterix
{
protected:
    virtual void parse_000(QDataStream &stream, AsterixData &result);
    virtual void parse_010(QDataStream &stream, AsterixData &result);
    virtual void parse_020(QDataStream &stream, AsterixData &result);
    virtual void parse_030(QDataStream &stream, AsterixData &result);
    virtual void parse_041(QDataStream &stream, AsterixData &result);
    virtual void parse_050(QDataStream &stream, AsterixData &result);
    virtual void parse_060(QDataStream &stream, AsterixData &result);
    virtual void parse_070(QDataStream &stream, AsterixData &result);
    virtual void parse_080(QDataStream &stream, AsterixData &result);
    virtual void parse_090(QDataStream &stream, AsterixData &result);
    virtual void parse_100(QDataStream &stream, AsterixData &result);

    virtual bool generate_000(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_010(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_020(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_030(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_041(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_050(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_060(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_070(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_080(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_090(AsterixData &asterix, QDataStream &stream);
    virtual bool generate_100(AsterixData &asterix, QDataStream &stream);

public:
    Asterix002Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    enum EMessageType
    {
        NorthMarker                     = 1,
        SectorCrossing                  = 2,
        SouthMarker                     = 3,
        ActivationBlindZoneFiltering    = 8,
        StopBlindZoneFiltering          = 9
    };

    // Data Item I001/000, Message Type
    static const QString MessageType;

    // Data Item I001/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC; // System Area Code
    static const QString DataSourceIdentifier_SIC; // System Identification Code

    // Data Item I002/020, Sector Number
    static const QString SectorNumber;     // degree

    // Data Item I002/030, Time of Day
    static const QString TimeOfDay;     // QTime

    // Data Item I002/041, Antenna Rotation Speed
    static const QString AntennaRotationSpeed;  // doubled time in seconds

    // Data Item I002/050, Station Configuration Status
    static const QString StationConfigurationStatus;    // QList<quint8>

    // Data Item I002/060, Station Processing Mode
    static const QString StationProcessingMode;    // QList<quint8>

    // Data Item I002/070, Plot Count Values
    static const QString PlotCountValues;    // QList<I002_070>

    struct I002_070
    {
        I002_070() : A(0), IDENT(0), COUNTER(0) {}
        quint8 A : 1;
        quint8 IDENT : 5;
        quint16 COUNTER : 10;
    };

    // Data Item I002/080, Warning/Error Conditions
    static const QString WarningErrorConditions;    // QList<quint8>

    // Data Item I002/090, Collimation Error
    static const QString Collimation_ErrorRangeError;        // Nautical Miles
    static const QString Collimation_ErrorAzimuthError;      // degree

    // Data Item I002/100, Dynamic Window - Type 1
    static const QString DynamicWindow_RhoStart;
    static const QString DynamicWindow_RhoEnd;
    static const QString DynamicWindow_ThetaStart;
    static const QString DynamicWindow_ThetaEnd;
};

Q_DECLARE_METATYPE(QList<Asterix002Proto::I002_070>)

#endif // ASTERIX002_PROTO_H
