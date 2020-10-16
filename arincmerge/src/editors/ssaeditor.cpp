#include "ssaeditor.h"

#include "dbscheme.h"
#include "widgetsaver.h"

#include "coorddelegate.h"
#include "qtautotransaction.h"
#include "Database.h"

#include "ssamodel.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>

#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>

#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlRelationalTableModel>

int id_QList_int = qRegisterMetaType<QList<int> >();

using namespace dbscheme;

SsaEditor::SsaEditor(QWidget *parent, const QSqlDatabase &db) :
        BaseEditor(parent), mDatabase(db), runwayId(0), ssaId(0), isRunwayChanged(false)
      , isSsaChanged(false), isSsaPointChanged(false), scheme(-1)
{
    setupUi(this);

    QPushButton *saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    connect(airportsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(airportChanged(int)));
    connect(schemeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(schemeChanged(int)));
    connect(addRunwayButton, SIGNAL(clicked()), this, SLOT(addRunway()));
    connect(deleteRunwayButton, SIGNAL(clicked(bool)), this, SLOT(deleteRunway()));
    connect(addSsaButton, SIGNAL(clicked()), this, SLOT(addSsa()));
    connect(deleteSsaButton, SIGNAL(clicked()), this, SLOT(deleteSsa()));
    connect(addSsaPointButton, SIGNAL(clicked()), this, SLOT(addSsaPoint()));
    connect(deletSsaPointButton, SIGNAL(clicked()), this, SLOT(deleteSsaPoint()));
    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));

    airportComboModel = new QSqlQueryModel(this);
    airportsComboBox->setModel(airportComboModel);

    // Runway view
    runwayTableModel = new RunwayModel(this, mDatabase);
    runwayTableModel->setTable(Runway::tableName);
    runwayTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    runwayIndexes[Runway::id] = runwayTableModel->fieldIndex(Runway::id);
    runwayIndexes[Runway::name] = runwayTableModel->fieldIndex(Runway::name);
    runwayIndexes[Runway::lat] = runwayTableModel->fieldIndex(Runway::lat);
    runwayIndexes[Runway::lon] = runwayTableModel->fieldIndex(Runway::lon);
    runwayIndexes[Runway::length] = runwayTableModel->fieldIndex(Runway::length);
    runwayIndexes[Runway::azimuth] = runwayTableModel->fieldIndex(Runway::azimuth);
    runwayIndexes[Runway::width] = runwayTableModel->fieldIndex(Runway::width);
    runwayIndexes[Runway::airportid] = runwayTableModel->fieldIndex(Runway::airportid);
    runwayIndexes[Runway::widget] = runwayTableModel->fieldIndex(Runway::widget);

    runwayView->setModel(runwayTableModel);
    runwayView->horizontalHeader()->moveSection(runwayView->horizontalHeader()->visualIndex(runwayIndexes[Runway::name]), 0);
    runwayView->horizontalHeader()->moveSection(runwayView->horizontalHeader()->visualIndex(runwayIndexes[Runway::lat]), 1);
    runwayView->horizontalHeader()->moveSection(runwayView->horizontalHeader()->visualIndex(runwayIndexes[Runway::lon]), 2);
    runwayView->horizontalHeader()->moveSection(runwayView->horizontalHeader()->visualIndex(runwayIndexes[Runway::length]), 3);
    runwayView->horizontalHeader()->moveSection(runwayView->horizontalHeader()->visualIndex(runwayIndexes[Runway::azimuth]), 4);
    runwayView->horizontalHeader()->moveSection(runwayView->horizontalHeader()->visualIndex(runwayIndexes[Runway::width]), 5);
    runwayView->horizontalHeader()->moveSection(runwayView->horizontalHeader()->visualIndex(runwayIndexes[Runway::widget]), 6);
    runwayView->setColumnHidden(runwayIndexes[Runway::id], true);
    runwayView->setColumnHidden(runwayIndexes[Runway::airportid], true);
    runwayView->setColumnHidden(runwayIndexes[Runway::widget], true);
    runwayView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    runwayDelegate = new RelModifyDelegate(this);
    runwayView->setItemDelegate(runwayDelegate);
    QMap<int, QVariant::Type> TypeMap;
    TypeMap[runwayIndexes[Runway::lat]] = QVariant::Double;
    TypeMap[runwayIndexes[Runway::lon]] = QVariant::Double;
    runwayDelegate->SetColsType(TypeMap);

    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    runwayView->setItemDelegateForColumn(runwayIndexes[Runway::lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    runwayView->setItemDelegateForColumn(runwayIndexes[Runway::lon], coordDelegateLon);

    connect(runwayTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(OnRunDataChanged()));
    connect(runwayTableModel, SIGNAL(beforeInsert(QSqlRecord&)), SLOT(OnRunBeforeInsert(QSqlRecord&)));
    connect(runwayView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , SLOT(OnRunSelect(QModelIndex)));

    // SSA view
    ssaTableModel = new SSAModel(this, mDatabase);
    ssaTableModel->setTable(Ssa::tableName);
    ssaTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ssaTableModel->setFilter("0 = -1");

    ssaIndexes[Ssa::id] = ssaTableModel->fieldIndex(Ssa::id);
    ssaIndexes[Ssa::runwayid] = ssaTableModel->fieldIndex(Ssa::runwayid);
    ssaIndexes[Ssa::name] = ssaTableModel->fieldIndex(Ssa::name);
    ssaIndexes[Ssa::type] = ssaTableModel->fieldIndex(Ssa::type);
    ssaIndexes[Ssa::transalt] = ssaTableModel->fieldIndex(Ssa::transalt);

    ssaTableModel->setRelation(ssaIndexes[Ssa::type], QSqlRelation(SsaType::tableName, SsaType::id, SsaType::name));

    ssaView->setModel(ssaTableModel);
    ssaView->horizontalHeader()->moveSection(ssaView->horizontalHeader()->visualIndex(ssaIndexes[Ssa::name]), 0);
    ssaView->horizontalHeader()->moveSection(ssaView->horizontalHeader()->visualIndex(ssaIndexes[Ssa::type]), 1);
    ssaView->horizontalHeader()->moveSection(ssaView->horizontalHeader()->visualIndex(ssaIndexes[Ssa::transalt]), 2);
    ssaView->setColumnHidden(ssaIndexes[Ssa::id], true);
    ssaView->setColumnHidden(ssaIndexes[Ssa::runwayid], true);
    ssaView->setColumnHidden(ssaIndexes[Ssa::transalt], true);
    ssaView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    ssaDelegate = new RelModifyDelegate(this);
    ssaView->setItemDelegate(ssaDelegate);

    connect(ssaTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(OnSSADataChanged()));
    connect(ssaTableModel, SIGNAL(beforeInsert(QSqlRecord&)), SLOT(OnSSABeforeInsert(QSqlRecord&)));
    connect(ssaView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , SLOT(OnSSASelect(QModelIndex)));

    // SSA point view
    ssaPointTableModel = new QSqlRelationalTableModel(this, mDatabase);
    ssaPointTableModel->setTable(SsaPoint::tableName);
    ssaPointTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ssaPointIndexes[SsaPoint::id] = ssaPointTableModel->fieldIndex(SsaPoint::id);
    ssaPointIndexes[SsaPoint::ssaid] = ssaPointTableModel->fieldIndex(SsaPoint::ssaid);
    ssaPointIndexes[SsaPoint::pointid] = ssaPointTableModel->fieldIndex(SsaPoint::pointid);
    ssaPointIndexes[SsaPoint::sn] = ssaPointTableModel->fieldIndex(SsaPoint::sn);
    ssaPointIndexes[SsaPoint::flevel] = ssaPointTableModel->fieldIndex(SsaPoint::flevel);
    ssaPointIndexes[SsaPoint::speed] = ssaPointTableModel->fieldIndex(SsaPoint::speed);
    ssaPointIndexes[SsaPoint::type] = ssaPointTableModel->fieldIndex(SsaPoint::type);
    ssaPointIndexes[SsaPoint::course] = ssaPointTableModel->fieldIndex(SsaPoint::course);
    ssaPointIndexes[SsaPoint::turn] = ssaPointTableModel->fieldIndex(SsaPoint::turn);
    ssaPointIndexes[SsaPoint::alt] = ssaPointTableModel->fieldIndex(SsaPoint::alt);
    ssaPointIndexes[SsaPoint::flyover] = ssaPointTableModel->fieldIndex(SsaPoint::flyover);

    ssaPointTableModel->setRelation(ssaPointIndexes[SsaPoint::type], QSqlRelation(SsaPointType::tableName, SsaPointType::id, SsaPointType::name));
    ssaPointTableModel->setRelation(ssaPointIndexes[SsaPoint::pointid], QSqlRelation("point_view", "id", "name"));
    ssaPointTableModel->setRelation(ssaPointIndexes[SsaPoint::turn], QSqlRelation(SsaPointTurn::tableName, SsaPointTurn::id, SsaPointTurn::name));
    selectAll(ssaPointTableModel);

    ssaPointTableModel->setFilter("0 = 1");

    ssaPointView->setModel(ssaPointTableModel);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::sn]), 0);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::type]), 1);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::pointid]), 2);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::flevel]), 3);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::speed]), 4);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::course]), 5);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::turn]), 6);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::alt]), 7);
    ssaPointView->horizontalHeader()->moveSection(ssaPointView->horizontalHeader()->visualIndex(ssaPointIndexes[SsaPoint::flyover]), 8);
    ssaPointView->setColumnHidden(ssaPointIndexes[SsaPoint::id], true);
    ssaPointView->setColumnHidden(ssaPointIndexes[SsaPoint::ssaid], true);
    //ssaPointView->setColumnHidden(ssaPointIndexes[SsaPoint::flyover], true);
    ssaPointView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    ssaPointDelegate = new RelModifyDelegate(this);
    ssaPointView->setItemDelegate(ssaPointDelegate);

    connect(ssaPointTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(OnSSAPointDataChanged()));
    connect(ssaPointTableModel, SIGNAL(beforeInsert(QSqlRecord&)), this, SLOT(OnSSAPointBeforeInsert(QSqlRecord&)));

    schemeComboModel = new QSqlQueryModel(this);
    schemeComboModel->setQuery(
                "SELECT -1, '' "
                "UNION "
                "SELECT id, name FROM ssa_type", mDatabase);
    schemeComboBox->setModel(schemeComboModel);
    schemeComboBox->setModelColumn(1);

    connect(runwayView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , runwayView->horizontalHeader(), SLOT(resizeSections()));
    connect(ssaView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , ssaView->horizontalHeader(), SLOT(resizeSections()));
    connect(ssaPointView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , ssaPointView->horizontalHeader(), SLOT(resizeSections()));

    runwayView->installEventFilter(this);
    ssaView->installEventFilter(this);
    ssaPointView->installEventFilter(this);

    runwayView->setDragDropMode(QAbstractItemView::DropOnly);
    runwayView->setAcceptDrops(true);
    runwayView->setDropIndicatorShown(true);
    connect(runwayTableModel, SIGNAL(changedRunwaySSA(int,int,QList<int>)), this, SLOT(OnChangedRunwaySSA(int,int,QList<int>)), Qt::QueuedConnection);

    ssaView->setDragDropMode(QAbstractItemView::DragOnly);
    ssaView->setDragEnabled(true);

    QSqlTableModel *SsaPointTurnModel = new QSqlTableModel(this);
    SsaPointTurnModel->setTable(SsaPointTurn::tableName);
    SsaPointTurnModel->select();
    IDturn = 0;
    for (int i = 0; i < SsaPointTurnModel->rowCount(); i++){
        if (SsaPointTurnModel->index(i, 1).data().toString() == "Unknown"){
            IDturn = SsaPointTurnModel->index(i, 0).data().toInt();
        }
    }
    QSqlTableModel *SsaPointTypeModel = new QSqlTableModel(this);
    SsaPointTypeModel->setTable(SsaPointType::tableName);
    SsaPointTypeModel->select();
    IDtype = 0;
    for (int i = 0; i < SsaPointTypeModel->rowCount(); i++){
        if (SsaPointTypeModel->index(i, 1).data().toString() == "Unknown"){
            IDtype = SsaPointTypeModel->index(i, 0).data().toInt();
        }
    }
    QSqlTableModel *SsaPointModel = new QSqlTableModel(this);
    SsaPointModel->setTable("point_view");
    SsaPointModel->select();
    IDpoint = 0;
    for (int i = 0; i < SsaPointModel->rowCount(); i++){
        if (SsaPointModel->index(i, 3).data().toString() == "_NULL_"){
            IDpoint = SsaPointModel->index(i, 0).data().toInt();
        }
    }

    translate();
}

SsaEditor::~SsaEditor()
{
    SaveState(this);
}

void SsaEditor::translate()
{
    retranslateUi(this);

    runwayTableModel->setHeaderData(runwayIndexes[Runway::name], Qt::Horizontal, Runway::tr_name());
    runwayTableModel->setHeaderData(runwayIndexes[Runway::lat], Qt::Horizontal, Runway::tr_lat());
    runwayTableModel->setHeaderData(runwayIndexes[Runway::lon], Qt::Horizontal, Runway::tr_lon());
    runwayTableModel->setHeaderData(runwayIndexes[Runway::length], Qt::Horizontal, Runway::tr_length());
    runwayTableModel->setHeaderData(runwayIndexes[Runway::azimuth], Qt::Horizontal, Runway::tr_azimuth());
    runwayTableModel->setHeaderData(runwayIndexes[Runway::width], Qt::Horizontal, Runway::tr_width());

    ssaTableModel->setHeaderData(ssaIndexes[Ssa::name], Qt::Horizontal, Ssa::tr_name());
    ssaTableModel->setHeaderData(ssaIndexes[Ssa::type], Qt::Horizontal, Ssa::tr_type());
    ssaTableModel->setHeaderData(ssaIndexes[Ssa::transalt], Qt::Horizontal, Ssa::tr_transalt());

    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::sn], Qt::Horizontal, SsaPoint::tr_sn());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::type], Qt::Horizontal, SsaPoint::tr_type());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::pointid], Qt::Horizontal, SsaPoint::tr_pointid());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::flevel], Qt::Horizontal, SsaPoint::tr_flevel());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::speed], Qt::Horizontal, SsaPoint::tr_speed());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::course], Qt::Horizontal, SsaPoint::tr_course());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::turn], Qt::Horizontal, SsaPoint::tr_turn());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::alt], Qt::Horizontal, SsaPoint::tr_alt());
    ssaPointTableModel->setHeaderData(ssaPointIndexes[SsaPoint::flyover], Qt::Horizontal, SsaPoint::tr_flyover());
}

void SsaEditor::refresh()
{
    runwayTableModel->revertAll();
    selectAll(runwayTableModel);
    isRunwayChanged = false;
    ssaTableModel->revertAll();
    selectAll(ssaTableModel);
    isSsaChanged = false;
    ssaPointTableModel->revertAll();
    selectAll(ssaPointTableModel);
    isSsaChanged = false;
    operButtons->setEnabled(false);
}

void SsaEditor::updateFirName()
{
    firLabel->setText(tr("Current FIR: %1").arg(firName()));
}

void SsaEditor::setFirId()
{
    if (isFirValid()) {
        airportComboModel->setQuery(QString("SELECT airport.id, airport.name from airport, zone_airport "
                                            "WHERE zone_airport.airport_id = airport.id AND "
                                            "zone_airport.zone_id = %1 ORDER BY airport.name").arg(firId()), mDatabase);
        airportsComboBox->setModelColumn(1);
        airportsComboBox->setCurrentIndex(0);
    } else {
        airportsComboBox->clear();
    }
}

void SsaEditor::updateSsaFilter()
{
    if (runwayId) {
        QStringList filter(QString("%1 = %2").arg(Ssa::runwayid).arg(runwayId));
        if (scheme >= 0)
            filter << QString("%1 = %2").arg(Ssa::type).arg(scheme);
        ssaTableModel->setFilter(filter.join(" AND "));
    } else {
        ssaTableModel->setFilter("0 = -1");
    }

    selectAll(ssaTableModel);
}

void SsaEditor::closeEvent(QCloseEvent *pEvent)
{
    if (operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?")
                                     , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            try
            {
                QtAutoTransaction dbTrans(mDatabase);
                if (isRunwayChanged && !runwayTableModel->submitAll())
                    throw(runwayTableModel->lastError().text());
                if (isSsaChanged && !ssaTableModel->submitAll())
                    throw(ssaTableModel->lastError().text());
                if (isSsaPointChanged && !ssaPointTableModel->submitAll())
                    throw(ssaPointTableModel->lastError().text());
                dbTrans.commit();
                pEvent->accept();
            }
            catch(const QString &err)
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

void SsaEditor::changeEvent(QEvent *event)
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

void SsaEditor::airportChanged(int iIndex)
{
    if(iIndex != -1)
        runwayTableModel->setFilter(QString("%1 = %2")
                                    .arg(Runway::airportid)
                                    .arg(airportsComboBox->model()->index(iIndex, 0).data().toString()));
    else
        runwayTableModel->setFilter("1=0");
    selectAll(runwayTableModel);
}

void SsaEditor::schemeChanged(int iIndex)
{
    scheme = schemeComboBox->model()->index(iIndex, 0).data().toInt();
    updateSsaFilter();
}

void SsaEditor::addRunway()
{
    int iRow = 0;
    runwayTableModel->insertRow(iRow);
    QModelIndex qmIndex = runwayTableModel->index(iRow, runwayIndexes[Runway::name]);
    runwayView->setCurrentIndex(qmIndex);
    runwayView->edit(qmIndex);
    OnRunDataChanged();
}

void SsaEditor::deleteRunway()
{
    QModelIndexList selectList = runwayView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && runwayView->currentIndex().isValid())
        selectList.append(runwayView->currentIndex());
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == runwayTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        runwayTableModel->removeRow(rowForDelte);
        OnRunDataChanged();
    }
}

void SsaEditor::OnRunBeforeInsert(QSqlRecord& qRecord)
{
    qRecord.setValue(runwayIndexes[Runway::airportid], airportsComboBox->model()->index(airportsComboBox->currentIndex(), 0).data().toUInt());
    qRecord.setGenerated(runwayIndexes[Runway::airportid], true);
}

void SsaEditor::OnRunSelect(const QModelIndex& qIndex)
{
    QVariant qID;
    if (qIndex.isValid())
        qID = runwayTableModel->record(qIndex.row()).value("id");

    if (qIndex.isValid() && !qID.isNull())
    {
        runwayId = qID.toUInt();
        addSsaButton->setEnabled(true);
    }
    else
    {
        runwayId = 0;
        addSsaButton->setEnabled(false);
    }
    updateSsaFilter();
}

void SsaEditor::addSsa()
{
    int iRow = 0;
    ssaTableModel->insertRow(iRow);
    ssaTableModel->setData(ssaTableModel->index(iRow, ssaIndexes[Ssa::type]), 0);
    ssaTableModel->setData(ssaTableModel->index(iRow, ssaIndexes[Ssa::transalt]), 0);
    QModelIndex qmIndex = ssaTableModel->index(iRow, ssaIndexes[Ssa::name]);
    ssaView->setCurrentIndex(qmIndex);
    ssaView->edit(qmIndex);
    OnSSADataChanged();
}

void SsaEditor::deleteSsa()
{
    QModelIndexList selectList = ssaView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ssaView->currentIndex().isValid())
        selectList.append(ssaView->currentIndex());
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == ssaTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        ssaTableModel->removeRow(rowForDelte);
        OnSSADataChanged();
    }
}

void SsaEditor::OnSSABeforeInsert(QSqlRecord& qRecord)
{
    qRecord.setValue(ssaIndexes[Ssa::runwayid], runwayId);
    qRecord.setGenerated(ssaIndexes[Ssa::runwayid], true);
}

void SsaEditor::OnSSASelect(const QModelIndex& qIndex)
{
    QVariant qID;
    if (qIndex.isValid())
        qID = ssaTableModel->record(qIndex.row()).value("id");

    if (qIndex.isValid() && !qID.isNull())
    {
        ssaId = qID.toUInt();
        ssaPointTableModel->setFilter(SsaPoint::ssaid + " = " + qID.toString());

        QSqlTableModel* pRelationModel = ssaPointTableModel->relationModel(ssaPointIndexes[SsaPoint::pointid]);
        pRelationModel->setFilter(QString("point_view.type = 100 OR "
                                          "((point_view.type != %1) AND point_view.id IN "
                                          "(SELECT zone_point.point_id FROM zone_point WHERE zone_id = %2))")
                                  .arg(0) .arg(firId()));
        pRelationModel->setSort(ssaPointTableModel->relationModel(ssaPointIndexes[SsaPoint::pointid])->fieldIndex(Point::name), Qt::AscendingOrder);
        selectAll(pRelationModel);
        addSsaPointButton->setEnabled(true);
    }
    else
    {
        ssaId = 0;
        ssaPointTableModel->setFilter("0 = -1");
        addSsaPointButton->setEnabled(false);
    }
    selectAll(ssaPointTableModel);
}

void SsaEditor::addSsaPoint()
{
    int iRow = 0;
    ssaPointTableModel->insertRow(iRow);
    ssaPointTableModel->setRelation(ssaPointIndexes[SsaPoint::turn], QSqlRelation(SsaPointTurn::tableName, SsaPointTurn::id, SsaPointTurn::name));

    ssaPointTableModel->setData(ssaPointTableModel->index(iRow, ssaPointIndexes[SsaPoint::type]), IDtype);
    ssaPointTableModel->setData(ssaPointTableModel->index(iRow, ssaPointIndexes[SsaPoint::pointid]), IDpoint);
    ssaPointTableModel->setData(ssaPointTableModel->index(iRow, ssaPointIndexes[SsaPoint::turn]), IDturn);
    ssaPointTableModel->setData(ssaPointTableModel->index(iRow, ssaPointIndexes[SsaPoint::flyover]), 0);

    QModelIndex qmIndex = ssaPointTableModel->index(iRow, ssaPointIndexes[SsaPoint::sn]);
    ssaPointView->setCurrentIndex(qmIndex);
    ssaPointView->edit(qmIndex);
    OnSSAPointDataChanged();
}

void SsaEditor::deleteSsaPoint()
{
    QModelIndexList selectList = ssaPointView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ssaPointView->currentIndex().isValid())
        selectList.append(ssaPointView->currentIndex());
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == ssaPointTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        ssaPointTableModel->removeRow(rowForDelte);
        OnSSAPointDataChanged();
    }
}

void SsaEditor::OnSSAPointBeforeInsert(QSqlRecord& qRecord)
{
    qRecord.setValue(ssaPointIndexes[SsaPoint::ssaid], ssaId);
    qRecord.setGenerated(ssaPointIndexes[SsaPoint::ssaid], true);
}

void SsaEditor::onOperButtons(QAbstractButton* pButton)
{
    switch(operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        try
        {
            QVariant runwaySelectedRow = runwayTableModel->data(runwayTableModel->index(runwayView->selectionModel()->currentIndex().row(), runwayIndexes[Runway::name]));
            QVariant ssaSelectedRow = ssaTableModel->data(ssaTableModel->index(ssaView->selectionModel()->currentIndex().row(), ssaIndexes[Ssa::name]));
            QVariant ssaPointSelectedRow = ssaPointTableModel->data(ssaPointTableModel->index(ssaPointView->selectionModel()->currentIndex().row(), ssaPointIndexes[SsaPoint::sn]));
            QtAutoTransaction dbTrans(mDatabase);
            if (isRunwayChanged && !runwayTableModel->submitAll())
                throw(runwayTableModel->lastError().text());
            if (isSsaChanged && !ssaTableModel->submitAll())
                throw(ssaTableModel->lastError().text());
            if (isSsaPointChanged && !ssaPointTableModel->submitAll())
                throw(ssaPointTableModel->lastError().text());
            dbTrans.commit();
            isRunwayChanged = false;
            isSsaChanged = false;
            isSsaPointChanged = false;
            operButtons->setEnabled(false);
            while(runwayTableModel->canFetchMore())
                runwayTableModel->fetchMore();
            QModelIndexList runwayIndexList = runwayTableModel->match(runwayTableModel->index(0, runwayIndexes[Runway::name]), Qt::DisplayRole, runwaySelectedRow, 1, Qt::MatchExactly);
            while(ssaTableModel->canFetchMore())
                ssaTableModel->fetchMore();
            QModelIndexList ssaIndexList = ssaTableModel->match(ssaTableModel->index(0, ssaIndexes[Ssa::name]), Qt::DisplayRole, ssaSelectedRow, 1, Qt::MatchExactly);
            while(ssaPointTableModel->canFetchMore())
                ssaPointTableModel->fetchMore();
            QModelIndexList ssaPointIndexList = ssaPointTableModel->match(ssaPointTableModel->index(0, ssaPointIndexes[SsaPoint::sn]), Qt::DisplayRole, ssaPointSelectedRow, 1, Qt::MatchExactly);
            if(runwayIndexList.count() && !ssaIndexList.count() && !ssaPointIndexList.count())
            {
                runwayView->selectRow(runwayIndexList.first().row());
                runwayView->scrollTo(runwayIndexList.first());
            }
            if(ssaIndexList.count() && !ssaPointIndexList.count())
            {
                ssaView->selectRow(ssaIndexList.first().row());
                ssaView->scrollTo(ssaIndexList.first());
            }
            if(ssaPointIndexList.count())
            {
                ssaPointView->selectRow(ssaPointIndexList.first().row());
                ssaPointView->scrollTo(ssaPointIndexList.first());
            }
        }
        catch(const QString &err)
        {
            qCritical() << err;
            QMessageBox::critical(this, tr("Error"), err);
        }
        break;
    case QDialogButtonBox::RejectRole:
    case QDialogButtonBox::DestructiveRole:
        if (isRunwayChanged)
        {
            runwayTableModel->revertAll();
            selectAll(runwayTableModel);
            isRunwayChanged = false;
        }
        if (isSsaChanged)
        {
            ssaTableModel->revertAll();
            selectAll(ssaTableModel);
            isSsaChanged = false;
        }
        if (isSsaPointChanged)
        {
            ssaPointTableModel->revertAll();
            selectAll(ssaPointTableModel);
            isSsaChanged = false;
        }
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }
}

void SsaEditor::OnRunDataChanged()
{
    isRunwayChanged = true;
    operButtons->setEnabled(true);
}

void SsaEditor::OnSSADataChanged()
{
    isSsaChanged = true;
    operButtons->setEnabled(true);
}

void SsaEditor::OnSSAPointDataChanged()
{
    isSsaPointChanged = true;
    operButtons->setEnabled(true);
}

void SsaEditor::OnChangedRunwaySSA(int newRunwayID, int oldRunwayID, QList<int> listSSA)
{
    if(newRunwayID && oldRunwayID && (newRunwayID != oldRunwayID) && listSSA.size())
    {
        try
        {
            QModelIndex curInd = runwayView->currentIndex();
            QString sSSA;

            QSet<int> setSSA = QSet<int>::fromList(listSSA);
            foreach(int iSSA, setSSA)
            {
                if(!sSSA.isEmpty()) sSSA += ",";
                sSSA += QString::number(iSSA);
            }
            QString sQuery = QString("UPDATE %1 SET %2 = %3 WHERE %4 IN (%5)")
                    .arg(Ssa::tableName)
                    .arg(Ssa::runwayid)
                    .arg(newRunwayID)
                    .arg(Ssa::id)
                    .arg(sSSA);

            QtAutoTransaction dbTrans(mDatabase);
            QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDatabase);
            query->Prepare(sQuery);
            if(!query->exec())
                throw query->lastError().text();
            dbTrans.commit();
            OnRunSelect(curInd);
        }
        catch(const QString &err)
        {
            qCritical() << err;
            QMessageBox::critical(this, tr("Error"), err);
        }
    }
}
