#ifndef RESTRICTIONLISTWIDGETITEM_H
#define RESTRICTIONLISTWIDGETITEM_H

#include <QListWidgetItem>

#include "Restriction.h"

class SeparatorItem : public QListWidgetItem
{
public:
    bool operator<(const QListWidgetItem &other) const
    {
        return !other.data(Qt::UserRole + 1).toBool();
    }
};

class RestrListWidgetItem : public QListWidgetItem
{
public:

    explicit RestrListWidgetItem(const QSharedPointer<CRestrictionUser> &restriction, bool is_edit = false);

    QSharedPointer<CRestrictionUser> restriction();
    QSharedPointer<CRestrictionUser> restrictionPointer();

    void setRestriction(const QSharedPointer<CRestrictionUser> &restriction);

    bool favorites() const;
    void setFavorites(bool favorites);

    bool operator<(const QListWidgetItem &other) const;

    static Qt::CheckState actual(const CRestrictionUser *restriction);
    static void setActual(CRestrictionUser *restriction, int state);

    static QString NameText(const CRestrictionUser *restriction);
    static void SetNameText(CRestrictionUser *restriction, const QString &name);


    static int TypeRestrictionID(const CRestrictionUser *restriction);
    static QString TypeText(const CRestrictionUser *restriction);
    static QString IDTypeToText(unsigned idType);

    static QString HeightText(const CRestrictionUser *restriction);


    static QString TimeText(const CRestrictionUser *restriction);

    static QDateTime StartTime(const CRestrictionUser *restriction);
    static void SetStartTime(CRestrictionUser *restriction, QDateTime time);

    static QDateTime EndTime(const CRestrictionUser *restriction);
    static void SetEndTime(CRestrictionUser *restriction, QDateTime time);

    static QTime GetTime(const uint &dateTime);


    static QString heightRangeLo(const CRestrictionUser *restriction);
    static void setHeightRangeLo(CRestrictionUser *restriction, const double value);

    static QString heightRangeHi(const CRestrictionUser *restriction);
    static void setHeightRangeHi(CRestrictionUser *restriction, const double value);

    static double Radius(const CRestrictionUser *restriction);
    static void SetRadius(CRestrictionUser *restriction, double radius);

    static double SmallAxis(const CRestrictionUser *restriction);
    static void SetSmallAxis(CRestrictionUser *restriction, double value);

    static double BigAxis(const CRestrictionUser *restriction);
    static void SetBigAxis(CRestrictionUser *restriction, double value);

    static double Angle(const CRestrictionUser *restriction);
    static void SetAngle(CRestrictionUser *restriction, double value);



    static QTime defaultStartTime();
    static QTime defaultEndTime();
    static QDate defaultStartDate();
    static QDate defaultEndDate();

private:
    QSharedPointer<CRestrictionUser> restriction_;
};

#endif // RESTRICTIONLISTWIDGETITEM_H
