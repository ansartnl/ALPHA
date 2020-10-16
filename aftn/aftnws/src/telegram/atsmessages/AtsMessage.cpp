#include "AtsMessage.h"

AtsMessage::AtsMessage()
{
}

QString AtsMessage::type() const
{
    return mInfo.type;
}

QString removeColonFromTime(const QString &time)
{
    if (time.size() < 5)
        return time;
    return time.mid(0, 2) + time.mid(3, 2);
}
