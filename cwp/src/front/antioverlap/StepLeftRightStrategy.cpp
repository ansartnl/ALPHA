#include "StepLeftRightStrategy.h"

#include <QLineF>
#include <QPainterPath>

#include "FormularDefs.h"

namespace
{

const int kAnglesCount = 17;
const qreal kAngleStep = 10.f;

} // namespace



bool StepLeftRightStrategy::separate(quint16 id, const QRectF &reposition_rect, FormularInfos &labels, qreal &angle, int &length)
{
    FormularInfos::iterator label = labels.find(id);
    if (label == labels.end()/* || !reposition_rect.contains(label.value().rect)*/)
        return false;

    QPainterPath path(createPath(label.value().rect, label.value().pos));
    if (!rect_has_intersection(id, path, labels))
        return false;

    QRectF new_rect(label.value().rect);
    QLineF line(label.value().pos, new_rect.topLeft());
    const qreal line_angle = line.angle();

    int i = 1;
    while (i < kAnglesCount) {
        line.setAngle(line_angle - kAngleStep * i);
        new_rect.moveTo(line.p2());
        path = createPath(new_rect, label.value().pos);
        if (!rect_has_intersection(id, path, labels))
            break;

        line.setAngle(line_angle + kAngleStep * i);
        new_rect.moveTo(line.p2());
        path = createPath(new_rect, label.value().pos);
        if (!rect_has_intersection(id, path, labels))
            break;

        ++i;
    }

    if (i < kAnglesCount) {
        angle = line.angle();
        length = line.length();
        FormularInfo formular_info(label.value().pos, new_rect, false);
        labels[id] = formular_info;
    }

    return (i < kAnglesCount);
}
