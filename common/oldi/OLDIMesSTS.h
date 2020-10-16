#ifndef OLDI_MESSAGE_STS_H
#define OLDI_MESSAGE_STS_H

#include "OLDIMessage.h"

struct OLDIMesDataSTS
{
    OLDIMesDataSTS()
        : iMesNum(0), iPriority(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sText;
    quint16 iPriority;
};

//! OLDI message
class OLDIMesSTS : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesSTS(const OLDIMesDataSTS& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesSTS(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesSTS();

    OLDIMesDataSTS GetDataSTS() const;

private:
    Q_DISABLE_COPY(OLDIMesSTS)
};

#endif // OLDI_MESSAGE_STS_H
