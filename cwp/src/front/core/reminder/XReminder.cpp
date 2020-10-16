#include "XReminder.h"
#include "GUIAzimuth.h"
#include "ReminderAdd.h"
#include "front/core/GUIAirplane.h"

#include <QtCore/QTimer>

#include <QGridLayout>

XReminder::XReminder(QWidget* pParent)
	: QWidget(pParent)
{
	m_pAzimuth = new CGUIAzimuth(this);
	connect(m_pAzimuth, SIGNAL(changedAzimuth(int)), this, SLOT(OnChangedAzimuth(int)));

	m_pReminderAdd = new CReminderAdd(this);
	connect(m_pReminderAdd, SIGNAL(addReminderItem(CReminder::ReminderType, QString)),
				this, SLOT(OnAddReminderItem(CReminder::ReminderType, QString)));

	m_pAirLabel = new QLabel(this);

	m_pGrid = new QGridLayout();
	m_pGrid->setHorizontalSpacing(2);
	m_pGrid->setVerticalSpacing(1);
	m_pGrid->setContentsMargins(0,0,0,0);

	QHBoxLayout* pHor1 = new QHBoxLayout();
	pHor1->setSpacing(3);
	pHor1->setContentsMargins(0,0,0,0);

	pHor1->addWidget(m_pAirLabel);
	pHor1->addWidget(m_pReminderAdd);

	QVBoxLayout* pVer = new QVBoxLayout();
	pVer->setSpacing(3);
	pVer->setContentsMargins(0,0,0,0);

	pVer->addLayout(pHor1, 0);
	pVer->addLayout(m_pGrid, 0);
	pVer->addStretch(1);

	QHBoxLayout* pHor2 = new QHBoxLayout();
	pHor2->setSpacing(3);
	pHor2->setContentsMargins(2,2,2,2);

	//pHor2->addWidget(m_pAzimuth, 0, Qt::AlignLeft|Qt::AlignTop);
	pHor2->addWidget(m_pAzimuth, 0);
	pHor2->addLayout(pVer, 1);

	setLayout(pHor2);

	Translate();
}

XReminder::~XReminder()
{
	TListReminderItems::const_iterator itItem = m_listReminderItems.begin();
	for(; itItem != m_listReminderItems.end(); ++itItem)
		(*itItem)->close();
	m_listReminderItems.clear();
}

void XReminder::Translate()
{
	m_pAzimuth->update();
}

void XReminder::OnChangedAzimuth(int degrees)
{
	if(m_pPlane && m_pPlane->IsOur())
	{
		m_pPlane->ChangeAzimuth(degrees);
		CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(m_pPlane.data());
		if(pPlane) pPlane->SetAzimuth2(degrees);
	}
}

void XReminder::OnAddReminderItem(CReminder::ReminderType type, QString sValue)
{
	if(!FindReminder(type, sValue))
	{
		/*QSharedPointer<CReminderItem> pItem = QSharedPointer<CReminderItem>(new CReminderItem(type, sValue, this));
		connect(pItem.data(), SIGNAL(closeReminderItem(CReminder::ReminderType,QString)), this, SLOT(OnCloseReminderItem(CReminder::ReminderType,QString)));

		m_pGrid->addWidget(pItem.data(), m_pGrid->count()/GRID_COLUMN, m_pGrid->count()%GRID_COLUMN);
		m_listReminderItems.append(pItem);*/
		QSharedPointer<CReminder> pItem = QSharedPointer<CReminder>(new CReminder(type, sValue));
		if (m_pPlane)
            m_pPlane->AddReminder(pItem);
	}
}

void XReminder::OnCloseReminderItem(CReminder::ReminderType type, QString sValue)
{
	if(m_pPlane && m_pPlane->IsOur())
	{
		QSharedPointer<CReminderItem> pItem = FindReminder(type, sValue);
		if(pItem)
		{
			m_pPlane->RemoveReminder(pItem);
			/*QTimer::singleShot(0, this, SLOT(OnDeleteReminderItem()));
			m_DelItem = qMakePair(type, sValue);
			m_pGrid->removeWidget(pItem.data());*/
		}
	}
}

void XReminder::OnDeleteReminderItem()
{
	QSharedPointer<CReminderItem> pItem = FindReminder(m_DelItem.first, m_DelItem.second);
	if(pItem)
	{
		pItem->close();
		m_listReminderItems.removeOne(pItem);
		TListReminderItems::const_iterator itItem = m_listReminderItems.begin();
		for(int i = 0; itItem != m_listReminderItems.end(); ++itItem, ++i)
		{
			m_pGrid->addWidget(itItem->data(), i/GRID_COLUMN, i%GRID_COLUMN);
		}
	}
}

QSharedPointer<CReminderItem> XReminder::FindReminder(CReminder::ReminderType type, QString sValue)
{
	TListReminderItems::const_iterator itItem = m_listReminderItems.begin();
	for(; itItem != m_listReminderItems.end(); ++itItem)
	{
		if(((*itItem)->GetType() == type) && ((*itItem)->GetValue() == sValue))
			return *itItem;
	}
	return QSharedPointer<CReminderItem>();
}

void XReminder::SetReminders(const QMap<quint32, QSharedPointer<CReminder> >& pReminders)
{
	TListReminderItems::const_iterator itItem = m_listReminderItems.begin();
	for(; itItem != m_listReminderItems.end(); ++itItem)
		(*itItem)->close();
	m_listReminderItems.clear();

	QMap<quint32, QSharedPointer<CReminder> >::const_iterator itReminder = pReminders.begin();
	for(; itReminder != pReminders.end(); ++itReminder)
	{
		//OnAddReminderItem((*itReminder)->GetType(), (*itReminder)->GetValue());
        QSharedPointer<CReminderItem> pItem = QSharedPointer<CReminderItem>(new CReminderItem((*itReminder)->GetType(), (*itReminder)->GetValue()));
		pItem->SetID((*itReminder)->GetID());
		pItem->SetComplete((*itReminder)->GetComplete());
		pItem->SetTime((*itReminder)->GetTime());
		pItem->SetItemCompleted(pItem->GetComplete());
		connect(pItem.data(), SIGNAL(closeReminderItem(CReminder::ReminderType,QString)), this, SLOT(OnCloseReminderItem(CReminder::ReminderType,QString)));

		m_pGrid->addWidget(pItem.data(), m_pGrid->count()/GRID_COLUMN, m_pGrid->count()%GRID_COLUMN);
		m_listReminderItems.append(pItem);
	}
}

void XReminder::UpdateAzimuth()
{
	if(m_pPlane)
	{
		m_pAzimuth->setAzimuth1(m_pPlane->GetAzimuth());
		m_pAzimuth->setAzimuth2(m_pPlane->GetAzimuth());
		CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(m_pPlane.data());
		if(pPlane)
		{
			int az2 = pPlane->GetAzimuth2();
			if(az2 != 361) m_pAzimuth->setAzimuth2(az2);
		}
	}
}

void XReminder::SetPlane(const QSharedPointer<CAirplane>& pPlane)
{
	m_pPlane = pPlane;
	m_pReminderAdd->SetPlane(m_pPlane);
	bool bCanExecute = m_pPlane && m_pPlane->IsOur();
	m_pReminderAdd->pushButton->setEnabled(bCanExecute);
	m_pAzimuth->SetReadOnly(!bCanExecute);
	if(!bCanExecute)
	{
		TListReminderItems::const_iterator itItem = m_listReminderItems.begin();
		for(; itItem != m_listReminderItems.end(); ++itItem)
			(*itItem)->close();
		m_listReminderItems.clear();
	}
	if(m_pPlane && m_pPlane->GetSFPL())
		m_pAirLabel->setText(m_pPlane->GetSFPL()->GetACID());
	else
		m_pAirLabel->setText(QString());
}

void XReminder::UpdatePlane(const QSharedPointer<CAirplane>& pPlane)
{
	if(pPlane && m_pPlane)
	{
		if(pPlane->GetID() == m_pPlane->GetID())
			SetReminders(pPlane->GetReminders());
	}
}

void XReminder::DeletePlane(const QSharedPointer<CAirplane>& pPlane)
{
	if(pPlane && m_pPlane)
	{
		if(pPlane->GetID() == m_pPlane->GetID())
			SetPlane(QSharedPointer<CAirplane>());
	}
}
