#ifndef OLDI_MESSAGE_MAS_H
#define OLDI_MESSAGE_MAS_H

#include "OLDIMessage.h"

struct OLDIMesDataMAS
{
    OLDIMesDataMAS()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sACID;
};

//! OLDI message
class OLDIMesMAS : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesMAS(const OLDIMesDataMAS& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesMAS(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesMAS();

    OLDIMesDataMAS GetDataMAS() const;

private:
    Q_DISABLE_COPY(OLDIMesMAS)
};

#endif // OLDI_MESSAGE_MAS_H
