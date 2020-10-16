#include "AntioverlapStrategy.h"

QPainterPath AntioverlapStrategy::createPath(const QRectF & rect, const QPointF & pos)
{
    QPainterPath path;
    path.addRect(rect);
    path.moveTo(rect.center());
    path.lineTo(pos);
    path.addEllipse(pos, 10, 10);
    return path;
}

bool AntioverlapStrategy::rect_has_intersection(quint16 id, const QPainterPath & formular_path, AntioverlapStrategy::FormularInfos &labels)
{
    QHash<quint16, FormularInfo>::const_iterator it = labels.begin();
    while (it != labels.end()) {
        if (it.key() != id) {
            const QPainterPath path(createPath(it.value().rect, it.value().pos));
            if (path.intersects(formular_path))
                return true;
        }

        ++it;
    }
    return false;
}
