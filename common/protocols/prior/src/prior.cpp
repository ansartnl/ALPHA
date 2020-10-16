#include "prior.h"
#include "shareutility.h"

#include "prior001.h"


Factory<Prior, int> priorFactory;

const QString Prior::SAC = PRIOR_SAC;
const QString Prior::SIC = PRIOR_SIC;

Prior::Prior()
    : mLength(0)
{}

Prior::~Prior()
{}

PriorErrorMandatory *Prior::mandatory(const QString &field)
{
    return new PriorErrorMandatory(category(), field);
}

bool Prior::isRegistered(int cat)
{
    return priorFactory.isRegistered(cat);
}

void Prior::registre()
{
    priorFactory.reg<Prior001>(Prior001::mCategory);
}

void Prior::unRegistre()
{
    priorFactory.unRegister(Prior001::mCategory);
}
