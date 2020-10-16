#ifndef OLDI_MESSAGE_ERR_H
#define OLDI_MESSAGE_ERR_H

#include "OLDIMessage.h"

struct OLDIMesDataERR
{
    OLDIMesDataERR()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sAnwTo;
    QString sAnwFrom;
    quint16 iAnwMesNum;
    QStringList sListError;
};

//! OLDI message
class OLDIMesERR : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesERR(const OLDIMesDataERR& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesERR(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesERR();

    OLDIMesDataERR GetDataERR() const;

    //@{
    //! Inherited from OLDIMessage
    virtual bool isAnswer(const OLDIMessage* const pMesLAM);
    virtual bool isExecute();
    //@}

private:
    Q_DISABLE_COPY(OLDIMesERR)
};

#endif // OLDI_MESSAGE_ERR_H
