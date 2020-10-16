#ifndef FORMULAR_INFO_H_
#define FORMULAR_INFO_H_

#include <QRectF>
#include <QPointF>


struct FormularInfo {

    QPointF pos;

    QRectF rect;

    bool fixed;

    FormularInfo() {}

    FormularInfo(const QPointF &pos_, const QRectF &rect_, bool fixed_) : pos(pos_), rect(rect_), fixed(fixed_) {}
};


#endif // FORMULAR_INFO_H_
