#ifndef ASTERIX253EURO_H
#define ASTERIX253EURO_H

#include "asterix.h"

namespace Eurocontrol {

// MORLI cat.253
class ASTERIX_EXPORT Asterix253 : public Asterix
{
protected:
    void parse_010(QDataStream &stream, AsterixData &map);
    void parse_015(QDataStream &stream, AsterixData &map);
    void parse_030(QDataStream &stream, AsterixData &map);
    void parse_036(QDataStream &stream, AsterixData &map);
    void parse_040(QDataStream &stream, AsterixData &map);
    void parse_101(QDataStream &stream, AsterixData &map);
    void parse_111(QDataStream &stream, AsterixData &map);

public:
    enum Type
    {
        UnknownType,
        ChannelStateType,
        RadioStateType,
        BearingType
    };

    Asterix253();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &/*asterix*/) {return QByteArray();}

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    static const QString DataSourceIdentifier_SAC;
    static const QString DataSourceIdentifier_SIC;
    static const QString ServiceIdentifier;
    static const QString ApplicationIdentifier;
    static const QString State;
    static const QString MessageType;
    static const QString AdapterState_First;
    static const QString AdapterState_Second;
    static const QString Frequency;
    static const QString Bearing;
};

}

Q_DECLARE_METATYPE(Eurocontrol::Asterix253::Type)

#endif // ASTERIX253EURO_H
