#ifndef QXTSPANSLIDERRESTRICTED_H
#define QXTSPANSLIDERRESTRICTED_H

#include <qxtspanslider.h>

class QxtSpanSliderRestricted : public QxtSpanSlider
{
    Q_OBJECT
    static const int kInvalidLimit = -1;
public:
    explicit QxtSpanSliderRestricted(Qt::Orientation orientation, QWidget* parent = 0);

public Q_SLOTS:
    void setRestrictedSpan(int lower, int upper);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    void update();

private:
    int lower_;
    int upper_;

};

#endif // QXTSPANSLIDERRESTRICTED_H
