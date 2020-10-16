#include "fpldlg.h"
#include "GUIControlDlg.h"
#include "GUIView.h"

#include "back/core/Airplane.h"

#include "main.h"

#include <QGraphicsScene>
#include <QMessageBox>
#include <QTextEdit>
#include <QScrollBar>

CFPLDlg::CFPLDlg(const QSharedPointer<CSFPL>& FPL, QWidget *parent, Qt::WindowFlags f) :
		QWidget(parent, f), m_FPL(FPL), m_pAirplane(0)
{
	setupUi(this);

    {
        QList<QLabel*> listWidgets = findChildren<QLabel*>();
        QList<QLabel*>::iterator itList = listWidgets.begin();
        for(; itList != listWidgets.end(); ++itList)
            (*itList)->setText(QString::null);
    }

    if(FPL)
    {
        label_1->setText(tr("Type"));
        label_2->setText(m_FPL->GetAirplanePtr() ? m_FPL->GetAirplanePtr()->GetName() : QString::null);

        label_3->setText(tr("FRules"));
        label_4->setText(m_FPL->GetFRules());

        label_5->setText(tr("TFlight"));
        label_6->setText(m_FPL->GetTypeFlight());

        label_7->setText(tr("Equipment"));
        label_8->setText(m_FPL->GetEquipment());
        label_8->installEventFilter(this);

        label_9->setText(tr("DepAerodrome"));
        label_10->setText(m_FPL->GetDepAerodrome());

        label_11->setText(tr("Time"));
        label_12->setText(m_FPL->GetDepTime().toString("hh:mm"));

        label_13->setText(tr("DestAerodrome"));
        label_14->setText(m_FPL->GetDestAerodrome());

        label_15->setText(tr("Time"));
        label_16->setText(m_FPL->GetDestTime());

        // Route
        QString sTemp = m_FPL->GetRoute();
        QStringList sList = sTemp.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QStringList::const_iterator itList = sList.begin();
        sTemp.clear();
        for(int i = -1; itList != sList.end(); ++itList)
        {
            if(++i%5 == 0)
            {
                if(i) sTemp += "\n";
            }
            else
            {
                if(i) sTemp += " - ";
            }
            sTemp += (*itList);
        }
        label_98->setText(sTemp);
        label_98->installEventFilter(this);

        sTemp = m_FPL->GetOther();
        label_99->setText(sTemp);
        if(sTemp.isEmpty()) label_99->hide();
    }
//	connect(buttonActivate, SIGNAL(clicked()), this, SLOT(OnButtonActivate()));

    {
        QList<QLabel*> listWidgets = findChildren<QLabel*>();
        QList<QLabel*>::iterator itList = listWidgets.begin();
        for(; itList != listWidgets.end(); ++itList)
            if((*itList)->text().isEmpty()) (*itList)->hide();
    }

    if ( gActiveView )
        connect(this, SIGNAL(changeACID(quint16)), gActiveView, SLOT(OnChangeACID(quint16)),
                Qt::QueuedConnection);
    pushButtonACID->setEnabled(false);
}

CFPLDlg::~CFPLDlg()
{
}

bool CFPLDlg::eventFilter(QObject* obj, QEvent* ev)
{
	if((ev->type() == QEvent::MouseButtonPress) && (obj == label_8))
	{
        if(false/*Pasha m_pAirplane && m_pAirplane->IsOur()*/)
		{
			CGUIControlDlg* pDlg(0);
			emit getControlDlg(pDlg, CGUIControlDlg::ListType);
			if(pDlg)
			{
				QStringList sList;
				sList << "S/C" << "W";
				pDlg->SetValues(sList, sList);
				pDlg->Init(label_8->text());
				pDlg->resize(40,50);
//				pDlg->setPos(label_8->mapToParent(label_8->rect().bottomLeft()));
				connect(pDlg, SIGNAL( itemChanged(const QStringList) ),
								this, SLOT( OnEndEdit(const QStringList) ));
			}
		}
	}
    if((ev->type() == QEvent::MouseButtonDblClick) && (obj == label_98))
    {
        QMetaObject::invokeMethod(this, "OnSNinfo", Qt::QueuedConnection);
    }
    return QWidget::eventFilter(obj, ev);
}

void CFPLDlg::OnEndEdit(const QStringList sValues)
{
    return; //Pasha
    if(m_FPL && sValues.size())
	{
		label_8->setText(sValues.first());
		m_FPL->SetEquipment(sValues.first());
		if(m_pAirplane)
		{
			TVA_CATCHALL_TRY
			{
				dao()->UpdateFPL(*m_FPL);
			} TVA_CATCHALL(err)
			{
                                QMessageBox::critical(this, tr("FPL"), err.reason());
			}
		}
	}
}

void CFPLDlg::OnButtonActivate()
{
//    if(!m_pAirplane) return;
//    CACTDlg dlg(m_pAirplane->GetDatabase(), m_pAirplane->GetMaster(), m_FPL->GetCODE(), m_FPL->GetID(), this);
//    if(dlg.exec() == QDialog::Accepted)
//    {
//        m_pAirplane->ChangeSFPL(dlg.GetID());
//    }
}

void CFPLDlg::SetAirplane(CAirplane* pPlane)
{
	m_pAirplane = pPlane;
    if(false/*Pasha m_pAirplane && m_pAirplane->IsOur()*/)
	{
		QPalette pal(label_8->palette());
		pal.setBrush(QPalette::WindowText, QBrush(Qt::blue));
		label_8->setPalette(pal);
	}

    // Enable ACID button if FPL is not assigned.
    if ( m_pAirplane && !m_pAirplane->GetSFPL() )
    {
        pushButtonACID->setEnabled(true);
    }
    else
    {
        pushButtonACID->setEnabled(false);
        pushButtonACID->hide();
        buttonActivate->setText(tr("ReACT"));
    }
}

void CFPLDlg::on_pushButtonACID_clicked()
{
    if ( m_pAirplane && !m_pAirplane->GetSFPL() )
        emit changeACID( m_pAirplane->GetID() );
}

void CFPLDlg::OnSNinfo()
{
    if(m_pAirplane && m_pAirplane->GetSFPL())
    {
        QTextEdit* edit = new QTextEdit(this);
        edit->setReadOnly(true);
        edit->setLineWrapMode(QTextEdit::NoWrap);
        edit->resize(size());

        edit->append(tr("Sectors:"));
        edit->append(m_pAirplane->GetSFPL()->GetAllSectors());
        edit->append("\n" + tr("InSector:"));
        edit->append(m_pAirplane->GetSFPL()->GetInSector());
        edit->append("\n" + tr("CurSector:"));
        edit->append(m_pAirplane->GetSFPL()->GetCurSector());
        edit->append("\n" + tr("NextSector:"));
        edit->append(m_pAirplane->GetSFPL()->GetNextSector());

        edit->show();
        edit->verticalScrollBar()->triggerAction(QScrollBar::SliderToMinimum);
    }
}
