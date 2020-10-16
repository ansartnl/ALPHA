#ifndef OLDI_MESSAGE_URZ_H
#define OLDI_MESSAGE_URZ_H

#include "OLDIMessage.h"

struct OLDIMesDataURZ
{
    OLDIMesDataURZ()
        : iMesNum(0) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;
    QString sDBType;
    QString sFirIdName;
    QString sHexData;
};

//! OLDI message
class OLDIMesURZ : public OLDIMessage
{
    Q_OBJECT
public:
    enum Type
    {
        enRequest = 'A',
        enInsert = 'I',
        enUpdate = 'U',
        enDelete = 'D',
        enMeteo = 'M'
    };

    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesURZ(const OLDIMesDataURZ& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesURZ(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesURZ();

    OLDIMesDataURZ GetDataURZ() const;

private:
    Q_DISABLE_COPY(OLDIMesURZ)
};

#endif // OLDI_MESSAGE_URZ_H
