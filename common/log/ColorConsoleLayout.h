#ifndef COLORCONSOLELAYOUT_H
#define COLORCONSOLELAYOUT_H

#ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "patternlayout.h"
#else
#include <Log4Qt/patternlayout.h>
#endif

namespace Log4Qt {

class ColorConsoleLayout: public PatternLayout {
public:
    ColorConsoleLayout(const QString &rPattern, QObject *parent = 0);
    virtual QString format(const LoggingEvent &rEvent);
    virtual QDebug debug(QDebug &rDebug) const;
};

}

#endif // COLORCONSOLELAYOUT_H
