#include "holdingeditor.h"

#include "BoundaryPoint.h"
#include "dbscheme.h"
#include "Holding.h"
#include "OldMasterSupport.h"
#include "widgetsaver.h"

#include "Database.h"
#include "qtautotransaction.h"

#include <QtCore/QDebug>

#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>

#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlRelationalTableModel>

HoldingEditor::HoldingEditor(QWidget* parent, const QSqlDatabase &db) :
        BaseEditor(parent), mDatabase(db), zoneId(0)
{
    setupUi(this);

    QPushButton* saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addHolding()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteHolding()));

    holdingTableModel = new QSqlRelationalTableModel(this, mDatabase);
    holdingTableModel->setTable(dbscheme::Holding::tableName);
    holdingTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    holdingIndex.insert(dbscheme::Holding::id, holdingTableModel->fieldIndex(dbscheme::Holding::id));
    holdingIndex.insert(dbscheme::Holding::pointid, holdingTableModel->fieldIndex(dbscheme::Holding::pointid));
    holdingIndex.insert(dbscheme::Holding::type, holdingTableModel->fieldIndex(dbscheme::Holding::type));
    holdingIndex.insert(dbscheme::Holding::incouese, holdingTableModel->fieldIndex(dbscheme::Holding::incouese));
    holdingIndex.insert(dbscheme::Holding::turn, holdingTableModel->fieldIndex(dbscheme::Holding::turn));
    holdingIndex.insert(dbscheme::Holding::leglength, holdingTableModel->fieldIndex(dbscheme::Holding::leglength));
    holdingIndex.insert(dbscheme::Holding::legtime, holdingTableModel->fieldIndex(dbscheme::Holding::legtime));
    holdingIndex.insert(dbscheme::Holding::speed, holdingTableModel->fieldIndex(dbscheme::Holding::speed));
    holdingIndex.insert(dbscheme::Holding::zoneid, holdingTableModel->fieldIndex(dbscheme::Holding::zoneid));
    holdingIndex.insert(dbscheme::Holding::h_min, holdingTableModel->fieldIndex(dbscheme::Holding::h_min));
    holdingIndex.insert(dbscheme::Holding::h_max, holdingTableModel->fieldIndex(dbscheme::Holding::h_max));
    holdingTableModel->setRelation(holdingIndex[dbscheme::Holding::pointid], QSqlRelation(dbscheme::Point::tableName, dbscheme::Point::id, dbscheme::Point::name));
    //holdingTableModel->setRelation(holdingIndex[dbscheme::Holding::type], QSqlRelation(dbscheme::HoldingType::tableName, dbscheme::HoldingType::id, dbscheme::HoldingType::name));
    holdingTableModel->setRelation(holdingIndex[dbscheme::Holding::turn], QSqlRelation(dbscheme::HoldingTurn::tableName, dbscheme::HoldingTurn::id, dbscheme::HoldingTurn::name));
    holdingTableModel->setRelation(holdingIndex[dbscheme::Holding::zoneid], QSqlRelation(dbscheme::Zone::tableName, dbscheme::Zone::id, dbscheme::Zone::name));
    selectAll(holdingTableModel);

    QSqlTableModel* pRelationModel = holdingTableModel->relationModel(holdingIndex[dbscheme::Holding::pointid]);
    if(pRelationModel)
    {
        selectAll(pRelationModel);
        holdingTableModel->data(holdingTableModel->index(0, holdingIndex[dbscheme::Holding::pointid]));
    }
    pRelationModel = holdingTableModel->relationModel(holdingIndex[dbscheme::Holding::zoneid]);
    if(pRelationModel)
    {
        selectAll(pRelationModel);
        holdingTableModel->data(holdingTableModel->index(0, holdingIndex[dbscheme::Holding::zoneid]));
    }

    holdingView->setModel(holdingTableModel);

    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::id]), 0);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::pointid]), 1);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::type]), 2);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::incouese]), 3);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::turn]), 4);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::leglength]), 5);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::legtime]), 6);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::speed]), 7);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::zoneid]), 8);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::h_min]), 9);
    holdingView->horizontalHeader()->moveSection(holdingView->horizontalHeader()->visualIndex(holdingIndex[dbscheme::Holding::h_max]), 10);
    holdingView->setColumnHidden(holdingIndex[dbscheme::Holding::id], true);
    holdingView->setColumnHidden(holdingIndex[dbscheme::Holding::zoneid], true);
    holdingView->setColumnHidden(holdingIndex[dbscheme::Holding::type], true);
    holdingView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    holdingView->resizeColumnsToContents();

    holdingDelegate = new RelModifyDelegate(this);
    holdingView->setItemDelegate(holdingDelegate);

    connect(holdingTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(enableOperButtons()));

    RelModifyDelegate::TColMap colMap;
    colMap.insert(holdingIndex[dbscheme::Holding::id], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::pointid], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::type], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::incouese], QVariant::Double);
    colMap.insert(holdingIndex[dbscheme::Holding::turn], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::leglength], QVariant::Double);
    colMap.insert(holdingIndex[dbscheme::Holding::legtime], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::speed], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::zoneid], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::h_min], QVariant::UInt);
    colMap.insert(holdingIndex[dbscheme::Holding::h_max], QVariant::UInt);
    holdingDelegate->SetColsType(colMap);
    holdingTableModel->setFilter("0 = 1");

    connect(holdingView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , holdingView->horizontalHeader(), SLOT(resizeSections()));

    holdingView->installEventFilter(this);

    translate();

    verifyHoldingPointsForOldMaster();
}

HoldingEditor::~HoldingEditor()
{
    SaveState(this);
}

void HoldingEditor::translate()
{
    retranslateUi(this);
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::pointid], Qt::Horizontal, dbscheme::Holding::tr_pointid());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::type], Qt::Horizontal, dbscheme::Holding::tr_type());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::incouese], Qt::Horizontal, dbscheme::Holding::tr_incouese());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::turn], Qt::Horizontal, dbscheme::Holding::tr_turn());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::leglength], Qt::Horizontal, dbscheme::Holding::tr_leglength());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::legtime], Qt::Horizontal, dbscheme::Holding::tr_legtime());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::speed], Qt::Horizontal, dbscheme::Holding::tr_speed());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::zoneid], Qt::Horizontal, dbscheme::Holding::tr_zoneid());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::h_min], Qt::Horizontal, dbscheme::Holding::tr_h_min());
    holdingTableModel->setHeaderData(holdingIndex[dbscheme::Holding::h_max], Qt::Horizontal, dbscheme::Holding::tr_h_max());
}

void HoldingEditor::refresh()
{
    holdingTableModel->revertAll();
    selectAll(holdingTableModel);
    operButtons->setEnabled(false);
}

void HoldingEditor::updateFirName()
{
    firLabel->setText(tr("Current FIR: %1").arg(firName()));
}

void HoldingEditor::setFirId()
{
    addButton->setEnabled(isFirValid());
    deleteButton->setEnabled(isFirValid());

    if(isFirValid())
        zoneId = firId();
    else
        zoneId = 0;
    QSqlTableModel* pRelationModel = holdingTableModel->relationModel(holdingIndex[dbscheme::Holding::pointid]);
    if(pRelationModel)
    {
        pRelationModel->setFilter(
                QString("%1 IN(1, 2, 3) AND %2 IN(SELECT %3 FROM %4 WHERE %5 = %6)")
                .arg(dbscheme::Point::type, dbscheme::Point::id, dbscheme::ZonePoint::pointId
                     , dbscheme::ZonePoint::tableName, dbscheme::ZonePoint::zoneId)
                .arg(zoneId)); // filter entry points by FIR and select only VHF, NDB, Waypoint
        pRelationModel->setSort(2, Qt::AscendingOrder);
        selectAll(pRelationModel);
    }
    holdingTableModel->setFilter(QString("%1 = %2").arg(dbscheme::Holding::zoneid).arg(zoneId));
    selectAll(holdingTableModel);
}

void HoldingEditor::closeEvent(QCloseEvent *pEvent)
{
    if (operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?")
                                     , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            {
                QMap<int, int> changesIDmap;
                for(int i = 0; i < holdingTableModel->rowCount(); ++i)
                {
                    for(int j = 0; j < holdingTableModel->columnCount(); ++j)
                    {
                        int id = holdingTableModel->data(
                                    holdingTableModel->index(i, holdingIndex[dbscheme::Holding::id])).toInt();
                        if(holdingTableModel->isDirty(holdingTableModel->index(i, j)))
                        {
                            if(id)
                            {
                                changesIDmap.insert(id, id);
                            }
                            else
                            {
                                // new record
                            }
                            break;
                        }
                    }
                }
                QtAutoTransaction dbTrans;
                if (!holdingTableModel->submitAll())
                {
                    qCritical() << holdingTableModel->lastError().text();
                    QMessageBox::critical(this, tr("Error"), holdingTableModel->lastError().text());
                    pEvent->ignore();
                }
                else
                {
                    dbTrans.commit();
                    pEvent->accept();

                    updateHoldingPointsForOldMaster(changesIDmap, zoneId);
                    verifyHoldingPointsForOldMaster();
                }
            }
            break;
        case QMessageBox::No:
            pEvent->accept();
            break;
        case QMessageBox::Cancel:
            pEvent->ignore();
            break;
        default:
            break;
        }

    }
    else
        pEvent->accept();
}

void HoldingEditor::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch(event->type())
    {
    case QEvent::LanguageChange:
        translate();
        updateFirName();
        break;
    default:
        break;
    }
}

void HoldingEditor::addHolding()
{
    if (zoneId)
    {
        int iRow = holdingTableModel->rowCount();
        holdingTableModel->insertRow(iRow);
        holdingTableModel->setData(holdingTableModel->index(iRow, holdingIndex[dbscheme::Holding::zoneid]), zoneId);
        holdingTableModel->setData(holdingTableModel->index(iRow, holdingIndex[dbscheme::Holding::type]), 48);
        QModelIndex qmIndex = holdingTableModel->index(iRow, holdingIndex[dbscheme::Holding::pointid]);
        holdingView->setCurrentIndex(qmIndex);
        holdingView->edit(qmIndex);
        operButtons->setEnabled(true);
    }
}

void HoldingEditor::deleteHolding()
{
    QModelIndexList selectList = holdingView->selectionModel()->selectedRows();
    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == holdingTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        holdingTableModel->removeRow(rowForDelte);
        operButtons->setEnabled(true);
    }
}

void HoldingEditor::onOperButtons(QAbstractButton* pButton)
{
    QDialogButtonBox::ButtonRole BtnRole = operButtons->buttonRole(pButton);
    switch(BtnRole)
    {
    case QDialogButtonBox::AcceptRole:
        {
            QVariant holdingSelectedRow = holdingTableModel->data(
                        holdingTableModel->index(
                            holdingView->selectionModel()->currentIndex().row()
                            , holdingIndex[dbscheme::Holding::pointid]));
            try
            {
                QMap<int, int> changesIDmap;
                for(int i = 0; i < holdingTableModel->rowCount(); ++i)
                {
                    for(int j = 0; j < holdingTableModel->columnCount(); ++j)
                    {
                        int id = holdingTableModel->data(
                                    holdingTableModel->index(i, holdingIndex[dbscheme::Holding::id])).toInt();
                        if(holdingTableModel->isDirty(holdingTableModel->index(i, j)))
                        {
                            if(id)
                            {
                                changesIDmap.insert(id, id);
                            }
                            else
                            {
                                // new record
                            }
                            break;
                        }
                    }
                }
                QtAutoTransaction dbTrans;
                if (!holdingTableModel->submitAll())
                    throw (holdingTableModel->lastError().text());
                dbTrans.commit();
                operButtons->setEnabled(false);
                while(holdingTableModel->canFetchMore())
                    holdingTableModel->fetchMore();
                QModelIndexList holdingIndexList = holdingTableModel->match(
                            holdingTableModel->index(0, holdingIndex[dbscheme::Holding::pointid])
                            , Qt::DisplayRole, holdingSelectedRow, 1, Qt::MatchExactly);
                if(holdingIndexList.count())
                {
                    holdingView->selectRow(holdingIndexList.first().row());
                    holdingView->scrollTo(holdingIndexList.first());
                }
                updateHoldingPointsForOldMaster(changesIDmap, zoneId);
                verifyHoldingPointsForOldMaster();
            }
            catch (const QString &err)
            {
                qCritical() << err;
                QMessageBox::critical(this, tr("Error"), err);
            }
        }
        break;
    case QDialogButtonBox::RejectRole:
    case QDialogButtonBox::DestructiveRole:
        holdingTableModel->revertAll();
        selectAll(holdingTableModel);
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }
}

void HoldingEditor::enableOperButtons()
{
    operButtons->setEnabled(true);
}

void HoldingEditor::verifyHoldingPointsForOldMaster()
{
    QMap<int, QMap<int, int> > idMap;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDatabase);
    query->Prepare("SELECT id, zone_id FROM holding WHERE id NOT IN "
                   "(SELECT holding_id FROM holding_point_master)");
    if(query->exec())
    {
        while(query->next())
        {
            idMap[query->value(1).toInt()].insert(query->value(0).toInt(), query->value(0).toInt());
        }
    }
    else
    {
        qWarning() << query->lastError().text();
    }
    for(QMap<int, QMap<int, int> >::const_iterator itMap = idMap.constBegin(); itMap != idMap.constEnd(); ++itMap)
    {
        updateHoldingPointsForOldMaster(itMap.value(), itMap.key());
    }
}

void HoldingEditor::updateHoldingPointsForOldMaster(const QMap<int, int>& changesIDmap, int idFir)
{
    GeoPoint centerPoint;
    if(getFirCenterPoint(idFir, centerPoint))
        updateHoldingPointsForOldMaster(changesIDmap, centerPoint);
}

void HoldingEditor::updateHoldingPointsForOldMaster(const QMap<int, int>& changesIDmap
                                                    , const GeoPoint &centerPoint)
{
    for(QMap<int, int>::const_iterator itMap = changesIDmap.constBegin(); itMap != changesIDmap.constEnd(); ++itMap)
    {
        QtAutoTransaction trans(mDatabase);
        bool bRes = trans.isTransactionStarted();
        if(bRes)
        {
            bRes = deleteHoldingPointsForOldMaster(*itMap);
            if(bRes)
            {
                Holding holding;
                bRes = loadHolding(*itMap, holding);
                if(bRes)
                {
                    GeoPoint point;
                    bRes = getPointCoord(holding.pointId.toInt(), point);
                    if(bRes)
                    {
                        QList<oldmaster::OldMasterPoint> points = oldmaster::createHoldingPointsForOldMaster(point, centerPoint, holding);
                        QSharedPointer<db::CDBQuery> insertHoldingPoint = db::CreateQuery(mDatabase);
                        insertHoldingPoint->Prepare("INSERT INTO holding_point_master(holding_id, latitude, longitude, sn) VALUES(:holding_id, :latitude, :longitude, :sn)");
                        foreach (const oldmaster::OldMasterPoint &p, points)
                        {
                            insertHoldingPoint->BindValue(":holding_id", *itMap);
                            insertHoldingPoint->BindValue(":latitude", p.coord.latitude());
                            insertHoldingPoint->BindValue(":longitude", p.coord.longitude());
                            insertHoldingPoint->BindValue(":sn", p.sn);
                            if (!insertHoldingPoint->exec())
                            {
                                qWarning() << insertHoldingPoint->lastError().text() << holding.toString();
                                bRes = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if(bRes)
            trans.commit();
    }
}

bool HoldingEditor::deleteHoldingPointsForOldMaster(int idHolding)
{
    bool bRes = true;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDatabase);
    query->Prepare("DELETE FROM holding_point_master WHERE holding_id = :1");
    query->BindValue(":1", idHolding);
    if(!query->exec())
    {
        bRes = false;
        qWarning() << query->lastError().text();
    }
    return bRes;
}

bool HoldingEditor::loadHolding(int idHolding, Holding &holding)
{
    bool bRes = false;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDatabase);
    query->Prepare("SELECT point_id, type, in_course, turn_direction, leg_length, leg_time, speed FROM holding WHERE id = :1");
    query->BindValue(":1", idHolding);
    if(query->exec())
    {
        if(query->next())
        {
            holding.pointId = query->value(0).toString();
            holding.type = Holding::Type(query->value(1).toInt());
            holding.inboundHoldingCourse = query->value(2).toDouble();
            holding.turn = ArincGlobal::TurnDirection(query->value(3).toInt());
            holding.legLength = query->value(4).toDouble();
            holding.legTime = query->value(5).toInt();
            holding.speed = query->value(6).toDouble();
            bRes = true;
        }
    }
    else
    {
        qWarning() << query->lastError().text();
    }
    return bRes;
}

bool HoldingEditor::getFirCenterPoint(int idFir, GeoPoint &centerPoint)
{
    bool bRes = false;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDatabase);
    query->Prepare("SELECT type, latitude, longitude, arc_latitude, arc_longitude FROM zone_boundary WHERE zone_id = :1 ORDER BY sn");
    query->BindValue(":1", idFir);
    if(query->exec())
    {
        if(query->next())
        {
            int type = query->value(0).toInt();
            centerPoint.setLatitude(type == BoundaryPoint::GreatCircle ? query->value(1).toDouble() : query->value(3).toDouble());
            centerPoint.setLongitude(type == BoundaryPoint::GreatCircle ? query->value(2).toDouble() : query->value(4).toDouble());
            if(centerPoint.latitude() || centerPoint.longitude())
                bRes = true;
        }
    }
    else
    {
        qWarning() << query->lastError().text();
    }
    return bRes;
}

bool HoldingEditor::getPointCoord(int idPoint, GeoPoint &point)
{
    bool bRes = false;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDatabase);
    query->Prepare("SELECT latitude, longitude FROM point WHERE id = :1");
    query->BindValue(":1", idPoint);
    if(query->exec())
    {
        if(query->next())
        {
            point.setLatitude(query->value(0).toDouble());
            point.setLongitude(query->value(1).toDouble());
            if(point.latitude() || point.longitude())
                bRes = true;
        }
    }
    else
    {
        qWarning() << query->lastError().text();
    }
    return bRes;
}
