#include "WindEditor.h"
#include "TVAError.h"
#include "Scene.h"

#include "main.h"

#include <math.h>

#include <QtCore/QList>

#include <QtGui/QIntValidator>
#include <QtGui/QFocusEvent>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <QHeaderView>

namespace gui {

    WindSpeedLineEdit::WindSpeedLineEdit(int iRow, unsigned int uiWindID, int iSpeed, QWidget *pParent) :
        QLineEdit(pParent), m_iRow(iRow), m_pValidator(NULL), m_uiWindID(uiWindID), m_iSpeed(iSpeed)
    {
        setMaxLength(4);
        setText(QString("%1").arg(m_iSpeed));

        m_pValidator = new QIntValidator(this);
        m_pValidator->setRange(0, 9999);
        setValidator(m_pValidator);

        connect(this, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
    }

    void WindSpeedLineEdit::OnEditFinished()
    {
        int iSpeed = text().toInt();
        if (m_iSpeed != iSpeed)
        {
            m_iSpeed = iSpeed;
            emit speedChanged(m_iRow, m_uiWindID, m_iSpeed);
        }
    }

    void WindSpeedLineEdit::focusInEvent(QFocusEvent *pEvent)
    {
        if (pEvent->reason() == Qt::MouseFocusReason)
            emit speedActivated(m_iRow, m_uiWindID);
        QLineEdit::focusInEvent(pEvent);
    }

    WindAzimuthLineEdit::WindAzimuthLineEdit(int iRow, unsigned int uiWindID, int iAzimuth, QWidget *pParent) :
        QLineEdit(pParent), m_iRow(iRow), m_pValidator(NULL), m_uiWindID(uiWindID), m_iAzimuth(iAzimuth)
    {
        setMaxLength(3);
        setText(QString("%1").arg(m_iAzimuth));

        m_pValidator = new QIntValidator(this);
        m_pValidator->setRange(0, 359);
        setValidator(m_pValidator);

        connect(this, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
    }

    void WindAzimuthLineEdit::OnEditFinished()
    {

        int iAzimuth = text().toInt();
        if (m_iAzimuth != iAzimuth)
        {
            m_iAzimuth = iAzimuth;
            emit azimuthChanged(m_iRow, m_uiWindID, m_iAzimuth);
        }
    }

    void WindAzimuthLineEdit::focusInEvent(QFocusEvent *pEvent)
    {
        if (pEvent->reason() == Qt::MouseFocusReason)
            emit azimuthActivated(m_iRow, m_uiWindID);
        QLineEdit::focusInEvent(pEvent);
    }

    WindEditor::WindEditor(QWidget *pParent) :
        QTableWidget(pParent), m_pZonesCombo(NULL)
    {
        horizontalHeader()->setMinimumHeight(30);
        showMaximized();
        setAutoFillBackground(false);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setAlternatingRowColors(true);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setSelectionBehavior(QAbstractItemView::SelectItems);
        setSortingEnabled(true);
        horizontalHeader()->setCascadingSectionResizes(false);
        horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        verticalHeader()->setDefaultSectionSize(20);
        setColumnCount(3);
        Translate();

    }

    WindEditor::~WindEditor()
    {
    }

    void WindEditor::Translate()
    {
        setWindowTitle(tr("Wind"));
        setHorizontalHeaderLabels(QStringList() << tr("FLevel") << tr("Speed") << tr("Azimuth"));
    }

    void WindEditor::UpdateActions(QComboBox *pZonesCombo)
    {
        m_pZonesCombo = pZonesCombo;
        disconnect(m_pZonesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnZoneChanged(int)));
        connect(m_pZonesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnZoneChanged(int)));

        TVA_CATCHALL_TRY
            {
                // load zones to combo
                Dao::TSceneMap FIRs;
                dao()->LoadFIRs(FIRs);

                m_pZonesCombo->clear();
                for (Dao::TSceneMap::const_iterator FIR = FIRs.begin(); FIR != FIRs.end(); ++FIR)
                {
                    QSharedPointer<Scene> pscene = *FIR;
                    m_pZonesCombo->addItem(pscene->GetIDName(), pscene->GetID());
                }
                m_pZonesCombo->setCurrentIndex(0);

                // update winds
                if (false == FIRs.empty())
                    OnZoneChanged(m_pZonesCombo->currentIndex());
            }TVA_CATCHALL(err){
        QMessageBox::critical(this, tr("Error"), err.reason());
    }
}

void WindEditor::OnZoneChanged(int iZoneIndex)
{
    TVA_CATCHALL_TRY
    {
        unsigned int uiID = m_pZonesCombo->itemData(iZoneIndex).toUInt();

        // load winds from DB
        typedef QList<core::CWind> CWindsList;
        CWindsList Winds;
        dao()->LoadWinds(Winds, uiID);

        // prepare table
        clearContents();
        setRowCount(Winds.count());

        // show all winds in table
        m_Winds.clear();
        int Row = 0;
        for(CWindsList::const_iterator Iter = Winds.begin(); Iter != Winds.end(); ++ Iter, ++ Row)
        {
            core::CWind Wind = *Iter;
            m_Winds[Wind.GetID()] = Wind;

            QLabel *qFLevel = new QLabel;
            qFLevel->setText(tr("F%1-F%2")
                            .arg(MetresToF(Wind.GetFLevelStart()))
                            .arg(MetresToF(Wind.GetFLevelEnd())));

            WindSpeedLineEdit *qSpeed = new WindSpeedLineEdit(Row, Wind.GetID(), (int) ceil(Wind.GetSpeed()));
            connect(qSpeed, SIGNAL(speedChanged(int,uint,int)), this, SLOT(OnSpeedChanged(int,uint,int)));
            connect(qSpeed, SIGNAL(speedActivated(int,uint)), this, SLOT(OnSpeedActivated(int,uint)));

            WindAzimuthLineEdit *qAzimuth = new WindAzimuthLineEdit(Row, Wind.GetID(), (int) ceil(Wind.GetAzimuth()));
            connect(qAzimuth, SIGNAL(azimuthChanged(int,uint,int)), this, SLOT(OnAzimuthChanged(int,uint,int)));
            connect(qAzimuth, SIGNAL(azimuthActivated(int,uint)), this, SLOT(OnAzimuthActivated(int,uint)));

            setCellWidget(Row, 0, qFLevel);
            setCellWidget(Row, 1, qSpeed);
            setCellWidget(Row, 2, qAzimuth);
        }
    }
    TVA_CATCHALL(err)
    {
        QMessageBox::critical(this, tr("Error"), err.reason());
    }
}

int WindEditor::MetresToF(double dMetres)
{
    return (int) ::ceil(dMetres / (100 * 0.3048));
}

void WindEditor::OnSpeedChanged(int, unsigned int uiWindID, int iSpeed)
{
    TVA_CATCHALL_TRY
    {
        core::CWind &Wind = m_Winds[uiWindID];
        Wind.SetSpeed(iSpeed);
        dao()->SaveWind(Wind);
    }
    TVA_CATCHALL(err)
    {
        QMessageBox::critical(this, tr("Error"), err.reason());
    }
}
void WindEditor::OnSpeedActivated(int iRow, unsigned int)
{
    setCurrentCell(iRow, 1);
}

void WindEditor::OnAzimuthChanged(int, unsigned int uiWindID, int iAzimuth)
{
    TVA_CATCHALL_TRY
    {
        core::CWind &Wind = m_Winds[uiWindID];
        Wind.SetAzimuth(iAzimuth);
        dao()->SaveWind(Wind);
    }
    TVA_CATCHALL(err)
    {
        QMessageBox::critical(this, tr("Error"), err.reason());
    }
}
void WindEditor::OnAzimuthActivated(int iRow, unsigned int)
{
    setCurrentCell(iRow, 2);
}

}
