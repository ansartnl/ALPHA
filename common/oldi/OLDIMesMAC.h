#ifndef OLDI_MESSAGE_MAC_H
#define OLDI_MESSAGE_MAC_H

#include "OLDIMessage.h"

struct OLDIMesDataMAC
{
    OLDIMesDataMAC()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sACID;
};

//! OLDI message
class OLDIMesMAC : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesMAC(const OLDIMesDataMAC& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesMAC(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesMAC();

    OLDIMesDataMAC GetDataMAC() const;

private:
    Q_DISABLE_COPY(OLDIMesMAC)
};

#endif // OLDI_MESSAGE_MAC_H
