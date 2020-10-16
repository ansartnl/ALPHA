#include "pointeditor.h"

#include "dbscheme.h"
#include "widgetsaver.h"

#include "Database.h"
#include "qtautotransaction.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>

#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>

#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlRelationalTableModel>

#include "coorddelegate.h"

using namespace dbscheme;

PointEditor::PointEditor(QWidget* parent, const QSqlDatabase &db) :
    BaseEditor(parent), mDatabase(db), isPointsChanged(false), isFirPointsChanged(false)
{
    setupUi(this);

    QPushButton* saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));
    connect(deleteFirPointButton, SIGNAL(clicked()), this, SLOT(deleteFirPoint()));
    connect(addFirPointButton, SIGNAL(clicked()), this, SLOT(addFirPoint()));
    connect(addPointButton, SIGNAL(clicked()), this, SLOT(addPoint()));
    connect(deletePointButton, SIGNAL(clicked()), this, SLOT(deletePoint()));

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
    pointIndexes[Point::category_type] = pointsTableModel->fieldIndex(Point::category_type);

    // Point view
    pointsTableModel->setRelation(pointIndexes[Point::type], QSqlRelation(PointType::tableName, PointType::id, PointType::name));
    selectAll(pointsTableModel);

    pointsView->setModel(pointsTableModel);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::name]), 0);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::type]), 1);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::lat]), 2);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::lon]), 3);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::icao]), 4);
    pointsView->horizontalHeader()->moveSection(pointsView->horizontalHeader()->visualIndex(pointIndexes[Point::airport]), 5);
    pointsView->setColumnHidden(pointIndexes[Point::id], true);
    pointsView->setColumnHidden(pointIndexes[Point::icao], true);
    pointsView->setColumnHidden(pointIndexes[Point::airport], true);
    pointsView->setColumnHidden(pointIndexes[Point::category_type], true);
    pointsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    connect(pointsTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(pointsDataChanged()));
    connect(pointsView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , SLOT(pointSelected(QModelIndex)));

    pointsDelegate = new RelModifyDelegate(this);
    QMap<int, QVariant::Type> TypeMap;
    TypeMap[pointIndexes[Point::lat]] = QVariant::Double;
    TypeMap[pointIndexes[Point::lon]] = QVariant::Double;
    pointsDelegate->SetColsType(TypeMap);
    pointsView->setItemDelegate(pointsDelegate);

    UpperCaseDelegate *nameDelegate = new UpperCaseDelegate(1, 7, this);
    pointsView->setItemDelegateForColumn(pointIndexes[Point::name], nameDelegate);
    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    pointsView->setItemDelegateForColumn(pointIndexes[Point::lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    pointsView->setItemDelegateForColumn(pointIndexes[Point::lon], coordDelegateLon);

    // FIR view
    firPointsTableModel = new QSqlRelationalTableModel(this, mDatabase);
    firPointsTableModel->setTable(VwZonePoint::tableName);
    firPointsTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    firPointIndexes[VwZonePoint::id] = firPointsTableModel->fieldIndex(VwZonePoint::id);
    firPointIndexes[VwZonePoint::pointId] = firPointsTableModel->fieldIndex(VwZonePoint::pointId);
    firPointIndexes[VwZonePoint::zoneId] = firPointsTableModel->fieldIndex(VwZonePoint::zoneId);
    firPointIndexes[VwZonePoint::name] = firPointsTableModel->fieldIndex(VwZonePoint::name);
    firPointIndexes[VwZonePoint::type] = firPointsTableModel->fieldIndex(VwZonePoint::type);
    firPointIndexes[VwZonePoint::lat] = firPointsTableModel->fieldIndex(VwZonePoint::lat);
    firPointIndexes[VwZonePoint::lon] = firPointsTableModel->fieldIndex(VwZonePoint::lon);
    firPointIndexes[VwZonePoint::icao] = firPointsTableModel->fieldIndex(VwZonePoint::icao);
    firPointIndexes[VwZonePoint::airport] = firPointsTableModel->fieldIndex(VwZonePoint::airport);
    firPointIndexes[VwZonePoint::isFirBorder] = firPointsTableModel->fieldIndex(VwZonePoint::isFirBorder);
    firPointIndexes[VwZonePoint::isSectorBorder] = firPointsTableModel->fieldIndex(VwZonePoint::isSectorBorder);

    firPointsView->setModel(firPointsTableModel);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::name]), 0);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::type]), 1);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::lat]), 2);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::lon]), 3);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::icao]), 4);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::airport]), 5);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::isFirBorder]), 6);
    firPointsView->horizontalHeader()->moveSection(firPointsView->horizontalHeader()->visualIndex(firPointIndexes[VwZonePoint::isSectorBorder]), 7);
    firPointsView->setColumnHidden(firPointIndexes[VwZonePoint::id], true);
    firPointsView->setColumnHidden(firPointIndexes[VwZonePoint::pointId], true);
    firPointsView->setColumnHidden(firPointIndexes[VwZonePoint::zoneId], true);
    firPointsView->setColumnHidden(firPointIndexes[VwZonePoint::icao], true);
    firPointsView->setColumnHidden(firPointIndexes[VwZonePoint::airport], true);
    firPointsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    connect(firPointsTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&))
            , SLOT(firPointsDataChanged(QModelIndex,QModelIndex)));

    firPointsDelegate = new RelModifyDelegate(this);
    firPointsDelegate->setReadOnly(true);
    firPointsView->setItemDelegate(firPointsDelegate);

    firPointsView->setItemDelegateForColumn(firPointIndexes[VwZonePoint::name], nameDelegate);
    coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, true, this);
    firPointsView->setItemDelegateForColumn(firPointIndexes[VwZonePoint::lat], coordDelegateLat);
    coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, true, this);
    firPointsView->setItemDelegateForColumn(firPointIndexes[VwZonePoint::lon], coordDelegateLon);
    YesNoDelegate *yesNoDelegate = new YesNoDelegate(this);
    firPointsView->setItemDelegateForColumn(firPointIndexes[VwZonePoint::isFirBorder], yesNoDelegate);
    firPointsView->setItemDelegateForColumn(firPointIndexes[VwZonePoint::isSectorBorder], yesNoDelegate);

    firPointsTableModel->setFilter("1 = 0");

    connect(pointsView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , pointsView->horizontalHeader(), SLOT(resizeSections()));
    connect(firPointsView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , firPointsView->horizontalHeader(), SLOT(resizeSections()));

    pointsView->installEventFilter(this);
    firPointsView->installEventFilter(this);

    translate();
}

PointEditor::~PointEditor()
{
    SaveState(this);
}

void PointEditor::translate()
{
    retranslateUi(this);

    pointsTableModel->setHeaderData(pointIndexes[Point::type], Qt::Horizontal, Point::tr_type());
    pointsTableModel->setHeaderData(pointIndexes[Point::name], Qt::Horizontal, Point::tr_name());
    pointsTableModel->setHeaderData(pointIndexes[Point::lat], Qt::Horizontal, Point::tr_lat());
    pointsTableModel->setHeaderData(pointIndexes[Point::lon], Qt::Horizontal, Point::tr_lon());
    pointsTableModel->setHeaderData(pointIndexes[Point::icao], Qt::Horizontal, Point::tr_icao());
    pointsTableModel->setHeaderData(pointIndexes[Point::airport], Qt::Horizontal, Point::tr_airport());

    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::type], Qt::Horizontal, VwZonePoint::tr_type());
    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::name], Qt::Horizontal, VwZonePoint::tr_name());
    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::lat], Qt::Horizontal, VwZonePoint::tr_lat());
    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::lon], Qt::Horizontal, VwZonePoint::tr_lon());
    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::icao], Qt::Horizontal, VwZonePoint::tr_icao());
    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::airport], Qt::Horizontal, VwZonePoint::tr_airport());
    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::isFirBorder], Qt::Horizontal, VwZonePoint::tr_isFirBorder());
    firPointsTableModel->setHeaderData(firPointIndexes[VwZonePoint::isSectorBorder], Qt::Horizontal, VwZonePoint::tr_isSectorBorder());
}

void PointEditor::refresh()
{
    pointsTableModel->revertAll();
    selectAll(pointsTableModel);
    firPointsTableModel->revertAll();
    selectAll(firPointsTableModel);
    isFirPointsChanged = false;
    isPointsChanged = false;
    operButtons->setEnabled(false);
}

void PointEditor::updateFirName()
{
    firLabel->setText(tr("Current FIR: %1").arg(firName()));
}

void PointEditor::setFirId()
{
    deleteFirPointButton->setEnabled(isFirValid());

    if(isFirValid())
        firPointsTableModel->setFilter(
                    QString("%1 IN (SELECT %2 FROM %3 WHERE %4 = %5) AND %6 = %5")
                    .arg(VwZonePoint::tableName + "." + VwZonePoint::pointId)
                    .arg(ZonePoint::tableName + "." + ZonePoint::pointId)
                    .arg(ZonePoint::tableName)
                    .arg(ZonePoint::zoneId)
                    .arg(firId())
                    .arg(VwZonePoint::tableName + "." + VwZonePoint::zoneId)
                    );
    else
        firPointsTableModel->setFilter("1=0");
    selectAll(firPointsTableModel);

    updatePointsTableFilter();

    firPointsView->horizontalHeader()->update();
}

void PointEditor::closeEvent(QCloseEvent* pEvent)
{
    if (operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            {
                try
                {
                    QtAutoTransaction dbTrans;
                    if (isPointsChanged && !pointsTableModel->submitAll())
                        throw (pointsTableModel->lastError().text());
                    if (isFirPointsChanged && !firPointsTableModel->submitAll())
                        throw (firPointsTableModel->lastError().text());
                    dbTrans.commit();
                    pEvent->accept();
                }
                catch (const QString &err)
                {
                    qCritical() << err;
                    QMessageBox::critical(this, tr("Error"), err);
                    pEvent->ignore();
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

void PointEditor::changeEvent(QEvent* event)
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

void PointEditor::pointsDataChanged()
{
    isPointsChanged = true;
    operButtons->setEnabled(true);
}

void PointEditor::firPointsDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    const int topRow = topLeft.row();
    const int bottomRow = bottomRight.row();
    for (int row = topRow; row <= bottomRow; ++row) {
        if ("*" != firPointsTableModel->headerData(row, Qt::Vertical)) {
            isFirPointsChanged = true;
            operButtons->setEnabled(true);
            return;
        }
    }
}

void PointEditor::onOperButtons(QAbstractButton* pButton)
{
    switch(operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        {
            QVariant pointsSelectedRow = pointsTableModel->data(pointsTableModel->index(pointsView->selectionModel()->currentIndex().row(), pointIndexes[Point::name]));
            QVariant firPointsSelectedRow = firPointsTableModel->data(firPointsTableModel->index(firPointsView->selectionModel()->currentIndex().row(), firPointIndexes[VwZonePoint::name]));
            try
            {
                QtAutoTransaction dbTrans(mDatabase);
                if (isPointsChanged && !pointsTableModel->submitAll())
                    throw (pointsTableModel->lastError().text());

                if (isFirPointsChanged)
                    saveFirPoint();

                if(!dbTrans.commit())
                    qDebug() << "Error commit";
                isPointsChanged = false;
                isFirPointsChanged = false;
                operButtons->setEnabled(false);
                selectAll(pointsTableModel);
                selectAll(firPointsTableModel);
                while(pointsTableModel->canFetchMore())
                    pointsTableModel->fetchMore();
                QModelIndexList pointsIndexList = pointsTableModel->match(pointsTableModel->index(0, pointIndexes[Point::name]), Qt::DisplayRole, pointsSelectedRow, 1, Qt::MatchExactly);
                while(firPointsTableModel->canFetchMore())
                    firPointsTableModel->fetchMore();
                QModelIndexList firPointsIndexList = firPointsTableModel->match(firPointsTableModel->index(0, firPointIndexes[VwZonePoint::name]), Qt::DisplayRole, firPointsSelectedRow, 1, Qt::MatchExactly);
                if(pointsIndexList.count() && !firPointsIndexList.count())
                {
                    pointsView->selectRow(pointsIndexList.first().row());
                    pointsView->scrollTo(pointsIndexList.first());
                }
                if(firPointsIndexList.count())
                {
                    firPointsView->selectRow(firPointsIndexList.first().row());
                    firPointsView->scrollTo(firPointsIndexList.first());
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
        if (isPointsChanged)
        {
            pointsTableModel->revertAll();
            selectAll(pointsTableModel);
        }
        if (isFirPointsChanged)
        {
            firPointsTableModel->revertAll();
            selectAll(firPointsTableModel);
        }
        isFirPointsChanged = false;
        isPointsChanged = false;
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }

    updatePointsTableFilter();
}

void PointEditor::addPoint()
{
    int addPosition = 0;
    pointsTableModel->insertRow(0);
    QModelIndex qmIndex = pointsTableModel->index(addPosition, pointsTableModel->fieldIndex(Point::name));
    pointsView->setCurrentIndex(qmIndex);
    pointsView->edit(qmIndex);
    isPointsChanged = true;
    operButtons->setEnabled(true);
}

void PointEditor::deletePoint()
{
    QModelIndexList selectList = pointsView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && pointsView->currentIndex().isValid())
        selectList.append(pointsView->currentIndex());
    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == pointsTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        pointsTableModel->removeRow(rowForDelte);
        isPointsChanged = true;
    }
    if (isPointsChanged)
        operButtons->setEnabled(true);
}

void PointEditor::pointSelected(const QModelIndex& qIndex)
{
    bool ableToAddPointToFir = qIndex.isValid()
            && !pointsTableModel->record(qIndex.row()).value(Point::id).isNull()
            && isFirValid();
    addFirPointButton->setEnabled(ableToAddPointToFir);
}

void PointEditor::addFirPoint()
{
    QModelIndexList selectList = pointsView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && pointsView->currentIndex().isValid())
        selectList.append(pointsView->currentIndex());

    foreach (const QModelIndex &selected, selectList)
    {
        int iRow = 0;
        QSqlRecord qRecord = pointsTableModel->record(selected.row());
        QSqlRecord rRecord;
        QSqlField id(VwZonePoint::id, QVariant::Int);
        QSqlField pointId(VwZonePoint::pointId, QVariant::Int);
        pointId.setValue(qRecord.value(Point::id));
        QSqlField zoneId(VwZonePoint::zoneId, QVariant::Int);
        zoneId.setValue(firId());
        QSqlField type(VwZonePoint::type, QVariant::String);
        type.setValue(qRecord.value(pointIndexes[Point::type]));
        QSqlField name(VwZonePoint::name, QVariant::String);
        name.setValue(qRecord.value(Point::name));
        QSqlField latitude(VwZonePoint::lat, QVariant::Double);
        latitude.setValue(qRecord.value(Point::lat));
        QSqlField longitude(VwZonePoint::lon, QVariant::Double);
        longitude.setValue(qRecord.value(Point::lon));
        QSqlField icao(VwZonePoint::icao, QVariant::String);
        icao.setValue(qRecord.value(Point::icao));
        QSqlField airport(VwZonePoint::airport, QVariant::String);
        airport.setValue(qRecord.value(Point::airport));
        QSqlField isFirBorder(VwZonePoint::isFirBorder, QVariant::Bool);
        isFirBorder.setValue(false);
        QSqlField isSectorBorder(VwZonePoint::isSectorBorder, QVariant::Bool);
        isSectorBorder.setValue(false);
        rRecord.insert(firPointIndexes[VwZonePoint::id], id);
        rRecord.insert(firPointIndexes[VwZonePoint::pointId], pointId);
        rRecord.insert(firPointIndexes[VwZonePoint::zoneId], zoneId);
        rRecord.insert(firPointIndexes[VwZonePoint::type], type);
        rRecord.insert(firPointIndexes[VwZonePoint::name], name);
        rRecord.insert(firPointIndexes[VwZonePoint::lat], latitude);
        rRecord.insert(firPointIndexes[VwZonePoint::lon], longitude);
        rRecord.insert(firPointIndexes[VwZonePoint::icao], icao);
        rRecord.insert(firPointIndexes[VwZonePoint::airport], airport);
        rRecord.insert(firPointIndexes[VwZonePoint::isFirBorder], isFirBorder);
        rRecord.insert(firPointIndexes[VwZonePoint::isSectorBorder], isSectorBorder);
        firPointsTableModel->insertRecord(iRow, rRecord);

        QModelIndex qmIndex = firPointsTableModel->index(iRow, firPointIndexes[VwZonePoint::name]);
        firPointsView->setCurrentIndex(qmIndex);
        isFirPointsChanged = true;
        operButtons->setEnabled(true);
    }

    updatePointsTableFilter();
}

void PointEditor::deleteFirPoint()
{
    QModelIndexList selectList = firPointsView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && firPointsView->currentIndex().isValid())
        selectList.append(firPointsView->currentIndex());
    int correction = 0;

    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == firPointsTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        else
            mFirPointsToDelete.insert(rowForDelte, firPointsTableModel->index(rowForDelte, firPointIndexes[Point::id]).data().toInt());
        firPointsTableModel->removeRow(rowForDelte);
        isFirPointsChanged = true;
    }

    if (isFirPointsChanged)
        operButtons->setEnabled(true);

    updatePointsTableFilter();
}

void PointEditor::updatePointsTableFilter()
{
    QStringList ids;
    const int count = firPointsTableModel->rowCount();
    for (int i = 0; i < count; ++i) {
        const QString id = firPointsTableModel->index(i, firPointIndexes[VwZonePoint::pointId]).data().toString();
        if (!id.isEmpty())
            ids << id;
    }

    if (!ids.isEmpty()) {
        pointsTableModel->setFilter(QString("\"%1\".\"%2\" NOT IN (%3)")
                                    .arg(Point::tableName, Point::id, ids.join(", ")));
    } else {
        pointsTableModel->setFilter("1!=0");
    }
    if (!selectAll(pointsTableModel)) {
        qDebug() << pointsTableModel->lastError();
    }
}

void PointEditor::saveFirPoint()
{
    //int uiZoneID = firId();
    //int iRowCount = firPointsTableModel->rowCount();

    firPointsTableModel->submitAll();

    /*for(int iIndex = 0; iIndex < iRowCount; ++iIndex)
    {
        if (firPointsTableModel->headerData(iIndex, Qt::Vertical) == "*")
        {
            QSharedPointer<db::CDBQuery> query = db::CreateQuery();
            query->Prepare("INSERT INTO zone_point(zone_id, point_id, is_fir_border, is_sector_border) VALUES(:1, :2, :3, :4)");
            query->BindValue(":1", (uiZoneID));
            query->BindValue(":2", firPointsTableModel->data(firPointsTableModel->index(iIndex, firPointIndexes[VwZonePoint::pointId])));

            query->BindValue(":3", firPointsTableModel->data(firPointsTableModel->index(iIndex, firPointIndexes[VwZonePoint::isFirBorder])));
            query->BindValue(":4", firPointsTableModel->data(firPointsTableModel->index(iIndex, firPointIndexes[VwZonePoint::isSectorBorder])));

            if (!query->exec())
                throw(tr("Execution error") + ". %1").arg(query->lastError().text());

        }
        else
            if (firPointsTableModel->headerData(iIndex, Qt::Vertical) == "!")
        {
            if (!mFirPointsToDelete.contains(iIndex))
            {
                QStringList ids;
                const QList < int > rows = mFirPointsToDelete.keys();
                foreach (const int row, rows) {
                    ids << QString("(%1; %2)").arg(row).arg(mFirPointsToDelete[row]);
                }
                qWarning() << tr("Error removing point from fir; id = %1; map = %2").arg(iIndex).arg(ids.join(" "));
            }

            QSharedPointer<db::CDBQuery> query = db::CreateQuery();
            query->Prepare("DELETE FROM aero.vw_zone_point WHERE zone_id = :1 AND id = :2");
            query->BindValue(":1", (uiZoneID));
            query->BindValue(":2", mFirPointsToDelete[iIndex]);

            mFirPointsToDelete.remove(iIndex);

            if (!query->exec())
                throw(tr("Execution error") + ". %1").arg(query->lastError().text());
        }
    }*/
}

