#include "SvgItem.h"

QDataStream &operator<<(QDataStream &out, SvgItem &value)
{
    out << value.GetPath();
    out << value.GetSvgItemID();
    out << value.GetAngle();
    out << value.GetCoordStart().GetLatitude().GetAngle();
    out << value.GetCoordStart().GetLongitude().GetAngle();
    out << value.GetCoordFinish().GetLatitude().GetAngle();
    out << value.GetCoordFinish().GetLongitude().GetAngle();
    out << value.GetName();
    out << value.GetCode();
    return out;
}

QDataStream &operator>>(QDataStream &in, SvgItem &value)
{
    quint32 SvgItemID;
    qreal d_angle;
    QString path;
    in >> path;
    value.SetPath(path);
    double coord_start_lat;
    double coord_start_long;
    double coord_finish_lat;
    double coord_finish_long;
    in >> SvgItemID;
    in >> d_angle;
    value.SetSvgItemID(SvgItemID);
    value.SetAngle(d_angle);
    in >> coord_start_lat;
    in >> coord_start_long;
    in >> coord_finish_lat;
    in >> coord_finish_long;
    CAngle angle;
    CCoord coord;

    angle.SetAngle(coord_start_lat);
    coord.SetLatitude(angle);
    angle.SetAngle(coord_start_long);
    coord.SetLongitude(angle);
    value.SetCoordStart(coord);

    angle.SetAngle(coord_finish_lat);
    coord.SetLatitude(angle);
    angle.SetAngle(coord_finish_long);
    coord.SetLongitude(angle);
    value.SetCoordFinish(coord);
    QString name;
    in >> name;
    value.SetName(name);
    QString code;
    in >> code;
    value.SetCode(code);
    return in;
}
