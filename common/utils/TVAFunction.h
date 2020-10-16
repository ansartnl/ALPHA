#ifndef TVAFUNCTION_H
#define TVAFUNCTION_H

namespace tva
{
    //! Wrapper of function void(void*)
    class CFunction
    {
    public:
        typedef void(*TFunc)(void*);

        //! Constructor
        CFunction();
        //! Constructor
        /*!
            \param pFunc pointer to function
            \param pObj pointer to object
        */
        CFunction(TFunc pFunc, void* pObj);
        //! Operator execution
        void operator()();

    private:
        TFunc m_Func;
        void* m_pObj;
    };

    template < class FunctionPtr >
    tva::CFunction::TFunc union_cast( FunctionPtr funcPtr ) {
        union {
            tva::CFunction::TFunc obj;
            FunctionPtr func;
        } var;

        var.func = funcPtr;
        return var.obj;
    }

    //! Transform function pointer to function wrapper
    template<class T>
    CFunction bind(void(T::*pFun)(void), T* pObj)
    {
        tva::CFunction::TFunc pPureFun;
        //pPureFun = (void(*)(void*))(pFun);
        pPureFun = union_cast<void(T::*)(void)>(pFun);
        return tva::CFunction(pPureFun, (void*)pObj);
    }

    template<class T>
    void CNoDeleter(T* Obj)
    {
            ; // Nothing to do
    }
}

#endif // TVAFUNCTION_H
