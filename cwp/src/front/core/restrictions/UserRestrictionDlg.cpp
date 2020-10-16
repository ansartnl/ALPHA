#include "UserRestrictionDlg.h"
#include "ui_UserRestrictionDlg.h"
#include "GUIView.h"
#include "main.h"
#include "Restriction.h"
#include "Coordinate.h"

#include "coorddelegate.h"

#include <QSqlRelationalDelegate>
#include <QClipboard>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QItemEditorFactory>
#include <QMessageBox>
#include <QInputDialog>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QMimeData>

static const QString g_tableName = "restriction_point_user";
static const QString g_id = "id"; //PK
static const QString g_sn = "sn";
static const QString g_type = "type";
static const QString g_lat = "latitude";
static const QString g_lon = "longitude";
static const QString g_arclat = "arc_latitude";
static const QString g_arclon = "arc_longitude";
static const QString g_arcdist = "arc_distance";
static const QString g_restid = "restriction_id";

static const QString g_tableNameType = "restriction_point_type";
static const QString g_idType = "id";
static const QString g_nameType = "name";


//! Class extends QItemDelegate
class PoModifyDelegate : public QSqlRelationalDelegate
{
public:
    PoModifyDelegate(QObject* pObject = 0)
        : QSqlRelationalDelegate(pObject)
    {}

    //! Inherited from QSqlRelationalDelegate
    virtual QWidget* createEditor(
            QWidget* parent,
            const QStyleOptionViewItem & option,
            const QModelIndex & index ) const;
};

QWidget* PoModifyDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem & option,
                                        const QModelIndex & index ) const
{
    const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *> (index.model());
    QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
    if (!childModel)
    {
        if (!index.isValid())
            return 0;
        QVariant::Type t = static_cast<QVariant::Type> (index.data(Qt::EditRole).userType());

        const QItemEditorFactory *factory = QItemEditorFactory::defaultFactory();
        QWidget* pEditor = factory->createEditor(t, parent);

        if (dynamic_cast<QDoubleSpinBox*> (pEditor))
            dynamic_cast<QDoubleSpinBox*> (pEditor)->setDecimals(4);
        if (dynamic_cast<QDateTimeEdit*> (pEditor))
            dynamic_cast<QDateTimeEdit*> (pEditor)->setCalendarPopup(true);
        return pEditor;
    }

    QComboBox *combo = new QComboBox(parent);
    combo->setModel(childModel);
    combo->setModelColumn(childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn()));
    combo->installEventFilter(const_cast<PoModifyDelegate *> (this));

    return combo;
}


////////////////////////////////////////////////////////////////////////////////
// UserRestrictionDlg implementation.

UserRestrictionDlg::UserRestrictionDlg(UserRole role, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserRestrictionDlg),
    mRestrictionId(0), mRole(role)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi();
}

UserRestrictionDlg::~UserRestrictionDlg()
{
    delete ui;
}

void UserRestrictionDlg::setupUi()
{
    QSqlRelationalTableModel *pointsModel = new QSqlRelationalTableModel(this);
    pointsModel->setTable(g_tableName);
    pointsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mPointsModelIndex.insert(g_id, pointsModel->fieldIndex(g_id));
    mPointsModelIndex.insert(g_sn, pointsModel->fieldIndex(g_sn));
    mPointsModelIndex.insert(g_type, pointsModel->fieldIndex(g_type));
    mPointsModelIndex.insert(g_lat, pointsModel->fieldIndex(g_lat));
    mPointsModelIndex.insert(g_lon, pointsModel->fieldIndex(g_lon));
    mPointsModelIndex.insert(g_arclat, pointsModel->fieldIndex(g_arclat));
    mPointsModelIndex.insert(g_arclon, pointsModel->fieldIndex(g_arclon));
    mPointsModelIndex.insert(g_arcdist, pointsModel->fieldIndex(g_arcdist));
    mPointsModelIndex.insert(g_restid, pointsModel->fieldIndex(g_restid));
    pointsModel->setRelation(mPointsModelIndex[g_type], QSqlRelation(g_tableNameType, g_idType, g_nameType));
    pointsModel->setFilter("0 = 1");
    pointsModel->select();

    ui->tableView->setModel(pointsModel);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_id]), 0);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_sn]), 1);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_type]), 2);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_lat]), 3);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_lon]), 4);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_arclat]), 5);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_arclon]), 6);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_arcdist]), 7);
    ui->tableView->horizontalHeader()->moveSection(ui->tableView->horizontalHeader()->visualIndex(
                                                       mPointsModelIndex[g_restid]), 8);

    PoModifyDelegate *pointDelegate = new PoModifyDelegate(ui->tableView);
    ui->tableView->setItemDelegate(pointDelegate);

    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    ui->tableView->setItemDelegateForColumn(mPointsModelIndex[g_lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    ui->tableView->setItemDelegateForColumn(mPointsModelIndex[g_lon], coordDelegateLon);
    CoordDelegate* coordDelegateArcLat = new CoordDelegate(CoordEdit::Latitude, this);
    ui->tableView->setItemDelegateForColumn(mPointsModelIndex[g_arclat], coordDelegateArcLat);
    CoordDelegate* coordDelegateArcLon = new CoordDelegate(CoordEdit::Longitude, this);
    ui->tableView->setItemDelegateForColumn(mPointsModelIndex[g_arclon], coordDelegateArcLon);

    ui->tableView->setColumnHidden(mPointsModelIndex[g_id], true);
    ui->tableView->setColumnHidden(mPointsModelIndex[g_restid], true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    ui->tableView->resizeColumnsToContents();

    connect(pointsModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));

    ui->dateTimeEditBegin->setDateTime( QDateTime::currentDateTime() );
    ui->dateTimeEditEnd->setDateTime( QDateTime::currentDateTime() );

    QSqlQueryModel* typeModel = new QSqlQueryModel(ui->comboBoxType);
    QString sTypeQuery;
    if ( Admin == mRole )
        sTypeQuery = "SELECT id, name FROM restriction_user_type";
    else
        sTypeQuery = "SELECT id, name FROM restriction_user_type WHERE id <> 79";
    typeModel->setQuery(sTypeQuery);
    ui->comboBoxType->setModel(typeModel);
    ui->comboBoxType->setModelColumn(1);

    translate();
}

void UserRestrictionDlg::translate()
{
    ui->retranslateUi(this);

    QSqlRelationalTableModel *pointsModel = static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );

    pointsModel->setHeaderData(mPointsModelIndex[g_sn], Qt::Horizontal, tr("SN"));
    pointsModel->setHeaderData(mPointsModelIndex[g_type], Qt::Horizontal, tr("Type"));
    pointsModel->setHeaderData(mPointsModelIndex[g_lat], Qt::Horizontal, tr("Latitude,\ndegree"));
    pointsModel->setHeaderData(mPointsModelIndex[g_lon], Qt::Horizontal, tr("Longitude,\ndegree"));
    pointsModel->setHeaderData(mPointsModelIndex[g_arclat], Qt::Horizontal, tr("Arc latitude,\ndegree"));
    pointsModel->setHeaderData(mPointsModelIndex[g_arclon], Qt::Horizontal, tr("Arc longitude,\ndegree"));
    pointsModel->setHeaderData(mPointsModelIndex[g_arcdist], Qt::Horizontal, tr("Arc distance,\nmeter"));

//    ui->comboBoxType->clear();
//    ui->comboBoxType->addItem(tr("Restriction"), CRestrictionUser::enRestriction);
//    if ( Admin == mRole )
//        ui->comboBoxType->addItem(tr("Obstacle"), CRestrictionUser::enObstacle);
//    ui->comboBoxType->addItem(tr("No Conflict Zone"), CRestrictionUser::enNFZ);
}

void UserRestrictionDlg::insertZone(const CRestrictionUser &r)
{
    ui->lineEditName->setText( r.GetName() );
    // Select restriction zone type.
    for ( int i = 0; i < ui->comboBoxType->count(); i++ )
    {
        CRestrictionUser::Type type = (CRestrictionUser::Type)
                ui->comboBoxType->model()->index(i, 0).data().toInt();
        if ( type == r.GetType() )
        {
            ui->comboBoxType->setCurrentIndex(i);
            break;
        }
    }
    ui->spinBoxMin->setValue( r.GetHMin() );
    ui->spinBoxMax->setValue( r.GetHMax() );
    ui->dateTimeEditBegin->setDateTime( r.GetTimeBegin() );
    ui->dateTimeEditEnd->setDateTime( r.GetTimeEnd() );

    if ( r.GetID() > 0 )
    {
        mRestrictionId = r.GetID();

        QSqlRelationalTableModel *pointsModel =
                static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );
        pointsModel->setFilter(QString("restriction_id = %1").arg(mRestrictionId));
        pointsModel->select();

        setButtonBoxEnabled(true);
    }
}

QList<QPointF> UserRestrictionDlg::getFromClipboard()
{
    typedef QList<QPointF>  TCoordList;
    TCoordList coords;

    QClipboard *clipboard = QApplication::clipboard();
    if ( !clipboard->mimeData() || !clipboard->mimeData()->hasText() )
    {
        QMessageBox::information(this, tr("Paste from Clipboard"), tr("Unknown data in the clipboard"));
        return coords;
    }

    QString textClp = clipboard->text();
    QTextStream stream(&textClp);
    while ( !stream.atEnd() )
    {
        QString lat, lon;
        stream >> lat >> lon;
        if ( QTextStream::Ok != stream.status() )
            continue;

        coords << QPointF(lat.toDouble(), lon.toDouble());
    }

    return coords;
}

int UserRestrictionDlg::findMaxSN() const
{
    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );

    int sn = 0;
    for ( int i = 0; i < pointsModel->rowCount(); i++ )
    {
        int currentSN = pointsModel->index(i, mPointsModelIndex[g_sn]).data().toInt();
        sn = qMax(sn, currentSN);
    }

    return sn;
}

void UserRestrictionDlg::pasteAsCircle()
{
    Q_ASSERT(gActiveView);

    typedef QList<QPointF>  TCoordList;
    TCoordList coords = getFromClipboard();

    if ( coords.empty() )
        return;

    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );

    QPointF point1 = coords.at(0);

    double arcdist = 0.;
    if ( coords.size() > 1 )
    {
        QPointF point2 = coords.at(1);
        CCoord coord1(point1.y(), point1.x()),
               coord2(point2.y(), point2.x());

        coord1.Center( gActiveView->GetCentralPoint()->GetCoord() );
        coord2.Center( gActiveView->GetCentralPoint()->GetCoord() );

        QPointF p1(coord1.GetX(), coord1.GetY());
        QPointF p2(coord2.GetX(), coord2.GetY());

        arcdist = QLineF(p1, p2).length();
    }

    int iRow = 0;
    pointsModel->insertRow(iRow);
    QSqlRecord qRec = pointsModel->record(iRow);
    qRec.setValue(mPointsModelIndex[g_sn], findMaxSN()+10);
    qRec.setValue(mPointsModelIndex[g_type], CBoundaryPoint::enCircle);
    qRec.setValue(mPointsModelIndex[g_lat], 0.);
    qRec.setValue(mPointsModelIndex[g_lon], 0.);
    qRec.setValue(mPointsModelIndex[g_arclat], point1.x());
    qRec.setValue(mPointsModelIndex[g_arclon], point1.y());
    qRec.setValue(mPointsModelIndex[g_arcdist], arcdist);
    pointsModel->setRecord(iRow, qRec);
    setButtonBoxEnabled(true);
}

void UserRestrictionDlg::pasteAsPolygon()
{
    typedef QList<QPointF>  TCoordList;
    TCoordList coords = getFromClipboard();

    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );

    int sn = findMaxSN() + 10;
    for ( int i = coords.size()-1; i >= 0; i--, sn += 10 )
    {
        const QPointF &coord = coords.at(i);

        int iRow = 0;
        pointsModel->insertRow(iRow);
        QSqlRecord qRec = pointsModel->record(iRow);
        qRec.setValue(mPointsModelIndex[g_sn], sn);
        qRec.setValue(mPointsModelIndex[g_type], CBoundaryPoint::enLine);
        qRec.setValue(mPointsModelIndex[g_lat], coord.x());
        qRec.setValue(mPointsModelIndex[g_lon], coord.y());
        qRec.setValue(mPointsModelIndex[g_arclat], 0.);
        qRec.setValue(mPointsModelIndex[g_arclon], 0.);
        qRec.setValue(mPointsModelIndex[g_arcdist], 0.);
        pointsModel->setRecord(iRow, qRec);
        setButtonBoxEnabled(true);
    }
}

void UserRestrictionDlg::insertPointsFromClipboard(Type typePoints)
{
    if ( typePoints == Circle )
        pasteAsCircle();
    else if ( typePoints == Polygon )
        pasteAsPolygon();
    else
        Q_ASSERT(false);
}

void UserRestrictionDlg::on_comboBoxType_currentIndexChanged(int index)
{
    Q_ASSERT(index < ui->comboBoxType->count());
    CRestrictionUser::Type typeRestriction =
            (CRestrictionUser::Type) ui->comboBoxType->model()->index(index, 0).data().toInt();

    if ( typeRestriction == CRestrictionUser::enObstacle )
    {
        ui->dateTimeEditEnd->hide();
        ui->labelTimeEnd->hide();
    }
    else
    {
        ui->dateTimeEditEnd->show();
        ui->labelTimeEnd->show();
    }

    if( typeRestriction == CRestrictionUser::enMap)
    {
        ui->dateTimeEditBegin->setDisabled(true);
        ui->dateTimeEditEnd->setDisabled(true);
        ui->spinBoxMax->setDisabled(true);
        ui->spinBoxMin->setDisabled(true);
    }
    else
    {
        ui->dateTimeEditBegin->setDisabled(false);
        ui->dateTimeEditEnd->setDisabled(false);
        ui->spinBoxMax->setDisabled(false);
        ui->spinBoxMin->setDisabled(false);
    }
}

void UserRestrictionDlg::on_toolButtonAdd_clicked()
{
    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );

    int iRow = 0;
    pointsModel->insertRow(iRow);
    QModelIndex qmIndex = pointsModel->index(iRow, mPointsModelIndex[g_sn]);
    ui->tableView->setCurrentIndex(qmIndex);
    setButtonBoxEnabled(true);
}

void UserRestrictionDlg::on_toolButtonDelete_clicked()
{
    QModelIndexList selectList = ui->tableView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->tableView->currentIndex().isValid())
        selectList.append(ui->tableView->currentIndex());

    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );

    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == pointsModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        pointsModel->removeRow(rowForDelte);
        setButtonBoxEnabled(true);
    }
}

void UserRestrictionDlg::on_toolButtonPaste_clicked()
{
    const QString typeCircle = tr("Circle");
    const QString typePolygon = tr("Polygon");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Choose points type"),
                                         tr("Points type:"),
                                         QStringList() << typeCircle << typePolygon,
                                         0, false, &ok);
    if ( ok && !item.isEmpty() )
        insertPointsFromClipboard(item == typeCircle ? Circle : Polygon);
}

void UserRestrictionDlg::on_toolButtonClear_clicked()
{
    QSqlRelationalTableModel *pointsModel =
            static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );

    if ( pointsModel->rowCount() > 0 )
        pointsModel->removeRows(0, pointsModel->rowCount());
    setButtonBoxEnabled(true);
}

void UserRestrictionDlg::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    setButtonBoxEnabled(true);
}

void UserRestrictionDlg::setButtonBoxEnabled(bool on)
{
    // Editing restriction zone.
    if ( mRestrictionId > 0 )
        ui->buttonBox->setEnabled(on);
    // Creating new restriction zone.
    else
        ui->buttonBox->setEnabled(true);
}

void UserRestrictionDlg::accept()
{
    TVA_CATCHALL_TRY
    {
        QSqlRelationalTableModel *pointsModel =
                static_cast<QSqlRelationalTableModel *>( ui->tableView->model() );
        if ( pointsModel->rowCount() < 1 )
            throw QString("No points to insert to database.");

        CRestrictionUser r;
        r.SetID( mRestrictionId );
        r.SetType( (CRestrictionUser::Type) ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(), 0).data().toInt() );
        r.SetName( ui->lineEditName->text() );
        r.SetHMin( ui->spinBoxMin->value() );
        r.SetHMax( ui->spinBoxMax->value() );
        r.SetTimeBegin( ui->dateTimeEditBegin->dateTime() );
        r.SetTimeEnd( r.GetType() != CRestrictionUser::enObstacle ? ui->dateTimeEditEnd->dateTime() : QDateTime() );

        // Save or update just restriction zone info.
        if ( mRestrictionId > 0 )
            dao()->UpdateUserRestriction(r, false);
        else
            mRestrictionId = dao()->SaveUserRestriction(mFirId, r, false);

        // Save points using sql table model feature.
        for ( int i = 0; i < pointsModel->rowCount(); i++ )
        {
            QModelIndex ind = pointsModel->index(i, mPointsModelIndex[g_restid]);
            pointsModel->setData(ind, mRestrictionId);
        }
        TVA_CHECK(pointsModel->submitAll(), tr("Execution error. %1").arg(pointsModel->lastError().text()), E_FAIL);

        QDialog::accept();
    }
    TVA_CATCHALL(ex)
    {
        QMessageBox::warning(this, tr("Add zone to DB"), ex.reason());
    }
}

void UserRestrictionDlg::changeEvent(QEvent* event)
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
