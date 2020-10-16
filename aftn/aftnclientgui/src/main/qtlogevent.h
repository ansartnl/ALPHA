#ifndef QTLOGEVENT_H
#define QTLOGEVENT_H

#include <QtCore/QEvent>
#include <QtCore/QString>

class QtLogEvent : public QEvent
{
public:
  static const QEvent::Type logEventType;

  enum Level {
    /*! NULL_INT is used for no level has been specified */
    NULL_INT = 0,
    ALL_INT = 32,
    TRACE_INT = 64,
    DEBUG_INT = 96,
    INFO_INT = 128,
    WARN_INT = 150,
    ERROR_INT = 182,
    FATAL_INT = 214,
    OFF_INT = 255
            };

  QtLogEvent(Level level, const QString& message);
  virtual ~QtLogEvent();


  Level level() const {return mLevel;}
  QString message() const {return mMessage;}
private:
  Level mLevel;
  QString mMessage;
};

#endif
