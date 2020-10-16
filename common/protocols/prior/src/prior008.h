#ifndef PRIOR008_H
#define PRIOR008_H

#include "prior.h"

#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>


class PriorData;

//! Сообщение "Приор" категории 8 - «Метео».
class PRIOR_EXPORT Prior008 : public Prior
{
protected:
    void parse_MessageType(QDataStream &stream, PriorData &map);
    void parse_DataSourceIdentifier(QDataStream &stream, PriorData &map);
    void parse_WeatherVector(QDataStream &stream, PriorData &map);
    void parse_TimeOfDay(QDataStream &stream, PriorData &map);

    bool generate_MessageType(PriorData &map, QDataStream &stream);
    bool generate_DataSourceIdentifier(PriorData &map, QDataStream &stream);
    bool generate_WeatherVector(PriorData &map, QDataStream &stream);
    bool generate_TimeOfDay(PriorData &map, QDataStream &stream);

public:
    enum EMessageType
    {
        MTWeatherVector = 0x82
    };

    //! Structure: Weather Vector in SPF Notation
    struct SWeatherVector
    {
        qreal   x1;         // meters
        qreal   y1;         // meters
        qreal   x2;         // meters
        qreal   y2;         // meters
    };

public:
    Prior008();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, PriorData &asterix);
    virtual QByteArray generate(PriorData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Message Type
    static const QString MessageType;                   // EMessageType

    // Data Source Identifier
    static const QString DataSourceIdentifier_SAC;      // System Area Code
    static const QString DataSourceIdentifier_SIC;      // System Identification Code

    // Weather Vector in SPF Notation
    static const QString WeatherVector;                 // SWeatherVector

    // Time of Day
    static const QString TimeOfDay;                     // QTime
};

Q_DECLARE_METATYPE(Prior008::SWeatherVector)

#endif // PRIOR008_H
