#ifndef MOXASETTINGSCOMMON_H
#define MOXASETTINGSCOMMON_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>

class MoxaSettingsTr : public QObject
{
    Q_OBJECT
};

namespace Field
{

enum Field {
    F_NONE
    , F_BAUD_RATE
};

inline const QString toString(Field f)
{
    switch (f)
    {
    case F_NONE:
        return MoxaSettingsTr::tr("Empty field");
    case F_BAUD_RATE:
        return MoxaSettingsTr::tr("Baud rate");
    }
    return MoxaSettingsTr::tr("Unhandled field");
}

} // namespace Field

const QStringList endLines();

struct SettingInfo
{
    QString path;
    QString save;
    QString leave;
    QString rexp;
    int cap;

    SettingInfo() :
        cap(-1)
    {}
    SettingInfo(const QString &p, const QString &s, const QString &l, const QString &r, int c) :
        path(p), save(s), leave(l), rexp(r), cap(c)
    {}

    inline const QString backPath() const
    {
        if (!save.isEmpty())
            return save;
        return leave;
    }
};

typedef QMap < Field::Field, SettingInfo > SettingInfoMap;

const SettingInfoMap infoMap(bool init = false);

const QChar baudRateIndex(int value);

#endif // MOXASETTINGSCOMMON_H
