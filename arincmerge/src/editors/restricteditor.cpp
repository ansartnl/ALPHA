#include "restricteditor.h"

#include "dbscheme.h"
#include "widgetsaver.h"

#include "coorddelegate.h"
#include "qtautotransaction.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>

#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>

#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlRelationalTableModel>

using namespace dbscheme;

RestrictEditor::RestrictEditor(QWidget* parent, const QSqlDatabase &db) :
        BaseEditor(parent), mDatabase(db), m_qCurRestrictID(0)
{
    setupUi(this);

    QPushButton* saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButton(QAbstractButton*)));
    connect(addRestrictionButton, SIGNAL(clicked()), this, SLOT(addRestriction()));
    connect(deleteRestrictionButton, SIGNAL(clicked()), this, SLOT(deleteRestriction()));
    connect(addPointButton, SIGNAL(clicked()), this, SLOT(addPoint()));
    connect(deletePointButtun, SIGNAL(clicked()), this, SLOT(deletePoint()));

    //Restriction zone
    restrictionsModel = new QSqlRelationalTableModel(this);
    restrictionsModel->setTable(Restriction::tableName);
    restrictionsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    restrictionsModelIndex.insert(Restriction::id, restrictionsModel->fieldIndex(Restriction::id));
    restrictionsModelIndex.insert(Restriction::name, restrictionsModel->fieldIndex(Restriction::name));
    restrictionsModelIndex.insert(Restriction::type, restrictionsModel->fieldIndex(Restriction::type));
    restrictionsModelIndex.insert(Restriction::designation, restrictionsModel->fieldIndex(Restriction::designation));
    restrictionsModelIndex.insert(Restriction::zoneid, restrictionsModel->fieldIndex(Restriction::zoneid));
    restrictionsModelIndex.insert(Restriction::multicode, restrictionsModel->fieldIndex(Restriction::multicode));
    restrictionsModelIndex.insert(Restriction::hmin, restrictionsModel->fieldIndex(Restriction::hmin));
    restrictionsModelIndex.insert(Restriction::hmax, restrictionsModel->fieldIndex(Restriction::hmax));
    restrictionsModelIndex.insert(Restriction::schedulemode, restrictionsModel->fieldIndex(Restriction::schedulemode));
    restrictionsModel->setRelation(restrictionsModelIndex[Restriction::type], QSqlRelation(RestrictionType::tableName, RestrictionType::id, RestrictionType::name));
    restrictionsModel->setRelation(restrictionsModelIndex[Restriction::zoneid], QSqlRelation(Zone::tableName, Zone::id, Zone::name));
    selectAll(restrictionsModel);
    QSqlTableModel* pRelationModel = restrictionsModel->relationModel(restrictionsModelIndex[Restriction::zoneid]);
    if(pRelationModel)
    {
        selectAll(pRelationModel);
        restrictionsModel->data(restrictionsModel->index(0, restrictionsModelIndex[Restriction::zoneid]));
    }

    restrictionsView->setModel(restrictionsModel);
    restrictionsView->horizontalHeader()->moveSection(restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[Restriction::id]), 0);
    restrictionsView->horizontalHeader()->moveSection(restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[Restriction::name]), 1);
    restrictionsView->horizontalHeader()->moveSection(restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[Restriction::type]), 2);
    restrictionsView->horizontalHeader()->moveSection(restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[Restriction::designation]), 3);
    restrictionsView->horizontalHeader()->moveSection(restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[Restriction::zoneid]), 4);
    restrictionsView->horizontalHeader()->moveSection(restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[Restriction::multicode]), 5);
    restrictionsView->horizontalHeader()->moveSection(restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[Restriction::schedulemode]), 6);
    restrictionsView->setColumnHidden(restrictionsModelIndex[Restriction::id], true);
    restrictionsView->setColumnHidden(restrictionsModelIndex[Restriction::zoneid], true);
    restrictionsView->setColumnHidden(restrictionsModelIndex[Restriction::multicode], true);
    restrictionsView->setColumnHidden(restrictionsModelIndex[Restriction::schedulemode], true);
    restrictionsView->setColumnHidden(restrictionsModelIndex[Restriction::hmin], true);
    restrictionsView->setColumnHidden(restrictionsModelIndex[Restriction::hmax], true);
    restrictionsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    restrictionsView->resizeColumnsToContents();

    m_pModDelegateType = new RelModifyDelegate(restrictionsView);
    connect(restrictionsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(activateOperButtons()));
    restrictionsView->setItemDelegate(m_pModDelegateType);

    RelModifyDelegate::TColMap colMap;
    colMap.insert(restrictionsModelIndex[Restriction::id], QVariant::UInt);
    colMap.insert(restrictionsModelIndex[Restriction::name], QVariant::String);
    colMap.insert(restrictionsModelIndex[Restriction::type], QVariant::UInt);
    colMap.insert(restrictionsModelIndex[Restriction::designation], QVariant::String);
    colMap.insert(restrictionsModelIndex[Restriction::zoneid], QVariant::UInt);
    colMap.insert(restrictionsModelIndex[Restriction::multicode], QVariant::Char);
    colMap.insert(restrictionsModelIndex[Restriction::schedulemode], QVariant::UInt);
    m_pModDelegateType->SetColsType(colMap);

    connect(restrictionsView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(restrictionChanged(const QModelIndex&)));

    //Points
    pointsModel = new QSqlRelationalTableModel(this);
    pointsModel->setTable(RestrictionPoint::tableName);
    pointsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    restrictionPointIndex.insert(RestrictionPoint::id, pointsModel->fieldIndex(RestrictionPoint::id));
    restrictionPointIndex.insert(RestrictionPoint::sn, pointsModel->fieldIndex(RestrictionPoint::sn));
    restrictionPointIndex.insert(RestrictionPoint::type, pointsModel->fieldIndex(RestrictionPoint::type));
    restrictionPointIndex.insert(RestrictionPoint::lat, pointsModel->fieldIndex(RestrictionPoint::lat));
    restrictionPointIndex.insert(RestrictionPoint::lon, pointsModel->fieldIndex(RestrictionPoint::lon));
    restrictionPointIndex.insert(RestrictionPoint::arclat, pointsModel->fieldIndex(RestrictionPoint::arclat));
    restrictionPointIndex.insert(RestrictionPoint::arclon, pointsModel->fieldIndex(RestrictionPoint::arclon));
    restrictionPointIndex.insert(RestrictionPoint::arcdist, pointsModel->fieldIndex(RestrictionPoint::arcdist));
    restrictionPointIndex.insert(RestrictionPoint::restid, pointsModel->fieldIndex(RestrictionPoint::restid));
    pointsModel->setRelation(restrictionPointIndex[RestrictionPoint::type], QSqlRelation(ZoneBoundaryPointType::tableName, ZoneBoundaryPointType::id, ZoneBoundaryPointType::name));
    selectAll(pointsModel);
    pointsModel->setFilter("0 = 1");

    pointView->setModel(pointsModel);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::id]), 0);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::sn]), 1);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::type]), 2);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::lat]), 3);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::lon]), 4);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::arclat]), 5);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::arclon]), 6);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::arcdist]), 7);
    pointView->horizontalHeader()->moveSection(pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::restid]), 8);
    pointView->setColumnHidden(restrictionPointIndex[RestrictionPoint::id], true);
    pointView->setColumnHidden(restrictionPointIndex[RestrictionPoint::restid], true);
    pointView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    pointView->resizeColumnsToContents();

    restrictionPointDelegate = new RelModifyDelegate(pointView);
    connect(pointsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(activateOperButtons()));
    pointView->setItemDelegate(restrictionPointDelegate);

    colMap.clear();
    colMap.insert(restrictionPointIndex[RestrictionPoint::id], QVariant::UInt);
    colMap.insert(restrictionPointIndex[RestrictionPoint::sn], QVariant::UInt);
    colMap.insert(restrictionPointIndex[RestrictionPoint::type], QVariant::UInt);
    colMap.insert(restrictionPointIndex[RestrictionPoint::lat], QVariant::Double);
    colMap.insert(restrictionPointIndex[RestrictionPoint::lon], QVariant::Double);
    colMap.insert(restrictionPointIndex[RestrictionPoint::arclat], QVariant::Double);
    colMap.insert(restrictionPointIndex[RestrictionPoint::arclon], QVariant::Double);
    colMap.insert(restrictionPointIndex[RestrictionPoint::arcdist], QVariant::Double);
    colMap.insert(restrictionPointIndex[RestrictionPoint::restid], QVariant::UInt);
    restrictionPointDelegate->SetColsType(colMap);

    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::lon], coordDelegateLon);
    CoordDelegate* coordDelegateArcLat = new CoordDelegate(CoordEdit::Latitude, this);
    pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::arclat], coordDelegateArcLat);
    CoordDelegate* coordDelegateArcLon = new CoordDelegate(CoordEdit::Longitude, this);
    pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::arclon], coordDelegateArcLon);

    connect(restrictionsView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , restrictionsView->horizontalHeader(), SLOT(resizeSections()));
    connect(pointView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , pointView->horizontalHeader(), SLOT(resizeSections()));

    restrictionsView->installEventFilter(this);
    pointView->installEventFilter(this);

    translate();
}

RestrictEditor::~RestrictEditor()
{
    SaveState(this);
}

void RestrictEditor::translate()
{
    retranslateUi(this);

    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::name], Qt::Horizontal, Restriction::tr_name());
    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::type], Qt::Horizontal, Restriction::tr_type());
    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::designation], Qt::Horizontal, Restriction::tr_designation());
    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::zoneid], Qt::Horizontal, Restriction::tr_zoneid());
    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::multicode], Qt::Horizontal, Restriction::tr_multicode());
    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::hmin], Qt::Horizontal, Restriction::tr_hmin());
    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::hmax], Qt::Horizontal, Restriction::tr_hmax());
    restrictionsModel->setHeaderData(restrictionsModelIndex[Restriction::schedulemode], Qt::Horizontal, Restriction::tr_schedulemode());

    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::sn], Qt::Horizontal, RestrictionPoint::tr_sn());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::type], Qt::Horizontal, RestrictionPoint::tr_type());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::lat], Qt::Horizontal, RestrictionPoint::tr_lat());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::lon], Qt::Horizontal, RestrictionPoint::tr_lon());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::arclat], Qt::Horizontal, RestrictionPoint::tr_arclat());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::arclon], Qt::Horizontal, RestrictionPoint::tr_arclon());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::arcdist], Qt::Horizontal, RestrictionPoint::tr_arcdist());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::restid], Qt::Horizontal, RestrictionPoint::tr_restid());
}

void RestrictEditor::refresh()
{
    pointsModel->revertAll();
    restrictionsModel->revertAll();
    operButtons->setEnabled(false);
}

void RestrictEditor::updateFirName()
{
    firLabel->setText(tr("Current FIR: %1").arg(firName()));
}

void RestrictEditor::setFirId()
{
    if (isFirValid())
    {
        m_qZoneID = firId();
        restrictionsModel->setFilter(QString("zone_id = %1").arg(m_qZoneID));
    }
    else
    {
        m_qZoneID = 0;
        restrictionsModel->setFilter("0 = 1");
    }
    selectAll(restrictionsModel);
}

void RestrictEditor::closeEvent(QCloseEvent *pEvent)
{
    if (operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?")
                                     , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            try
            {
                QtAutoTransaction dbTrans;
                if (!pointsModel->submitAll())
                    throw (pointsModel->lastError().text());
                if (!restrictionsModel->submitAll())
                    throw (restrictionsModel->lastError().text());
                dbTrans.commit();
                pEvent->accept();
            }
            catch (const QString &err)
            {
                qCritical() << err;
                QMessageBox::critical(this, tr("Error"), err);
                pEvent->ignore();
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

void RestrictEditor::changeEvent(QEvent *event)
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

void RestrictEditor::onOperButton(QAbstractButton* pButton)
{
    switch(operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        try
        {
        QVariant restrictionsSelectedRow = restrictionsModel->data(restrictionsModel->index(restrictionsView->selectionModel()->currentIndex().row(), restrictionsModelIndex[Restriction::designation]));
        QVariant pointsSelectedRow = pointsModel->data(pointsModel->index(pointView->selectionModel()->currentIndex().row(), restrictionPointIndex[RestrictionPoint::sn]));
            QtAutoTransaction dbTrans;
            if (!pointsModel->submitAll())
                throw (pointsModel->lastError().text());
            if (!restrictionsModel->submitAll())
                throw (restrictionsModel->lastError().text());
            dbTrans.commit();
            operButtons->setEnabled(false);
            while(restrictionsModel->canFetchMore())
                restrictionsModel->fetchMore();
            QModelIndexList restrictionsIndexList = restrictionsModel->match(restrictionsModel->index(0, restrictionsModelIndex[Restriction::designation]), Qt::DisplayRole, restrictionsSelectedRow, 1, Qt::MatchExactly);
            while(pointsModel->canFetchMore())
                pointsModel->fetchMore();
            QModelIndexList pointsIndexList = pointsModel->match(pointsModel->index(0, restrictionPointIndex[RestrictionPoint::sn]), Qt::DisplayRole, pointsSelectedRow, 1, Qt::MatchExactly);
            if(restrictionsIndexList.count() && !pointsIndexList.count())
            {
                restrictionsView->selectRow(restrictionsIndexList.first().row());
                restrictionsView->scrollTo(restrictionsIndexList.first());
            }
            if(pointsIndexList.count())
            {
                pointView->selectRow(pointsIndexList.first().row());
                pointView->scrollTo(pointsIndexList.first());
            }
        }
        catch (const QString &err)
        {
            qCritical() << err;
            QMessageBox::critical(this, tr("Error"), err);
        }
        break;
    case QDialogButtonBox::RejectRole:
    case QDialogButtonBox::DestructiveRole:
        pointsModel->revertAll();
        restrictionsModel->revertAll();
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }
}

void RestrictEditor::activateOperButtons()
{
    operButtons->setEnabled(true);
}

void RestrictEditor::addRestriction()
{
    if (m_qZoneID)
    {
        int iRow = 0;
        restrictionsModel->insertRow(iRow);
        restrictionsModel->setData(restrictionsModel->index(iRow, restrictionsModelIndex[Restriction::zoneid]), m_qZoneID);
        QModelIndex qmIndex = restrictionsModel->index(iRow, restrictionsModelIndex[Restriction::name]);
        restrictionsView->setCurrentIndex(qmIndex);
        restrictionsView->edit(qmIndex);
        operButtons->setEnabled(true);
    }
}

void RestrictEditor::deleteRestriction()
{
    QModelIndexList selectList = restrictionsView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && restrictionsView->currentIndex().isValid())
        selectList.append(restrictionsView->currentIndex());
    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == restrictionsModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        restrictionsModel->removeRow(rowForDelte);
        operButtons->setEnabled(true);
    }
}

void RestrictEditor::restrictionChanged(const QModelIndex& ModInd)
{
    if (ModInd.isValid())
    {
        QSqlRecord qRec = restrictionsModel->record(ModInd.row());
        m_qCurRestrictID = qRec.value(restrictionsModelIndex[Restriction::id]).toUInt();
        pointsModel->setFilter(QString("restriction_id = %1").arg(QString::number(m_qCurRestrictID)));
    }
    else
    {
        pointsModel->setFilter("0 = 1");
        m_qCurRestrictID = 0;
    }
    selectAll(pointsModel);
}

void RestrictEditor::addPoint()
{
    if (m_qCurRestrictID)
    {
        int iRow = 0;
        pointsModel->insertRow(iRow);
        pointsModel->setData(pointsModel->index(iRow, restrictionPointIndex[RestrictionPoint::restid]), m_qCurRestrictID);
        QModelIndex qmIndex = pointsModel->index(iRow, restrictionPointIndex[RestrictionPoint::sn]);
        pointView->setCurrentIndex(qmIndex);
        pointView->edit(qmIndex);
        operButtons->setEnabled(true);
    }
}

void RestrictEditor::deletePoint()
{
    QModelIndexList selectList = pointView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && pointView->currentIndex().isValid())
        selectList.append(pointView->currentIndex());
    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == pointsModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        pointsModel->removeRow(rowForDelte);
        operButtons->setEnabled(true);
    }
}
