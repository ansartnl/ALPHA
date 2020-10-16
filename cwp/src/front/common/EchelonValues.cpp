#include "EchelonValues.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QVariant>
#include "dbconnectionsmanager.h"

EchelonValues::EchelonValues()
{

}

EchelonValues::~EchelonValues()
{

}

qint32 EchelonValues::GetEchelonMeters(const qint32 echelon) const
{
    quint32 iRet = INT_MIN;
    QMap<qint32, QSharedPointer<echval> >::const_iterator itFind = m_mapEchelon.constFind(echelon);
    if(itFind != m_mapEchelon.constEnd())
    {
        if(*itFind)
        {
            iRet = (*itFind)->meters;
        }
    }
    return iRet;
}

void EchelonValues::init(const QSqlDatabase& db, bool bReload/* = false*/)
{
    if(DBConnectionsManager::instance().getConnectionStatus() == 1)
    //if(db.isOpen())
    {
        if(m_list.size() && !bReload) return;

        m_list.clear();
        m_mapEchelon.clear();
        m_mapFeet.clear();
        m_mapMeters.clear();

        QSqlQuery query(db);
        query.prepare("SELECT echelon, meters, feet, azimuth, vfr_ifr FROM echelons");
        if(query.exec())
        {
            while(query.next())
            {
                QSharedPointer<echval> ech(new echval);
                ech->echelon = query.record().value("echelon").toInt();
                ech->meters = query.record().value("meters").toInt();
                ech->feet = query.record().value("feet").toInt();
                ech->azimuth = query.record().value("azimuth").toInt();
                ech->vfr_ifr = query.record().value("vfr_ifr").toInt();

                m_list.append(ech);
                m_mapEchelon.insert(ech->echelon, ech);
            }
        }
        else
        {
            qDebug() << query.lastError().text();
        }
    }
}
