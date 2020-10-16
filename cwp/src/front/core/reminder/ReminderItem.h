#ifndef REMINDERITEM_H
#define REMINDERITEM_H

#include <QWidget>
#include "ui_ReminderItem.h"
#include "front/common/XAppSettings.h"
#include "back/core/airplane/Reminder.h"

//! Class for working with reminder
class CReminderItem
	: public QWidget
	, public Ui::ReminderItem
	, public AppSettings
	, public CReminder
{
	Q_OBJECT
	CReminderItem() : CReminder(FL, QString::null) {}
public:
	//! Constructor
	/*!
	  \param type a reminder type
	  \param sValue a reminder value
	  \param pParent a parent widget
	*/
	CReminderItem(ReminderType type, QString sValue, QWidget* pParent = 0);
	//! Destructor
	virtual ~CReminderItem();

	//! Set reminder status
	void SetItemCompleted(bool bFlag = true);

signals:
	//! Close reminder
	/*!
	  \param type a reminder type
	  \param sValue a reminder value
	*/
	void closeReminderItem(CReminder::ReminderType type, QString sValue);

protected slots:
	//! Press close button
	void OnToolButton();

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}
};

#endif // REMINDERITEM_H
