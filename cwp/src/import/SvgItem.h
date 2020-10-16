#ifndef SVGITEM_H
#define SVGITEM_H
#include <QtCore/QString>
#include "ItemHandler.h"
#include "Coordinate.h"
//! Class describes the point of a svg item
class SvgItem: public ItemHandler
{
public:
    SvgItem(const QString & path) :
      m_SvgItemID(0)
    {
        SetPath(path);
    }
    virtual ~SvgItem()
    {}
    //! identifier from database
    const QString &GetPath() const { return m_path; }
    void SetPath(const QString &V) { m_path = V; }
    const quint32 &GetSvgItemID() const { return m_SvgItemID; }
    void SetSvgItemID(const quint32 &V) { m_SvgItemID = V; }
    const qreal & GetAngle() const {return m_angle;}
    void SetAngle(const qreal &V) { m_angle = V; }
    CCoord & GetCoordStart() {return m_coord_start;}
    void SetCoordStart(const CCoord &V) { m_coord_start = V; }
    CCoord & GetCoordFinish() {return m_coord_finish;}
    void SetCoordFinish(const CCoord &V) { m_coord_finish = V; }
    const QString &GetName() const { return m_name; }
    void SetName(const QString &V) { m_name = V; }
    const QString &GetCode() const { return m_code; }
    void SetCode(const QString &V) { m_code = V; }
private:
    QString m_path;
    quint32 m_SvgItemID;
    qreal m_angle;
    CCoord m_coord_start;
    CCoord m_coord_finish;
    QString m_name;
    QString m_code;
};

QDataStream &operator<<(QDataStream &out, SvgItem &value);
QDataStream &operator>>(QDataStream &in, SvgItem &value);


#endif // SVGITEM_H
