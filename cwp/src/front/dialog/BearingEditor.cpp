#include "BearingEditor.h"
#include "ui_BearingEditor.h"

#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QtCore/QDebug>

#include "coorddelegate.h"
#include "texteditdelegate.h"
#include "front/common/ViewDelegate.h"
#include "qtautotransaction.h"
#include "main.h"

const char CBearingEditor::DB_BEARING_TABLE[]           = "bearing_point";
const char CBearingEditor::DB_BEARING_ID[]              = "id";
const char CBearingEditor::DB_BEARING_NAME[]            = "name";
const char CBearingEditor::DB_BEARING_FIR_NAME[]        = "airport_id";
const char CBearingEditor::DB_BEARING_RDF_LATITUDE[]    = "rdf_latitude";
const char CBearingEditor::DB_BEARING_RDF_LONGITUDE[]   = "rdf_longitude";
const char CBearingEditor::DB_BEARING_SAC[]             = "sac";
const char CBearingEditor::DB_BEARING_SIC[]             = "sic";
const char CBearingEditor::DB_BEARING_CHANNEL[]         = "channel";
const char CBearingEditor::DB_BEARING_DECLINATION[]     = "declination";


CBearingEditor::CBearingEditor(QWidget *pParent)
    : QDialog(pParent),
    ui(new Ui::BearingEditor)

{
    ui->setupUi(this);
    ui->operButtons->setEnabled(false);

    //Bearing point
    QSqlRelationalTableModel *bearingModel = new QSqlRelationalTableModel(this);
    bearingModel->setTable(DB_BEARING_TABLE);
    bearingModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bearingModelIndex.insert(DB_BEARING_ID, bearingModel->fieldIndex(DB_BEARING_ID));
    bearingModelIndex.insert(DB_BEARING_NAME, bearingModel->fieldIndex(DB_BEARING_NAME));
    bearingModelIndex.insert(DB_BEARING_FIR_NAME, bearingModel->fieldIndex(DB_BEARING_FIR_NAME));
    bearingModelIndex.insert(DB_BEARING_RDF_LATITUDE, bearingModel->fieldIndex(DB_BEARING_RDF_LATITUDE));
    bearingModelIndex.insert(DB_BEARING_RDF_LONGITUDE, bearingModel->fieldIndex(DB_BEARING_RDF_LONGITUDE));
    bearingModelIndex.insert(DB_BEARING_SAC, bearingModel->fieldIndex(DB_BEARING_SAC));
    bearingModelIndex.insert(DB_BEARING_SIC, bearingModel->fieldIndex(DB_BEARING_SIC));
    bearingModelIndex.insert(DB_BEARING_CHANNEL, bearingModel->fieldIndex(DB_BEARING_CHANNEL));
    bearingModelIndex.insert(DB_BEARING_DECLINATION, bearingModel->fieldIndex(DB_BEARING_DECLINATION));
    bearingModel->select();

    ui->bearingView->setModel(bearingModel);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_ID]), 0);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_NAME]), 1);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_FIR_NAME]), 2);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_RDF_LATITUDE]), 3);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_RDF_LONGITUDE]), 4);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_SAC]), 5);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_SIC]), 6);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_CHANNEL]), 7);
    ui->bearingView->horizontalHeader()->moveSection(ui->bearingView->horizontalHeader()->visualIndex(bearingModelIndex[DB_BEARING_DECLINATION]), 8);
    ui->bearingView->setColumnHidden(bearingModelIndex[DB_BEARING_ID], true);
    ui->bearingView->setColumnHidden(bearingModelIndex[DB_BEARING_FIR_NAME], true);
    ui->bearingView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    CRelModifyDelegate *pModDelegateType = new CRelModifyDelegate(ui->bearingView);
    ui->bearingView->setItemDelegate(pModDelegateType);

    CRelModifyDelegate::TColMap colMap;
    colMap.insert(bearingModelIndex[DB_BEARING_ID], QVariant::UInt);
    colMap.insert(bearingModelIndex[DB_BEARING_NAME], QVariant::String);
    colMap.insert(bearingModelIndex[DB_BEARING_FIR_NAME], QVariant::UInt);
    colMap.insert(bearingModelIndex[DB_BEARING_RDF_LATITUDE], QVariant::Double);
    colMap.insert(bearingModelIndex[DB_BEARING_RDF_LONGITUDE], QVariant::Double);
    colMap.insert(bearingModelIndex[DB_BEARING_SAC], QVariant::UInt);
    colMap.insert(bearingModelIndex[DB_BEARING_SIC], QVariant::UInt);
    colMap.insert(bearingModelIndex[DB_BEARING_CHANNEL], QVariant::UInt);
    colMap.insert(bearingModelIndex[DB_BEARING_DECLINATION], QVariant::UInt);
    pModDelegateType->SetColsType(colMap);

    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, ui->bearingView);
    ui->bearingView->setItemDelegateForColumn(bearingModelIndex[DB_BEARING_RDF_LATITUDE], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, ui->bearingView);
    ui->bearingView->setItemDelegateForColumn(bearingModelIndex[DB_BEARING_RDF_LONGITUDE], coordDelegateLon);

    CModifyDelegate* teDelegate = new CModifyDelegate(ui->bearingView);
    ui->bearingView->setItemDelegateForColumn(bearingModelIndex[DB_BEARING_DECLINATION], teDelegate);

    connect(bearingModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(activateOperButtons()));

    // FIR combo
    QSqlQueryModel *firQueryModel = new QSqlQueryModel(ui->firCombo);


    firName = networkSettings()->value("FPL_handler/fir").toString();
    firList = firName.split(",", QString::SkipEmptyParts);


    queryStr = "SELECT a.id, a.name FROM airport a "
            "LEFT JOIN zone_airport za on a.id = za.airport_id "
            "LEFT JOIN zone z on za.zone_id = z.id "
            "WHERE z.id_name IN (''";
    foreach(QString fir, firList)
    {
        queryStr += ",'" + fir + "'";
    }
    queryStr += ")";


    firQueryModel->setQuery(queryStr);
    ui->firCombo->setModel(firQueryModel);
    ui->firCombo->setModelColumn(1);
    ui->firCombo->setCurrentIndex(0);

    Translate();
}

CBearingEditor::~CBearingEditor()
{
    delete ui;
}

void CBearingEditor::Translate()
{
    ui->retranslateUi(this);

    //setWindowTitle(tr("Bearing point editor"));
    QSqlRelationalTableModel *bearingModel = static_cast<QSqlRelationalTableModel *>( ui->bearingView->model() );
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_NAME], Qt::Horizontal, tr("Name"));
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_FIR_NAME], Qt::Horizontal, tr("FIR"));
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_RDF_LATITUDE], Qt::Horizontal, tr("Latitude"));
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_RDF_LONGITUDE], Qt::Horizontal, tr("Longitude"));
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_SAC], Qt::Horizontal, tr("SAC"));
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_SIC], Qt::Horizontal, tr("SIC"));
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_CHANNEL], Qt::Horizontal, tr("Channel/Frequency"));
    bearingModel->setHeaderData(bearingModelIndex[DB_BEARING_DECLINATION], Qt::Horizontal, tr("Declination"));
}


void CBearingEditor::closeEvent(QCloseEvent* pEvent)
{
    if (ui->operButtons->button(QDialogButtonBox::Save)->isEnabled())
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            try
            {
                QSqlRelationalTableModel *bearingModel =
                        static_cast<QSqlRelationalTableModel *>( ui->bearingView->model() );
                QtAutoTransaction transaction(QSqlDatabase::database());
                if (!bearingModel->submitAll())
                    throw (bearingModel->lastError().text());
                transaction.commit();
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

void CBearingEditor::on_operButtons_clicked(QAbstractButton* pButton)
{
    QSqlRelationalTableModel *bearingModel =
            static_cast<QSqlRelationalTableModel *>( ui->bearingView->model() );
    switch(ui->operButtons->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        try
        {
            QVariant bearingSelectedRow = bearingModel->data(bearingModel->index(ui->bearingView->selectionModel()->currentIndex().row(), bearingModelIndex[DB_BEARING_ID]));
            QtAutoTransaction transaction(QSqlDatabase::database());
            if (!bearingModel->submitAll())
                throw (bearingModel->lastError().text());
            transaction.commit();

            bearingModel->select();
            ui->operButtons->setEnabled(false);
            while(bearingModel->canFetchMore())
                bearingModel->fetchMore();
            QModelIndexList bearingIndexList = bearingModel->match(bearingModel->index(0, bearingModelIndex[DB_BEARING_ID]), Qt::DisplayRole, bearingSelectedRow, 1, Qt::MatchExactly);
            if(bearingIndexList.count())
            {
                ui->bearingView->selectRow(bearingIndexList.first().row());
                ui->bearingView->scrollTo(bearingIndexList.first());
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
        bearingModel->revertAll();
        ui->operButtons->setEnabled(false);
        break;
    default:
        break;
    }
}

void CBearingEditor::activateOperButtons()
{
    ui->operButtons->setEnabled(true);
}

void CBearingEditor::on_addBearingButton_clicked()
{
    if (!m_sFirID.isNull())
    {
        QSqlRelationalTableModel *bearingModel =
                static_cast<QSqlRelationalTableModel *>( ui->bearingView->model() );

        int row = 0;
        QSqlRecord record = bearingModel->record();
        record.setValue(bearingModelIndex[DB_BEARING_FIR_NAME], m_sFirID);
        bearingModel->insertRecord(row, record);

        QModelIndex qmIndex = bearingModel->index(row, bearingModelIndex[DB_BEARING_NAME]);
        ui->bearingView->setCurrentIndex(qmIndex);
        ui->bearingView->edit(qmIndex);
        ui->operButtons->setEnabled(true);
    }
}

void CBearingEditor::on_deleteBearingButton_clicked()
{
    QModelIndexList selectList = ui->bearingView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->bearingView->currentIndex().isValid())
        selectList.append(ui->bearingView->currentIndex());

    QSqlRelationalTableModel *bearingModel =
            static_cast<QSqlRelationalTableModel *>( ui->bearingView->model() );

    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == bearingModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        bearingModel->removeRow(rowForDelte);
        ui->operButtons->setEnabled(true);
    }
}

void CBearingEditor::on_firCombo_currentIndexChanged(int iIndex)
{
    QSqlRelationalTableModel *bearingModel =
            static_cast<QSqlRelationalTableModel *>( ui->bearingView->model() );

    if ((iIndex != -1) && (iIndex < ui->firCombo->count()))
    {
        m_sFirID = ui->firCombo->model()->index(iIndex, 0).data().toString();
        bearingModel->setFilter(QString("airport_id = '%1'").arg(m_sFirID));
    }
    else
    {
        m_sFirID = "";
        bearingModel->setFilter("0 = 1");
    }
    bearingModel->select();
    ui->operButtons->setEnabled(false);
}
