#ifndef OLDI_MESSAGE_ACT_H
#define OLDI_MESSAGE_ACT_H

#include "OLDIMessage.h"

//! OLDI message
class OLDIMesACT : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesACT(const OLDIMesDataABI& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesACT(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesACT();

    OLDIMesDataABI GetDataACT() const;

private:
    Q_DISABLE_COPY(OLDIMesACT)
};

#endif // OLDI_MESSAGE_ACT_H
