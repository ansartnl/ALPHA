#ifndef OLDI_MESSAGE_FPL_H
#define OLDI_MESSAGE_FPL_H

#include "OLDIMessage.h"

//! OLDI message
class OLDIMesFPL : public OLDIMessage
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMesFPL(const OLDIMesDataABI& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMesFPL(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMesFPL();

    OLDIMesDataABI GetDataFPL() const;

private:
    Q_DISABLE_COPY(OLDIMesFPL)
};

#endif // OLDI_MESSAGE_FPL_H
