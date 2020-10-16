#ifndef GUIAIRPLANEPOINT_H
#define GUIAIRPLANEPOINT_H

#include "front/core/GUIObject.h"
#include "Coordinate.h"

class CGUIAirplane;

//! Class for the graphical representation of aircraft
class CGUIAirplanePoint: public CGUIObject
{
    public:
        //! Constructor
        CGUIAirplanePoint(QGraphicsItem * parent = 0);
        //@{
        //! Inherited from QGraphicsItem
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        //@}

        //! Set new position at point
        /*!
         \param point a QPointF
         \param coord a CCoord airplane coordinate
         */
        void SetPosition(QPointF point, CCoord coord);
        //! Recalculate position
        void UpdatePosition(qint64 qllXMin, qint64 qllYMax, double dScale);
        //! Set pointer CGUIAirplane
        /*! \param pPlane a pointer CGUIAirplane */
        void SetAirplane(CGUIAirplane* pPlane);

        qreal angle() const { return angle_; };

    protected:
        //! Inherited from QGraphicsTextItem
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    private:
        int m_iPenWidth;
        QPainterPath m_painterPath;
        //QPixmap			m_image;
        QPointF m_CenterPoint;
        CCoord m_Coord;

        /** Number of intervals that velocity vector occupies */
        int m_intervalNum;
        /** Interval increase factor for velocity vector marks */
        int m_intervalInc;

        CGUIAirplane* m_pAirplane;

        qreal angle_;
};

#endif // GUIAIRPLANEPOINT_H
