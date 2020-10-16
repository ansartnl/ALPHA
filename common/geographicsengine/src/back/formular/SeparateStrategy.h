#ifndef SEPARATE_STRATEGY_H
#define SEPARATE_STRATEGY_H

#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

#include "FormularDefs.h"

namespace FormularSeparation
{

    //! Abstract class for defining new separation algorithm.
    class Strategy
    {
        friend class ProcessingThread;

    public:
        Strategy()
            : mFormulars(0), mLocker(0)
        {}

        //! Separation operation for one formular against others.
        virtual bool separate(quint16 id, QPointF &pos) = 0;

        //! Returns true if algorithm does not depend on changing scale on the map.
        /*! Some simple algorithms does not have a possibility to disperse and squeeze
            thus additinal actions are required to have display nicer.
        */
        virtual bool isScaleFree() const = 0;

        //! Remove formular from processing.
        void remove(quint16 id)
        {
            SeparatedCont::iterator it = mSeparated.find(id);
            if ( it != mSeparated.end() )
                mSeparated.erase(it);
        }

    private:
        //! Initialization.
        void init(FormularMap &formularCont, QMutex &locker, QRectF &rectRepos)
        { mFormulars = &formularCont; mLocker = &locker; mRepositionRect = &rectRepos; }

    protected:
        typedef QSet<quint16>   SeparatedCont;

        /** Placed formulars */
        SeparatedCont   mSeparated;
        /** Reference to formular paramters */
        FormularMap     *mFormulars;
        /** Sinhronization object for formular map */
        QMutex          *mLocker;
        /** Window rectangle for repositioning */
        QRectF          *mRepositionRect;
    };

    typedef QSharedPointer<Strategy>    TStrategy;

}

#endif // SEPARATE_STRATEGY_H
