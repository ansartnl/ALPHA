#ifndef GUIREMINDER_H
#define GUIREMINDER_H

#include <QGraphicsWidget>
#include "back/core/airplane/Reminder.h"
#include "front/common/XAppSettings.h"

class CGUILabelText;
class CAirplane;

//! Class for showing airplane's reminders
class CGUIReminder : public QGraphicsWidget, public AppSettings
{
Q_OBJECT
public:
	//! Constructor
	CGUIReminder(QGraphicsItem * parent = 0);
	//! Destructor
	~CGUIReminder();

	//! Set reminders
	/*! \param list a list of reminders */
	void SetReminders(const CReminder::TListReminder& list);
	//! Set widget width
	void UpdateWidth();
	//! Set pointer CAirplane, current airplane
	/*! \param pPlane a pointer CAirplane */
	void SetPlane(CAirplane* pPlane);

	//@{
	//! Inherited from QGraphicsItem
	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void paint(
			QPainter *painter,
			const QStyleOptionGraphicsItem *option,
			QWidget *widget);
	//@}

protected:
	//! Inherited from QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont()
    {
    }
    virtual void SI()
    {
    }
    //@}

private:
	void adjustGrid();
	void SetReminderText(int index);
	QString GetShortName(CReminder::TReminder& pReminder);

private:
	int				m_iPenWidth;
	QColor			m_color;
	QBrush			m_backgroundBrush;
	QRectF			m_clRect;

	CReminder::TListReminder	m_listReminder;
	int							m_iCurIndex;

	CAirplane*					m_pPlane;

	CGUILabelText*	m_itemText;
	CGUILabelText*	m_itemClose;
};

#endif // GUIREMINDER_H
