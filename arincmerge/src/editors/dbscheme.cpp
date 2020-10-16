#include "dbscheme.h"

namespace dbscheme
{

const QString Airport::tableName("airport");
const QString Airport::id("id");
const QString Airport::icao("icao");
const QString Airport::name("name");
const QString Airport::lat("latitude");
const QString Airport::lon("longitude");
const QString Airport::alt("altitude");
const QString Airport::declination("declination");

const QString Zone::tableName("zone");
const QString Zone::id("id");
const QString Zone::name("name");
const QString Zone::type("type");
const QString Zone::idname("id_name");
const QString Zone::parentid("parent_id");
const QString Zone::lowlimit("low_limit");
const QString Zone::uplimit("up_limit");
const QString Zone::shortname("short_name");
const QString Zone::lablename("label_name");
const QString Zone::frequency("frequency");

const QString ZonePoint::tableName("zone_point");
const QString ZonePoint::id("id");
const QString ZonePoint::zoneId("zone_id");
const QString ZonePoint::pointId("point_id");
const QString ZonePoint::isFirBorder("is_fir_border");
const QString ZonePoint::isSectorBorder("is_sector_border");

const QString ZoneAirport::tableName("zone_airport");
const QString ZoneAirport::id("id");
const QString ZoneAirport::zoneid("zone_id");
const QString ZoneAirport::airportid("airport_id");

const QString ZoneBoundaryPointType::tableName("boundary_point_type");
const QString ZoneBoundaryPointType::id("id");
const QString ZoneBoundaryPointType::name("name");

const QString ZoneBoundary::tableName("zone_boundary");
const QString ZoneBoundary::id("id");
const QString ZoneBoundary::type("type");
const QString ZoneBoundary::sn("sn");
const QString ZoneBoundary::lat("latitude");
const QString ZoneBoundary::lon("longitude");
const QString ZoneBoundary::zoneid("zone_id");
const QString ZoneBoundary::arclat("arc_latitude");
const QString ZoneBoundary::arclon("arc_longitude");
const QString ZoneBoundary::arcdist("arc_distance");

const QString Holding::tableName("holding");
const QString Holding::id("id");
const QString Holding::pointid("point_id");
const QString Holding::type("type");
const QString Holding::incouese("in_course");
const QString Holding::turn("turn_direction");
const QString Holding::leglength("leg_length");
const QString Holding::legtime("leg_time");
const QString Holding::speed("speed");
const QString Holding::zoneid("zone_id");
const QString Holding::h_min("h_min");
const QString Holding::h_max("h_max");

const QString HoldingType::tableName("holding_type");
const QString HoldingType::id("id");
const QString HoldingType::name("name");

const QString HoldingTurn::tableName("holding_turn");
const QString HoldingTurn::id("id");
const QString HoldingTurn::name("name");

const QString Point::tableName("point");
const QString Point::id("id");
const QString Point::type("type");
const QString Point::name("name");
const QString Point::lat("latitude");
const QString Point::lon("longitude");
const QString Point::icao("icao");
const QString Point::airport("airport");
const QString Point::category_type("category_type");

const QString RestrictionPoint::tableName("restriction_point");
const QString RestrictionPoint::id("id");
const QString RestrictionPoint::sn("sn");
const QString RestrictionPoint::type("type");
const QString RestrictionPoint::lat("latitude");
const QString RestrictionPoint::lon("longitude");
const QString RestrictionPoint::arclat("arc_latitude");
const QString RestrictionPoint::arclon("arc_longitude");
const QString RestrictionPoint::arcdist("arc_distance");
const QString RestrictionPoint::restid("restriction_id");

const QString RestrictionPointUser::tableName("restriction_point_user");

const QString Restriction::tableName("restriction");
const QString Restriction::id("id");
const QString Restriction::name("name");
const QString Restriction::type("type");
const QString Restriction::designation("designation");
const QString Restriction::zoneid("zone_id");
const QString Restriction::multicode("multi_code");
const QString Restriction::hmin("h_min");
const QString Restriction::hmax("h_max");
const QString Restriction::schedulemode("schedule_mode");

const QString RestrictionUser::tableName("restriction_user");
const QString RestrictionUser::id("id");
const QString RestrictionUser::name("name");
const QString RestrictionUser::zoneid("zone_id");
const QString RestrictionUser::hmin("h_min");
const QString RestrictionUser::hmax("h_max");
const QString RestrictionUser::timebegin("time_start");
const QString RestrictionUser::timeend("time_end");
const QString RestrictionUser::type("type");
const QString RestrictionUser::guid("guid");

const QString RestrictionType::tableName("restriction_type");
const QString RestrictionType::id("id");
const QString RestrictionType::name("name");

const QString Route::tableName("route");
const QString Route::id("id");
const QString Route::name("name");
const QString Route::zoneid("zone_id");

const QString RoutePoint::tableName("route_point");
const QString RoutePoint::id("id");
const QString RoutePoint::sn("sn");
const QString RoutePoint::pointid("point_id");
const QString RoutePoint::routeid("route_id");
const QString RoutePoint::endflag("end_flag");

const QString Runway::tableName("runway");
const QString Runway::id("id");
const QString Runway::name("name");
const QString Runway::lat("latitude");
const QString Runway::lon("longitude");
const QString Runway::length("length");
const QString Runway::azimuth("azimuth");
const QString Runway::width("width");
const QString Runway::airportid("airport_id");
const QString Runway::widget("widget");

const QString Ssa::tableName("ssa");
const QString Ssa::id("id");
const QString Ssa::runwayid("runway_id");
const QString Ssa::name("name");
const QString Ssa::type("type");
const QString Ssa::transalt("trans_alt");

const QString SsaPoint::tableName("ssa_point");
const QString SsaPoint::id("id");
const QString SsaPoint::ssaid("ssa_id");
const QString SsaPoint::pointid("point_id");
const QString SsaPoint::sn("sn");
const QString SsaPoint::flevel("flevel");
const QString SsaPoint::speed("speed");
const QString SsaPoint::type("type");
const QString SsaPoint::course("course");
const QString SsaPoint::turn("turn");
const QString SsaPoint::alt("altitude1");
const QString SsaPoint::flyover("fly_over");

const QString SsaType::tableName("ssa_type");
const QString SsaType::id("id"); //PK
const QString SsaType::name("name");

const QString SsaPointTurn::tableName("ssa_point_turn");
const QString SsaPointTurn::id("id"); //PK
const QString SsaPointTurn::name("name");

const QString SsaPointType::tableName("ssa_point_type");
const QString SsaPointType::id("id"); //PK
const QString SsaPointType::name("name");

const QString ZoneType::tableName("zone_type");
const QString ZoneType::id("id"); //PK
const QString ZoneType::name("name");

const QString PointType::tableName("point_type");
const QString PointType::id("id"); //PK
const QString PointType::name("name");

const QString RestrictionPointType::tableName("restriction_point_type");
const QString RestrictionPointType::id("id"); //PK
const QString RestrictionPointType::name("name");

const QString RestrictionUserType::tableName("restriction_user_type");
const QString RestrictionUserType::id("id"); //PK
const QString RestrictionUserType::name("name");

const QString VwZonePoint::tableName("vw_zone_point");
const QString VwZonePoint::id("id");
const QString VwZonePoint::pointId("point_id");
const QString VwZonePoint::zoneId("zone_id");
const QString VwZonePoint::type("type");
const QString VwZonePoint::name("name");
const QString VwZonePoint::lat("latitude");
const QString VwZonePoint::lon("longitude");
const QString VwZonePoint::icao("icao");
const QString VwZonePoint::airport("airport");
const QString VwZonePoint::isFirBorder("is_fir_border");
const QString VwZonePoint::isSectorBorder("is_sector_border");

}
