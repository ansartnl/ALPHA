#ifndef KRAMS_4_METAR_SPECI_H
#define KRAMS_4_METAR_SPECI_H

#include "krams.h"
#include "MetarSpeci.h"

#include <QtCore/QPair>
#include <QtCore/QStringList>
#include <QtCore/QVariant>


//! Обертка для парсера телеграм METAR или SPECI. Данные получены из дампа Душанбе.
class KRAMS_EXPORT Krams4MetarSpeci : public Krams
{
    Q_DECLARE_TR_FUNCTIONS(Krams4MetarSpeci)

public:
    typedef QSharedPointer<MetarSpeci>  TMetarSpeci;

    Krams4MetarSpeci();

    static Type classType();
    static QString classVersion();

    virtual Type type() const;
    virtual QString version() const;

    virtual QList<TKramsError> parse(const QByteArray &input);
    virtual QList<TKramsError> generate(QByteArray &result) const;

    TMetarSpeci getTelegram() const { return mTelegram; }

private:
    /** Parsed telegram */
    TMetarSpeci mTelegram;
};

#endif // KRAMS_4_METAR_SPECI_H
