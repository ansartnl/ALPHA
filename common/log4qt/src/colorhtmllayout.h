#ifndef COLORHTMLLAYOUT_H
#define COLORHTMLLAYOUT_H

#include "layout.h"

namespace Log4Qt
{
    class ColorHtmlLayout: public Layout
    {
    public:
        ColorHtmlLayout(QObject* parent = 0);
        virtual QString format(const LoggingEvent &rEvent);
        virtual QDebug debug(QDebug &rDebug) const;
    };
}

#endif // COLORHTMLLAYOUT_H
