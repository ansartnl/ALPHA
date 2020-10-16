#include "sectoreditor.h"

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
#include <QtSql/QSqlField>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlRelationalTableModel>

using namespace dbscheme;

SectorEditor::SectorEditor(QWidget *parent, const QSqlDatabase &db) :
    BaseEditor(parent), mDatabase(db), isSectorModifyed(false)
{
    setupUi(this);

    QPushButton *saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    connect(addSectorButton, SIGNAL(clicked()), this, SLOT(addSector()));
    connect(deleteSectorButton, SIGNAL(clicked()), this, SLOT(deleteSector()));
    connect(addPointButton, SIGNAL(clicked()), this, SLOT(addEdgePoint()));
    connect(deletePointButton, SIGNAL(clicked()), this, SLOT(deleteEdgePoint()));
    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));

    // sector editor
    sectorTableModel = new QSqlTableModel(this, mDatabase);
    sectorTableModel->setTable(Zone::tableName);
    sectorTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sectorTableModel->setFilter("0 = -1");
    selectAll(sectorTableModel);

    connect(sectorTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onSectorDataChanged()));

    sectorView->setModel(sectorTableModel);
    sectorView->horizontalHeader()->moveSection(sectorView->horizontalHeader()->visualIndex(sectorTableModel->fieldIndex(Zone::name)), 0);
    sectorView->horizontalHeader()->moveSection(sectorView->horizontalHeader()->visualIndex(sectorTableModel->fieldIndex(Zone::idname)), 1);
    sectorView->horizontalHeader()->moveSection(sectorView->horizontalHeader()->visualIndex(sectorTableModel->fieldIndex(Zone::id)), 2);
    sectorView->setColumnHidden(sectorTableModel->fieldIndex(Zone::id), true);
    sectorView->setColumnHidden(sectorTableModel->fieldIndex(Zone::type), true);
    sectorView->setColumnHidden(sectorTableModel->fieldIndex(Zone::parentid), true);
    sectorView->setColumnHidden(sectorTableModel->fieldIndex(Zone::lablename), true);
    sectorView->setColumnHidden(sectorTableModel->fieldIndex(Zone::frequency), true);
    sectorView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    UpperCaseDelegate *nameDelegate = new UpperCaseDelegate(4, this);
    sectorView->setItemDelegateForColumn(sectorTableModel->fieldIndex(Zone::idname), nameDelegate);

    connect(sectorView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onSectorChanged(const QModelIndex&)));

    RelModifyDelegate *sectorDelegate = new RelModifyDelegate(sectorView);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::id), QVariant::Char);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::name), QVariant::Char);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::type), QVariant::Int);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::idname), QVariant::Char);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::parentid), QVariant::Int);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::lowlimit), QVariant::Int);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::uplimit), QVariant::Int);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::shortname), QVariant::Char);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::lablename), QVariant::Char);
    sectorDelegate->setColumn(sectorTableModel->fieldIndex(Zone::frequency), QVariant::Int);
    sectorView->setItemDelegate(sectorDelegate);

    // edge points editor
    edgeTableModel = new QSqlRelationalTableModel(this, mDatabase);
    edgeTableModel->setTable(ZoneBoundary::tableName);
    edgeTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    connect(edgeTableModel, SIGNAL(beforeInsert(QSqlRecord&)), this, SLOT(beforeEdgeInsert(QSqlRecord&)));

    edgeView->setModel(edgeTableModel);
    edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::sn)), 0);
    edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::type)), 1);
    edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::lat)), 2);
    edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::lon)), 3);
    edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::arclat)), 4);
    edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::arclon)), 5);
    edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::arcdist)), 6);
        edgeView->horizontalHeader()->moveSection(edgeView->horizontalHeader()->visualIndex(edgeTableModel->fieldIndex(ZoneBoundary::zoneid)), 7);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::id), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::zoneid), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::type), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::arclat), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::arclon), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::arcdist), true);
    edgeView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    // delegate for editing edge points
    edgeDelegate = new RelModifyDelegate(edgeView);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::id), QVariant::Char);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::type), QVariant::Char);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::sn), QVariant::Int);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::lat), QVariant::Double);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::lon), QVariant::Double);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::arclat), QVariant::Double);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::arclon), QVariant::Double);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::arcdist), QVariant::Double);
    edgeDelegate->setColumn(edgeTableModel->fieldIndex(ZoneBoundary::zoneid), QVariant::Int);

    // for default value = 71
//    edgeTableModel->setRelation(edgeTableModel->fieldIndex(ZoneBoundary::type), QSqlRelation(ZoneBoundaryPointType::tableName, ZoneBoundaryPointType::id, ZoneBoundaryPointType::name));

    edgeView->setItemDelegate(edgeDelegate);

    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    edgeView->setItemDelegateForColumn(edgeTableModel->fieldIndex(ZoneBoundary::lat), coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    edgeView->setItemDelegateForColumn(edgeTableModel->fieldIndex(ZoneBoundary::lon), coordDelegateLon);
    CoordDelegate* coordDelegateArcLat = new CoordDelegate(CoordEdit::Latitude, this);
    edgeView->setItemDelegateForColumn(edgeTableModel->fieldIndex(ZoneBoundary::arclat), coordDelegateArcLat);
    CoordDelegate* coordDelegateArcLon = new CoordDelegate(CoordEdit::Longitude, this);
    edgeView->setItemDelegateForColumn(edgeTableModel->fieldIndex(ZoneBoundary::arclon), coordDelegateArcLon);

    connect(edgeTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onEdgeDataChanged()));

    connect(edgeView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onCurrentEdgeChanged(const QModelIndex&)));

    connect(sectorView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , sectorView->horizontalHeader(), SLOT(resizeSections()));
    connect(edgeView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , edgeView->horizontalHeader(), SLOT(resizeSections()));

    sectorView->installEventFilter(this);
    edgeView->installEventFilter(this);

    translate();
}

SectorEditor::~SectorEditor()
{
    SaveState(this);
}

void SectorEditor::translate()
{
    retranslateUi(this);

    sectorTableModel->setHeaderData(sectorTableModel->fieldIndex(Zone::name), Qt::Horizontal, Zone::tr_name());
    sectorTableModel->setHeaderData(sectorTableModel->fieldIndex(Zone::idname), Qt::Horizontal, Zone::tr_idname());
    sectorTableModel->setHeaderData(sectorTableModel->fieldIndex(Zone::lowlimit), Qt::Horizontal, Zone::tr_lowlimit());
    sectorTableModel->setHeaderData(sectorTableModel->fieldIndex(Zone::uplimit), Qt::Horizontal, Zone::tr_uplimit());
    sectorTableModel->setHeaderData(sectorTableModel->fieldIndex(Zone::shortname), Qt::Horizontal, Zone::tr_shortname());
    sectorTableModel->setHeaderData(sectorTableModel->fieldIndex(Zone::lablename), Qt::Horizontal, Zone::tr_lablename());
    sectorTableModel->setHeaderData(sectorTableModel->fieldIndex(Zone::frequency), Qt::Horizontal, Zone::tr_frequency());

    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::sn), Qt::Horizontal, ZoneBoundary::tr_sn());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::type), Qt::Horizontal, ZoneBoundary::tr_type());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::lat), Qt::Horizontal, ZoneBoundary::tr_lat());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::lon), Qt::Horizontal, ZoneBoundary::tr_lon());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::arclat), Qt::Horizontal, ZoneBoundary::tr_arclat());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::arclon), Qt::Horizontal, ZoneBoundary::tr_arclon());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::arcdist), Qt::Horizontal, ZoneBoundary::tr_arcdist());
}

void SectorEditor::refresh()
{
    edgeTableModel->revertAll();
    selectAll(edgeTableModel);
    sectorTableModel->revertAll();
    selectAll(sectorTableModel);
    isSectorModifyed = false;
    operButtons->setEnabled(false);
}

void SectorEditor::updateFirName()
{
    firLabel->setText(tr("Current FIR: %1").arg(firName()));
}

void SectorEditor::setFirId()
{
    deleteSectorButton->setEnabled(isFirValid());

    if (isFirValid())
        sectorTableModel->setFilter(QString("%1 = %2 AND (%3 = %4)")
                                    .arg(Zone::type)
                                    .arg(2)
                                    .arg(Zone::parentid)
                                    .arg(firId()));
    else
        sectorTableModel->setFilter("1=0");
    selectAll(sectorTableModel);
    isSectorModifyed = false;
    operButtons->setEnabled(false);
}

void SectorEditor::closeEvent(QCloseEvent *pEvent)
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
                    QtAutoTransaction dbTrans(mDatabase);
                    if (!edgeTableModel->submitAll())
                        throw (edgeTableModel->lastError().text());
                    if (isSectorModifyed && !sectorTableModel->submitAll())
                        throw (sectorTableModel->lastError().text());
                    dbTrans.commit();
                    isSectorModifyed = false;
                    operButtons->setEnabled(false);
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

    } else
        pEvent->accept();
}

void SectorEditor::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        translate();
        updateFirName();
        break;
    default:
        break;
    }
}

void SectorEditor::addSector()
{
    int iRow = sectorTableModel->rowCount();
    sectorTableModel->insertRow(iRow);
    QModelIndex qmIndex = sectorTableModel->index(iRow, sectorTableModel->fieldIndex(Zone::id));
    sectorTableModel->setData(qmIndex, QVariant());
    qmIndex = sectorTableModel->index(iRow, sectorTableModel->fieldIndex(Zone::type));
    sectorTableModel->setData(qmIndex, 2);
    qmIndex = sectorTableModel->index(iRow, sectorTableModel->fieldIndex(Zone::parentid));
    sectorTableModel->setData(qmIndex, firId());
    qmIndex = sectorTableModel->index(iRow, 1);
    sectorView->setCurrentIndex(qmIndex);
    sectorView->edit(qmIndex);
    isSectorModifyed = true;
    operButtons->setEnabled(true);
}

void SectorEditor::deleteSector()
{
    QModelIndexList selectList = sectorView->selectionModel()->selectedRows();
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == sectorTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        sectorTableModel->removeRow(rowForDelte);
        isSectorModifyed = true;
    }
    if (isSectorModifyed)
        operButtons->setEnabled(true);
}

void SectorEditor::onOperButtons(QAbstractButton* pButton)
{
    switch(operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        {
        QVariant sectorSelectedRow = sectorTableModel->data(
                    sectorTableModel->index(
                        sectorView->selectionModel()->currentIndex().row()
                        , sectorTableModel->fieldIndex(Zone::idname)));
        QVariant edgeSelectedRow = edgeTableModel->data(
                    edgeTableModel->index(
                        edgeView->selectionModel()->currentIndex().row()
                        , edgeTableModel->fieldIndex(ZoneBoundary::sn)));
            try
            {
                QtAutoTransaction dbTrans(mDatabase);
                if (!edgeTableModel->submitAll())
                    throw (edgeTableModel->lastError().text());
                if (isSectorModifyed && !sectorTableModel->submitAll())
                    throw (sectorTableModel->lastError().text());
                dbTrans.commit();
                isSectorModifyed = false;
                operButtons->setEnabled(false);
                while(sectorTableModel->canFetchMore())
                    sectorTableModel->fetchMore();
                QModelIndexList sectorIndexList = sectorTableModel->match(
                            sectorTableModel->index(0, sectorTableModel->fieldIndex(Zone::idname))
                            , Qt::DisplayRole, sectorSelectedRow, 1, Qt::MatchExactly);
                while(edgeTableModel->canFetchMore())
                    edgeTableModel->fetchMore();
                QModelIndexList edgeIndexList = edgeTableModel->match(
                            edgeTableModel->index(0, edgeTableModel->fieldIndex(ZoneBoundary::sn))
                            , Qt::DisplayRole, edgeSelectedRow, 1, Qt::MatchExactly);
                if(sectorIndexList.count())
                {
                    sectorView->selectRow(sectorIndexList.first().row());
                    sectorView->scrollTo(sectorIndexList.first());
                }
                if(edgeIndexList.count())
                {
                    edgeView->selectRow(edgeIndexList.first().row());
                    edgeView->scrollTo(edgeIndexList.first());
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
        edgeTableModel->revertAll();
        selectAll(edgeTableModel);
        if (isSectorModifyed)
        {
            sectorTableModel->revertAll();
            selectAll(sectorTableModel);
        }
        isSectorModifyed = false;
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }
}

void SectorEditor::beforeEdgeInsert(QSqlRecord& qRecord)
{
    qRecord.setValue(ZoneBoundary::id, QVariant());
    qRecord.setValue(ZoneBoundary::zoneid
                     , sectorTableModel->index(sectorView->selectionModel()->currentIndex().row()
                                               , sectorTableModel->fieldIndex(Zone::id)).data());
}

void SectorEditor::onSectorChanged(const QModelIndex& modInd)
{
    int colZoneid = sectorTableModel->fieldIndex(Zone::id);
    if (modInd.isValid() && !sectorTableModel->index(modInd.row(), colZoneid).data().isNull())
    {
        addPointButton->setEnabled(true);
        deleteSectorButton->setEnabled(true);
        edgeTableModel->setFilter(
                    QString("%1 = %2")
                    .arg(ZoneBoundary::zoneid
                         , sectorTableModel->index(modInd.row(), colZoneid).data().toString())
                    );
    }
    else
    {
        addPointButton->setEnabled(false);
        deleteSectorButton->setEnabled(false);
        edgeTableModel->setFilter("0 = -1");
    }
    operButtons->setEnabled(isSectorModifyed);
    selectAll(edgeTableModel);
}

void SectorEditor::onCurrentEdgeChanged(const QModelIndex& curInd)
{
    deletePointButton->setEnabled(curInd.isValid());
}

void SectorEditor::addEdgePoint()
{
    int iRow = edgeTableModel->rowCount();
    edgeTableModel->insertRow(iRow);
    QSqlRecord qRec = edgeTableModel->record(iRow);
    edgeTableModel->setRecord(iRow, qRec);
    QModelIndex t_index = edgeTableModel->index(iRow,edgeTableModel->fieldIndex(ZoneBoundary::type));
    edgeTableModel->setData(t_index,71);
    t_index = edgeTableModel->index(iRow,edgeTableModel->fieldIndex(ZoneBoundary::zoneid));
    edgeTableModel->setData(t_index,sectorTableModel->index(sectorView->selectionModel()->currentIndex().row(), sectorTableModel->fieldIndex(Zone::id)).data());
    QModelIndex qmIndex = edgeTableModel->index(iRow, 2);
    edgeView->setCurrentIndex(qmIndex);
    edgeView->edit(qmIndex);
    operButtons->setEnabled(true);
}

void SectorEditor::deleteEdgePoint()
{
    QModelIndexList selectList = edgeView->selectionModel()->selectedRows();
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == edgeTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        edgeTableModel->removeRow(rowForDelte);
        operButtons->setEnabled(true);
    }
    if (edgeTableModel)
        operButtons->setEnabled(true);
}

void SectorEditor::onSectorDataChanged()
{
    isSectorModifyed = true;
    operButtons->setEnabled(true);
}

void SectorEditor::onEdgeDataChanged()
{
    operButtons->setEnabled(true);
}
