#include "colorhtmllayout.h"
#include "helpers/datetime.h"
#include "loggingevent.h"

#include <QtDebug>


#include <QtCore/QLocale>


namespace Log4Qt
{
static QString escape(const QString& plain)
{
    QString rich;
    rich.reserve(int(plain.length() * 1.1));
    for (int i = 0; i < plain.length(); ++i)
    {
        if (plain.at(i) == QLatin1Char('<'))
            rich += QLatin1String("&lt;");
        else if (plain.at(i) == QLatin1Char('>'))
            rich += QLatin1String("&gt;");
        else if (plain.at(i) == QLatin1Char('&'))
            rich += QLatin1String("&amp;");
        else if (plain.at(i) == QLatin1Char('"'))
            rich += QLatin1String("&quot;");
        else
            rich += plain.at(i);
    }
    return rich;
}


ColorHtmlLayout::ColorHtmlLayout(QObject *parent) :
    Layout(parent)
{
}
QString ColorHtmlLayout::format(const LoggingEvent &rEvent)
{
    QString formattedMessage = DateTime::fromMilliSeconds(rEvent.timeStamp()).toString(QLocale().dateTimeFormat(
                                                                                           QLocale::ShortFormat));
    formattedMessage.append(" [");
    formattedMessage.append(rEvent.threadName());
    formattedMessage.append("] ");
    formattedMessage.append(rEvent.level().toString());
    formattedMessage.append(" ");
    formattedMessage.append(rEvent.loggerName());
    formattedMessage.append(" ");
    switch (rEvent.level().toInt())
    {
    case Level::DEBUG_INT:
        formattedMessage.append("<font color=lightblue>%1</font>");
        break;
    case Level::INFO_INT:
        formattedMessage.append("<font color=green>%1</font>");
        break;
    case Level::WARN_INT:
        formattedMessage.append("<font color=red>%1</font>");
        break;
    case Level::ERROR_INT:
        formattedMessage.append("<font color=red>%1</font>");
        break;
    case Level::FATAL_INT:
        formattedMessage.append("<font color=red>%1</font>");
        break;
    }
    //formattedMessage.append("<br />");
    return formattedMessage.arg(escape(rEvent.message()));
}
QDebug ColorHtmlLayout::debug(QDebug &rDebug) const
{
    rDebug.nospace() << metaObject()->className() << "(" << "name:" << objectName() << ")";
    return rDebug.space();
}
}
