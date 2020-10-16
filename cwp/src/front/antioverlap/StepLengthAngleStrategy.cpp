#include "StepLengthAngleStrategy.h"

namespace
{

const int kAnglesCount = 17;
const qreal kAngleStep = 10.f;

} // namespace


StepLengthAngleStrategy::StepLengthAngleStrategy(int minLength, int maxLength, int preferredLength, int step)
{
    mAvailableLength.append(preferredLength);

    int length;
    int i = 1;
    forever {
        bool done1 = false;
        bool done2 = false;

        length = preferredLength - step * i;
        if (length < minLength) {
            done1 = true;
        }
        else {
            mAvailableLength.append(length);
        }

        length = preferredLength + step * i;
        if (length > maxLength) {
            done2 = true;
        }
        else {
            mAvailableLength.append(length);
        }

        if (done1 && done2)
            break;

        ++i;
    }
}

bool StepLengthAngleStrategy::separate(quint16 id, const QRectF &reposition_rect, FormularInfos &labels, qreal &angle, int &length)
{
    FormularInfos::iterator label = labels.find(id);
    if (label == labels.end() || label.value().fixed/* || !reposition_rect.contains(label.value().rect)*/)
        return false;

    QPainterPath path(createPath(label.value().rect, label.value().pos));
    if (!rect_has_intersection(id, path, labels))
        return false;

    QRectF new_rect(label.value().rect);
    QPointF new_rect_center(new_rect.width() / 2, new_rect.height() / 2);

    QLineF line(label.value().pos, new_rect.center());
    const qreal line_angle = line.angle();

    foreach (int new_length, mAvailableLength) {
        line.setLength(new_length);

        int i = 1;
        while (i < kAnglesCount) {
            line.setAngle(line_angle - kAngleStep * i);
            new_rect.moveTo(line.p2() - new_rect_center);
            path = createPath(new_rect, label.value().pos);
            if (!rect_has_intersection(id, path, labels))
                break;

            line.setAngle(line_angle + kAngleStep * i);
            new_rect.moveTo(line.p2() - new_rect_center);
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

            return true;
        }
    }

    return false;
}
