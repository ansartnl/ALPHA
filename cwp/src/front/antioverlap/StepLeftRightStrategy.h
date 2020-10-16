#ifndef STEPLEFTRIGHT_STRATEGY_H_
#define STEPLEFTRIGHT_STRATEGY_H_

#include "AntioverlapStrategy.h"


class StepLeftRightStrategy : public AntioverlapStrategy
{
public:

    virtual bool separate(quint16 id, const QRectF &rect, FormularInfos &labels, qreal &angle, int &length);

};


#endif // STEPLEFTRIGHT_STRATEGY_H_
