#ifndef CGUISector_H
#define CGUISector_H

#include "import/Scene.h"
#include "GUIInterface.h"
#include <QGraphicsItem>

class GUISectorBorder;

//! Graphic zone extension
class CGUISector
	: public Scene,
	public QGraphicsItem,
	public IGUIItemHandler
{
public:
	CGUISector();
	//@{
	//! Inherited from QGraphicsItem
	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
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

	void SetBorderVisible(bool bVisible);
    GUISectorBorder* GetBorderItem() { return m_Border.data(); }
    inline void setBorderColor(const QColor &color) { penColor = color; }
    inline void setActiveColor(const QColor &color) { brushColor = color; }

private:
	QPainterPath	m_Path;
	int				m_iPenWidth;
	bool			m_bBorderDraw;

    QColor penColor;
    QColor brushColor;

    QScopedPointer<GUISectorBorder>  m_Border;
    friend class GUISectorBorder;
};

//! Class for the graphical representation of sector border
/*!
    Sector should have color + different color for its border,
    thus using QGraphicsScene concept new item should be created
    to display sector's border above other sectors.
*/
class GUISectorBorder : public QGraphicsItem
{
public:
    //! Constructor
    /*! \param scene a pointer to CGUISector */
    GUISectorBorder(CGUISector *scene);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

private:
    /** Reference to the scene object */
    CGUISector   *mScene;
};

#endif // CGUISector_H
