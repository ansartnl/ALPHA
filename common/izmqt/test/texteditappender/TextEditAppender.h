/******************************************************************************
 *
 * package:     Log4Qt
 * file:        writerappender.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * Copyright 2007 Martin Heinrich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef TEXT_EDIT_APPENDER_H
#define TEXT_EDIT_APPENDER_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "appenderskeleton.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

class QPlainTextEdit;

namespace Log4Qt
{

    class TextEditAppender : public AppenderSkeleton
    {
            Q_OBJECT

    public:
            TextEditAppender(QPlainTextEdit *textEdit, QObject *pParent = 0);
            TextEditAppender(QPlainTextEdit *textEdit, Layout *pLayout, QObject *pParent = 0);

            virtual bool requiresLayout() const;
    protected:
            virtual void append(const LoggingEvent &rEvent);

#ifndef QT_NO_DEBUG_STREAM
            /*!
             * Writes all object member variables to the given debug stream
             * \a rDebug and returns the stream.
             *
             * <tt>
             * %WriterAppender(name:"WA" encoding:"" immediateFlush:true
             *                 isactive:false isclosed:false layout:"TTCC"
             *                 referencecount:1 threshold:"NULL"
             *                 writer:0x0)
             * </tt>
             * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject    )
             */
            virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM

    private:
            QPlainTextEdit  *mpTextEdit;
    };


    /**************************************************************************
     * Operators, Helper
     **************************************************************************/


} // namespace Log4Qt



#endif // TEXT_EDIT_APPENDER_H
