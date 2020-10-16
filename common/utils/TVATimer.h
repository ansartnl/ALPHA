#ifndef TVATIMER_H
#define TVATIMER_H

#include <QTimer>
#include "TVAFunction.h"

namespace tva
{
    //! Class extends QTimer
    class CTimer
        : public QTimer
    {
        Q_OBJECT
    public:
        //! Constructor
        CTimer();

        //! Set handler
        /*!
            \param Func handler
        */
        void Connect(const CFunction& Func);
        //! Disable handler
        void Diconnect();

    public slots:
        void OnTimer();
    private:
        CFunction m_Func;
    };
}
#endif // TVATIMER_H
