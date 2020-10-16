#include "TextEditAppender.h"

#include <QDebug>
#include <QPlainTextEdit>

#include "layout.h"
#include "loggingevent.h"



namespace Log4Qt
{
    
    
    /**************************************************************************
     * Declarations
     **************************************************************************/
    
    
    
    /**************************************************************************
     * C helper functions
     **************************************************************************/
    
    
    
    /**************************************************************************
     * Class implementation: WriterAppender
     **************************************************************************/
    
    
    TextEditAppender::TextEditAppender(QPlainTextEdit *textEdit, QObject *pParent) :
        AppenderSkeleton(false, pParent),
        mpTextEdit(textEdit)
    {
    }
    
    TextEditAppender::TextEditAppender(QPlainTextEdit *textEdit, Layout *pLayout, QObject *pParent)
        : AppenderSkeleton(false, pParent),
          mpTextEdit(textEdit)
    {
        setLayout(pLayout);
    }

    void TextEditAppender::append(const LoggingEvent &rEvent)
    {
        // Q_ASSERT_X(, "WriterAppender::append()", "Lock must be held by caller");
        Q_ASSERT_X(layout(), "WriterAppender::append()", "Layout must not be null");
        Q_ASSERT(mpTextEdit);
    
        QString message(layout()->format(rEvent));
    
        QTextCursor cursor = mpTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(message);
    }
    
    bool TextEditAppender::requiresLayout() const
    {
        return true;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug TextEditAppender::debug(QDebug &rDebug) const
    {
        QString layout_name;
        if (layout())
            layout_name = layout()->name();

        rDebug.nospace() << "TextEditAppender("
            << "name:" << name() << " "
            << "filter:" << firstFilter()
            << "isactive:" << isActive()
            << "isclosed:" << isClosed()
            << "layout:" << layout_name
            << "referencecount:" << referenceCount() << " "
            << "threshold:" << threshold().toString()
            << ")";
        return rDebug.space();
    }
#endif // QT_NO_DEBUG_STREAM

} // namespace Log4Qt
