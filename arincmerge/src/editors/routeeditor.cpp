#include "routeeditor.h"

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

RouteEditor::RouteEditor(QWidget* parent, const QSqlDatabase &db) :
        BaseEditor(parent), mDatabase(db), routeID(0), idRouteChanged(false), isRoutePointChanged(false)
{
    setupUi(this);

    QPushButton* saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    connect(addRouteButton, SIGNAL(clicked()), this, SLOT(addRoute()));
    connect(deleteRouteButton, SIGNAL(clicked()), this, SLOT(deleteRoute()));
    connect(addRoutePointButton, SIGNAL(clicked()), this, SLOT(addRoutePoints()));
    connect(deleteRoutePointButton, SIGNAL(clicked()), this, SLOT(deleteRoutePoints()));
    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));

    // Point view
    pointsTableModel = new QSqlRelationalTableModel(this, mDatabase);
    pointsTableModel->setTable(Point::tableName);
    pointsTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    pointIndexes[Point::id] = pointsTableModel->fieldIndex(Point::id);
    pointIndexes[Point::name] = pointsTableModel->fieldIndex(Point::name);
    pointIndexes[Point::type] = pointsTableModel->fieldIndex(Point::type);
    pointIndexes[Point::lat] = pointsTableModel->fieldIndex(Point::lat);
    pointIndexes[Point::lon] = pointsTableModel->fieldIndex(Point::lon);
    pointIndexes[Point::icao] = pointsTableModel->fieldIndex(Point::icao);
    pointIndexes[Point::airport] = pointsTableModel->fieldIndex(Point::airport);

    pointsTableModel->setRelation(pointIndexes[Point::type], QSqlRelation(PointType::tableName, PointType::id, PointType::name));

    pointsView->setModel(pointsTableModel);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::name]), 0);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::type]), 1);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::lat]), 2);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::lon]), 3);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::icao]), 4);
    pointsView->setColumnHidden(pointIndexes[Point::id], true);
    pointsView->setColumnHidden(pointIndexes[Point::airport], true);
    pointsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    pointDelegate = new RelModifyDelegate(this);
    QMap<int, QVariant::Type> Types;
    Types[pointIndexes[Point::lat]] = QVariant::Double;
    Types[pointIndexes[Point::lon]] = QVariant::Double;
    pointsView->setItemDelegate(pointDelegate);
    pointDelegate->SetColsType(Types);

    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    pointsView->setItemDelegateForColumn(pointIndexes[Point::lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    pointsView->setItemDelegateForColumn(pointIndexes[Point::lon], coordDelegateLon);

    // Route view
    routeTableModel = new QSqlTableModel(this, mDatabase);
    routeTableModel->setTable(Route::tableName);
    routeTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    routeIndexes[Route::id] = routeTableModel->fieldIndex(Route::id);
    routeIndexes[Route::name] = routeTableModel->fieldIndex(Route::name);
    routeIndexes[Route::zoneid] = routeTableModel->fieldIndex(Route::zoneid);

    routeView->setModel(routeTableModel);
    routeView->horizontalHeader()->moveSection(routeView->horizontalHeader()->visualIndex(routeIndexes[Route::name]), 0);
    routeView->setColumnHidden(routeIndexes[Route::id], true);
    routeView->setColumnHidden(routeIndexes[Route::zoneid], true);
    routeView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    connect(routeTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(routeDataChanged()));
    connect(routeTableModel, SIGNAL(beforeInsert(QSqlRecord&))
            , SLOT(BeforeInsertRoute(QSqlRecord&)));
    connect(routeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , SLOT(OnRouteSelect(QModelIndex)));

    // Route point view
    routePointTableModel = new QSqlRelationalTableModel(this, mDatabase);
    routePointTableModel->setTable(RoutePoint::tableName);
    routePointTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    routePointIndexes[RoutePoint::id] = routePointTableModel->fieldIndex(RoutePoint::id);
    routePointIndexes[RoutePoint::sn] = routePointTableModel->fieldIndex(RoutePoint::sn);
    routePointIndexes[RoutePoint::pointid] = routePointTableModel->fieldIndex(RoutePoint::pointid);
    routePointIndexes[RoutePoint::routeid] = routePointTableModel->fieldIndex(RoutePoint::routeid);
    routePointIndexes[RoutePoint::endflag] = routePointTableModel->fieldIndex(RoutePoint::endflag);

    InitRelation();

    routePointView->setModel(routePointTableModel);

    routePointView->horizontalHeader()->moveSection(routePointView->horizontalHeader()->visualIndex(routePointIndexes[RoutePoint::sn]), 0);
    routePointView->horizontalHeader()->moveSection(routePointView->horizontalHeader()->visualIndex(routePointIndexes[RoutePoint::pointid]), 1);
    routePointView->setColumnHidden(routePointIndexes[RoutePoint::id], true);
    routePointView->setColumnHidden(routePointIndexes[RoutePoint::routeid], true);
    routePointView->setColumnHidden(routePointIndexes[RoutePoint::endflag], true);

    routePointView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    routePointDelegate = new RelModifyDelegate(routePointView);
    routePointView->setItemDelegate(routePointDelegate);

    connect(routePointTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex))
            , SLOT(routePointsDataChanged()));

    connect(pointsView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , pointsView->horizontalHeader(), SLOT(resizeSections()));

    pointsView->installEventFilter(this);
    routeView->installEventFilter(this);
    routePointView->installEventFilter(this);

    translate();
}

RouteEditor::~RouteEditor()
{
    SaveState(this);
}

void RouteEditor::translate()
{
    retranslateUi(this);

    pointsTableModel->setHeaderData(pointIndexes[Point::type], Qt::Horizontal, Point::tr_type());
    pointsTableModel->setHeaderData(pointIndexes[Point::name], Qt::Horizontal, Point::tr_name());
    pointsTableModel->setHeaderData(pointIndexes[Point::lat], Qt::Horizontal, Point::tr_lat());
    pointsTableModel->setHeaderData(pointIndexes[Point::lon], Qt::Horizontal, Point::tr_lon());
    pointsTableModel->setHeaderData(pointIndexes[Point::icao], Qt::Horizontal, Point::tr_icao());
    pointsTableModel->setHeaderData(pointIndexes[Point::airport], Qt::Horizontal, Point::tr_airport());

    routeTableModel->setHeaderData(routeIndexes[Route::name], Qt::Horizontal, Route::tr_name());

    routePointTableModel->setHeaderData(routePointIndexes[RoutePoint::sn], Qt::Horizontal, RoutePoint::tr_sn());
    routePointTableModel->setHeaderData(routePointIndexes[RoutePoint::pointid], Qt::Horizontal, RoutePoint::tr_pointid());
}

void RouteEditor::refresh()
{
    routeTableModel->revertAll();
    selectAll(routeTableModel);
    idRouteChanged = false;
    routePointTableModel->revertAll();
    selectAll(routePointTableModel);
    isRoutePointChanged = false;
    operButtons->setEnabled(false);
}

void RouteEditor::updateFirName()
{
    firLabel->setText(tr("Current FIR: %1").arg(firName()));
}

void RouteEditor::setFirId()
{
    deleteRouteButton->setEnabled(isFirValid());
    addRouteButton->setEnabled(isFirValid());

    QString sID = "0";
    if (isFirValid())
        sID = QString::number(firId());

    pointsTableModel->setFilter(QString("%1 IN (%2, %3, %4) AND %5 IN (SELECT %6 FROM %7 WHERE %8 = %9)")
              .arg(Point::type)
              .arg(1)
              .arg(2)
              .arg(3)
              .arg(Point::tableName + "." + Point::id)
              .arg(ZonePoint::tableName + "." + ZonePoint::pointId)
              .arg(ZonePoint::tableName)
              .arg(ZonePoint::zoneId)
              .arg(sID));

    if (!selectAll(pointsTableModel)) {
        qDebug() << pointsTableModel->lastError();
    }
    routeTableModel->setFilter(QString("%1 = %2")
                               .arg(Route::tableName + "." + Route::zoneid)
                               .arg(sID));
    if (!selectAll(routeTableModel)) {
        qDebug() << routeTableModel->lastError();
    }

    QSqlTableModel* pRelationModel = routePointTableModel->relationModel(
                routePointIndexes[RoutePoint::pointid]);
    pRelationModel->setFilter(
            QString("%1 IN (%2, %3, %4) AND %5 IN (SELECT %6 FROM %7 WHERE %8 = %9)")
            .arg(Point::type)
            .arg(1)
            .arg(2)
            .arg(3)
            .arg(Point::id)
            .arg(ZonePoint::pointId)
            .arg(ZonePoint::tableName)
            .arg(ZonePoint::zoneId)
            .arg(sID));
    pRelationModel->setSort(pointIndexes[Point::name], Qt::AscendingOrder);
    selectAll(pRelationModel);

    routeID = 0;
    routePointTableModel->setFilter("0 = -1");

    //updatePointsTableFilter();
}

void RouteEditor::closeEvent(QCloseEvent *pEvent)
{
    if (operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?")
                                     , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            {
                try
                {
                    QtAutoTransaction dbTrans;
                    if (idRouteChanged && !routeTableModel->submitAll())
                        throw (routeTableModel->lastError().text());
                    else if (isRoutePointChanged && !routePointTableModel->submitAll())
                        throw (routePointTableModel->lastError().text());
                    dbTrans.commit();
                    pEvent->accept();
                }
                catch (const QString &err)
                {
                    pEvent->ignore();
                    qCritical() << err;
                    QMessageBox::critical(this, tr("Error"), err);
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

void RouteEditor::changeEvent(QEvent *event)
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

void RouteEditor::addRoute()
{
    int iRow = 0;
    routeTableModel->insertRow(iRow);
    routeTableModel->setData(routeTableModel->index(iRow, routeIndexes[Route::zoneid]), firId());
    QModelIndex qmIndex = routeTableModel->index(0, routeIndexes[Route::name]);
    routeView->setCurrentIndex(qmIndex);
    routeView->edit(qmIndex);

    idRouteChanged = true;
    operButtons->setEnabled(true);
}

void RouteEditor::deleteRoute()
{
    QModelIndexList selectList = routeView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && routeView->currentIndex().isValid())
        selectList.append(routeView->currentIndex());
    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == routeTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        routeTableModel->removeRow(rowForDelte);
        idRouteChanged = true;
    }
    if (idRouteChanged)
        operButtons->setEnabled(true);
}

void RouteEditor::BeforeInsertRoute(QSqlRecord& qRecord)
{
    //qRecord.setValue(Route::zoneid, firId());
}

void RouteEditor::OnRouteSelect(const QModelIndex& qIndex)
{
    if (qIndex.isValid() && !routeTableModel->record(qIndex.row()).value(Route::id).isNull())
    {
        addRoutePointButton->setEnabled(true);
        deleteRoutePointButton->setEnabled(true);
        routeID = routeTableModel->record(qIndex.row()).value(Route::id).toUInt();
        routePointTableModel->setFilter(QString("%1 = %2").arg(RoutePoint::tableName + "."
                                                               + RoutePoint::routeid).arg(routeID));
    }
    else
    {
        addRoutePointButton->setEnabled(false);
        deleteRoutePointButton->setEnabled(false);
        routePointTableModel->setFilter("0 = -1");
        routeID = 0;
    }
    selectAll(routePointTableModel);

    updatePointsTableFilter();
}

void RouteEditor::addRoutePoints()
{   InitRelation();
    QModelIndexList selectList = pointsView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && pointsView->currentIndex().isValid())
        selectList.append(pointsView->currentIndex());
    foreach (const QModelIndex &selected, selectList)
    {
        int iRow = 0;
        uint uID = pointsTableModel->data(
                    pointsTableModel->index(selected.row(), pointIndexes[Point::id])).toUInt();

        routePointTableModel->insertRow(iRow);
        routePointTableModel->setData(routePointTableModel->index(iRow, routePointIndexes[RoutePoint::pointid]), uID);
        routePointTableModel->setData(routePointTableModel->index(iRow, routePointIndexes[RoutePoint::routeid]), routeID);
        routePointTableModel->setData(routePointTableModel->index(iRow, routePointIndexes[RoutePoint::endflag]), 0);

        QModelIndex qmIndex = routePointTableModel->index(iRow, routePointIndexes[RoutePoint::sn]);
        routePointView->setCurrentIndex(qmIndex);
        routePointView->edit(qmIndex);

        isRoutePointChanged = true;
        operButtons->setEnabled(true);
    }

    updatePointsTableFilter();
}

void RouteEditor::deleteRoutePoints()
{
    QModelIndexList selectList = routePointView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && routePointView->currentIndex().isValid())
        selectList.append(routePointView->currentIndex());
    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == routePointTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        routePointTableModel->removeRow(rowForDelte);
        isRoutePointChanged = true;
    }
    if (isRoutePointChanged)
        operButtons->setEnabled(true);

    updatePointsTableFilter();
}

void RouteEditor::onOperButtons(QAbstractButton* pButton)
{
    switch(operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        {
        QVariant routeSelectedRow = routeTableModel->data(
                    routeTableModel->index(
                        routeView->selectionModel()->currentIndex().row()
                        , routeIndexes[Route::name]));
        QVariant routePointSelectedRow = routePointTableModel->data(
                    routePointTableModel->index(
                        routePointView->selectionModel()->currentIndex().row()
                        , routePointIndexes[RoutePoint::sn]));
            try
            {
                QtAutoTransaction dbTrans;
                if (idRouteChanged && !routeTableModel->submitAll())
                    throw (routeTableModel->lastError().text());
                if (isRoutePointChanged && !routePointTableModel->submitAll())
                    throw (routePointTableModel->lastError().text());
                dbTrans.commit();
                idRouteChanged = false;
                isRoutePointChanged = false;
                operButtons->setEnabled(false);
                while(routeTableModel->canFetchMore())
                    routeTableModel->fetchMore();
                QModelIndexList routeIndexList = routeTableModel->match(
                            routeTableModel->index(0, routeIndexes[Route::name])
                            , Qt::DisplayRole, routeSelectedRow, 1, Qt::MatchExactly);
                while(routePointTableModel->canFetchMore())
                    routePointTableModel->fetchMore();
                QModelIndexList routePointIndexList = routePointTableModel->match(
                            routePointTableModel->index(0, routePointIndexes[RoutePoint::pointid])
                            , Qt::DisplayRole, routePointSelectedRow, 1, Qt::MatchExactly);
                if(routeIndexList.count() && !routePointIndexList.count())
                {
                    routeView->selectRow(routeIndexList.first().row());
                    routeView->scrollTo(routeIndexList.first());
                }
                if(routePointIndexList.count())
                {
                    routePointView->selectRow(routePointIndexList.first().row());
                    routePointView->scrollTo(routePointIndexList.first());
                }
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
        if (idRouteChanged)
        {
            routeTableModel->revertAll();
            selectAll(routeTableModel);
            idRouteChanged = false;
        }
        if (isRoutePointChanged)
        {
            routePointTableModel->revertAll();
            selectAll(routePointTableModel);
            isRoutePointChanged = false;
        }
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }

    updatePointsTableFilter();
}

void RouteEditor::routeDataChanged()
{
    idRouteChanged = true;
    operButtons->setEnabled(true);
}

void RouteEditor::routePointsDataChanged()
{
    isRoutePointChanged = true;
    operButtons->setEnabled(true);
}

void RouteEditor::updatePointsTableFilter()
{
    QString sID = "0";
    if (isFirValid())
        sID = QString::number(firId());

    QStringList filter(QString("%1 IN (%2, %3, %4) AND %5 IN (SELECT %6 FROM %7 WHERE %8 = %9)")
              .arg(Point::type)
              .arg(1)
              .arg(2)
              .arg(3)
              .arg(Point::tableName + "." + Point::id)
              .arg(ZonePoint::tableName + "." + ZonePoint::pointId)
              .arg(ZonePoint::tableName)
              .arg(ZonePoint::zoneId)
              .arg(sID));

    if (routeID) {
         filter << (QString("\"%1\".\"%2\" NOT IN (SELECT %3 FROM %4 WHERE %5 = %6)")
                    .arg(Point::tableName)
                    .arg(Point::id)
                    .arg(RoutePoint::pointid)
                    .arg(RoutePoint::tableName)
                    .arg(RoutePoint::routeid)
                    .arg(routeID)
                    );
    }
    pointsTableModel->setFilter(filter.join(" AND "));
    if (!selectAll(pointsTableModel)) {
        qDebug() << pointsTableModel->lastError();
    }
}

void RouteEditor::InitRelation()
{
    routePointTableModel->setRelation(routePointIndexes[RoutePoint::pointid], QSqlRelation(Point::tableName, Point::id, Point::name));
    selectAll(routePointTableModel);
    QSqlTableModel *pRelationModel = routePointTableModel->relationModel(routePointIndexes[RoutePoint::pointid]);
    if(pRelationModel)
    {
        selectAll(pRelationModel);
        routePointTableModel->data(routePointTableModel->index(0, routePointIndexes[RoutePoint::pointid]));
    }
    //routePointTableModel->setFilter("0 = 1");
}
