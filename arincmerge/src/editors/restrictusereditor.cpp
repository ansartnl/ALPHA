#include "restrictusereditor.h"
#include "ui_restrictusereditor.h"
#include "dbscheme.h"
#include "qtautotransaction.h"
#include "viewdelegate.h"
#include "widgetsaver.h"
#include "Vincenty.h"
#include "misc.h"

#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtGui/QCloseEvent>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QUuid>

#include <QMessageBox>
#include <QPushButton>

#include "coorddelegate.h"

using namespace dbscheme;

RestrictUserEditor::RestrictUserEditor(QWidget *parent) :
    BaseEditor(parent),
    ui(new Ui::RestrictUserEditor), m_qZoneID(0), m_qCurRestrictID(0)
{
    ui->setupUi(this);

    QPushButton* saveButton = ui->operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    //Restriction zone
    QSqlRelationalTableModel *restrictionsModel = new QSqlRelationalTableModel(this);
    restrictionsModel->setTable(RestrictionUser::tableName);
    restrictionsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    restrictionsModelIndex.insert(RestrictionUser::id, restrictionsModel->fieldIndex(RestrictionUser::id));
    restrictionsModelIndex.insert(RestrictionUser::name, restrictionsModel->fieldIndex(RestrictionUser::name));
    restrictionsModelIndex.insert(RestrictionUser::zoneid, restrictionsModel->fieldIndex(RestrictionUser::zoneid));
    restrictionsModelIndex.insert(RestrictionUser::hmin, restrictionsModel->fieldIndex(RestrictionUser::hmin));
    restrictionsModelIndex.insert(RestrictionUser::hmax, restrictionsModel->fieldIndex(RestrictionUser::hmax));
    restrictionsModelIndex.insert(RestrictionUser::timebegin, restrictionsModel->fieldIndex(RestrictionUser::timebegin));
    restrictionsModelIndex.insert(RestrictionUser::timeend, restrictionsModel->fieldIndex(RestrictionUser::timeend));
    restrictionsModelIndex.insert(RestrictionUser::type, restrictionsModel->fieldIndex(RestrictionUser::type));
    restrictionsModelIndex.insert(RestrictionUser::guid, restrictionsModel->fieldIndex(RestrictionUser::guid));
    restrictionsModel->setRelation(restrictionsModelIndex[RestrictionUser::type], QSqlRelation(RestrictionUserType::tableName, RestrictionUserType::id, RestrictionUserType::name));
    selectAll(restrictionsModel);

    ui->restrictionsView->setModel(restrictionsModel);
    ui->restrictionsView->horizontalHeader()->moveSection(ui->restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[RestrictionUser::id]), 0);
    ui->restrictionsView->horizontalHeader()->moveSection(ui->restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[RestrictionUser::name]), 1);
    ui->restrictionsView->horizontalHeader()->moveSection(ui->restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[RestrictionUser::zoneid]), 2);
    ui->restrictionsView->horizontalHeader()->moveSection(ui->restrictionsView->horizontalHeader()->visualIndex(restrictionsModelIndex[RestrictionUser::type]), 3);
    ui->restrictionsView->setColumnHidden(restrictionsModelIndex[RestrictionUser::id], true);
    ui->restrictionsView->setColumnHidden(restrictionsModelIndex[RestrictionUser::zoneid], true);
    ui->restrictionsView->setColumnHidden(restrictionsModelIndex[RestrictionUser::guid], true);
    ui->restrictionsView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    ui->restrictionsView->resizeColumnsToContents();

    RelModifyDelegate *m_pModDelegateType = new RelModifyDelegate(ui->restrictionsView);
    connect(restrictionsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(activateOperButtons()));
    ui->restrictionsView->setItemDelegate(m_pModDelegateType);

    RelModifyDelegate::TColMap colMap;
    colMap.insert(restrictionsModelIndex[RestrictionUser::name], QVariant::String);
    colMap.insert(restrictionsModelIndex[RestrictionUser::zoneid], QVariant::String);
    colMap.insert(restrictionsModelIndex[RestrictionUser::hmin], QVariant::UInt);
    colMap.insert(restrictionsModelIndex[RestrictionUser::hmax], QVariant::UInt);
    colMap.insert(restrictionsModelIndex[RestrictionUser::timebegin], QVariant::DateTime);
    colMap.insert(restrictionsModelIndex[RestrictionUser::timeend], QVariant::DateTime);
    colMap.insert(restrictionsModelIndex[RestrictionUser::type], QVariant::String);
    m_pModDelegateType->SetColsType(colMap);

    connect(ui->restrictionsView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(restrictionChanged(const QModelIndex&)));

    //Points
    QSqlRelationalTableModel *pointsModel = new QSqlRelationalTableModel(this);
    pointsModel->setTable(RestrictionPointUser::tableName);
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

    ui->pointView->setModel(pointsModel);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::id]), 0);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::sn]), 1);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::type]), 2);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::lat]), 3);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::lon]), 4);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::arclat]), 5);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::arclon]), 6);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::arcdist]), 7);
    ui->pointView->horizontalHeader()->moveSection(ui->pointView->horizontalHeader()->visualIndex(restrictionPointIndex[RestrictionPoint::restid]), 8);
    ui->pointView->setColumnHidden(restrictionPointIndex[RestrictionPoint::id], true);
    ui->pointView->setColumnHidden(restrictionPointIndex[RestrictionPoint::restid], true);
    ui->pointView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    ui->pointView->resizeColumnsToContents();

    RelModifyDelegate *restrictionPointDelegate = new RelModifyDelegate(ui->pointView);
    connect(pointsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(activateOperButtons()));
    ui->pointView->setItemDelegate(restrictionPointDelegate);

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
    ui->pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    ui->pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::lon], coordDelegateLon);
    CoordDelegate* coordDelegateArcLat = new CoordDelegate(CoordEdit::Latitude, this);
    ui->pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::arclat], coordDelegateArcLat);
    CoordDelegate* coordDelegateArcLon = new CoordDelegate(CoordEdit::Longitude, this);
    ui->pointView->setItemDelegateForColumn(restrictionPointIndex[RestrictionPoint::arclon], coordDelegateArcLon);

    // FIR combo
    QSqlQueryModel *firQueryModel = new QSqlQueryModel(ui->firCombo);
    firQueryModel->setQuery(QString("select %1, %2 || '(' || %3 || ')' as a from %4 where %5 = 1 order by a")
                            .arg(Zone::id, Zone::idname, Zone::name, Zone::tableName, Zone::type));
    ui->firCombo->setModel(firQueryModel);
    ui->firCombo->setModelColumn(1);
    ui->firCombo->setCurrentIndex(0);

    ui->restrictionsView->installEventFilter(this);
    ui->pointView->installEventFilter(this);

    translate();
}

RestrictUserEditor::~RestrictUserEditor()
{
    SaveState(this);
    delete ui;
}


void RestrictUserEditor::translate()
{
    ui->retranslateUi(this);

    QSqlRelationalTableModel *restrictionsModel = static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );

    restrictionsModel->setHeaderData(restrictionsModelIndex[RestrictionUser::name], Qt::Horizontal, RestrictionUser::tr_name());
    restrictionsModel->setHeaderData(restrictionsModelIndex[RestrictionUser::type], Qt::Horizontal, RestrictionUser::tr_type());
    restrictionsModel->setHeaderData(restrictionsModelIndex[RestrictionUser::hmin], Qt::Horizontal, RestrictionUser::tr_hmin());
    restrictionsModel->setHeaderData(restrictionsModelIndex[RestrictionUser::hmax], Qt::Horizontal, RestrictionUser::tr_hmax());
    restrictionsModel->setHeaderData(restrictionsModelIndex[RestrictionUser::timebegin], Qt::Horizontal, RestrictionUser::tr_timebegin());
    restrictionsModel->setHeaderData(restrictionsModelIndex[RestrictionUser::timeend], Qt::Horizontal, RestrictionUser::tr_timeend());

    QSqlRelationalTableModel *pointsModel = static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::sn], Qt::Horizontal, RestrictionPoint::tr_sn());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::type], Qt::Horizontal, RestrictionPoint::tr_type());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::lat], Qt::Horizontal, RestrictionPoint::tr_lat());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::lon], Qt::Horizontal, RestrictionPoint::tr_lon());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::arclat], Qt::Horizontal, RestrictionPoint::tr_arclat());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::arclon], Qt::Horizontal, RestrictionPoint::tr_arclon());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::arcdist], Qt::Horizontal, RestrictionPoint::tr_arcdist());
    pointsModel->setHeaderData(restrictionPointIndex[RestrictionPoint::restid], Qt::Horizontal, RestrictionPoint::tr_restid());
}


void RestrictUserEditor::closeEvent(QCloseEvent* pEvent)
{
    if (ui->operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            try
            {
                QSqlRelationalTableModel *restrictionsModel =
                        static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );
                QSqlRelationalTableModel *pointsModel =
                        static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

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


void RestrictUserEditor::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    switch(event->type())
    {
    case QEvent::LanguageChange:
        translate();
        break;
    default:
        break;
    }
}

void RestrictUserEditor::on_operButtons_clicked(QAbstractButton* pButton)
{
    QSqlRelationalTableModel *restrictionsModel =
            static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );
    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

    switch(ui->operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        try
        {
            QVariant restrictionsSelectedRow = restrictionsModel->data(restrictionsModel->index(ui->restrictionsView->selectionModel()->currentIndex().row(), restrictionsModelIndex[RestrictionUser::name]));
            QVariant pointsSelectedRow = pointsModel->data(pointsModel->index(ui->pointView->selectionModel()->currentIndex().row(), restrictionPointIndex[RestrictionPoint::sn]));
            QtAutoTransaction dbTrans;
            if (!pointsModel->submitAll())
                throw (pointsModel->lastError().text());
            disconnect(ui->restrictionsView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
                       , this, SLOT(restrictionChanged(const QModelIndex&)));
            if (!restrictionsModel->submitAll())
                throw (restrictionsModel->lastError().text());
            dbTrans.commit();
            ui->operButtons->setEnabled(false);
            while(restrictionsModel->canFetchMore())
                restrictionsModel->fetchMore();
            QModelIndexList restrictionsIndexList = restrictionsModel->match(restrictionsModel->index(0, restrictionsModelIndex[Restriction::name]), Qt::DisplayRole, restrictionsSelectedRow, 1, Qt::MatchExactly);
            while(pointsModel->canFetchMore())
                pointsModel->fetchMore();
            selectAll(pointsModel);
            QModelIndexList pointsIndexList = pointsModel->match(pointsModel->index(0, restrictionPointIndex[RestrictionPoint::sn]), Qt::DisplayRole, pointsSelectedRow, 1, Qt::MatchExactly);
            if(restrictionsIndexList.count() && !pointsIndexList.count())
            {
                ui->restrictionsView->selectRow(restrictionsIndexList.first().row());
                ui->restrictionsView->scrollTo(restrictionsIndexList.first());
            }
            if(pointsIndexList.count())
            {
                ui->pointView->selectRow(pointsIndexList.first().row());
                ui->pointView->scrollTo(pointsIndexList.first());
            }
        }
        catch (const QString &err)
        {
            qCritical() << err;
            QMessageBox::critical(this, tr("Error"), err);
        }
        connect(ui->restrictionsView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
                               , this, SLOT(restrictionChanged(const QModelIndex&)));
        break;
    case QDialogButtonBox::RejectRole:
    case QDialogButtonBox::DestructiveRole:
        restrictionsModel->revertAll();
        pointsModel->revertAll();
        ui->operButtons->setEnabled(false);
        break;
    default:
        break;
    }
}

void RestrictUserEditor::activateOperButtons()
{
    ui->operButtons->setEnabled(true);
}

void RestrictUserEditor::on_addRestrictionButton_clicked()
{
    if (m_qZoneID)
    {
        QSqlRelationalTableModel *restrictionsModel =
                static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );

        int iRow = 0;
        restrictionsModel->insertRow(iRow);
        QSqlRecord qRec = restrictionsModel->record(iRow);
        qRec.setValue(restrictionsModelIndex[RestrictionUser::zoneid], m_qZoneID);
        qRec.setValue(restrictionsModelIndex[RestrictionUser::guid], QUuid::createUuid().toString());
        restrictionsModel->setRecord(iRow, qRec);
        QModelIndex qmIndex = restrictionsModel->index(iRow, restrictionsModelIndex[RestrictionUser::name]);
        ui->restrictionsView->setCurrentIndex(qmIndex);
        ui->restrictionsView->edit(qmIndex);
        ui->operButtons->setEnabled(true);
    }
}

void RestrictUserEditor::on_deleteRestrictionButton_clicked()
{
    QModelIndexList selectList = ui->restrictionsView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->restrictionsView->currentIndex().isValid())
        selectList.append(ui->restrictionsView->currentIndex());

    QSqlRelationalTableModel *restrictionsModel =
            static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );

    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == restrictionsModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        restrictionsModel->removeRow(rowForDelte);
        ui->operButtons->setEnabled(true);
    }
}

void RestrictUserEditor::restrictionChanged(const QModelIndex& ModInd)
{
    QSqlRelationalTableModel *restrictionsModel =
            static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );
    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

    if (ModInd.isValid())
    {
        QSqlRecord qRec = restrictionsModel->record(ModInd.row());
        m_qCurRestrictID = qRec.value(restrictionsModelIndex[RestrictionUser::id]).toUInt();
        pointsModel->setFilter(QString("restriction_id = %1").arg(QString::number(m_qCurRestrictID)));
    }
    else
    {
        pointsModel->setFilter("0 = 1");
        m_qCurRestrictID = 0;
    }
    selectAll(pointsModel);
}


void RestrictUserEditor::on_addPointButton_clicked()
{
    if (m_qCurRestrictID)
    {
        QSqlRelationalTableModel *pointsModel =
                static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

        int iRow = 0;
        pointsModel->insertRow(iRow);
        QSqlRecord qRec = pointsModel->record(iRow);
        qRec.setValue(restrictionPointIndex[RestrictionPoint::restid], m_qCurRestrictID);
        pointsModel->setRecord(iRow, qRec);
        QModelIndex qmIndex = pointsModel->index(iRow, restrictionPointIndex[RestrictionPoint::sn]);
        ui->pointView->setCurrentIndex(qmIndex);
        ui->pointView->edit(qmIndex);
        ui->operButtons->setEnabled(true);
    }
}

void RestrictUserEditor::on_deletePointButtun_clicked()
{
    QModelIndexList selectList = ui->pointView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->pointView->currentIndex().isValid())
        selectList.append(ui->pointView->currentIndex());

    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == pointsModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        pointsModel->removeRow(rowForDelte);
        ui->operButtons->setEnabled(true);
    }
}

void RestrictUserEditor::on_firCombo_currentIndexChanged(int iIndex)
{
    QSqlRelationalTableModel *restrictionsModel =
            static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );

    if ((iIndex != -1) && (iIndex < ui->firCombo->count()))
    {
        m_qZoneID = ui->firCombo->model()->index(iIndex, 0).data().toUInt();
        restrictionsModel->setFilter(QString("zone_id = %1").arg(m_qZoneID));
    }
    else
    {
        m_qZoneID = 0;
        restrictionsModel->setFilter("0 = 1");
    }
    selectAll(restrictionsModel);
}

void RestrictUserEditor::refresh()
{
    QSqlRelationalTableModel *restrictionsModel =
            static_cast<QSqlRelationalTableModel *>( ui->restrictionsView->model() );
    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

    pointsModel->revertAll();
    restrictionsModel->revertAll();
    ui->operButtons->setEnabled(false);
}

int RestrictUserEditor::findMaxSN() const
{
    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

    int sn = 0;
    for ( int i = 0; i < pointsModel->rowCount(); i++ )
    {
        int currentSN = pointsModel->index(i, restrictionPointIndex[RestrictionPoint::sn]).data().toInt();
        sn = qMax(sn, currentSN);
    }

    return sn;
}

void RestrictUserEditor::on_toolButtonPastePolygon_clicked()
{
    if (m_qCurRestrictID)
    {
        typedef QList<QPointF>  TCoordList;
        TCoordList coords = getPointsFromClipboard(this);

        QSqlRelationalTableModel *pointsModel =
                static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

        int sn = findMaxSN() + 10;
        for ( int i = coords.size()-1; i >= 0; i--, sn += 10 )
        {
            const QPointF &coord = coords.at(i);

            int iRow = 0;
            pointsModel->insertRow(iRow);
            QSqlRecord qRec = pointsModel->record(iRow);
            qRec.setValue(restrictionPointIndex[RestrictionPoint::restid], m_qCurRestrictID);
            qRec.setValue(restrictionPointIndex[RestrictionPoint::sn], sn);
            qRec.setValue(restrictionPointIndex[RestrictionPoint::type], int('G'));
            qRec.setValue(restrictionPointIndex[RestrictionPoint::lat], coord.x());
            qRec.setValue(restrictionPointIndex[RestrictionPoint::lon], coord.y());
            qRec.setValue(restrictionPointIndex[RestrictionPoint::arclat], 0.);
            qRec.setValue(restrictionPointIndex[RestrictionPoint::arclon], 0.);
            qRec.setValue(restrictionPointIndex[RestrictionPoint::arcdist], 0.);
            pointsModel->setRecord(iRow, qRec);
            ui->operButtons->setEnabled(true);
        }
    }
}

void RestrictUserEditor::on_toolButtonPasteCircle_clicked()
{
    if (m_qCurRestrictID)
    {
        typedef QList<QPointF>  TCoordList;
        TCoordList coords = getPointsFromClipboard(this);

        if ( coords.empty() )
            return;

        QSqlRelationalTableModel *pointsModel =
                static_cast<QSqlRelationalTableModel *>( ui->pointView->model() );

        QPointF point1 = coords.at(0);

        double arcdist = 0.;
        if ( coords.size() > 1 )
        {
            QPointF point2 = coords.at(1);
            QPointF p1 = Vincenty::toCartesian(GeoPoint(point1.y(),point1.x()), GeoPoint(point1.y(),point1.x()));
            QPointF p2 = Vincenty::toCartesian(GeoPoint(point2.y(),point2.x()), GeoPoint(point1.y(),point1.x()));

            arcdist = QLineF(p1, p2).length();
        }

        int iRow = 0;
        pointsModel->insertRow(iRow);
        QSqlRecord qRec = pointsModel->record(iRow);
        qRec.setValue(restrictionPointIndex[RestrictionPoint::restid], m_qCurRestrictID);
        qRec.setValue(restrictionPointIndex[RestrictionPoint::sn], findMaxSN()+10);
        qRec.setValue(restrictionPointIndex[RestrictionPoint::type], int('C'));
        qRec.setValue(restrictionPointIndex[RestrictionPoint::lat], 0.);
        qRec.setValue(restrictionPointIndex[RestrictionPoint::lon], 0.);
        qRec.setValue(restrictionPointIndex[RestrictionPoint::arclat], point1.x());
        qRec.setValue(restrictionPointIndex[RestrictionPoint::arclon], point1.y());
        qRec.setValue(restrictionPointIndex[RestrictionPoint::arcdist], arcdist);
        pointsModel->setRecord(iRow, qRec);
        ui->operButtons->setEnabled(true);
    }
}
