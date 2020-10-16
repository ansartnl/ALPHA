#ifndef REMINDERADD_H
#define REMINDERADD_H

#include <QWidget>
#include "ui_ReminderAdd.h"
#include "front/common/XAppSettings.h"
#include "ReminderItem.h"
#include "back/core/Airplane.h"

//! Class for working with reminder
class CReminderAdd
	: public QWidget
	, public Ui::ReminderAdd
	, public AppSettings
{
	Q_OBJECT
public:
	//! Constructor
	CReminderAdd(QWidget* pParent = 0);
	//! Destructor
	virtual ~CReminderAdd();
	//! Set current CAirplane object
	void SetPlane(const QSharedPointer<CAirplane>& pPlane);

signals:
	//! Add new reminder
	/*!
	  \param type a reminder type
	  \param sValue a reminder value
	*/
	void addReminderItem(CReminder::ReminderType type, QString sValue);

protected slots:
	//! Add new reminder
	void OnButtonAdd();
	//! Change reminder type
	void OnCombo(int index);

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}
private:
	QSharedPointer<CAirplane>	m_pPlane;
};

#endif // REMINDERADD_H
