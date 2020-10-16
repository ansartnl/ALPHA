#include "airporteditor.h"

#include "dbscheme.h"
#include "widgetsaver.h"

#include "coorddelegate.h"
#include "Database.h"
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
#include <QString>

using namespace dbscheme;

AirportEditor::AirportEditor(QWidget *parent, const QSqlDatabase &db) :
        BaseEditor(parent), mDatabase(db), isAirportsChanged(false), isAirportsByFirChanged(false)
{
    setupUi(this);

    QPushButton* saveButton = operButtons->button(QDialogButtonBox::Save);
    if(saveButton)
        saveButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    connect(addAirportButton, SIGNAL(clicked()), this, SLOT(addAirport()));
    connect(deleteAirportButton, SIGNAL(clicked()), this, SLOT(deleteAirport()));
    connect(addFirAirportButton, SIGNAL(clicked()), this, SLOT(addFirAirport()));
    connect(deleteFirAirportButton, SIGNAL(clicked()), this, SLOT(deleteFirAirport()));
    connect(operButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));

    RestoreState(this);

    // Airport view
    airportsTableModel = new QSqlTableModel(this, mDatabase);
    airportsTableModel->setTable(Airport::tableName);
    airportsTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    airportIndexes[Airport::id] = airportsTableModel->fieldIndex(Airport::id);
    airportIndexes[Airport::icao] = airportsTableModel->fieldIndex(Airport::icao);
    airportIndexes[Airport::name] = airportsTableModel->fieldIndex(Airport::name);
    airportIndexes[Airport::lat] = airportsTableModel->fieldIndex(Airport::lat);
    airportIndexes[Airport::lon] = airportsTableModel->fieldIndex(Airport::lon);
    airportIndexes[Airport::alt] = airportsTableModel->fieldIndex(Airport::alt);
    airportIndexes[Airport::declination] = airportsTableModel->fieldIndex(Airport::declination);

    selectAll(airportsTableModel);

    airportsView->setModel(airportsTableModel);
    airportsView->horizontalHeader()->moveSection(airportsView->horizontalHeader()->visualIndex(airportIndexes[Airport::name]), 0);
    airportsView->horizontalHeader()->moveSection(airportsView->horizontalHeader()->visualIndex(airportIndexes[Airport::icao]), 1);
    airportsView->horizontalHeader()->moveSection(airportsView->horizontalHeader()->visualIndex(airportIndexes[Airport::lat]), 2);
    airportsView->horizontalHeader()->moveSection(airportsView->horizontalHeader()->visualIndex(airportIndexes[Airport::lon]), 3);
    airportsView->horizontalHeader()->moveSection(airportsView->horizontalHeader()->visualIndex(airportIndexes[Airport::alt]), 4);
    airportsView->horizontalHeader()->moveSection(airportsView->horizontalHeader()->visualIndex(airportIndexes[Airport::declination]), 5);
    airportsView->setColumnHidden(airportIndexes[Airport::id], true);
    airportsView->setColumnHidden(airportIndexes[Airport::icao], true);
    airportsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    airportsDelegate = new RelModifyDelegate(this);
    airportsView->setItemDelegate(airportsDelegate);
    QMap<int, QVariant::Type> TypeMap;
    TypeMap[airportIndexes[Airport::lat]] = QVariant::Double;
    TypeMap[airportIndexes[Airport::lon]] = QVariant::Double;
    TypeMap[airportIndexes[Airport::alt]] = QVariant::Double;
    airportsDelegate->SetColsType(TypeMap);

    UpperCaseDelegate *nameDelegate = new UpperCaseDelegate(4, this);
    airportsView->setItemDelegateForColumn(airportIndexes[Airport::name], nameDelegate);
    airportsView->setItemDelegateForColumn(airportIndexes[Airport::icao], nameDelegate);
    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    airportsView->setItemDelegateForColumn(airportIndexes[Airport::lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    airportsView->setItemDelegateForColumn(airportIndexes[Airport::lon], coordDelegateLon);

    connect(airportsTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&))
            , SLOT(airportDataChanged()));
    connect(airportsView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onSelectAirport(const QModelIndex&)));

    // FIR airport view
    airportsByFirTableModel = new QSqlTableModel(this, mDatabase);
    airportsByFirTableModel->setTable(Airport::tableName);
    airportsByFirTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    airportsByFirTableModel->setFilter("0 = -1");

    airportsByFirView->setModel(airportsByFirTableModel);
    airportsByFirView->horizontalHeader()->moveSection(airportsByFirView->horizontalHeader()->visualIndex(airportIndexes[Airport::name]), 0);
    airportsByFirView->horizontalHeader()->moveSection(airportsByFirView->horizontalHeader()->visualIndex(airportIndexes[Airport::icao]), 1);
    airportsByFirView->horizontalHeader()->moveSection(airportsByFirView->horizontalHeader()->visualIndex(airportIndexes[Airport::lat]), 2);
    airportsByFirView->horizontalHeader()->moveSection(airportsByFirView->horizontalHeader()->visualIndex(airportIndexes[Airport::lon]), 3);
    airportsByFirView->horizontalHeader()->moveSection(airportsByFirView->horizontalHeader()->visualIndex(airportIndexes[Airport::alt]), 4);
    airportsByFirView->horizontalHeader()->moveSection(airportsByFirView->horizontalHeader()->visualIndex(airportIndexes[Airport::declination]), 5);
    airportsByFirView->setColumnHidden(airportIndexes[Airport::id], true);
    airportsByFirView->setColumnHidden(airportIndexes[Airport::icao], true);
    airportsByFirView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    airportsByFirDelegate = new RelModifyDelegate(airportsByFirView);
    airportsByFirView->setItemDelegate(airportsByFirDelegate);
    airportsByFirDelegate->SetColsType(TypeMap);

    coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    airportsByFirView->setItemDelegateForColumn(airportIndexes[Airport::lat], coordDelegateLat);
    coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    airportsByFirView->setItemDelegateForColumn(airportIndexes[Airport::lon], coordDelegateLon);

    connect(airportsByFirTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&))
            , SLOT(firAirportDataChanged()));
    connect(airportsView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , airportsView->horizontalHeader(), SLOT(resizeSections()));
    connect(airportsByFirView->verticalScrollBar(), SIGNAL(valueChanged(int))
            , airportsByFirView->horizontalHeader(), SLOT(resizeSections()));

    airportsView->installEventFilter(this);
    airportsByFirView->installEventFilter(this);

    translate();
}

AirportEditor::~AirportEditor()
{
    SaveState(this);
}

void AirportEditor::translate()
{
    retranslateUi(this);

    airportsTableModel->setHeaderData(airportIndexes[Airport::name], Qt::Horizontal, Airport::tr_name());
    airportsTableModel->setHeaderData(airportIndexes[Airport::icao], Qt::Horizontal, Airport::tr_icao());
    airportsTableModel->setHeaderData(airportIndexes[Airport::lat], Qt::Horizontal, Airport::tr_lat());
    airportsTableModel->setHeaderData(airportIndexes[Airport::lon], Qt::Horizontal, Airport::tr_lon());
    airportsTableModel->setHeaderData(airportIndexes[Airport::alt], Qt::Horizontal, Airport::tr_alt());
    airportsTableModel->setHeaderData(airportIndexes[Airport::declination], Qt::Horizontal, Airport::tr_declination());

    airportsByFirTableModel->setHeaderData(airportIndexes[Airport::name], Qt::Horizontal, Airport::tr_name());
    airportsByFirTableModel->setHeaderData(airportIndexes[Airport::icao], Qt::Horizontal, Airport::tr_icao());
    airportsByFirTableModel->setHeaderData(airportIndexes[Airport::lat], Qt::Horizontal, Airport::tr_lat());
    airportsByFirTableModel->setHeaderData(airportIndexes[Airport::lon], Qt::Horizontal, Airport::tr_lon());
    airportsByFirTableModel->setHeaderData(airportIndexes[Airport::alt], Qt::Horizontal, Airport::tr_alt());
    airportsByFirTableModel->setHeaderData(airportIndexes[Airport::declination], Qt::Horizontal, Airport::tr_declination());

    updateFirName();
}

void AirportEditor::refresh()
{
    airportsTableModel->revertAll();
    selectAll(airportsTableModel);
    airportsByFirTableModel->revertAll();
    selectAll(airportsByFirTableModel);
    isAirportsChanged = false;
    isAirportsByFirChanged = false;
    operButtons->setEnabled(false);
}

void AirportEditor::updateFirName()
{
    firLabel->setText(tr("Current FIR: %1").arg(firName()));
}

void AirportEditor::setFirId()
{
    deleteFirAirportButton->setEnabled(isFirValid());

    if (isFirValid()) {
        airportsByFirTableModel->setFilter(QString("%1 IN (SELECT %2 FROM %3 WHERE %4 = %5)")
                                       .arg(Airport::id
                                            , ZoneAirport::airportid
                                            , ZoneAirport::tableName
                                            , ZoneAirport::zoneid
                                            , QString::number(firId())));
    } else {
        airportsByFirTableModel->setFilter("1=0");
    }
    selectAll(airportsByFirTableModel);

    updateAirportsTableFilter();
}

void AirportEditor::closeEvent(QCloseEvent* pEvent)
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
                if (isAirportsChanged && !airportsTableModel->submitAll())
                    throw(airportsTableModel->lastError().text());
                if (isAirportsByFirChanged)
                    saveFirAirport();
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

void AirportEditor::changeEvent(QEvent* event)
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

void AirportEditor::addAirport()
{
    int iRow = 0;
    airportsTableModel->insertRow(iRow);
    QModelIndex qmIndex = airportsTableModel->index(iRow, airportIndexes[Airport::name]);
    airportsView->setCurrentIndex(qmIndex);
    airportsView->edit(qmIndex);
    isAirportsChanged = true;
    operButtons->setEnabled(true);
}

void AirportEditor::deleteAirport()
{
    QModelIndexList selectList = airportsView->selectionModel()->selectedRows();
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == airportsTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        airportsTableModel->removeRow(rowForDelte);
        isAirportsChanged = true;
    }
    if (isAirportsChanged)
        operButtons->setEnabled(true);
}

void AirportEditor::onSelectAirport(const QModelIndex& qIndex)
{
    bool ableAddAirportToFir = qIndex.isValid()
            && !airportsTableModel->record(qIndex.row()).value(Airport::id).isNull()
            && isFirValid();
    addFirAirportButton->setEnabled(ableAddAirportToFir);
}

void AirportEditor::addFirAirport()
{
    QModelIndexList selectList = airportsView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && airportsView->currentIndex().isValid())
        selectList.append(airportsView->currentIndex());
    foreach(const QModelIndex &selected, selectList)
    {
        int iRow = 0;
        QSqlRecord qRecord = airportsTableModel->record(selected.row());
        airportsByFirTableModel->insertRecord(iRow, qRecord);
        QModelIndex qmIndex = airportsByFirTableModel->index(iRow, airportIndexes[Airport::name]);
        airportsByFirView->setCurrentIndex(qmIndex);
        isAirportsByFirChanged = true;
        operButtons->setEnabled(true);
    }

    updateAirportsTableFilter();
}

void AirportEditor::deleteFirAirport()
{
    QModelIndexList selectList = airportsByFirView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && airportsByFirView->currentIndex().isValid())
        selectList.append(airportsByFirView->currentIndex());
    int correction = 0;
    foreach(const QModelIndex &selected, selectList)
    {
        int rowForDelete = selected.row() - correction;
        if ("*" == airportsByFirTableModel->headerData(rowForDelete, Qt::Vertical))
            ++correction;
        else
            mFirAirportsToDelete.insert(rowForDelete, airportsByFirTableModel->index(rowForDelete, airportIndexes[Airport::id]).data().toInt());
        airportsByFirTableModel->removeRow(rowForDelete);
        isAirportsByFirChanged = true;
    }
    if (isAirportsByFirChanged)
        operButtons->setEnabled(true);

    updateAirportsTableFilter();
}

void AirportEditor::onOperButtons(QAbstractButton* pButton)
{
    switch(operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        try
        {
            QVariant airportsSelectedRow = airportsTableModel->data(
                        airportsTableModel->index(
                            airportsView->selectionModel()->currentIndex().row()
                            , airportIndexes[Airport::name]));
            QVariant airportsByFirSelectedRow = airportsByFirTableModel->data(
                        airportsByFirTableModel->index(
                            airportsByFirView->selectionModel()->currentIndex().row()
                            , airportIndexes[Airport::name]));
            QtAutoTransaction dbTrans(mDatabase);
            if (isAirportsChanged)
                saveFir();
            if (isAirportsChanged && !airportsTableModel->submitAll())
                throw(airportsTableModel->lastError().text());
            if (isAirportsByFirChanged)
                saveFirAirport();
            dbTrans.commit();
            isAirportsChanged = false;
            isAirportsByFirChanged = false;
            operButtons->setEnabled(false);
            selectAll(airportsByFirTableModel);
            while(airportsTableModel->canFetchMore())
                airportsTableModel->fetchMore();
            QModelIndexList airportsIndexList = airportsTableModel->match(
                        airportsTableModel->index(0, airportIndexes[Airport::name])
                        , Qt::DisplayRole, airportsSelectedRow, 1, Qt::MatchExactly);
            while(airportsByFirTableModel->canFetchMore())
                airportsByFirTableModel->fetchMore();
            QModelIndexList airportsByFirIndexList = airportsByFirTableModel->match(
                        airportsByFirTableModel->index(0, airportIndexes[Airport::name])
                        , Qt::DisplayRole, airportsByFirSelectedRow, 1, Qt::MatchExactly);
            if(airportsIndexList.count() && !airportsByFirIndexList.count())
            {
                airportsView->selectRow(airportsIndexList.first().row());
                airportsView->scrollTo(airportsIndexList.first());
            }
            if(airportsByFirIndexList.count())
            {
                airportsByFirView->selectRow(airportsByFirIndexList.first().row());
                airportsByFirView->scrollTo(airportsByFirIndexList.first());
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
        if (isAirportsChanged)
        {
            airportsTableModel->revertAll();
            selectAll(airportsTableModel);
        }
        if (isAirportsByFirChanged)
        {
            airportsByFirTableModel->revertAll();
            selectAll(airportsByFirTableModel);
        }
        isAirportsChanged = false;
        isAirportsByFirChanged = false;
        operButtons->setEnabled(false);
        break;
    default:
        break;
    }

    updateAirportsTableFilter();
}

void AirportEditor::airportDataChanged()
{
    isAirportsChanged = true;
    operButtons->setEnabled(true);
}

void AirportEditor::firAirportDataChanged()
{
    isAirportsByFirChanged = true;
    operButtons->setEnabled(true);
}

void AirportEditor::saveFirAirport()
{
    int uiZoneID = firId();
    int iRowCount = airportsByFirTableModel->rowCount();
    for(int iIndex = 0; iIndex < iRowCount; ++iIndex)
    {
        if (airportsByFirTableModel->headerData(iIndex, Qt::Vertical) == "*")
        {
            QSharedPointer<db::CDBQuery> query = db::CreateQuery();
            query->Prepare("INSERT INTO zone_airport(zone_id, airport_id) VALUES(:1, :2)");
            query->BindValue(":1", (uiZoneID));
            query->BindValue(":2", airportsByFirTableModel->data(
                                 airportsByFirTableModel->index(iIndex, airportIndexes[Airport::id])));
            if (!query->exec())
                throw(tr("Execution error") + ". %1").arg(query->lastError().text());
        }
        else if (airportsByFirTableModel->headerData(iIndex, Qt::Vertical) == "!")
        {
            if (!mFirAirportsToDelete.contains(iIndex))
            {
                QStringList ids;
                const QList < int > rows = mFirAirportsToDelete.keys();
                foreach (const int row, rows) {
                    ids << QString("(%1; %2)").arg(row).arg(mFirAirportsToDelete[row]);
                }
                qWarning() << tr("Error removing airport from fir; id = %1; map = %2").arg(iIndex).arg(ids.join(" "));
            }
            QSharedPointer<db::CDBQuery> query = db::CreateQuery();
            query->Prepare("DELETE FROM zone_airport WHERE zone_id = :1 AND airport_id = :2");
            query->BindValue(":1", (uiZoneID));
            query->BindValue(":2", mFirAirportsToDelete[iIndex]);
            mFirAirportsToDelete.remove(iIndex);
            if (!query->exec())
                throw(tr("Execution error") + ". %1").arg(query->lastError().text());
        }
    }
}

void AirportEditor::saveFir()
{
    int iRowCount = airportsTableModel->rowCount();
    for(int iIndex = 0; iIndex < iRowCount; ++iIndex)
    {
        if (airportsTableModel->headerData(iIndex, Qt::Vertical) == "*")
        {
            QString icao = airportsTableModel->index(iIndex, airportIndexes[Airport::name]).data().toString();
            icao.truncate(2);
            airportsTableModel->setData(airportsTableModel->index(iIndex, airportIndexes[Airport::icao]), icao);
        }
    }
}

void AirportEditor::updateAirportsTableFilter()
{
    QStringList ids;
    const int count = airportsByFirTableModel->rowCount();
    for (int i = 0; i < count; ++i) {
        const QString id = airportsByFirTableModel->index(i, airportIndexes[Airport::id]).data().toString();
        if (!id.isEmpty())
            ids << id;
    }

    if (!ids.isEmpty()) {
        airportsTableModel->setFilter(QString("%1 NOT IN (%2)").arg(Airport::id, ids.join(", ")));
    } else {
        airportsTableModel->setFilter("1!=0");
    }
    selectAll(airportsTableModel);
}


