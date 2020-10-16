#ifndef ASTERIX008_PROTO_H
#define ASTERIX008_PROTO_H

#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>


class AsterixData;

//! Generator and parser for Asterix 8 protocol.
/*!
    If Start Of Picture (SOP) message contains scaling factor differ
    from default (0 - zero) then use "scaleFactor" variable to assing it.
*/
class ASTERIX_EXPORT Asterix008Proto : public Asterix
{
protected:
    void parse_000(QDataStream &stream, AsterixData &result);
    void parse_010(QDataStream &stream, AsterixData &result);
    void parse_020(QDataStream &stream, AsterixData &result);
    void parse_034(QDataStream &stream, AsterixData &result);
    void parse_036(QDataStream &stream, AsterixData &result);
    void parse_038(QDataStream &stream, AsterixData &result);
    void parse_040(QDataStream &stream, AsterixData &result);
    void parse_050(QDataStream &stream, AsterixData &result);
    void parse_090(QDataStream &stream, AsterixData &result);
    void parse_100(QDataStream &stream, AsterixData &result);
    void parse_110(QDataStream &stream, AsterixData &result);
    void parse_120(QDataStream &stream, AsterixData &result);

    bool generate_000(AsterixData &map, QDataStream &stream);
    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_020(AsterixData &map, QDataStream &stream);
    bool generate_034(AsterixData &map, QDataStream &stream);
    bool generate_036(AsterixData &map, QDataStream &stream);
    bool generate_038(AsterixData &map, QDataStream &stream);
    bool generate_040(AsterixData &map, QDataStream &stream);
    bool generate_050(AsterixData &map, QDataStream &stream);
    bool generate_090(AsterixData &map, QDataStream &stream);
    bool generate_100(AsterixData &map, QDataStream &stream);
    bool generate_110(AsterixData &map, QDataStream &stream);
    bool generate_120(AsterixData &map, QDataStream &stream);

public:
    enum ORG
    {
        LocalCoordinates,
        SystemCoordinates
    };

    /*!
        Precipitation intensity level, shading orientation of the
        vectors representing the precipitation area and coordinate
        system used.
    */
    struct SVectorQualifier
    {
        SVectorQualifier()
            : isTestVector(false), isErrorCondition(false)
        {}

        ORG     org;
        quint8  intensity;          // 0-7
        float   shadingOrientation; // degree
        bool    isTestVector;
        bool    isErrorCondition;
    };

    //! Structure: Polar Vector in SPF Notation
    struct SPolarVector
    {
        qreal   startRange;     // NM - nautical miles
        qreal   endRange;       // NM - nautical miles
        qreal   azimuth;        // degree
    };
    // Sequence of Polar Vectors in SPF Notation
    typedef QVariantList        TSequencePolarVectorList;

    //! Structure: Cartesian Vector in SPF Notation
    struct SCartesianVector
    {
        qreal   x;          // NM
        qreal   y;          // NM
        qreal   length;     // NM
    };
    // Sequence of Cartesian Vectors in SPF Notation
    typedef QVariantList        TSequenceCartesianVectorList;

    //! Structure: Weather Vector in SPF Notation
    struct SWeatherVector
    {
        qreal   x1;         // NM
        qreal   y1;         // NM
        qreal   x2;         // NM
        qreal   y2;         // NM
    };
    // Sequence of Weather Vectors in SPF Notation
    typedef QVariantList        TSequenceWeatherVectorList;

    //! Structure: Contour Identifier
    struct SContourId
    {
        enum FST_LST
        {
            IntermediateRecord  = 0x00,     //!< Intermediate record of a contour
            LastRecord          = 0x01,     //!< Last record of a contour of at least two records
            FirstRecord         = 0x10,     //!< First record of a contour of at least two records
            FirstOnlyRecord     = 0x11      //!< First and only record, fully defining a contour
        };

        ORG     org;
        quint8  intensity;
        FST_LST fstLst;
        quint8  serialNumber;
    };

    //! Structure: Contour Point in SPF Notation
    struct SPoint
    {
        qreal   x;      // NM
        qreal   y;      // NM
    };
    // Sequence of Contour Points in SPF Notation
    typedef QVariantList        TSequenceContourPointsList;

    //! Structure: Processing Status
    struct SProcessingStatus
    {
        qint8       scaleFactor;
        quint8      reductionStage;
        quint16     parameters;
    };

    // Station Configuration Status
    typedef QVariantList        TStationConfigurationStatusList;

public:
    Asterix008Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    /*!
        The scaling factor, f, shall be a parameter of the Start Of Picture (SOP)
        message.
        Default value is 0.
    */
    static qint8 scaleFactor;

    enum EMessageType
    {
        PolarVector         = 1,
        CartesianVector     = 2,
        ContourPoints       = 3,
        WeatherVector       = 4,
        SOPMessage          = 254,
        EOPMessage          = 255
    };

    // Data Item I001/000, Message Type
    static const QString MessageType;                   // EMessageType

    // Data Item I001/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC; // System Area Code
    static const QString DataSourceIdentifier_SIC; // System Identification Code

    // Data Item I008/020, Vector Qualifier
    static const QString VectorQualifier;           // SVectorQualifier

    // Data Item I008/034, Sequence of Polar Vectors in SPF Notation
    static const QString SequencePolarVectors;

    // Data Item I008/036, Sequence of Cartesian Vectors in SPF Notation
    static const QString SequenceCartesianVectors;

    // Data Item I008/038, Sequence of Weather Vectors in SPF Notation
    static const QString SequenceWeatherVectors;

    // Data Item I008/040, Contour Identifier
    static const QString ContourIdentifier;

    // Data Item I008/050, Sequence of Contour Points in SPF Notation
    static const QString SequenceContourPoints;

    // Data Item I008/090, Time of Day
    static const QString TimeOfDay;                     // QTime

    // Data Item I008/100, Processing Status
    static const QString ProcessingStatus;

    // Data Item I008/110, Station Configuration Status
    static const QString StationConfigurationStatus;

    // Data Item I008/120, Total number of Items Constituting One Weather Picture
    static const QString TotalNumberItems;
};

Q_DECLARE_METATYPE(Asterix008Proto::SVectorQualifier)
Q_DECLARE_METATYPE(Asterix008Proto::SPolarVector)
Q_DECLARE_METATYPE(Asterix008Proto::SCartesianVector)
Q_DECLARE_METATYPE(Asterix008Proto::SWeatherVector)
Q_DECLARE_METATYPE(Asterix008Proto::SContourId)
Q_DECLARE_METATYPE(Asterix008Proto::SPoint)
Q_DECLARE_METATYPE(Asterix008Proto::SProcessingStatus)

#endif // ASTERIX008_PROTO_H
