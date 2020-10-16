#ifndef OLDI_MESSAGE_ROF_H
#define OLDI_MESSAGE_ROF_H

#include "OLDIMessage.h"

struct OLDIMesDataROF
{
    OLDIMesDataROF()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sACID;
};

//! OLDI message
class OLDIMesROF : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesROF(const OLDIMesDataROF& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesROF(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesROF();

    OLDIMesDataROF GetDataROF() const;

private:
    Q_DISABLE_COPY(OLDIMesROF)
};

#endif // OLDI_MESSAGE_ROF_H
