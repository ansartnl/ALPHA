#ifndef DBSCHEME_H
#define DBSCHEME_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

namespace dbscheme
{

class Airport
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Airport)
public:
    static const QString tableName;
    static const QString id;
    static const QString icao;
    static const QString name;
    static const QString lat;
    static const QString lon;
    static const QString alt;
    static const QString declination;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_icao() { return tr("ICAO"); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_lat() { return tr("Latitude,\ndegree"); }
    inline static QString tr_lon() { return tr("Longitude,\ndegree"); }
    inline static QString tr_alt() { return tr("Altitude,\nmeter"); }
    inline static QString tr_declination() { return tr("Declination"); }
};

class ZoneAirport
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::ZoneAirport)
public:
    static const QString tableName;
    static const QString id;
    static const QString zoneid;
    static const QString airportid;
};

class Zone
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Zone)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString name;
    static const QString type;
    static const QString idname;
    static const QString parentid;
    static const QString lowlimit;
    static const QString uplimit;
    static const QString shortname;
    static const QString lablename;
    static const QString frequency;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_idname() { return tr("ICAO code"); }
    inline static QString tr_parentid() { return tr(""); }
    inline static QString tr_lowlimit() { return tr("From"); }
    inline static QString tr_uplimit() { return tr("To"); }
    inline static QString tr_shortname() { return tr("Short name"); }
    inline static QString tr_lablename() { return tr("Lable"); }
    inline static QString tr_frequency() { return tr("Frequency"); }

};

class ZonePoint
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::ZonePoint)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString zoneId;
    static const QString pointId;
    static const QString isFirBorder;
    static const QString isSectorBorder;

    inline static QString tr_isFirBorder() { return tr("FIR border"); }
    inline static QString tr_isSectorBorder() { return tr("Sector border"); }
};

class ZoneBoundaryPointType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::ZoneBoundaryPointType)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString name; // type of boundary point
};

class ZoneBoundary
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::ZoneBoundary)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString type; // FK ZoneBoundaryPointType::id
    static const QString sn;
    static const QString lat;
    static const QString lon;
    static const QString zoneid;
    static const QString arclat;
    static const QString arclon;
    static const QString arcdist;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_sn() { return tr("SN"); }
    inline static QString tr_lat() { return tr("Latitude,\ndegree"); }
    inline static QString tr_lon() { return tr("Longitude,\ndegree"); }
    inline static QString tr_zoneid() { return tr(""); }
    inline static QString tr_arclat() { return tr("Arc latitude,\ndegree"); }
    inline static QString tr_arclon() { return tr("Arc longitude,\ndegree"); }
    inline static QString tr_arcdist() { return tr("Arc distance,\nmeter"); }
};

class Holding
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Holding)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString pointid;
    static const QString type; //FK HoldingType::id
    static const QString incouese;
    static const QString turn; //FK HoldingTurn::id
    static const QString leglength;
    static const QString legtime;
    static const QString speed;
    static const QString zoneid; //FK Zone::id
    static const QString h_min;
    static const QString h_max;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_pointid() { return tr("Point"); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_incouese() { return tr("Course,\ndeg"); }
    inline static QString tr_turn() { return tr("Direction"); }
    inline static QString tr_leglength() { return tr("Leg length,\nm"); }
    inline static QString tr_legtime() { return tr("Leg time,\ns"); }
    inline static QString tr_speed() { return tr("Speed,\nm/s"); }
    inline static QString tr_zoneid() { return tr("Zone"); }
    inline static QString tr_h_min() { return tr("Lower limit,\nmeter"); }
    inline static QString tr_h_max() { return tr("Upper limit,\nmeter"); }
};

class HoldingType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::HoldingType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class HoldingTurn
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::HoldingTurn)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class Point
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Point)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString type;
    static const QString name;
    static const QString lat;
    static const QString lon;
    static const QString icao;
    static const QString airport;
    static const QString category_type;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_lat() { return tr("Latitude,\ndegree"); }
    inline static QString tr_lon() { return tr("Longitude,\ndegree"); }
    inline static QString tr_icao() { return tr("ICAO"); }
    inline static QString tr_airport() { return tr("Airport"); }
    inline static QString tr_category_type() { return tr("Category type"); }
};

class RestrictionType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::RestrictionType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class RestrictionPoint
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::RestrictionPoint)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString sn;
    static const QString type;
    static const QString lat;
    static const QString lon;
    static const QString arclat;
    static const QString arclon;
    static const QString arcdist;
    static const QString restid;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_sn() { return tr("SN"); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_lat() { return tr("Latitude,\ndegree"); }
    inline static QString tr_lon() { return tr("Longitude,\ndegree"); }
    inline static QString tr_arclat() { return tr("Arc latitude,\ndegree"); }
    inline static QString tr_arclon() { return tr("Arc longitude,\ndegree"); }
    inline static QString tr_arcdist() { return tr("Arc distance,\nmeter"); }
    inline static QString tr_restid() { return tr(""); }
};

class RestrictionPointUser
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::RestrictionPointUser)
public:
    static const QString tableName;
};

class Restriction
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Restriction)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
    static const QString type;
    static const QString designation;
    static const QString zoneid;
    static const QString multicode;
    static const QString hmin;
    static const QString hmax;
    static const QString schedulemode;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_designation() { return tr("Designation"); }
    inline static QString tr_zoneid() { return tr("Zone"); }
    inline static QString tr_multicode() { return tr("Multi code"); }
    inline static QString tr_hmin() { return tr("H MIN"); }
    inline static QString tr_hmax() { return tr("H MAX"); }
    inline static QString tr_schedulemode() { return tr("Shedule mode"); }
};

class RestrictionUser
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::RestrictionUser)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
    static const QString zoneid;
    static const QString type;
    static const QString hmin;
    static const QString hmax;
    static const QString timebegin;
    static const QString timeend;
    static const QString guid;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_zoneid() { return tr("Zone"); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_hmin() { return tr("H MIN"); }
    inline static QString tr_hmax() { return tr("H MAX"); }
    inline static QString tr_timebegin() { return tr("Time Begin"); }
    inline static QString tr_timeend() { return tr("Time End"); }
    inline static QString tr_guid() { return tr("GUID"); }
};

class Route
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Route)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString name;
    static const QString zoneid; // FK Zone::id

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_zoneid() { return tr(""); }
};

class RoutePoint
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::RoutePoint)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString sn;
    static const QString pointid; // FK Point::id
    static const QString routeid; // FK Route::id
    static const QString endflag;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_sn() { return tr("SN"); }
    inline static QString tr_pointid() { return tr("Name"); }
    inline static QString tr_routeid() { return tr(""); }
    inline static QString tr_endflag() { return tr(""); }
};

class Runway
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Runway)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString name;
    static const QString lat;
    static const QString lon;
    static const QString length;
    static const QString azimuth;
    static const QString width;
    static const QString airportid; // Airport::id
    static const QString widget;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_lat() { return tr("Latitude,\ndegree"); }
    inline static QString tr_lon() { return tr("Longitude,\ndegree"); }
    inline static QString tr_length() { return tr("Length,\nmeter"); }
    inline static QString tr_azimuth() { return tr("Azimuth,\ndegree"); }
    inline static QString tr_width() { return tr("Width,\nmeter"); }
    inline static QString tr_airportid() { return tr(""); }
    inline static QString tr_widget() { return tr("Widget"); }
};

class Ssa
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::Ssa)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString runwayid; // FK Runway::id
    static const QString name;
    static const QString type; // FK SsaType::id
    static const QString transalt;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_runwayid() { return tr(""); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_transalt() { return tr("Trans. alt.,\nmeter"); }
};

class SsaPoint
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::SsaPoint)
public:
    static const QString tableName;
    static const QString id; // PK
    static const QString ssaid;
    static const QString pointid; // FK POINT_VIEW::id
    static const QString sn;
    static const QString flevel;
    static const QString speed;
    static const QString type; // FK SSA_POINT_TYPE::id
    static const QString course;
    static const QString turn; // FK SSA_TURN::id
    static const QString alt;
    static const QString flyover;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_ssaid() { return tr(""); }
    inline static QString tr_pointid() { return tr("Point"); }
    inline static QString tr_sn() { return tr("SN"); }
    inline static QString tr_flevel() { return tr("Level,\nmeter"); }
    inline static QString tr_speed() { return tr("Speed,\nm/sec"); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_course() { return tr("Course,\ndegree"); }
    inline static QString tr_turn() { return tr("Turn direct."); }
    inline static QString tr_alt() { return tr("Altitude,\nmeter"); }
    inline static QString tr_flyover() { return tr("Fly over"); }
};

class SsaType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::SsaType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class SsaPointTurn
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::SsaPointTurn)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class SsaPointType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::SsaPointType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class ZoneType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::ZoneType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class PointType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::PointType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class RestrictionPointType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::RestrictionPointType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class RestrictionUserType
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::RestrictionUserType)
public:
    static const QString tableName;
    static const QString id; //PK
    static const QString name;
};

class VwZonePoint
{
    Q_DECLARE_TR_FUNCTIONS(dbscheme::VwZonePoint)
public:
    static const QString tableName;
    static const QString id;
    static const QString pointId;
    static const QString zoneId;
    static const QString type;
    static const QString name;
    static const QString lat;
    static const QString lon;
    static const QString icao;
    static const QString airport;
    static const QString isFirBorder;
    static const QString isSectorBorder;

    inline static QString tr_tableName() { return tr(""); }
    inline static QString tr_id() { return tr(""); }
    inline static QString tr_type() { return tr("Type"); }
    inline static QString tr_name() { return tr("Name"); }
    inline static QString tr_lat() { return tr("Latitude,\ndegree"); }
    inline static QString tr_lon() { return tr("Longitude,\ndegree"); }
    inline static QString tr_icao() { return tr("ICAO"); }
    inline static QString tr_airport() { return tr("Airport"); }
    inline static QString tr_isFirBorder() { return tr("FIR border"); }
    inline static QString tr_isSectorBorder() { return tr("Sector border"); }
};

}

#endif // DBSCHEME_H
