#include "restrictionlistwidgetitem.h"

#include <QDateTime>

#include "BoundaryPoint.h"
#include "restrictionwidget.h"

RestrListWidgetItem::RestrListWidgetItem(const QSharedPointer<CRestrictionUser> &restriction, bool is_edit) :
    restriction_(restriction)
{
    setData(Qt::DisplayRole, NameText(restriction_.data()));
    setData(Qt::UserRole, is_edit);

    setFavorites(false);
}

QSharedPointer<CRestrictionUser> RestrListWidgetItem::restriction()
{
    QSharedPointer<CRestrictionUser> restriction(new CRestrictionUser(*restriction_.data()));
    CRestrictionUser::TPointList points = restriction->GetPoints();
    CRestrictionUser::TPointList new_points;
    int key = 10;
    foreach (const CRestrictionUser::TPoint &point, points) {
        new_points.insert(key, CRestrictionUser::TPoint(new CRestrictPoint(*point.data())));
        key += 10;
    }
    restriction->SetPoints(new_points);
    return restriction;
}

QSharedPointer<CRestrictionUser> RestrListWidgetItem::restrictionPointer()
{
    return restriction_;
}

void RestrListWidgetItem::setRestriction(const QSharedPointer<CRestrictionUser> &restriction)
{
    restriction_ = restriction;
    setData(Qt::DisplayRole, NameText(restriction_.data()));
    setData(Qt::UserRole, true);
}

bool RestrListWidgetItem::favorites() const
{
    return data(Qt::UserRole + 1).toBool();
}

void RestrListWidgetItem::setFavorites(bool favorites)
{
    setData(Qt::UserRole + 1, favorites);
}

bool RestrListWidgetItem::operator<(const QListWidgetItem &other) const
{
    QVariant value = other.data(Qt::UserRole + 1);
    if (value.isValid()) {
        if (favorites() && !value.toBool())
            return true;
        else if (!favorites() && value.toBool())
            return false;
        else
            return QListWidgetItem::operator<(other);
    }
    else {  // separator
        return favorites();
    }

    return false;
}

Qt::CheckState RestrListWidgetItem::actual(const CRestrictionUser *restriction)
{
    return restriction->actual();
}

void RestrListWidgetItem::setActual(CRestrictionUser *restriction, int state)
{
    restriction->setActual(static_cast<Qt::CheckState>(state));
}

QString RestrListWidgetItem::NameText(const CRestrictionUser *restriction)
{
    return restriction->GetName();
}

void RestrListWidgetItem::SetNameText(CRestrictionUser *restriction, const QString &name)
{
    restriction->SetName(name);
}

int RestrListWidgetItem::TypeRestrictionID(const CRestrictionUser *restriction)
{
    QList<CRestrictionUser::TPoint> points = restriction->GetPoints().values();
    if (points.empty())
        return RestrictionWidget::POLYGON;

    CRestrictionUser::TPoint point = points[0];
    switch (point->GetType()) {
    case CBoundaryPoint::enLine:
    return RestrictionWidget::POLYGON;
    case CBoundaryPoint::enCircle:
    return RestrictionWidget::CIRCLE;
    default:
    break;
    }
    return RestrictionWidget::POLYGON;
}

QString RestrListWidgetItem::TypeText(const CRestrictionUser *restriction)
{
    //return IDTypeToText(restriction->type);
    return QString();
}

QString RestrListWidgetItem::IDTypeToText(unsigned id_type)
{
    switch (id_type) {
    case CBoundaryPoint::enLine:
        return QString(QObject::tr("POLYGON"));

    /*case CRestrictionUser::polyline:
        return QString(QObject::tr("POLYLINE"));*/

    case CBoundaryPoint::enCircle:
        return QString(QObject::tr("CIRCLE"));

    /*case CRestrictionUser::ellipse:
        return QString(QObject::tr("ELLIPSE"));

    case CRestrictionUser::route_section:
        return QString(QObject::tr("ROUTE_SECTION"));*/
    }
    return QString();
}

QString RestrListWidgetItem::HeightText(const CRestrictionUser *restriction)
{
    /*double hid;
    double lod;

    switch (restriction->type) {
    case CRestrictionUser::polygon:
    case CRestrictionUser::polyline:
    case CRestrictionUser::circle:
    case CRestrictionUser::ellipse:
    case CRestrictionUser::route_section:
        lod = restriction->height_range.lo;
        hid = restriction->height_range.hi;
        return QString::number((int)lod) + QString(" - ") + QString::number((int)hid);
    }*/
    return QString("N/A");
}

QString RestrListWidgetItem::TimeText(const CRestrictionUser *restriction)
{
    QString loString = restriction->GetTimeBegin().toString("hh:mm");
    QString hiString = restriction->GetTimeEnd().toString("hh:mm");

    return loString + QString(" - ") + hiString;
}

QDateTime RestrListWidgetItem::StartTime(const CRestrictionUser *restriction)
{
    return restriction->GetTimeBegin();
}

void RestrListWidgetItem::SetStartTime(CRestrictionUser *restriction, QDateTime time)
{
    restriction->SetTimeBegin(time);
}

QDateTime RestrListWidgetItem::EndTime(const CRestrictionUser *restriction)
{
    return restriction->GetTimeEnd();
}

void RestrListWidgetItem::SetEndTime(CRestrictionUser *restriction, QDateTime time)
{
    restriction->SetTimeEnd(time);
}

QTime RestrListWidgetItem::GetTime(const uint &dateTime)
{
    QDateTime temp_time;
    temp_time.setTime_t(dateTime);
    return temp_time.time();
}

QString RestrListWidgetItem::heightRangeLo(const CRestrictionUser *restriction)
{
    return QString("%1").arg(restriction->GetHMin() / 0.3048);
}

void RestrListWidgetItem::setHeightRangeLo(CRestrictionUser *restriction, const double value)
{
    restriction->SetHMin(value * 0.3048);
}

QString RestrListWidgetItem::heightRangeHi(const CRestrictionUser *restriction)
{
    return QString("%1").arg(restriction->GetHMax() / 0.3048);
}

void RestrListWidgetItem::setHeightRangeHi(CRestrictionUser *restriction, const double value)
{
    restriction->SetHMax(value * 0.3048);
}

double RestrListWidgetItem::Radius(const CRestrictionUser *restriction)
{
    QList<CRestrictionUser::TPoint> points = restriction->GetPoints().values();
    if (!points.empty())
        return points[0]->GetArcDistance();

    return 0;
}

void RestrListWidgetItem::SetRadius(CRestrictionUser *restriction, double radius)
{
    CRestrictionUser::TPointList &points = restriction->GetPoints();
    CRestrictionUser::TPoint point;
    if (!points.empty())
        point = *points.begin();
    else
        point = CRestrictionUser::TPoint(new CRestrictPoint);

    point->SetType(CBoundaryPoint::enCircle);
    point->SetSN(10);
    point->SetArcDistance(radius);
    points.clear();
    points.insert(10, point);
}

double RestrListWidgetItem::SmallAxis(const CRestrictionUser *restriction)
{
    //return restriction->small_axis;
    return 0;
}

void RestrListWidgetItem::SetSmallAxis(CRestrictionUser *restriction, double value)
{
    //restriction->small_axis = value;
}

double RestrListWidgetItem::BigAxis(const CRestrictionUser *restriction)
{
    //return restriction->big_axis;
    return 0;
}

void RestrListWidgetItem::SetBigAxis(CRestrictionUser *restriction, double value)
{
    //restriction->big_axis = value;
}

double RestrListWidgetItem::Angle(const CRestrictionUser *restriction)
{
    //return restriction->azimuth_big_axis;
    return 0;
}

void RestrListWidgetItem::SetAngle(CRestrictionUser *restriction, double value)
{
    //restriction->azimuth_big_axis = value;
}

QTime RestrListWidgetItem::defaultStartTime()
{
    QTime t(0, 0);
    return t;
}

QTime RestrListWidgetItem::defaultEndTime()
{
    QTime t(23, 59, 59);
    return t;
}

QDate RestrListWidgetItem::defaultStartDate()
{
    return QDate::currentDate();
}

QDate RestrListWidgetItem::defaultEndDate()
{
    return QDate::currentDate();
}



