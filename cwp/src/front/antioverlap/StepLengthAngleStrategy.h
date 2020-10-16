#ifndef STEPLENGTHANGLESTRATEGY_H
#define STEPLENGTHANGLESTRATEGY_H

#include "AntioverlapStrategy.h"

class StepLengthAngleStrategy : public AntioverlapStrategy
{
public:
    StepLengthAngleStrategy(int minLength, int maxLength, int preferredLength, int step);

    virtual bool separate(quint16 id, const QRectF &rect, FormularInfos &labels, qreal &angle, int &length);

private:
    QList<int> mAvailableLength;
};

#endif // STEPLENGTHANGLESTRATEGY_H
