#ifndef GUISVGITEMS_H
#define GUISVGITEMS_H

#include <qgraphicssvgitem.h>
#include "GUIInterface.h"
#include "SvgItem.h"
//! Class for the graphical representation of path
class CGUISvgItem:
        public QGraphicsSvgItem,
        public SvgItem,
        public IGUIItemHandler
{
    Q_OBJECT

    public:
        //! Constructor
    CGUISvgItem(const QString & pathImage, QGraphicsItem * parent = 0);
     virtual void MarkItemActive(bool bActive);

    //! Destructor
    virtual ~CGUISvgItem();
        //@{
        //! Inherited from QGraphicsItem
        virtual QRectF boundingRect() const;
        QPainterPath shape() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        //@}

        //! Set new position at point
        /*!
         \param point a QPointF
         \param coord a CCoord airplane coordinate
         */
        void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
        //! Recalculate position
        //void UpdatePosition(qint64 qllXMin, qint64 qllYMax, double dScale);
        //! Set scale coefficient
        //void SetScaleCoeff(double dCoeff);
        //void SetCoords(const CCoord & coord1, const CCoord & coord2, qreal angle);
//        void SetColor(const QColor& color)
//        { m_color = color; }

//        const QColor& GetColor() const
//        { return m_color; }

    protected:
        //! Inherited from QGraphicsTextItem
        //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    private:
        //double GetScale() const;

    private:
        int m_iPenWidth;
        //QColor m_color;
        QPainterPath m_painterPath;
        //QPointF m_CenterPoint;
        //CCoord m_Coord;
        double m_dImageAngle;
        //double m_dScale;
        //double m_dScaleCoeff;
        QPointF point;
        QPointF pointNext;
        //qreal old_rotate;
        double scale_x;
        double scale_y;
};

#endif // GUISVGITEMS_H
