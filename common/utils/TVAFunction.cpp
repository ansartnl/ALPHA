#include "TVAFunction.h"

namespace tva
{
    CFunction::CFunction()
        : m_Func(0), m_pObj(0)
    {
    }

    CFunction::CFunction(TFunc pFunc, void* pObj)
        : m_Func(pFunc), m_pObj(pObj)
    {
    }
    void CFunction::operator()()
    {
        if (m_Func && m_pObj)
            m_Func(m_pObj);
    }
}
