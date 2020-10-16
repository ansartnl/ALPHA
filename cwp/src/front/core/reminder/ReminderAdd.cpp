#include "ReminderAdd.h"

CReminderAdd::CReminderAdd(QWidget *pParent)
	: QWidget(pParent)
{
	setupUi(this);
	comboBox->addItem(" ", CReminder::FL);
	comboBox->addItem(" ", CReminder::PassFL);
	comboBox->addItem(" ", CReminder::Point);
	comboBox->addItem(" ", CReminder::Local);
	comboBox->addItem(" ", CReminder::Airport);
	Translate();
	comboBox_2->hide();
	connect(pushButton, SIGNAL(clicked()), this, SLOT(OnButtonAdd()));
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnCombo(int)));
}

CReminderAdd::~CReminderAdd()
{

}

void CReminderAdd::Translate()
{
	pushButton->setText(tr("Add"));
	for(int i = 0; i < comboBox->count(); ++i)
	{
		comboBox->setItemText(i, CReminder::GetReminderName(CReminder::ReminderType(comboBox->itemData(i).toInt())));
	}
}

void CReminderAdd::OnButtonAdd()
{
	int iCurIndex = comboBox->currentIndex();
	if(iCurIndex != -1)
	{
		QString sValue = lineEdit->text();
		switch(comboBox->itemData(iCurIndex).toInt())
		{
		case CReminder::FL:
		case CReminder::PassFL:
			if(sValue.toInt() <= 0) return;
			break;
		case CReminder::Point:
			if(comboBox_2->currentIndex() == -1) return;
			sValue = comboBox_2->itemData(comboBox_2->currentIndex()).toString();
			if(sValue.isEmpty()) return;
			break;
		}
		emit addReminderItem(CReminder::ReminderType(comboBox->itemData(iCurIndex).toInt()), sValue);
	}
}

void CReminderAdd::OnCombo(int index)
{
	if(index != -1)
	{
		if(comboBox->itemData(index).toInt() == CReminder::Point)
		{
			lineEdit->hide();
			comboBox_2->show();
			comboBox_2->clear();
			if(m_pPlane)
			{
				/*Pasha
				int i(m_pPlane->GetCurFPLPlan());
				QVector<QSharedPointer<CRoutePoint> > pointList = m_pPlane->GetChain()->GetPointLL()[i];
				QVector<QSharedPointer<CRoutePoint> >::const_iterator iter = pointList.begin();
				for(int j = 0; iter != pointList.end(); ++j, ++iter)
					if(!(*iter)->GetCustom())
					{
						QString sText = QString::number(i) + " " +QString::number(j) + " " + (*iter)->GetPoint()->GetName();
						comboBox_2->addItem((*iter)->GetPoint()->GetName(), sText);
					}
				*/
			}
		}
		else
		{
			comboBox_2->hide();
			lineEdit->show();
		}
	}
}

void CReminderAdd::SetPlane(const QSharedPointer<CAirplane> &pPlane)
{
	m_pPlane = pPlane;
	OnCombo(comboBox->currentIndex());
}
