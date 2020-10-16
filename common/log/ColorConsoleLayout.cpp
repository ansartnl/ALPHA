#include "ColorConsoleLayout.h"

#ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "helpers/datetime.h"
#include "level.h"
#include "loggingevent.h"
#else
#include <Log4Qt/helpers/datetime.h>
#include <Log4Qt/level.h>
#include <Log4Qt/loggingevent.h>
#endif


#include <QtCore/QDebug>
#include <QtCore/QLocale>

namespace Log4Qt {

ColorConsoleLayout::ColorConsoleLayout(const QString &rPattern, QObject *parent) :
    PatternLayout(rPattern, parent) {
}

QString ColorConsoleLayout::format(const LoggingEvent &rEvent) {
    //    QString formattedMessage = DateTime::fromMilliSeconds(rEvent.timeStamp()).toString(QLocale().dateTimeFormat(
    //                                                                                           QLocale::ShortFormat));
    //    formattedMessage.append(" [");
    //    formattedMessage.append(rEvent.threadName());
    //    formattedMessage.append("] ");
    //    formattedMessage.append(rEvent.level().toString());
    //    formattedMessage.append(" ");
    //    formattedMessage.append(rEvent.loggerName());
    //    formattedMessage.append(" ");
    QString formattedMessage;
    switch (rEvent.level().toInt()) {
    case Level::DEBUG_INT:
        formattedMessage = QString("\e[33m%1\e[0m").arg(rEvent.message());
        break;
    case Level::INFO_INT:
        formattedMessage = QString("\e[1;34m%1\e[0m").arg(rEvent.message());
        break;
    case Level::WARN_INT:
        formattedMessage = QString("\e[1;31m%1\e[0m").arg(rEvent.message());
        break;
    case Level::ERROR_INT:
        formattedMessage = QString("\e[1;31m%1\e[0m").arg(rEvent.message());
        break;
    case Level::FATAL_INT:
        formattedMessage = QString("\e[1;31m%1\e[0m").arg(rEvent.message());
        break;
    }
    return PatternLayout::format(LoggingEvent(rEvent.logger()
                                              , rEvent.level()
                                              , formattedMessage
                                              , rEvent.ndc()
                                              , rEvent.properties()
                                              , rEvent.threadName()
                                              , rEvent.timeStamp()));
}

QDebug ColorConsoleLayout::debug(QDebug &rDebug) const {
    rDebug.nospace() << metaObject()->className() << "(" << "name:" << objectName() << ")";
    return rDebug.space();
}

}
