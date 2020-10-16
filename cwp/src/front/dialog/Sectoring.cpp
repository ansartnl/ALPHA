#include "Sectoring.h"

#include "back/core/airplane/BearingPoint.h"
#include "front/common/SqlModelHelper.h"
#include "LoadFromDB.h"
#include "front/common/SqlModelHelper.h"
#include "core/User.h"
#include "Sectoring.h"
#include "import/Zone.h"
#include "TVAError.h"
#include "main.h"

#include "qtautotransaction.h"

#include "qtcheckboxdelegate.h"

#include <QtDebug>

#include <QMessageBox>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlQueryModel>
#include <QTableWidget>
#include <QStandardItemModel>

Sectoring::Sectoring(QWidget* pPrnt) :
    QDialog(pPrnt)
{
    setupUi(this);

    // Load all records from sectoring_zone
    TVA_CATCHALL_TRY
    {
        dao()->LoadSectorZoneAll(m_SectZoneList);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

    sectorGroupModel = new QSqlRelationalTableModel(this);
    sectorGroupModel->setTable("sectoring");
    sectorGroupModel->setSort(sectorGroupModel->fieldIndex("name"), Qt::AscendingOrder);
    m_iSGTypeIDIndex = sectorGroupModel->fieldIndex("type_id");
    sectorGroupModel->setRelation(m_iSGTypeIDIndex, QSqlRelation("sectoring_type", "id", "name"));
    sectorGroupModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sectorGroupModel->select();

    SGroupView->setModel(sectorGroupModel);
    SGroupView->setItemDelegate(new QSqlRelationalDelegate(SGroupView));
    SGroupView->setColumnHidden(sectorGroupModel->fieldIndex("id"), true);
    SGroupView->setColumnHidden(sectorGroupModel->fieldIndex("bearing_point_id"), true);
    SGroupView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    SGroupView->horizontalHeader()->setVisible(true);
    SGroupView->setItemDelegateForColumn(sectorGroupModel->fieldIndex("name"), new CUniqueDelegate(sectorGroupModel,
        "name", this));
    connect(SGroupView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
        SLOT(OnSGroupChanged(const QModelIndex&)));


    sectorGroupModel->setHeaderData(1, Qt::Horizontal, tr("Group name"));
    sectorGroupModel->setHeaderData(2, Qt::Horizontal, tr("Type"));

    sectorsModel = new QSqlQueryModel(this);
    _UpdateSectors(0);
    SectorView->setModel(sectorsModel);
    SectorView->setColumnHidden(0, true);
    SectorView->setColumnHidden(1, true);
    SectorView->setColumnHidden(2, true);
    SectorView->setColumnHidden(3, true);
    SectorView->setColumnHidden(5, true);
    SectorView->setColumnHidden(6, true);

    SectorView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    connect(SectorView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
        SLOT(OnZoneChanged(const QModelIndex&)));

    //sectorsModel->setHeaderData(3, Qt::Horizontal, tr("Short name"));
    sectorsModel->setHeaderData(4, Qt::Horizontal, tr("Name"));
    //sectorsModel->setHeaderData(5, Qt::Horizontal, tr("Type"));
    //sectorsModel->setHeaderData(6, Qt::Horizontal, tr("FIR/UIR"));
    sectorsModel->setHeaderData(7, Qt::Horizontal, tr("Main"));

    SectorView->setItemDelegateForColumn(7, new QtCheckBoxDelegate(this));
    connect(SectorView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onSectorViewClicked(const QModelIndex &)));

    FirsModel = new QSqlQueryModel(this);

    _UpdateFirs(0);

    FirView->setModel(FirsModel);
    FirView->setColumnHidden(0, true);
    FirView->setColumnHidden(1, true);
    FirView->setColumnHidden(2, true);
    FirView->setColumnHidden(3, true);
    FirView->setColumnHidden(5, true);
    FirView->setColumnHidden(6, true);

    FirView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    FirView->horizontalHeader()->setVisible(true);
    //FirView->setItemDelegateForColumn(sectorsModel->fieldIndex(""),

    connect(FirView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onFirViewClicked(const QModelIndex &)));


    //FirsModel->setHeaderData(3, Qt::Horizontal, tr("Short name"));
    FirsModel->setHeaderData(4, Qt::Horizontal, tr("Name"));
    //FirsModel->setHeaderData(5, Qt::Horizontal, tr("Type"));
    //FirsModel->setHeaderData(6, Qt::Horizontal, tr("FIR/UIR"));
    FirsModel->setHeaderData(7, Qt::Horizontal, tr("Main"));



    FirView->setItemDelegateForColumn(7, new QtCheckBoxDelegate(this));
    connect(AddFir, SIGNAL(clicked()), this, SLOT(OnAddFir()));
    connect(RemoveFir, SIGNAL(clicked()), this, SLOT(OnRemoveFir()));

    sectorTypeModel = new QSqlTableModel(this);
    sectorTypeModel->setTable("sectoring_type");
    sectorTypeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sectorTypeModel->select();
    sectorTypeModel->setHeaderData(sectorTypeModel->fieldIndex("name"),Qt::Horizontal,tr("Name"));
    sectorTypeModel->setHeaderData(sectorTypeModel->fieldIndex("app"),Qt::Horizontal,tr("App attribute"));

    TypeView->setModel(sectorTypeModel);
    TypeView->setColumnHidden(sectorTypeModel->fieldIndex("id"), true);
    TypeView->setColumnHidden(sectorTypeModel->fieldIndex("permission"), true);
    TypeView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    TypeView->horizontalHeader()->setVisible(true);
    TypeView->setItemDelegateForColumn(sectorTypeModel->fieldIndex("name"), new CUniqueDelegate(sectorTypeModel, "name",
        this));

    // Permissions
    m_pModel = QSharedPointer<CSectoringModel> (new CSectoringModel());
    PrivilegeView->setModel(m_pModel.data());
    connect(PrivilegeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(OnTypeSelect()));
    connect(TypeView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
        SLOT(OnTypeSelect()));

    // Airports
    QSqlTableModel *sectorAirportModel = new QSqlTableModel(this);
    sectorAirportModel->setTable("AIRPORT");
    sectorAirportModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sectorAirportModel->setHeaderData(2, Qt::Horizontal, tr("Name"));
    sectorAirportModel->select();

    airportView->setModel(sectorAirportModel);
    airportView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    airportView->horizontalHeader()->setStretchLastSection(true);
    airportView->setColumnHidden(0, true);    //ID
    airportView->setColumnHidden(1, true);    //ICAO
    airportView->setColumnHidden(3, true);    //LATITUDE
    airportView->setColumnHidden(4, true);    //LONGITUDE
    airportView->setColumnHidden(5, true);    //ALTITUDE
    airportView->setColumnHidden(6, true);    //DECLINATION

    sectorBearingModel = new QSqlTableModel(this);
    sectorBearingModel->setTable("sectoring_bearing");
    sectorBearingModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sectorBearingModel->select();

    tabWidget->removeTab(2);
    loadSettings();

    TVA_CATCHALL_TRY
    {

        dao()->LoadSectorAirports(mSectorAirports);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

    connect(bearing, SIGNAL(currentIndexChanged(int)), SLOT(onBearingChanged(int)));
    connect(bearing, SIGNAL(checkedItemsChanged(QStringList)), SLOT(onCheckedItemsChanged(QStringList)));
    connect(cbAirportAllowed, SIGNAL(toggled(bool)), this, SLOT(OnAirportAllowed(bool)));

    connect(airportView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
        SLOT(OnAirportChanged(const QModelIndex&)));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
}

Sectoring::~Sectoring()
{
    saveSettings();
}

void Sectoring::setBearingPoints(const QList<CBearingPoint> &points)
{
    bearing->blockSignals(true);
    bearing->addItem(QString(), QVariant());
    foreach (const CBearingPoint &point, points)
    {
        bearing->addItem(point.GetName(), point.GetID());
    }
    bearing->blockSignals(false);
}

void Sectoring::OnAddSGroup()
{
    QString sNewName = CSqlModelHelper::GetUniqueName(*sectorGroupModel, "name", "New group");
    if (!sNewName.isEmpty())
    {
        int iRow = sectorGroupModel->rowCount();
        sectorGroupModel->insertRow(iRow);
        QSqlRecord qRec = sectorGroupModel->record(iRow);
        qRec.setValue("name", sNewName);
        sectorGroupModel->setRecord(iRow, qRec);
        QModelIndex qmIndex = sectorGroupModel->index(iRow, sectorGroupModel->fieldIndex("name"));
        SGroupView->setCurrentIndex(qmIndex);
        SGroupView->edit(qmIndex);
    }
}

void Sectoring::OnRemoveSGroup()
{
    QModelIndexList IndexList = SGroupView->selectionModel()->selectedRows();
    QModelIndexList::iterator itIndex = IndexList.begin();
    for (; itIndex != IndexList.end(); ++itIndex)
    {
        if (itIndex->isValid())
        {
            sectorGroupModel->removeRow(itIndex->row());
        }
    }
}

void Sectoring::OnOperButton(QAbstractButton* pButton)
{
    TVA_CATCHALL_TRY
        {
            QDialogButtonBox::ButtonRole BtnRole = ButtonBox->buttonRole(pButton);
            switch (BtnRole)
            {
            case QDialogButtonBox::AcceptRole:
            case QDialogButtonBox::ApplyRole:
            {
                QtAutoTransaction transaction(QSqlDatabase::database());
                TVA_CHECK(
                                sectorTypeModel->submitAll(),
                                sectorTypeModel->lastError().text(),
                                E_FAIL);
                TVA_CHECK(
                                sectorGroupModel->submitAll(),
                                sectorGroupModel->lastError().text(),
                                E_FAIL);
                TVA_CHECK(
                                sectorBearingModel->submitAll(),
                                sectorBearingModel->lastError().text(),
                                E_FAIL);

                dao()->SaveSectorZoneAll(m_SectZoneList);
                dao()->SaveSectorAirports(mSectorAirports);
                transaction.commit();
                // refresh types in combo box in groupsector
                sectorGroupModel->relationModel(m_iSGTypeIDIndex)->select();
            }
                break;
            case QDialogButtonBox::RejectRole:
            case QDialogButtonBox::DestructiveRole:
            {   sectorTypeModel->revertAll();
                sectorGroupModel->revertAll();
                sectorBearingModel->revertAll();
                dao()->LoadSectorZoneAll(m_SectZoneList);
            }
                break;
            default:
                break;
            }
        }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
    QMessageBox::critical(this, tr("Error"), err.reason());
}
}

void Sectoring::OnSGroupChanged(const QModelIndex& qIndex)
{
    if (qIndex.isValid())
    {
        QSqlRecord record = sectorGroupModel->record(qIndex.row());
        QVariant qID = record.value("id");
        if (!qID.isNull())
        {
            _UpdateSectors(qID.toUInt());
            _UpdateFirs(qID.toUInt());
            AddSector->setEnabled(true);
            RemoveSector->setEnabled(true);
            AddFir->setEnabled(true);
            RemoveFir->setEnabled(true);
        }
        else
        {
            _UpdateSectors(0);
            _UpdateFirs(0);
            AddSector->setEnabled(false);
            RemoveSector->setEnabled(false);
            AddFir->setEnabled(false);
            RemoveFir->setEnabled(false);
        }

        bearing->blockSignals(true);
        for (int i = 0; i < bearing->count(); ++i){
             bearing->setItemCheckState(i, Qt::Unchecked);
        }
        for (int iRow = 0; iRow < sectorBearingModel->rowCount(); iRow++){
            if (sectorBearingModel->record(iRow).value("sect_id").toUInt() == getSectorId()){
                int bID = sectorBearingModel->record(iRow).value("bearing_id").toInt();
                bearing->setItemCheckState(bearing->findData(bID), Qt::Checked);
           }
        }

        bearing->blockSignals(false);
    }
    else
    {
        _UpdateSectors(0);
        _UpdateFirs(0);
        AddSector->setEnabled(false);
        RemoveSector->setEnabled(false);
        AddFir->setEnabled(false);
        RemoveFir->setEnabled(false);
        bearing->blockSignals(true);
        bearing->setCurrentIndex(0);
        bearing->blockSignals(false);
    }

    if(tabWidget->currentIndex() == 2)
    {
        OnAirportChanged(airportView->currentIndex());
    }
}

void Sectoring::OnFirChanged(const QModelIndex& qIndex)
{
    if (qIndex.isValid())
    {
        QVariant qID = FirsModel->record(qIndex.row()).value("id");
        if (!qID.isNull())
        {
            _UpdateSectors(qID.toUInt());
           // _UpdateFirs(qID.toUInt());
            AddSector->setEnabled(true);
            RemoveSector->setEnabled(true);
        }
        else
        {
            _UpdateSectors(0);
            //_UpdateFirs(0);
            AddSector->setEnabled(false);
            RemoveSector->setEnabled(false);
        }
    }
    else
    {
        _UpdateSectors(0);
       //_UpdateFirs(0);
        AddSector->setEnabled(false);
        RemoveSector->setEnabled(false);
    }

    if(tabWidget->currentIndex() == 2)
    {
        OnAirportChanged(airportView->currentIndex());
    }
}

void Sectoring::OnZoneChanged(const QModelIndex& qZoneIndex)
{
    //MainFIRCheck->blockSignals(true);
    //MainFIRCheck->setChecked(false);
    //MainFIRCheck->setEnabled(false);

    QModelIndex qSectoringIndex = SGroupView->selectionModel()->currentIndex();
    if (qSectoringIndex.isValid() && qZoneIndex.isValid())
    {
        // Only FIR
        //if ((Zone::Type) sectorsModel->record(qZoneIndex.row()).value("ztype").toInt() == Zone::enFIR)
        {
            bool bOk1(false);
            const uint uiSectoringID = sectorGroupModel->record(qSectoringIndex.row()).value("id").toUInt(&bOk1);
            const uint uiZoneID = sectorsModel->record(qZoneIndex.row()).value("id").toUInt();
            if (bOk1)
            {
                Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uiSectoringID);
                while (itFind != m_SectZoneList.end() && itFind.key() == uiSectoringID)
                {
                    if (itFind.value()->GetZoneID() == uiZoneID)
                    {
                        //MainFIRCheck->setEnabled(true);
                        //MainFIRCheck->setChecked(itFind.value()->GetMainFIR());
                        break;
                    }
                    else
                    {
                        ++itFind;
                    }
                }
            }
        }
    }
    //MainFIRCheck->blockSignals(false);
}

void Sectoring::OnAirportChanged(const QModelIndex &index)
{
    QModelIndex qSectoringIndex = SGroupView->selectionModel()->currentIndex();
    if (qSectoringIndex.isValid() && index.isValid())
    {
        bool bOk(false);
        const QString airportName = static_cast<QSqlTableModel*>(airportView->model())->record(index.row()).value("name").toString();
        const uint uiSectoringID = sectorGroupModel->record(qSectoringIndex.row()).value("id").toUInt(&bOk);

        cbAirportAllowed->blockSignals(true);
        if (mSectorAirports.value(uiSectoringID).contains(airportName))
            cbAirportAllowed->setChecked(true);
        else
            cbAirportAllowed->setChecked(false);
        cbAirportAllowed->blockSignals(false);
    }
}

void Sectoring::OnAirportAllowed(bool allowed)
{
    QModelIndex qSectoringIndex = SGroupView->selectionModel()->currentIndex();
    QModelIndex qAirportIndex = airportView->selectionModel()->currentIndex();
    if(!qSectoringIndex.isValid() || !qAirportIndex.isValid())
        return;

    bool bOk(false);
    const QString airportName = static_cast<QSqlTableModel*>(airportView->model())->record(qAirportIndex.row()).value("name").toString();
    const uint uiSectoringID = sectorGroupModel->record(qSectoringIndex.row()).value("id").toUInt(&bOk);

    if (allowed)
        mSectorAirports[uiSectoringID].append(airportName);
    else
        mSectorAirports[uiSectoringID].removeAll(airportName);
}

void Sectoring::OnAddSector()
{
    QModelIndex qIndex = SGroupView->selectionModel()->currentIndex();
    if (qIndex.isValid())
    {
        bool bOk(false);
        const uint uiSGID = sectorGroupModel->record(qIndex.row()).value("id").toUInt(&bOk);
        if (bOk)
        {
            LoadFromDB load_from_db(tr("Choose FIR or sector..."), true, this);
            if (load_from_db.exec())
            {
                // Check, if there is FIR for loaded sector
                if ((Zone::Type) load_from_db.GetResultType() == Zone::enSector)
                {
                    bOk = false;
                    Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uiSGID);
                    while (itFind != m_SectZoneList.end() && itFind.key() == uiSGID)
                    {
                        if (itFind.value()->GetZoneID() == load_from_db.GetParentID().toUInt())
                        {
                            bOk = true;
                            break;
                        }
                        ++itFind;
                    }
                }
                if (bOk)
                {
                    foreach (const QVariant &result, load_from_db.results()) {
                        bOk = false;
                        uint uiSectorId = result.toUInt(&bOk);
                        if (bOk) {
                            // Check already existed row
                            Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uiSGID);
                            while (true) {
                                if (itFind != m_SectZoneList.end() && itFind.key() == uiSGID) {
                                    if (itFind.value()->GetZoneID() == uiSectorId) {
                                        QMessageBox::warning(this, tr("Warning"), "FIR/Sector has already been added");
                                        break;
                                    }
                                } else {
                                    QSharedPointer<CSectorZone> pSZ(new CSectorZone());
                                    pSZ->SetSectoringID(uiSGID);
                                    pSZ->SetZoneID(uiSectorId);
                                    m_SectZoneList.insert(uiSGID, pSZ);
                                    dao()->SaveSectorZoneAll(m_SectZoneList);///////
                                    _UpdateSectors(uiSGID);
                                    break;
                                }
                                ++itFind;
                            }
                        }
                    }
                }
                else
                    QMessageBox::warning(this, tr("Warning"), "At first FIR should be loaded");
            }
        }
    }
}

void Sectoring::OnAddFir()
{
    QModelIndex qIndex = SGroupView->selectionModel()->currentIndex();
    if (qIndex.isValid())
    {
        bool bOk(false);
        const uint uiSGID = sectorGroupModel->record(qIndex.row()).value("id").toUInt(&bOk);
        if (bOk)
        {
            LoadFromDB load_from_db(tr("Choose FIR or sector..."), true, this);
            if (load_from_db.exec())
            {
                // Check, if there is FIR for loaded sector
                if ((Zone::Type) load_from_db.GetResultType() == Zone::enSector)
                {
                    bOk = false;
                    Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uiSGID);
                    while (itFind != m_SectZoneList.end() && itFind.key() == uiSGID)
                    {
                        if (itFind.value()->GetZoneID() == load_from_db.GetParentID().toUInt())
                        {
                            bOk = true;
                            break;
                        }
                        ++itFind;
                    }
                }
                if (bOk)
                {
                    foreach (const QVariant &result, load_from_db.results()) {
                        bOk = false;
                        uint uiSectorId = result.toUInt(&bOk);
                        if (bOk) {
                            // Check already existed row
                            Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uiSGID);
                            while (true) {
                                if (itFind != m_SectZoneList.end() && itFind.key() == uiSGID) {
                                    if (itFind.value()->GetZoneID() == uiSectorId) {
                                        QMessageBox::warning(this, tr("Warning"), "FIR/Sector has already been added");
                                        break;
                                    }
                                } else {
                                    QSharedPointer<CSectorZone> pSZ(new CSectorZone());
                                    pSZ->SetSectoringID(uiSGID);
                                    pSZ->SetZoneID(uiSectorId);
                                    m_SectZoneList.insert(uiSGID, pSZ);
                                    dao()->SaveSectorZoneAll(m_SectZoneList);///////
                                     _UpdateFirs(uiSGID);
                                    break;
                                }
                                ++itFind;
                            }
                        }
                    }
                }
                else
                    QMessageBox::warning(this, tr("Warning"), "At first FIR should be loaded");
            }
        }
    }
}

void Sectoring::OnRemoveSector()
{
    QModelIndex qSectInd = SGroupView->selectionModel()->currentIndex();
    QModelIndex qZoneInd = SectorView->selectionModel()->currentIndex();
    if (qSectInd.isValid() && qZoneInd.isValid())
    {
        bool bOk1(false), bOk2(false);
        uint uSectID = sectorGroupModel->record(qSectInd.row()).value("id").toUInt(&bOk1);
        uint uZoneID = sectorsModel->record(qZoneInd.row()).value("id").toUInt(&bOk2);
        if (bOk1 && bOk2)
        {
            // Make list from id, which should be deleted
            QSet<uint> DeleteList;
            DeleteList.insert(uZoneID);
            if ((Zone::Type) sectorsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enFIR)
            {
                int iRow(0);
                for (; iRow < sectorsModel->rowCount(); ++iRow)
                {
                    if (sectorsModel->record(iRow).value("parent_id").toUInt() == uZoneID)
                        DeleteList.insert(sectorsModel->record(iRow).value("id").toUInt());
                }
            }
            // Delete records
            Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uSectID);
            while (itFind != m_SectZoneList.end() && itFind.key() == uSectID)
            {
                if (DeleteList.find(itFind.value()->GetZoneID()) != DeleteList.end())
                {
                    itFind = m_SectZoneList.erase(itFind);
                }
                else
                {
                    ++itFind;
                }
            }
            _UpdateSectors(uSectID);
        }
    }
}

void Sectoring::OnRemoveFir()
{
    QModelIndex qSectInd = SGroupView->selectionModel()->currentIndex();
    QModelIndex qZoneInd = FirView->selectionModel()->currentIndex();
    if (qSectInd.isValid() && qZoneInd.isValid())
    {
        bool bOk1(false), bOk2(false);
        uint uSectID = sectorGroupModel->record(qSectInd.row()).value("id").toUInt(&bOk1);
        uint uZoneID = FirsModel->record(qZoneInd.row()).value("id").toUInt(&bOk2);
        if (bOk1 && bOk2)
        {
            // Make list from id, which should be deleted
            QSet<uint> DeleteList;
            DeleteList.insert(uZoneID);
            if ((Zone::Type) FirsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enFIR)
            {
                int iRow(0);
                for (; iRow < FirsModel->rowCount(); ++iRow)
                {
                    if (FirsModel->record(iRow).value("parent_id").toUInt() == uZoneID)
                        DeleteList.insert(FirsModel->record(iRow).value("id").toUInt());
                }
            }
            // Delete records
            Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uSectID);
            while (itFind != m_SectZoneList.end() && itFind.key() == uSectID)
            {
                if (DeleteList.find(itFind.value()->GetZoneID()) != DeleteList.end())
                {
                    itFind = m_SectZoneList.erase(itFind);
                }
                else
                {
                    ++itFind;
                }
            }
            _UpdateFirs(uSectID);
        }
    }
}


void Sectoring::OnTypeSelect()
{
    AllowCheck->blockSignals(true);
    QModelIndex qTypeIndex = TypeView->selectionModel()->currentIndex();
    QModelIndex qPrivIndex;
    if(!PrivilegeView->selectionModel()->selectedRows(0).isEmpty())
        qPrivIndex = PrivilegeView->selectionModel()->selectedRows(0).first();
    if (qTypeIndex.isValid() && qPrivIndex.isValid())
    {
        CRoleItem* pRole = dynamic_cast<CRoleItem*> (m_pModel->itemFromIndex(qPrivIndex));
        if (pRole)
        {
            AllowCheck->setEnabled(true);
            QString sPermission = sectorTypeModel->record(qTypeIndex.row()).value("permission").toString();
            AllowCheck->setChecked(CUserRole::IsPermission(sPermission, pRole->GetUniqueName()));
        }
        else
        {
            AllowCheck->setChecked(false);
            AllowCheck->setEnabled(false);
        }
    }
    else
    {
        AllowCheck->setChecked(false);
        AllowCheck->setEnabled(false);
    }
    AllowCheck->blockSignals(false);
}

void Sectoring::OnAddType()
{
    QString sNewName = CSqlModelHelper::GetUniqueName(*sectorTypeModel, "name", "New type");
    if (!sNewName.isEmpty())
    {
        int iRow = sectorTypeModel->rowCount();
        sectorTypeModel->insertRow(iRow);
        QSqlRecord qRec = sectorTypeModel->record(iRow);
        qRec.setValue("name", sNewName);
        qRec.setValue("app", false);
        sectorTypeModel->setRecord(iRow, qRec);
        QModelIndex qmIndex = sectorTypeModel->index(iRow, sectorTypeModel->fieldIndex("name"));
        TypeView->setCurrentIndex(qmIndex);
        TypeView->edit(qmIndex);
    }
}

void Sectoring::OnRemoveType()
{
    QModelIndexList IndexList = TypeView->selectionModel()->selectedRows();
    QModelIndexList::iterator itIndex = IndexList.begin();
    for (; itIndex != IndexList.end(); ++itIndex)
    {
        if (itIndex->isValid())
        {
            sectorTypeModel->removeRow(itIndex->row());
        }
    }
}

void Sectoring::OnAllow(int iState)
{
    AllowCheck->blockSignals(true);
    QModelIndex qTypeIndex = TypeView->selectionModel()->currentIndex();
    QModelIndex qPrivIndex;
    if(!PrivilegeView->selectionModel()->selectedRows(0).isEmpty())
        qPrivIndex = PrivilegeView->selectionModel()->selectedRows(0).first();
    if (qTypeIndex.isValid() && qPrivIndex.isValid())
    {
        CRoleItem* pRole = dynamic_cast<CRoleItem*> (m_pModel->itemFromIndex(qPrivIndex));
        if (pRole)
        {
            QSqlRecord qRecord = sectorTypeModel->record(qTypeIndex.row());
            QString sPermission = qRecord.value("permission").toString();
            if (iState == Qt::Checked)
            {
                CUserRole::AddPermission(sPermission, pRole->GetUniqueName());
            }
            else if (iState == Qt::Unchecked)
            {
                CUserRole::DelPermission(sPermission, pRole->GetUniqueName());
            }
            qRecord.setValue("permission", sPermission);
            sectorTypeModel->setRecord(qTypeIndex.row(), qRecord);
        }
        else
        {
            AllowCheck->setChecked(false);
            AllowCheck->setEnabled(false);
        }
    }
    else
    {
        AllowCheck->setChecked(false);
        AllowCheck->setEnabled(false);
    }
    AllowCheck->blockSignals(false);
}

/*void Sectoring::OnMainFIR(int iState)
{
    //MainFIRCheck->blockSignals(true);
    if (iState == Qt::Unchecked)
        //MainFIRCheck->setCheckState(Qt::Checked);
    else
    {
        QModelIndex qSectInd = SGroupView->selectionModel()->currentIndex();
        QModelIndex qZoneInd = SectorView->selectionModel()->currentIndex();
        if (qSectInd.isValid() && qZoneInd.isValid())
        {
            bool bOk1(false), bOk2(false);
            uint uSectID = sectorGroupModel->record(qSectInd.row()).value("id").toUInt(&bOk1);
            uint uZoneID = sectorsModel->record(qZoneInd.row()).value("id").toUInt(&bOk2);
            if (bOk1 && bOk2)
            {
                // Reset all main FIR
//                Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uSectID);
//                while (itFind != m_SectZoneList.end() && itFind.key() == uSectID)
//                {
//                    itFind.value()->SetMainFIR(false);
//                    ++itFind;
//                }
                // FIR
                if ((Zone::Type) sectorsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enFIR)
                {
                    _SetMainFIR(uSectID, uZoneID);
                }
                // Sector
                if ((Zone::Type) sectorsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enSector)
                {
                    _SetMainSector(uSectID, uZoneID, sectorsModel->record(qZoneInd.row()).value("parent_id").toInt());
                }
            }
        }
    }
    //MainFIRCheck->blockSignals(false);
}*/

void Sectoring::onTabChanged(int index)
{
    if(index == 0)
    {
        splitter->insertWidget(0, Sectors);
    }
    else if(index == 2)
    {
        splitter_3->insertWidget(0, Sectors);
//        Sectors->show();
//        Settings->hide();
//        Airports->show();
    }
}

void Sectoring::_UpdateSectors(uint uiSectID)
{
   QString
                        queryString(
                            "SELECT zn.id id, zn.parent_id parent_id, zone_type.id ztype, zn.id_name id_name, zn.zname zname, zone_type.name type_name, zn.parent_name parent_name, sz.main_fir "
                                "FROM ("
                                "SELECT a1.id id, a1.id_name id_name, a1.name zname, a1.type ztype, a1.parent_id parent_id, a2.name parent_name "
                                "FROM zone a1, zone a2 "
                                "WHERE a1.parent_id IS NOT NULL AND  a1.parent_id = a2.id"
                                ") zn, zone_type, sectoring_zone sz "
                                "WHERE zn.ztype = zone_type.id AND zn.id IN (0");


    QList<QSharedPointer<CSectorZone> > SectList = m_SectZoneList.values(uiSectID);
    QList<QSharedPointer<CSectorZone> >::iterator itSect = SectList.begin();
    for (; itSect != SectList.end(); ++itSect)
    {
        queryString += ", " + QString::number((*itSect)->GetZoneID());

    }

    queryString.append(")");
    queryString += " AND sz.zone_id = zn.id AND sz.sect_id = ";
    queryString += QString::number(uiSectID);
    sectorsModel->setQuery(queryString);
    if (sectorsModel->lastError().isValid()) {
        qDebug() << sectorsModel->lastError().text();
    }

}

void Sectoring::_UpdateFirs(uint uiSectID)
{
    QString
                        queryString(
                            "SELECT zn.id id, zn.parent_id parent_id, zone_type.id ztype, zn.id_name id_name, zn.zname zname, zone_type.name type_name, zn.parent_name parent_name, sz.main_fir "
                                "FROM ("
                                "SELECT a1.id id, a1.id_name id_name, a1.name zname, a1.type ztype, a1.parent_id parent_id, a2.name parent_name "
                                "FROM zone a1, zone a2 "
                                "WHERE a1.parent_id IS NULL AND a1.id = a2.id "
                                ") zn, zone_type , sectoring_zone sz "
                                "WHERE zn.ztype = zone_type.id AND zn.id IN (0");

    QList<QSharedPointer<CSectorZone> > SectList = m_SectZoneList.values(uiSectID);
    QList<QSharedPointer<CSectorZone> >::iterator itSect = SectList.begin();
    for (; itSect != SectList.end(); ++itSect)
    {
        queryString += ", " + QString::number((*itSect)->GetZoneID());

    }

    queryString.append(")");
    queryString += " AND sz.zone_id = zn.id AND sz.sect_id = ";
    queryString += QString::number(uiSectID);
    FirsModel->setQuery(queryString);
    if (FirsModel->lastError().isValid()) {
        qDebug() << FirsModel->lastError().text();
    }
}

/*void Sectoring::_UpdateBearing(uint uiSectID)
{
    QString
                        queryString(
                            "SELECT sb.id,s.FROM sectoring_bearing sb
                                "SELECT a1.id id, a1.id_name id_name, a1.name zname, a1.type ztype, a1.parent_id parent_id, a2.name parent_name "
                                "FROM zone a1, zone a2 "
                                "WHERE a1.parent_id IS NULL AND a1.id = a2.id "
                                ") zn, zone_type , sectoring_zone sz "
                                "WHERE zn.ztype = zone_type.id AND zn.id IN (0");

    QList<QSharedPointer<CSectorZone> > SectList = m_SectZoneList.values(uiSectID);
    QList<QSharedPointer<CSectorZone> >::iterator itSect = SectList.begin();
    for (; itSect != SectList.end(); ++itSect)
    {
        queryString += ", " + QString::number((*itSect)->GetZoneID());

    }

    queryString.append(")");
    queryString += " AND sz.zone_id = zn.id AND sz.sect_id = ";
    queryString += QString::number(uiSectID);
    FirsModel->setQuery(queryString);
    if (FirsModel->lastError().isValid()) {
        qDebug() << FirsModel->lastError().text();
    }
}*/

void Sectoring::_SetMainFIR(uint uiSectID, uint uiZoneID)
{
    int iRow(0);
    for (; iRow < FirsModel->rowCount(); ++iRow)
    {
        //if ((Zone::Type) sectorsModel->record(iRow).value("ztype").toInt() == Zone::enFIR)
        //{
            uint idFIR = FirsModel->record(iRow).value("id").toInt();
            bool bCheck = false;
            if(idFIR == uiZoneID)
                bCheck = true;
            Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uiSectID);
            while (itFind != m_SectZoneList.end() && itFind.key() == uiSectID)
            {
                if (itFind.value()->GetZoneID() == idFIR)
                {
                    itFind.value()->SetMainFIR(bCheck);
                    break;
                }
                ++itFind;
            }
        //}
    }
}

void Sectoring::_SetMainSector(uint uiSectID, uint uiZoneID, uint uiParentZoneID)
{
    int iRow(0);
    for (; iRow < sectorsModel->rowCount(); ++iRow)
    {
        if ((Zone::Type) sectorsModel->record(iRow).value("ztype").toInt() == Zone::enSector
            && sectorsModel->record(iRow).value("parent_id").toUInt() == uiParentZoneID)
        {
            uint idSector = sectorsModel->record(iRow).value("id").toInt();
            bool bCheck = false;
            if(idSector == uiZoneID)
                bCheck = true;
            Dao::TSectorZoneList::iterator itFind = m_SectZoneList.find(uiSectID);
            while (itFind != m_SectZoneList.end() && itFind.key() == uiSectID)
            {
                if (itFind.value()->GetZoneID() == idSector)
                {
                    itFind.value()->SetMainFIR(bCheck);
                    break;
                }
                ++itFind;
            }
        }
    }
}

void Sectoring::changeEvent(QEvent* event)
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

void Sectoring::loadSettings()
{
    restoreGeometry(globalSettings()->value("XMasterWindow/Settings/SectoringGeometry").toByteArray());
}

void Sectoring::saveSettings()
{
    globalSettings()->setValue("XMasterWindow/Settings/SectoringGeometry", saveGeometry());
}

void Sectoring::onFirViewClicked(const QModelIndex& qIndex)
{
    if (qIndex.isValid())
    {
        QVariant qID = FirsModel->record(qIndex.row()).value("id");
        if (!qID.isNull())
        {
            QModelIndex qSectInd = SGroupView->selectionModel()->currentIndex();
            //QModelIndex qZoneInd = SectorView->selectionModel()->currentIndex();
            QModelIndex qZoneInd = qIndex;
            if (qSectInd.isValid() && qZoneInd.isValid())
            {
                bool bOk1(false), bOk2(false);
                uint uSectID = sectorGroupModel->record(qSectInd.row()).value("id").toUInt(&bOk1);
                uint uZoneID = FirsModel->record(qZoneInd.row()).value("id").toUInt(&bOk2);
                if (bOk1 && bOk2)
                     _SetMainFIR(uSectID, uZoneID);
                dao()->SaveSectorZoneAll(m_SectZoneList);
                _UpdateFirs(uSectID);
                /*{
                    //Fir
                    if ((Zone::Type) sectorsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enFIR)
                    {
                        _SetMainFIR(uSectID, uZoneID);
                    }
                    // Sector
                    if ((Zone::Type) sectorsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enSector)
                    {
                        _SetMainSector(uSectID, uZoneID, sectorsModel->record(qZoneInd.row()).value("parent_id").toInt());
                    }
                }*/
            }
        }
    }
}

void Sectoring::onSectorViewClicked(const QModelIndex& qIndex)
{
    if (qIndex.isValid())
    {
        QVariant qID = sectorsModel->record(qIndex.row()).value("id");
        if (!qID.isNull())
        {
            QModelIndex qSectInd = SGroupView->selectionModel()->currentIndex();
            //QModelIndex qZoneInd = SectorView->selectionModel()->currentIndex();
            QModelIndex qZoneInd = qIndex;
            if (qSectInd.isValid() && qZoneInd.isValid())
            {
                bool bOk1(false), bOk2(false);
                uint uSectID = sectorGroupModel->record(qSectInd.row()).value("id").toUInt(&bOk1);
                uint uZoneID = sectorsModel->record(qZoneInd.row()).value("id").toUInt(&bOk2);
                if (bOk1 && bOk2)
                     _SetMainSector(uSectID, uZoneID, sectorsModel->record(qZoneInd.row()).value("parent_id").toInt());
                dao()->SaveSectorZoneAll(m_SectZoneList);
                _UpdateSectors(uSectID);
                /*{
                    //Fir
                    if ((Zone::Type) sectorsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enFIR)
                    {
                        _SetMainFIR(uSectID, uZoneID);
                    }
                    // Sector
                    if ((Zone::Type) sectorsModel->record(qZoneInd.row()).value("ztype").toInt() == Zone::enSector)
                    {
                        _SetMainSector(uSectID, uZoneID, sectorsModel->record(qZoneInd.row()).value("parent_id").toInt());
                    }
                }*/
            }
        }
    }
}

void Sectoring::onBearingChanged(int index)
{
    //QModelIndex ind = sectorGroupModel->index(SGroupView->currentIndex().row(),
    //                                          sectorGroupModel->fieldIndex("bearing_point_id"));

    //sectorGroupModel->setData(ind, bearing->itemData(index));
    //QModelIndex qSectInd = SGroupView->selectionModel()->currentIndex();
    //if (qSectInd.isValid()){
    //    uint uSectID = sectorGroupModel->record(qSectInd.row()).value("id").toUInt();
}

void Sectoring::onCheckedItemsChanged(QStringList lst)
{
       for (int i = 0; i < bearing->count(); ++i){
            int bID = bearing->itemData(i).toInt();
            bool absent(true);
            bool check(false);
            check = (bearing->itemCheckState(i) == Qt::Checked);

            if(!check){
                for (int iRow = 0; iRow < sectorBearingModel->rowCount(); iRow++){
                    if (sectorBearingModel->record(iRow).value("sect_id").toInt() == getSectorId() &&
                       (sectorBearingModel->record(iRow).value("bearing_id").toInt() == bID)){
                               sectorBearingModel->removeRow(iRow);
                    }
                }
            }
            if(check){
                for (int iRow = 0; iRow < sectorBearingModel->rowCount(); iRow++){
                    if (sectorBearingModel->record(iRow).value("sect_id").toInt() == getSectorId() &&
                       (sectorBearingModel->record(iRow).value("bearing_id").toInt() == bID)){
                               absent = false;
                               break;
                    }
                }
                if (absent){
                    int iRow = sectorBearingModel->rowCount();
                    sectorBearingModel->insertRow(iRow);
                    sectorBearingModel->setData(sectorBearingModel->index(iRow, sectorBearingModel->fieldIndex("bearing_id")), bID);
                    sectorBearingModel->setData(sectorBearingModel->index(iRow, sectorBearingModel->fieldIndex("sect_id")), getSectorId());
                }
            }
        }
}


uint Sectoring::getSectorId()
{
    QModelIndex qSectInd = SGroupView->selectionModel()->currentIndex();
        uint uSectID = sectorGroupModel->record(qSectInd.row()).value("id").toUInt();
        return uSectID;
}

CSectoring::CSectoring() :
    m_uiID(0)
{

}

CSectoring::~CSectoring()
{

}

void CSectoring::SetID(uint uiID)
{
    m_uiID = uiID;
}

uint CSectoring::GetID() const
{
    return m_uiID;
}

void CSectoring::SetName(const QString& sName)
{
    m_sName = sName;
}

const QString& CSectoring::GetName() const
{
    return m_sName;
}

CSectorZone::CSectorZone() :
    m_uiID(0), m_uiSectoringID(0), m_uiZoneID(0), m_bMainFIR(false)
{

}

CSectorZone::~CSectorZone()
{

}

void CSectorZone::SetID(uint uiID)
{
    m_uiID = uiID;
}

uint CSectorZone::GetID() const
{
    return m_uiID;
}

void CSectorZone::SetSectoringID(uint uiID)
{
    m_uiSectoringID = uiID;
}

uint CSectorZone::GetSectoringID() const
{
    return m_uiSectoringID;
}

void CSectorZone::SetZoneID(uint uiID)
{
    m_uiZoneID = uiID;
}

uint CSectorZone::GetZoneID() const
{
    return m_uiZoneID;
}

void CSectorZone::SetMainFIR(bool bMain)
{
    m_bMainFIR = bMain;
}

bool CSectorZone::GetMainFIR() const
{
    return m_bMainFIR;
}









