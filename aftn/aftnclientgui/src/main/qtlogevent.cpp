#include "qtlogevent.h"

const QEvent::Type QtLogEvent::logEventType = static_cast<QEvent::Type>(QEvent::registerEventType());


QtLogEvent::QtLogEvent(Level level, const QString &message)
  :QEvent(logEventType)
  , mLevel(level)
  , mMessage(message)
{
}

QtLogEvent::~QtLogEvent()
{
}
