#ifndef ASTERIX_H
#define ASTERIX_H

#include "asterixexport.h"
#include "asterixvariety.h"
#include "asterixerrors.h"
#include "factory.h"

#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

class AsterixData;

class ASTERIX_EXPORT Asterix
{
public:
    Asterix();
    virtual ~Asterix();

    virtual int category() const = 0;
    virtual QString edition() const = 0;

    virtual void parse(QDataStream &stream, AsterixData &asterix) = 0;
    virtual QByteArray generate(AsterixData &asterix) = 0;

    virtual QStringList fields() const = 0;

    template<class T>
    quint8 calculateRepetition(const QList<T> &val)
    {
        quint8 result = val.size();
        return val.isEmpty() || result != val.size() ? 0 : result;
    }

    static const QString SAC; // System Area Code
    static const QString SIC; // System Identification Code

    static const QString SpecialPurposeField; // SP
    static const QString ReservedExpansionField; // RE

    void setLength(quint16 length) { mLength = length; }

    //! Returns true if input category is registered and supported.
    /*!
        cat Category to test whether parser is registered for it.
        variety Type of asterix: Eurocontrol, Aerotechnica, Peleng, ...
    */
    static bool isRegistered(int cat, AsterixVariety variety);

    //! Register available asterix parsers.
    static void registre();
    //! Clear all registered parsers.
    static void unRegistre();

protected:
    virtual void parse_sp(QDataStream &stream, AsterixData &result);
    virtual void parse_re(QDataStream &stream, AsterixData &result);

    virtual bool generate_sp(AsterixData &map, QDataStream &stream);
    virtual bool generate_re(AsterixData &map, QDataStream &stream);

    AsterixErrorMandatory *mandatory(const QString &field);

    /** Length of message */
    quint16 mLength;
};

Q_DECLARE_METATYPE(QList<quint8>)

typedef QPair<AsterixVariety, int>      AsterixKey;
extern ASTERIX_EXPORT Factory<Asterix, AsterixKey> asterixFactory;

#define ASTERIX_SAC     "asterix.DataSourceIdentifier.SAC"
#define ASTERIX_SIC     "asterix.DataSourceIdentifier.SIC"

#endif // ASTERIX_H
