#ifndef _KRAMS_PROTO_H__
#define _KRAMS_PROTO_H__

#include "kramsexport.h"
#include "kramserrors.h"
#include "krams.h"

#include <QtCore/QByteArray>

class KRAMS_EXPORT KramsProtocol
{
public:
    //! Parse raw buffer data.
    TKramsMessage fromArray(const QByteArray& data, int *readed = 0);
    //! Create raw buffer data from message data.
    QByteArray toArray(const TKramsMessage &message) const;

    //! Retrieve last errors happened during protocol parsing/generating.
    const QList<TKramsError> &lastErrors() const;

private:
    /** Errors collected during last message parsing/generating */
    mutable QList<TKramsError>  mLastErrors;
};

#endif // _KRAMS_PROTO_H__
