#ifndef _ECHELON_VALUES_H__
#define _ECHELON_VALUES_H__

#include <QtCore/QMap>
#include <QtCore/QSharedPointer>

class QSqlDatabase;

//! class for echelon meters conversion
class EchelonValues
{
    struct echval
    {
        qint32 echelon;
        qint32 meters;
        qint32 feet;
        qint32 azimuth;
        qint32 vfr_ifr;
    };

public:
    EchelonValues();
    ~EchelonValues();

    void init(const QSqlDatabase& db, bool bReload = false);

    //! Echelon -> meters, INT_MIN if not exist;
    qint32 GetEchelonMeters(const qint32 echelon) const;

private:
    QList<QSharedPointer<echval> >          m_list;
    QMap<qint32, QSharedPointer<echval> >   m_mapEchelon;
    QMap<qint32, QSharedPointer<echval> >   m_mapFeet;
    QMap<qint32, QSharedPointer<echval> >   m_mapMeters;
};

#endif // _ECHELON_VALUES_H__
