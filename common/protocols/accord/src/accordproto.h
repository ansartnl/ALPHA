#ifndef _ACCORD_PROTO_H__
#define _ACCORD_PROTO_H__

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QByteArray>

class ACCORD_EXPORT AccordProtocol
{
public:
    AccordProtocol();

    //! Parse raw buffer data.
    TAccordCodogram fromArray(const QByteArray& data);
    QByteArray toArray(const TAccordCodogram &codogram) const;

    //! Read codogram category - Codogram*::classCategory().
    int codogramCategory(const QByteArray& data);

    //! Retrieve a constant length of accord message (number of bytes).
    /*! Length of each message for Accord protocol is 3 words (4 bytes each)
    */
    static quint16 lengthMessage() { return 12; }

    const AccordErrors &lastError() const;

private:
    const AccordErrors &setLastResult(const AccordErrors &error) const;
    mutable AccordErrors lastResult;
};

#endif // _ACCORD_PROTO_H__
