#include "ACTDlg.h"

#include "front/core/airplane/AClist/SFPL.h"
#include "front/core/airplane/AClist/ListAC.h"
#include "back/Master.h"
#include "back/core/View.h"
#include "Scene.h"
#include "uppercasevalidator.h"

#include <QMessageBox>
#include <QCompleter>

CACTDlg::CACTDlg(CMaster* pMaster, int code, quint32 prevSFPLID, QWidget *parent) :
    QDialog(parent), m_qPrevSFPLID(prevSFPLID), m_sEmptyType(" "), m_pMaster(pMaster)
    , m_pViewFPL(0)
{
	setupUi(this);

    timeETO->setTime(QTime::currentTime());
    labelTYPE->setText(m_sEmptyType);
    labelADEP->setText(m_sEmptyType);
    labelADES->setText(m_sEmptyType);

    editACID->setValidator(new UppercaseValidator(this));
    editCOPin->setValidator(new UppercaseValidator(this));

    editFL->setValidator(new QDoubleValidator(0, 999, 1, this));

    QRegExp regExp("[0-7]{1,4}");
	editCODE->setValidator(new QRegExpValidator(regExp, this));

	if(pMaster)
	{
//		QSharedPointer<CView> pView = pMaster->GetView();
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(pMaster->GetView());
		if(pView)
		{
            QStringList listPoints;
            CView::TSceneList::const_iterator itFIR = pView->GetScenes().constBegin();
            for(; itFIR != pView->GetScenes().constEnd(); ++itFIR)
            {
                Scene::TPointsMap::const_iterator itPoint = (*itFIR)->GetPoints().begin();
                for(; itPoint != (*itFIR)->GetPoints().end(); ++itPoint)
                    listPoints << (*itPoint)->GetName();
            }

            QCompleter* completer = new QCompleter(listPoints, this);
            completer->setCompletionMode(QCompleter::PopupCompletion);
            editCOPin->setCompleter(completer);
        }
	}

    QList<CFPL> list;
	TVA_CATCHALL_TRY
	{
        dao()->LoadFPLs(list, "visibility = 1");
	} TVA_CATCHALL(err)
	{
		QMessageBox::critical(this, tr("FPL"), err.reason());
	}

    connect(editACID, SIGNAL(textChanged(const QString&)), this, SLOT(OnChangeACID(const QString&)));

    QStringList listFPLs;
    QList<CFPL>::iterator itList = list.begin();
	for(; itList != list.end(); ++itList)
	{
        m_mapFPL.insert(itList->GetACID(), *itList);
        listFPLs.append(itList->GetACID());
	}
    QCompleter* completer = new QCompleter(listFPLs, this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    editACID->setCompleter(completer);

    if(m_mapFPL.size())
        editACID->setText(m_mapFPL.begin().key());

    if(code)
    {
        QString sText;
        sText = QString::number(code, 8);
        sText = sText.rightJustified(4, '0');
        editCODE->setText(sText);

        if(prevSFPLID)
            editCODE->setReadOnly(true);
    }

	resize(minimumWidth(), height());
    connect(pushButtonFPL, SIGNAL(clicked()), this, SLOT(OnButFPL()));
}

void CACTDlg::accept()
{
    m_pSFPL.clear();

    TMapFPL::const_iterator itFPL = m_mapFPL.find(editACID->text());
    if(itFPL == m_mapFPL.constEnd())
    {
        editACID->setFocus();
        return;
    }
    const QString code = editCODE->text();
    if (code.toUInt() == 0)
	{
		editCODE->setFocus();
		return;
	}
	if(editCOPin->text().isEmpty())
	{
		editCOPin->setFocus();
		return;
	}
    if(editFL->text().toDouble() == 0)
    {
        editFL->setFocus();
        return;
    }
    QList<int> listCode = CListAC::GetMode3AReplyList();
    if(listCode.contains(code.toInt(0, 8)))
    {
        QMessageBox::critical(this, tr("SFPL"), tr("Double code"));
        editCODE->setFocus();
        return;
    }
	TVA_CATCHALL_TRY
	{
        if (m_pMaster)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_pMaster->GetView());
            if (view)
            {
                QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(m_pMaster->GetView());
                if (pView)
                {
                    Scene::TSceneList listSectors = pView->FindSectorsByPoint(editCOPin->text(), editFL->text().toDouble()*30.48);
                    if (!listSectors.size())
                    {
                        QMessageBox::critical(this, tr("SFPL"), tr("Can't find sector"));
                        return;
                    }
                }
            }
        }

        m_pSFPL = QSharedPointer<CSFPL>(new CSFPL);
        *(static_cast<CFPL*>(m_pSFPL.data())) = *(itFPL);
        m_pSFPL->SetCODE(code.toInt(0, 8));
        m_pSFPL->SetCOPin(editCOPin->text());
        m_pSFPL->SetETOin(timeETO->time());
        m_pSFPL->SetCOPinFL(editFL->text().toDouble());
        QDialog::accept();
	} TVA_CATCHALL(err)
	{
		QMessageBox::critical(this, tr("SFPL"), err.reason());
	}
}

void CACTDlg::OnChangeACID(const QString& text)
{
    TMapFPL::const_iterator itFPL = m_mapFPL.find(text);
    if(itFPL != m_mapFPL.constEnd())
    {
        if(itFPL->GetAirplanePtr())
            labelTYPE->setText(itFPL->GetAirplanePtr()->GetName());
        else
            labelTYPE->setText(m_sEmptyType);
        labelADEP->setText(itFPL->GetDepAerodrome());
        labelADES->setText(itFPL->GetDestAerodrome());
        QStringList sList = itFPL->GetRoute().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if(sList.size())
        {
            QStringList Word = sList.front().split("/");
            QString sWord = Word.front();
            if (sWord != ("VFR") &&
                sWord != ("IFR") &&
                sWord != ("C") &&
                sWord != ("DCT"))
            {
                editCOPin->setText(sWord);
            }
        }
        if(itFPL->GetLevel())
            editFL->setText(QString("%1").arg(itFPL->GetLevel()/30.48, 3, 'f', 0, QLatin1Char('0')));
        else
            editFL->setText("");
    }
    else
    {
        labelTYPE->setText(m_sEmptyType);
        labelADEP->setText(m_sEmptyType);
        labelADES->setText(m_sEmptyType);
    }
    SelFPLinView(text);
}

void CACTDlg::OnButFPL()
{
    if(m_pMaster)
    {
        if(m_pViewFPL)
        {
            QVBoxLayout* lt = qobject_cast<QVBoxLayout*>(layout());
            if(lt)
            {
                lt->removeItem(m_pHBox);
                lt->removeWidget(m_pViewFPL);
            }
            delete m_pButAct;
            m_pButAct = 0;
            delete m_pHBox;
            m_pHBox = 0;
            delete m_pViewFPL;
            m_pViewFPL = 0;
            adjustSize();
        }
        else
        {
            QVBoxLayout* lt = qobject_cast<QVBoxLayout*>(layout());
            if(lt)
            {
                m_pViewFPL = new FplBrowser(this);
                m_pViewFPL->setCanOtherInfoEmpty(networkSettings()->value("FPL_handler/field_18", false).toBool());
                lt->addWidget(m_pViewFPL, 1);
                m_pHBox = new QHBoxLayout();
                m_pButAct = new QPushButton(tr("ACT"), this);
                m_pHBox->addWidget(m_pButAct);
                m_pHBox->addStretch();
                lt->addLayout(m_pHBox);
                adjustSize();
                resize(width(), 400);
                SelFPLinView(editACID->text());
                connect(m_pViewFPL, SIGNAL(selectionItemChanged(const QString&)), this, SLOT(OnSelFPLinView(const QString&)));
                connect(m_pButAct, SIGNAL(clicked()), this, SLOT(OnButAct()));
            }
        }
    }
}

void CACTDlg::SelFPLinView(QString sACID)
{
    if(m_pViewFPL)
    {
        TMapFPL::const_iterator itFPL = m_mapFPL.find(sACID);
        if(itFPL != m_mapFPL.constEnd())
        {
            m_pViewFPL->SetCurSelByID(QString::number(itFPL->GetID()));
        }
    }
}

void CACTDlg::OnSelFPLinView(const QString& sID)
{
    QString sACID;
    TMapFPL::const_iterator itFPL = m_mapFPL.constBegin();
    for(; itFPL != m_mapFPL.constEnd(); ++itFPL)
    {
        if(itFPL->GetID() == sID.toUInt())
        {
            sACID = itFPL->GetACID();
            break;
        }
    }
    editACID->setText(sACID);
}

void CACTDlg::OnButAct()
{
    if(m_pViewFPL)
    {
        QString sID = m_pViewFPL->SelectedObject();
        if(!sID.isEmpty())
        {
            TVA_CATCHALL_TRY
            {
                dao()->InsertFPLinFPL_MASTER(sID.toUInt());

                QList<CFPL> list;
                TVA_CATCHALL_TRY
                {
                    dao()->LoadFPLs(list, "visibility = 1");
                } TVA_CATCHALL(err)
                {
                    QMessageBox::critical(this, tr("FPL"), err.reason());
                }

                m_mapFPL.clear();
                QStringList listFPLs;
                QList<CFPL>::iterator itList = list.begin();
                for(; itList != list.end(); ++itList)
                {
                    m_mapFPL.insert(itList->GetACID(), *itList);
                    listFPLs.append(itList->GetACID());
                }
                QCompleter* completer = new QCompleter(listFPLs, this);
                completer->setCompletionMode(QCompleter::PopupCompletion);
                editACID->setCompleter(completer);

                OnSelFPLinView(sID);
            }
            TVA_CATCHALL(err)
            {
				Q_UNUSED(err)
                QMessageBox::warning(this, tr("FPL"), tr("FPL exist"));
            }
        }
    }
}
