#ifndef OLDI_MESSAGE_LAM_H
#define OLDI_MESSAGE_LAM_H

#include "OLDIMessage.h"

struct OLDIMesDataLAM
{
    OLDIMesDataLAM()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sAnwTo;
    QString sAnwFrom;
    quint16 iAnwMesNum;
};

//! OLDI message
class OLDIMesLAM : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesLAM(const OLDIMesDataLAM& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesLAM(const OLDIMesData& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesLAM(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesLAM();

    OLDIMesDataLAM GetDataLAM() const;

    //@{
    //! Inherited from OLDIMessage
    virtual bool isAnswer(const OLDIMessage* const pMesLAM);
    virtual bool isExecute();
    //@}

private:
    Q_DISABLE_COPY(OLDIMesLAM)
};

#endif // OLDI_MESSAGE_LAM_H
