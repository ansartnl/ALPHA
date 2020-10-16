#ifndef GUIRESTRICTION_H
#define GUIRESTRICTION_H

#include "GUIInterface.h"
#include "Restriction.h"

#include <QGraphicsItem>
class QTimer;

//! Class for the graphical representation of restriction zone
class CGUIRestriction
	: public CRestriction,
	public QGraphicsItem,
	public IGUIItemHandler
{
public:
	//! Constructor
	CGUIRestriction(QGraphicsItem * parent = 0);
	//! Destructor
	virtual ~CGUIRestriction();

	//@{
	//! Inherited from QGraphicsItem
	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;
	virtual void paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget);
	//@}

	//@{
	//! Interface IGUIItemHandler
	virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
	virtual void SetSI(qint16 SI) {}
	//@}

	//! Inherited from ItemHandler
	virtual void MarkItemActive(bool bActive);

private:
        int             m_iPenWidth;
        QColor          m_color;
        QPainterPath    m_painterPath;
};


//! Class for the graphical representation of restriction user zone
class CGUIRestrictionUser
        : public QGraphicsObject,
        public CRestrictionUser,
        public IGUIItemHandler
{
    Q_OBJECT
public:
        //! Constructor
        CGUIRestrictionUser(QGraphicsItem * parent = 0);
        //! Destructor
        virtual ~CGUIRestrictionUser();

        //@{
        //! Inherited from QGraphicsItem
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(
                QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget);
        //@}

        //@{
        //! Interface IGUIItemHandler
        virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
        virtual void SetSI(qint16 SI) {}
        //@}

        //! Inherited from ItemHandler
        virtual void MarkItemActive(bool bActive);

        //! Update visibility state because of time restriction.
        void updateVisibility();

        //! Highlight the item for specified interval.
        /*!
            \param interval Interval in ms to highlight this zone.
                            If -1 - then highlight forever.
        */
        void highlight(bool on, int interval = -1);

protected:
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

private slots:
        void onTimeout();
        void on_highlight_stop();

private:
        int             m_iPenWidth;
        QColor          m_color;
        QPainterPath    m_painterPath;
        QTimer          *m_timer;

        bool            m_highlighted;
};

#endif // GUIRESTRICTION_H
