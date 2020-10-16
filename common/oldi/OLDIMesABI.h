#ifndef OLDI_MESSAGE_ABI_H
#define OLDI_MESSAGE_ABI_H

#include "OLDIMessage.h"

//! OLDI message
class OLDIMesABI : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesABI(const OLDIMesDataABI& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesABI(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesABI();

    OLDIMesDataABI GetDataABI() const;

private:
    Q_DISABLE_COPY(OLDIMesABI)
};

#endif // OLDI_MESSAGE_ABI_H
