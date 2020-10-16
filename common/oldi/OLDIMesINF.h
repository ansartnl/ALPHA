#ifndef OLDI_MESSAGE_INF_H
#define OLDI_MESSAGE_INF_H

#include "OLDIMessage.h"

struct OLDIMesDataINF
{
    OLDIMesDataINF()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sTypeINF;
    QString sACID;
    QString sADEP;
    QString sADES;
    QString sCODE;
    QString sSector;
};

//! OLDI message
class OLDIMesINF : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesINF(const OLDIMesDataINF& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesINF(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesINF();

    OLDIMesDataINF GetDataINF() const;

private:
    Q_DISABLE_COPY(OLDIMesINF)
};

#endif // OLDI_MESSAGE_INF_H
