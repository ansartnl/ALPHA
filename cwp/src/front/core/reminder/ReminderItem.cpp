#include "ReminderItem.h"

CReminderItem::CReminderItem(ReminderType type, QString sValue, QWidget* pParent)
	: QWidget(pParent), CReminder(type, sValue)
{
	setupUi(this);
	Translate();
	connect(toolButton, SIGNAL(clicked()), this, SLOT(OnToolButton()));
}

CReminderItem::~CReminderItem()
{

}

void CReminderItem::Translate()
{
	QString sText;
	switch(GetType())
	{
	case Local:
		break;
	case Point:
		{
			QStringList sList = GetValue().split(" ", QString::SkipEmptyParts);
			sText = sList.size() > 2 ? sList[2] : QString::null;
		}
		break;
	default:
		sText = GetValue();
		break;
	}
	QString sTemp(GetReminderName(GetType()) + ((GetType() == Local) ? QString::null : "   " + sText));
	if(GetComplete())
	{
		sTemp += " (" + GetTime().toString(Qt::ISODate) + ")";
	}
	label->setText(sTemp);
}

void CReminderItem::OnToolButton()
{
	emit closeReminderItem(GetType(), GetValue());
}

void CReminderItem::SetItemCompleted(bool bFlag/* = true*/)
{
	QPalette pal(palette());
	pal.setBrush(QPalette::WindowText, bFlag ? QBrush(Qt::red) : QLabel().palette().brush(QPalette::WindowText));
	setPalette(pal);
	Translate();
}
