#include "qxtspansliderrestricted.h"

#include <QKeyEvent>


QxtSpanSliderRestricted::QxtSpanSliderRestricted(Qt::Orientation orientation, QWidget* parent) : 
    QxtSpanSlider(orientation, parent),
    lower_(kInvalidLimit),
    upper_(kInvalidLimit)
{

}

void QxtSpanSliderRestricted::setRestrictedSpan(int lower, int upper)
{
    lower_ = lower;
    upper_ = upper;

    update();
}

void QxtSpanSliderRestricted::keyPressEvent(QKeyEvent* event)
{
    const int lower = QxtSpanSlider::lowerPosition();
    const int upper = QxtSpanSlider::upperPosition();
    const int step = QxtSpanSlider::singleStep();

    switch (event->key()) {
    case Qt::Key_Left:
    break;
    case Qt::Key_Right:
    if (kInvalidLimit == lower_)
        break;
    if ((lower + step) > lower_) {
        QxtSpanSlider::setLowerPosition(lower_);
        return;
    }
    break;
    case Qt::Key_Up:
    break;
    case Qt::Key_Down:
    if (kInvalidLimit == upper_)
        break;
    if ((upper - step) < upper_) {
        QxtSpanSlider::setUpperPosition(upper_);
        return;
    }
    break;
    case Qt::Key_Home:
    break;
    case Qt::Key_End:
    break;
    default:
    break;
    }

    QxtSpanSlider::keyPressEvent(event);
}

void QxtSpanSliderRestricted::mouseMoveEvent(QMouseEvent* event)
{
    QxtSpanSlider::mouseMoveEvent(event);

    update();
}

void QxtSpanSliderRestricted::mouseReleaseEvent(QMouseEvent* event)
{
    QxtSpanSlider::mouseReleaseEvent(event);
    
    update();
}

void QxtSpanSliderRestricted::update()
{
    const int lower_position = QxtSpanSlider::lowerPosition();
    const int upper_position = QxtSpanSlider::upperPosition();

    if ((kInvalidLimit != lower_) && (lower_position > lower_))
        QxtSpanSlider::setLowerPosition(lower_);
    if ((kInvalidLimit != upper_) && (upper_position < upper_))
        QxtSpanSlider::setUpperPosition(upper_);
}