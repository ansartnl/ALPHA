#ifndef PRIOR_H
#define PRIOR_H

#include "priorexport.h"
#include "priorerrors.h"
#include "factory.h"

#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

class PriorData;

class PRIOR_EXPORT Prior
{
public:
    Prior();
    virtual ~Prior();

    virtual int category() const = 0;
    virtual QString edition() const = 0;

    virtual void parse(QDataStream &stream, PriorData &prior) = 0;
    virtual QByteArray generate(PriorData &prior) = 0;

    virtual QStringList fields() const = 0;

    template<class T>
    quint8 calculateRepetition(const QList<T> &val)
    {
        quint8 result = val.size();
        return val.isEmpty() || result != val.size() ? 0 : result;
    }

    static const QString SAC; // System Area Code
    static const QString SIC; // System Identification Code

    void setLength(quint8 length) { mLength = length; }

    //! Returns true if input category is registered and supported.
    /*!
        cat Category to test whether parser is registered for it.
    */
    static bool isRegistered(int cat);

    //! Register available "Prior" parsers.
    static void registre();
    //! Clear all registered parsers.
    static void unRegistre();

protected:
    PriorErrorMandatory *mandatory(const QString &field);

    /** Length of message */
    quint8  mLength;
};

Q_DECLARE_METATYPE(QList<quint8>)

extern PRIOR_EXPORT Factory<Prior, int> priorFactory;

#define PRIOR_SAC     "prior.SAC"
#define PRIOR_SIC     "prior.SIC"

#endif // PRIOR_H
