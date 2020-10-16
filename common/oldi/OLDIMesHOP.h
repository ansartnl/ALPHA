#ifndef OLDI_MESSAGE_HOP_H
#define OLDI_MESSAGE_HOP_H

#include "OLDIMessage.h"

struct OLDIMesDataHOP
{
    OLDIMesDataHOP()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sACID;
};

//! OLDI message
class OLDIMesHOP : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesHOP(const OLDIMesDataHOP& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesHOP(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesHOP();

    OLDIMesDataHOP GetDataHOP() const;

private:
    Q_DISABLE_COPY(OLDIMesHOP)
};

#endif // OLDI_MESSAGE_HOP_H
