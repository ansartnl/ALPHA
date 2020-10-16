#ifndef ANTIOVERLAP_STRATEGY_H_
#define ANTIOVERLAP_STRATEGY_H_

#include <QHash>
#include <QSharedPointer>
#include <QtGui/QPainterPath>

#include "FormularDefs.h"

class AntioverlapStrategy
{
public:
    virtual ~AntioverlapStrategy() {}

    typedef QHash<quint16, FormularInfo> FormularInfos;

    virtual bool separate(quint16 id, const QRectF &reposition_rect, FormularInfos &labels, qreal &angle, int &length) = 0;

    static QPainterPath createPath(const QRectF & rect, const QPointF & pos);
    static bool rect_has_intersection(quint16 id, const QPainterPath & formular_path, AntioverlapStrategy::FormularInfos &labels);
};


typedef QSharedPointer<AntioverlapStrategy> AntioverlapStrategyPtr;


#endif // ANTIOVERLAP_STRATEGY_H_
