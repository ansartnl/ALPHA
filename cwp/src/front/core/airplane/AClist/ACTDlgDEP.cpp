#include "ACTDlgDEP.h"

#include "front/core/airplane/AClist/SFPL.h"
#include "front/core/airplane/AClist/ListAC.h"
#include "back/Master.h"
#include "back/core/View.h"
#include "Scene.h"
#include "uppercasevalidator.h"

#include <QMessageBox>
#include <QCompleter>

CACTDlgDEP::CACTDlgDEP(CMaster* pMaster, int code, quint32 prevSFPLID, QWidget *parent) :
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

    QRegExp regExp("[0-7]{1,4}");
	editCODE->setValidator(new QRegExpValidator(regExp, this));

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
    QStringList sAirportList = networkSettings()->value("users/current_sector_airports", "").toString().split(",", QString::SkipEmptyParts);
    for(; itList != list.end(); ++itList)
	{
        if(sAirportList.contains(itList->GetDepAerodrome()))
        {
            m_mapFPL.insert(itList->GetACID(), *itList);
            listFPLs.append(itList->GetACID());
        }
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
    }

	resize(minimumWidth(), height());
    connect(pushButtonFPL, SIGNAL(clicked()), this, SLOT(OnButFPL()));
}

void CACTDlgDEP::accept()
{
    m_pSFPL.clear();

    TMapFPL::const_iterator itFPL = m_mapFPL.find(editACID->text());
    if(itFPL == m_mapFPL.constEnd())
    {
        editACID->setFocus();
        return;
    }
	if(editCODE->text().toUInt() == 0)
	{
		editCODE->setFocus();
		return;
	}
	if(editCOPin->text().isEmpty())
	{
		editCOPin->setFocus();
		return;
	}
    QList<int> listCode = CListAC::GetMode3AReplyList();
    if(listCode.contains(editCODE->text().toInt(0, 8)))
    {
        QMessageBox::critical(this, tr("SFPL"), tr("Double code"));
        editCODE->setFocus();
        return;
    }
    TVA_CATCHALL_TRY
	{
        m_pSFPL = QSharedPointer<CSFPL>(new CSFPL);
        *(static_cast<CFPL*>(m_pSFPL.data())) = *(itFPL);
        m_pSFPL->SetCODE(editCODE->text().toInt(0, 8));
        m_pSFPL->SetCOPin(editCOPin->text());
        m_pSFPL->SetETOin(timeETO->time());
        QDialog::accept();
	} TVA_CATCHALL(err)
	{
		QMessageBox::critical(this, tr("SFPL"), err.reason());
	}
}

void CACTDlgDEP::OnChangeACID(const QString& text)
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

        if(m_pMaster)
        {
            QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(m_pMaster->GetView());
            if(pView)
            {
                QCompleter* oldCompleter = editCOPin->completer();
                QStringList listRunways;
                CView::TScenePtr pScene = pView->GetMainFIR();
                if(pScene)
                {
                    Scene::TAirportsMap::const_iterator itAirport = pScene->GetAirports().find(
                            CAirportKey(itFPL->GetDepAerodrome()));
                    if(itAirport != pScene->GetAirports().constEnd())
                    {
                        CAirport::TRunways::const_iterator itRunway =
                                (*itAirport)->GetRunways().constBegin();
                        for (; itRunway != (*itAirport)->GetRunways().constEnd(); ++itRunway)
                        {
                            listRunways << (*itRunway)->GetName();
                            CRunway::TSSA::const_iterator itSSA = (*itRunway)->GetSSA().constBegin();
                            for (; itSSA != (*itRunway)->GetSSA().constEnd(); ++itSSA)
                            {
                                listRunways << (*itSSA)->GetName();
                            }
                        }
                    }
                    listRunways.removeDuplicates();
                }
                QCompleter* completer = new QCompleter(listRunways, this);
                completer->setCompletionMode(QCompleter::PopupCompletion);
                editCOPin->setCompleter(completer);
                if(oldCompleter) delete oldCompleter;
            }
        }
    }
    else
    {
        labelTYPE->setText(m_sEmptyType);
        labelADEP->setText(m_sEmptyType);
        labelADES->setText(m_sEmptyType);

        QCompleter* oldCompleter = editCOPin->completer();
        editCOPin->setCompleter(0);
        if(oldCompleter) delete oldCompleter;
    }
    SelFPLinView(text);
}

void CACTDlgDEP::OnButFPL()
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

void CACTDlgDEP::SelFPLinView(QString sACID)
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

void CACTDlgDEP::OnSelFPLinView(const QString& sID)
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

void CACTDlgDEP::OnButAct()
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
