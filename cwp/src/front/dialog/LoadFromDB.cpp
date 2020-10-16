#include "LoadFromDB.h"

#include "import/Zone.h"
#include "Coordinate.h"

#include "main.h"

#include <QSqlTableModel>
#include <QPushButton>
#include <QSqlRecord>

const char LoadFromDB::DB_ZONE_ID[] = "id";
const char LoadFromDB::DB_ZONE_NAME[] = "name";
const char LoadFromDB::DB_ZONE_TYPE[] = "type";
const char LoadFromDB::DB_ZONE_IDNAME[] = "id_name";
const char LoadFromDB::DB_ZONE_PARENTID[] = "parent_id";
const char LoadFromDB::DB_ZONE_LOWLIMIT[] = "low_limit";
const char LoadFromDB::DB_ZONE_UPLIMIT[] = "up_limit";

LoadFromDB::LoadFromDB(const QString& sTitle, bool bSectorInclude, QWidget* parent) :
    QDialog(parent)
{
    // Dialog initialization
    setupUi(this);
    setWindowTitle(sTitle);
    SectorTab->setEnabled(bSectorInclude);
    label->setEnabled(bSectorInclude);
    FIRCombo->setEnabled(bSectorInclude);
    SectorView->setEnabled(bSectorInclude);

    // FIR
    m_pTMFIR = new QSqlTableModel;
    m_pTMFIR->setTable("zone");
    m_pTMFIR->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pTMFIR->setFilter("type = " + QString::number(Zone::enFIR));

    FIRView->setModel(m_pTMFIR);
    FIRView->horizontalHeader()->moveSection(FIRView->horizontalHeader()->visualIndex(m_pTMFIR->fieldIndex(
        DB_ZONE_IDNAME)), 0);
    FIRView->horizontalHeader()->moveSection(FIRView->horizontalHeader()->visualIndex(
        m_pTMFIR->fieldIndex(DB_ZONE_NAME)), 1);

    m_pTMFIR->setHeaderData(m_pTMFIR->fieldIndex(DB_ZONE_IDNAME), Qt::Horizontal, tr("Unique name"));
    m_pTMFIR->setHeaderData(m_pTMFIR->fieldIndex(DB_ZONE_NAME), Qt::Horizontal, tr("Name"));

    FIRView->setColumnHidden(m_pTMFIR->fieldIndex(DB_ZONE_ID), true);
    FIRView->setColumnHidden(m_pTMFIR->fieldIndex(DB_ZONE_TYPE), true);
    FIRView->setColumnHidden(m_pTMFIR->fieldIndex(DB_ZONE_PARENTID), true);
    FIRView->setColumnHidden(m_pTMFIR->fieldIndex(DB_ZONE_LOWLIMIT), true);
    FIRView->setColumnHidden(m_pTMFIR->fieldIndex(DB_ZONE_UPLIMIT), true);
    FIRView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    FIRView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_pTMFIR->select();

    // Sector
    m_pTMSector = new QSqlTableModel(this);
    m_pTMSector->setTable("zone");
    m_pTMSector->setFilter("1 = 0");
    m_pTMSector->setEditStrategy(QSqlTableModel::OnManualSubmit);

    SectorView->setModel(m_pTMSector);
    SectorView->horizontalHeader()->moveSection(SectorView->horizontalHeader()->visualIndex(m_pTMSector->fieldIndex(
        DB_ZONE_IDNAME)), 0);
    SectorView->horizontalHeader()->moveSection(SectorView->horizontalHeader()->visualIndex(m_pTMSector->fieldIndex(
        DB_ZONE_NAME)), 1);
    SectorView->horizontalHeader()->moveSection(SectorView->horizontalHeader()->visualIndex(m_pTMSector->fieldIndex(
        DB_ZONE_LOWLIMIT)), 2);
    SectorView->horizontalHeader()->moveSection(SectorView->horizontalHeader()->visualIndex(m_pTMSector->fieldIndex(
        DB_ZONE_UPLIMIT)), 3);

    m_pTMSector->setHeaderData(m_pTMSector->fieldIndex(DB_ZONE_IDNAME), Qt::Horizontal, tr("Airspace center"));
    m_pTMSector->setHeaderData(m_pTMSector->fieldIndex(DB_ZONE_NAME), Qt::Horizontal, tr("Description"));
    m_pTMSector->setHeaderData(m_pTMSector->fieldIndex(DB_ZONE_LOWLIMIT), Qt::Horizontal, tr("Lower limit\n meters"));
    m_pTMSector->setHeaderData(m_pTMSector->fieldIndex(DB_ZONE_UPLIMIT), Qt::Horizontal, tr("Upper limit\n meters"));

    SectorView->setColumnHidden(m_pTMSector->fieldIndex(DB_ZONE_ID), true);
    SectorView->setColumnHidden(m_pTMSector->fieldIndex(DB_ZONE_TYPE), true);
    SectorView->setColumnHidden(m_pTMSector->fieldIndex(DB_ZONE_PARENTID), true);
    SectorView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    SectorView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // FIR combo
    QList<Zone> ZoneList;
    TVA_CATCHALL_TRY
    {
        dao()->LoadZones(ZoneList);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
    QList<Zone>::const_iterator itZoneScan = ZoneList.begin();
    for (; itZoneScan != ZoneList.end(); ++itZoneScan)
    {
        if ((itZoneScan)->GetType() == Zone::enFIR)
        {
            FIRCombo->addItem(itZoneScan->GetIDName() + "(" + itZoneScan->GetName() + ")", itZoneScan->GetID());
        }
    }
    FIRCombo->model()->sort(0);
    FIRCombo->setCurrentIndex(0);

    connect(FIRView, SIGNAL(doubleClicked(const QModelIndex& )), this, SLOT(OnDblClick(const QModelIndex& )));
    connect(SectorView, SIGNAL(doubleClicked(const QModelIndex& )), this, SLOT(OnDblClick(const QModelIndex& )));

    loadSettings();
}

LoadFromDB::~LoadFromDB()
{
    saveSettings();
}

void LoadFromDB::changeEvent(QEvent *event)
{
    //LanguageChange events should not be confused with LocaleChange events.
    if (event->type() == QEvent::LocaleChange)
    {
        // HERE we can un|install qtranslator
        //    Locale-Change events are generated by the system and tell the application,
        //    "Maybe you should load a new translation."
    }
    else if (event->type() == QEvent::LanguageChange)
    {
        //    LanguageChange events are generated by Qt and tell the application's widgets,
        //    "Maybe you should retranslate all your strings."
        retranslateUi(this);
    }

    QDialog::changeEvent(event);
}

void LoadFromDB::loadSettings()
{
    restoreGeometry(globalSettings()->value("XMasterWindow/Settings/LoadFromDBGeometry").toByteArray());
}

void LoadFromDB::saveSettings()
{
    globalSettings()->setValue("XMasterWindow/Settings/LoadFromDBGeometry", saveGeometry());
}

void LoadFromDB::OnFIRSelect(int iIndex)
{
    if (FIRCombo->count() > 0)
        m_pTMSector->setFilter("type = " + QString::number(Zone::enSector) + " AND parent_id = " + QString::number(
            FIRCombo->itemData(FIRCombo->currentIndex()).toUInt()));
    else
        m_pTMSector->setFilter("1 = 0");
    m_pTMSector->select();
}

void LoadFromDB::OnButtonBox(QAbstractButton* pButton)
{
    QDialogButtonBox::ButtonRole BtnRole = buttonBox->buttonRole(pButton);
    switch (BtnRole)
    {
    case QDialogButtonBox::AcceptRole:
        if (tabWidget->currentWidget() == FIRTab)
        {
            QItemSelectionModel *selection_model = FIRView->selectionModel();
            m_iResultType = Zone::enFIR;
            m_ResultParent.clear();
            foreach (const QModelIndex &index, selection_model->selectedRows()) {
                results_.append(m_pTMFIR->record(index.row()).value(DB_ZONE_ID));
            }
            accept();
        }
        else if (tabWidget->currentWidget() == SectorTab)
        {
            QItemSelectionModel *selection_model = SectorView->selectionModel();
            m_iResultType = Zone::enSector;
            foreach (const QModelIndex &index, selection_model->selectedRows()) {
                results_.append(m_pTMSector->record(index.row()).value(DB_ZONE_ID));
                m_ResultParent = m_pTMSector->record(selection_model->currentIndex().row()).value(DB_ZONE_PARENTID);
            }
            accept();
        }
        break;
    case QDialogButtonBox::RejectRole:
        reject();
        break;
    default:
        break;
    }
}

void LoadFromDB::OnDblClick(const QModelIndex& qIndex)
{
    OnButtonBox(buttonBox->button(QDialogButtonBox::Ok));
}
