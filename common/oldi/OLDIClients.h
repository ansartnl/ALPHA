#ifndef OLDI_CLIENTS_H
#define OLDI_CLIENTS_H

#include <QString>
#include <QMap>
#include <QList>

//! OLDI clients
class OLDIClients
{
public:
    typedef QMap<QString, QList<OLDIClients> >  TClientMap;

    //! Constructor
    OLDIClients() : m_iStatus(0) {}

    QString GetClient() const
    { return m_sClient; }
    void SetClient(QString sClient)
    { m_sClient = sClient; }

    QString GetSector() const
    { return m_sSector; }
    void SetSector(QString sSector)
    { m_sSector = sSector; }

    int GetStatus() const
    { return m_iStatus; }
    void SetStatus(int iStatus)
    { m_iStatus = iStatus; }


private:
    QString m_sClient;
    QString m_sSector;
    int     m_iStatus;
};

#endif // OLDI_CLIENTS_H
