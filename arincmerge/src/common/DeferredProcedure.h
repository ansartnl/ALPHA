#ifndef DEFERREDPROCEDURE_H
#define DEFERREDPROCEDURE_H

class DeferredProcedure
{
public:
    inline DeferredProcedure() {}
    inline virtual ~DeferredProcedure() {}
    virtual void call() = 0;
};

#endif // DEFERREDPROCEDURE_H
