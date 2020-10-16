#ifndef XREMINDER_H
#define XREMINDER_H

#include <QWidget>
#include "front/common/XAppSettings.h"
#include "ReminderItem.h"
#include "back/core/Airplane.h"

class CGUIAzimuth;
class CReminderAdd;
class QGridLayout;

//! Class for working with reminders
class XReminder
	: public QWidget
	, public AppSettings
{
	Q_OBJECT
	static const int GRID_COLUMN = 3;
public:
	//! Constructor
	XReminder(QWidget* pParent = 0);
	//! Destructor
	virtual ~XReminder();

	//! Set list of reminders
	void SetReminders(const QMap<quint32, QSharedPointer<CReminder> >& pReminders);
	//! Update data
	void UpdateAzimuth();
	//! Set new airplane
	void SetPlane(const QSharedPointer<CAirplane>& pPlane);
	//! Update current airplane
	void UpdatePlane(const QSharedPointer<CAirplane>& pPlane);
	//! Delete current airplane
	void DeletePlane(const QSharedPointer<CAirplane>& pPlane);

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}

protected slots:
	//@{
	//! Slots on reminder items signals
	void OnChangedAzimuth(int degrees);
	void OnAddReminderItem(CReminder::ReminderType type, QString sValue);
	void OnCloseReminderItem(CReminder::ReminderType type, QString sValue);
	void OnDeleteReminderItem();
	//@}

private:
	QSharedPointer<CReminderItem> FindReminder(CReminder::ReminderType type, QString sValue);

private:
	typedef QList<QSharedPointer<CReminderItem> > TListReminderItems;

	CGUIAzimuth*		m_pAzimuth;
	CReminderAdd*		m_pReminderAdd;
	QLabel*				m_pAirLabel;
	TListReminderItems	m_listReminderItems;
	QGridLayout*		m_pGrid;
	QPair<CReminder::ReminderType, QString> m_DelItem;
	QSharedPointer<CAirplane>	m_pPlane;
};

#endif // XREMINDER_H
