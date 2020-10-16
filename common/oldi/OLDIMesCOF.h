#ifndef OLDI_MESSAGE_COF_H
#define OLDI_MESSAGE_COF_H

#include "OLDIMessage.h"

struct OLDIMesDataCOF
{
    OLDIMesDataCOF()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sACID;
};

//! OLDI message
class OLDIMesCOF : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesCOF(const OLDIMesDataCOF& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesCOF(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesCOF();

    OLDIMesDataCOF GetDataCOF() const;

private:
    Q_DISABLE_COPY(OLDIMesCOF)
};

#endif // OLDI_MESSAGE_COF_H
