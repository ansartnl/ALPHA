#ifndef OLDI_MESSAGE_COD_H
#define OLDI_MESSAGE_COD_H

#include "OLDIMessage.h"

struct OLDIMesDataCOD
{
    OLDIMesDataCOD()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sACID;
    QString sADEP;
    QString sADES;
    QString sCODE;
};

//! OLDI message
class OLDIMesCOD : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesCOD(const OLDIMesDataCOD& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesCOD(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesCOD();

    OLDIMesDataCOD GetDataCOD() const;

private:
    Q_DISABLE_COPY(OLDIMesCOD)
};

#endif // OLDI_MESSAGE_COD_H
