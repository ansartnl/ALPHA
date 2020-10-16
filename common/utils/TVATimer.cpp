#include "TVATimer.h"

namespace tva
{
    CTimer::CTimer()
    {

    }

    void CTimer::Connect(const CFunction& Func)
    {
        m_Func = Func;
        connect(this, SIGNAL(timeout()), this, SLOT(OnTimer()));
    }

    void CTimer::Diconnect()
    {
        disconnect(this, 0, 0, 0);
        m_Func = CFunction(NULL, NULL);
    }

    void CTimer::OnTimer()
    {
        m_Func();
    }
}
