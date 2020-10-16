#ifndef HTTPPINGGLOBAL_H
#define HTTPPINGGLOBAL_H

#include <QtCore/QObject>

class HttpPingTr : public QObject
{
    Q_OBJECT
};

const int DEFAULT_MAXIMUM_ERRORS_COUNT = 0;
const int DEFAULT_INTERVAL_SECONDS = 2;

namespace HttpPingGlobal
{

enum Status {
    S_UNKNOWN = 0
    , S_CONNECTED = 1
    , S_DISCONNECTED = 2
};

inline const QString toString(Status s)
{
    switch (s)
    {
    case S_UNKNOWN:
        return HttpPingTr::tr("Unknown");
    case S_CONNECTED:
        return HttpPingTr::tr("Connected");
    case S_DISCONNECTED:
        return HttpPingTr::tr("Disconnected");
    }
    return HttpPingTr::tr("Unhandled");
}

enum RequestType {
    RT_HEAD
    , RT_GET
};

} //namespace HttpPingGlobal

#endif // HTTPPINGGLOBAL_H
