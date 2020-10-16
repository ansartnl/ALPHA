#include "fireditor.h"

#include "dbscheme.h"
#include "widgetsaver.h"
#include "misc.h"

#include "coorddelegate.h"
#include "qtautotransaction.h"
#include "Vincenty.h"

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

FirEditor::FirEditor(QWidget *parent, const QSqlDatabase &db) :
    BaseEditor(parent), mDatabase(db), isFirModifyed(false)
{
    setupUi(this);

    QPushButton* saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    RestoreState(this);

    connect(addFirButton, SIGNAL(clicked()), this, SLOT(addFir()));
    connect(deleteFirButton, SIGNAL(clicked()), this, SLOT(deleteFir()));
    connect(addPointButton, SIGNAL(clicked()), this, SLOT(addEdgePoint()));
    connect(deletePointButton, SIGNAL(clicked()), this, SLOT(deleteEdgePoint()));
    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));

    // GroupBox1
    firTableModel = new QSqlTableModel(this, mDatabase);
    firTableModel->setTable(Zone::tableName);
    firTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    firTableModel->setFilter(QString("%1 = %2").arg(Zone::type).arg(1));
    selectAll(firTableModel);

    connect(firTableModel, SIGNAL(beforeInsert(QSqlRecord&)), this, SLOT(beforeFIRInsert(QSqlRecord&)));

    firView->setModel(firTableModel);
    firView->horizontalHeader()->moveSection(firView->horizontalHeader()->visualIndex(firTableModel->fieldIndex(Zone::name)), 0);
    firView->horizontalHeader()->moveSection(firView->horizontalHeader()->visualIndex(firTableModel->fieldIndex(Zone::idname)), 1);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::id), true);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::type), true);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::parentid), true);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::lowlimit), true);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::uplimit), true);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::shortname), true);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::lablename), true);
    firView->setColumnHidden(firTableModel->fieldIndex(Zone::frequency), true);
    firView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    UpperCaseDelegate *nameDelegate = new UpperCaseDelegate(4, this);
    firView->setItemDelegateForColumn(firTableModel->fieldIndex(Zone::idname), nameDelegate);

    connect(firTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onFirDataChanged()));

    // GroupBox2
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
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::id), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::zoneid), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::type), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::arclat), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::arclon), true);
    edgeView->setColumnHidden(edgeTableModel->fieldIndex(ZoneBoundary::arcdist), true);
    edgeView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    edgeDelegate = new RelModifyDelegate(edgeView);
    QMap<int, QVariant::Type> TypeMap;

    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::id)] = QVariant::Char;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::type)] = QVariant::Char;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::sn)] = QVariant::Int;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::lat)] = QVariant::Double;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::lon)] = QVariant::Double;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::arclat)] = QVariant::Double;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::arclon)] = QVariant::Double;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::arcdist)] = QVariant::Double;
    TypeMap[edgeTableModel->fieldIndex(ZoneBoundary::zoneid)] = QVariant::Int;

    // for default value = 71
//    edgeTableModel->setRelation(edgeTableModel->fieldIndex(ZoneBoundary::type), QSqlRelation(ZoneBoundaryPointType::tableName, ZoneBoundaryPointType::id, ZoneBoundaryPointType::name));

    edgeDelegate->SetColsType(TypeMap);
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

    connect(firView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onFirChangedOld(const QModelIndex&)));

    connect(firView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , firView->horizontalHeader(), SLOT(resizeSections()));
    connect(edgeView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , edgeView->horizontalHeader(), SLOT(resizeSections()));

    firView->installEventFilter(this);
    edgeView->installEventFilter(this);

    translate();
}

FirEditor::~FirEditor()
{
    SaveState(this);
}

void FirEditor::translate()
{
    retranslateUi(this);

    firTableModel->setHeaderData(firTableModel->fieldIndex(Zone::name), Qt::Horizontal, Zone::tr_name());
    firTableModel->setHeaderData(firTableModel->fieldIndex(Zone::idname), Qt::Horizontal, Zone::tr_idname());

    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::sn), Qt::Horizontal, ZoneBoundary::tr_sn());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::type), Qt::Horizontal, ZoneBoundary::tr_type());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::lat), Qt::Horizontal, ZoneBoundary::tr_lat());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::lon), Qt::Horizontal, ZoneBoundary::tr_lon());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::arclat), Qt::Horizontal, ZoneBoundary::tr_arclat());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::arclon), Qt::Horizontal, ZoneBoundary::tr_arclon());
    edgeTableModel->setHeaderData(edgeTableModel->fieldIndex(ZoneBoundary::arcdist), Qt::Horizontal, ZoneBoundary::tr_arcdist());
}

void FirEditor::closeEvent(QCloseEvent* pEvent)
{
    if (operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            {
                try
                {
                    QtAutoTransaction dbTrans(mDatabase);
                    if (!edgeTableModel->submitAll())
                        throw (edgeTableModel->lastError().text());
                    if (isFirModifyed && !firTableModel->submitAll())
                        throw (firTableModel->lastError().text());
                    dbTrans.commit();
                    isFirModifyed = false;
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

void FirEditor::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        translate();
        break;
    default:
        break;
    }
}

void FirEditor::addFir()
{
    int iRow = firTableModel->rowCount();
    firTableModel->insertRow(iRow);
    QModelIndex qmIndex = firTableModel->index(iRow, 1);
    firView->setCurrentIndex(qmIndex);
    firView->edit(qmIndex);
    isFirModifyed = true;
    operButtons->setEnabled(true);
}

void FirEditor::deleteFir()
{
    QModelIndexList selectList = firView->selectionModel()->selectedRows();
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == firTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        firTableModel->removeRow(rowForDelte);
        isFirModifyed = true;
    }
    if (isFirModifyed)
        operButtons->setEnabled(true);
}

void FirEditor::onOperButtons(QAbstractButton* pButton)
{
    switch(operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        {
        QVariant firSelectedRow = firTableModel->data(firTableModel->index(firView->selectionModel()->currentIndex().row(), firTableModel->fieldIndex(Zone::idname)));
        QVariant edgeSelectedRow = edgeTableModel->data(edgeTableModel->index(edgeView->selectionModel()->currentIndex().row(), edgeTableModel->fieldIndex(ZoneBoundary::sn)));
            try
            {
                QtAutoTransaction dbTrans(mDatabase);
                if (!edgeTableModel->submitAll())
                    throw (edgeTableModel->lastError().text());
                if (isFirModifyed && !firTableModel->submitAll())
                    throw (firTableModel->lastError().text());
                dbTrans.commit();
                isFirModifyed = false;
                operButtons->setEnabled(false);
                while(firTableModel->canFetchMore())
                    firTableModel->fetchMore();
                QModelIndexList firIndexList = firTableModel->match(firTableModel->index(0, firTableModel->fieldIndex(Zone::idname)), Qt::DisplayRole, firSelectedRow, 1, Qt::MatchExactly);
                while(edgeTableModel->canFetchMore())
                    edgeTableModel->fetchMore();
                QModelIndexList edgeIndexList = edgeTableModel->match(edgeTableModel->index(0, edgeTableModel->fieldIndex(ZoneBoundary::sn)), Qt::DisplayRole, edgeSelectedRow, 1, Qt::MatchExactly);
                if(firIndexList.count())
                {
                    firView->selectRow(firIndexList.first().row());
                    firView->scrollTo(firIndexList.first());
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
        if (isFirModifyed)
        {
            firTableModel->revertAll();
            selectAll(firTableModel);
        }
        isFirModifyed = false;
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }
}

void FirEditor::beforeFIRInsert(QSqlRecord& qRecord)
{
    qRecord.setValue(Zone::id, QVariant());
    qRecord.setValue(Zone::type, 1);
}

void FirEditor::beforeEdgeInsert(QSqlRecord& qRecord)
{
    qRecord.setValue(ZoneBoundary::id, QVariant());
    qRecord.setValue(ZoneBoundary::zoneid, firTableModel->index(firView->selectionModel()->currentIndex().row(), firTableModel->fieldIndex(Zone::id)).data());
}

void FirEditor::onFirChangedOld(const QModelIndex& modInd)
{
    int colZoneid = firTableModel->fieldIndex(Zone::id);
    if (modInd.isValid() && !firTableModel->index(modInd.row(), colZoneid).data().isNull())
    {
        addPointButton->setEnabled(true);
        edgeTableModel->setFilter(QString("%1 = %2").arg(ZoneBoundary::zoneid, firTableModel->index(modInd.row(), colZoneid).data().toString()));
    }
    else
    {
        addPointButton->setEnabled(false);
        edgeTableModel->setFilter("0 = -1");
    }
    operButtons->setEnabled(isFirModifyed);
    selectAll(edgeTableModel);
}

void FirEditor::addEdgePoint()
{
    int iRow = edgeTableModel->rowCount();
    edgeTableModel->insertRow(iRow);
    edgeTableModel->setData(edgeTableModel->index(iRow, edgeTableModel->fieldIndex(ZoneBoundary::type)), 71);
    edgeTableModel->setData(edgeTableModel->index(iRow, edgeTableModel->fieldIndex(ZoneBoundary::zoneid)),
                            firTableModel->index(firView->selectionModel()->currentIndex().row(), firTableModel->fieldIndex(Zone::id)).data());
    QModelIndex qmIndex = edgeTableModel->index(iRow, 2);
    edgeView->setCurrentIndex(qmIndex);
    edgeView->edit(qmIndex);
    operButtons->setEnabled(true);
}

void FirEditor::deleteEdgePoint()
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

void FirEditor::onFirDataChanged()
{
    isFirModifyed = true;
    operButtons->setEnabled(true);
}

void FirEditor::onEdgeDataChanged()
{
    operButtons->setEnabled(true);
}

void FirEditor::refresh()
{
    edgeTableModel->revertAll();
    selectAll(edgeTableModel);
    firTableModel->revertAll();
    selectAll(firTableModel);
    isFirModifyed = false;
    operButtons->setEnabled(false);
}
