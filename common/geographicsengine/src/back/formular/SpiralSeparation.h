#ifndef SPIRAL_SEPARATION_STRATEGY_H
#define SPIRAL_SEPARATION_STRATEGY_H

#include <QtCore/QPointF>

#include "SeparateStrategy.h"


namespace FormularSeparation
{

    //! Separation algorithm - spiral bypass to find a free space for formular rectangle.
    class SpiralSeparation : public Strategy
    {
    public:
        //! Separation operation for one formular against others.
        virtual bool separate(quint16 id, QPointF &pos);

        //! Algorithm depends on map scale.
        virtual bool isScaleFree() const
        { return false; }
    };

}

#endif // SPIRAL_SEPARATION_STRATEGY_H
