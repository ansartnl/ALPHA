#ifndef _SSA_LEGS_H__
#define _SSA_LEGS_H__

class QLineF;
class QPointF;
class QPainterPath;
#include "QtCore/QtGlobal"

namespace SsaPainter
{
    //! Draw CF leg.
    /*!
        \param turnDirection 0 - unknown, 1 - right, 2 - left turn
    */
    void drawCF(QPainterPath &path, const QLineF &linePrev, const QLineF &lineCF,
                int turnDirection, qreal radius);

    //! Draw TF leg.
    void drawTF(QPainterPath &path, const QPointF &pntNext);
}

#endif // _SSA_LEGS_H__
