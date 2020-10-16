#include "UserPicsWidget.h"
#include "ui_UserPicsWidget.h"

#include "ViewDelegate.h"
#include "QtCheckHeaderView.h"
#include "coorddelegate.h"
#include "qtautotransaction.h"
#include "front/common/CoordsDigitizer.h"
#include "import/BoundaryPoint.h"
#include "main.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTimer>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>

// UserPicsModel

UserPicsModel::UserPicsModel(QObject *parent, QSqlDatabase db)
    : QSqlRelationalTableModel(parent, db), mCheckedColumn(0)
{

}

void UserPicsModel::setCheckedColumn(int column)
{
    mCheckedColumn = column;
}

int UserPicsModel::getCheckedColumn() const
{
    return mCheckedColumn;
}

Qt::ItemFlags UserPicsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSqlRelationalTableModel::flags(index);
    if(index.isValid())
    {
        if(index.column() == mCheckedColumn)
            defaultFlags |= Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}

QVariant UserPicsModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::CheckStateRole)
    {
        if(index.column() == mCheckedColumn)
        {
            bool bCheck = mCheckedData.value(QPair<int, int>(index.row(), index.column()), false);
            return (bCheck ? Qt::Checked : Qt::Unchecked);
        }
    }
    return QSqlRelationalTableModel::data(index, role);
}

bool UserPicsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole)
    {
        if(index.column() == mCheckedColumn)
        {
            bool bOldCheck = mCheckedData.value(QPair<int, int>(index.row(), index.column()), false);
            bool bNewCheck = (value.toInt() == Qt::Checked ? true : false);
            mCheckedData.insert(QPair<int, int>(index.row(), index.column()), bNewCheck);
            emit dataChanged(index, index);
            if(bOldCheck != bNewCheck)
                emit checkChanged(index, bNewCheck);
            return true;
        }
    }
    return QSqlRelationalTableModel::setData(index, value, role);
}

void UserPicsModel::clear()
{
    mCheckedData.clear();
    QSqlRelationalTableModel::clear();
}

bool UserPicsModel::select()
{
    mCheckedData.clear();
    return QSqlRelationalTableModel::select();
}



// UserPicsWidget

namespace UserPics_T
{
const QString tableName("user_pics");

const QString id("id");
const QString name("name");
const QString zoneid("zone_id");

QString tr_id() { return QObject::tr("ID"); }
QString tr_name() { return QObject::tr("Name"); }
QString tr_zoneid() { return QObject::tr("FIR"); }
}

namespace UserPicsPoint_T
{
const QString tableName("user_pics_point");

const QString id("id");
const QString sn("sn");
const QString type("type");
const QString lat("latitude");
const QString lon("longitude");
const QString arclat("arc_latitude");
const QString arclon("arc_longitude");
const QString arcdist("arc_distance");
const QString userpicsid("user_pics_id");

QString tr_id() { return QObject::tr("ID"); }
QString tr_sn() { return QObject::tr("SN"); }
QString tr_type() { return QObject::tr("Type"); }
QString tr_lat() { return QObject::tr("Latitude,\ndegree"); }
QString tr_lon() { return QObject::tr("Longitude,\ndegree"); }
QString tr_arclat() { return QObject::tr("Arc latitude,\ndegree"); }
QString tr_arclon() { return QObject::tr("Arc longitude,\ndegree"); }
QString tr_arcdist() { return QObject::tr("Radius,\n100 meters"); }
QString tr_userpicsid() { return QObject::tr("User pics ID"); }
}

namespace Zone_T
{
const QString tableName("zone");

const QString id("id");
const QString name("name");
}

namespace BoundaryPointType_T
{
const QString tableName("boundary_point_type");

const QString id("id");
const QString name("name");
}

UserPicsWidget::TUPVMapFIRs UserPicsWidget::mMapUserPicsVisibility = UserPicsWidget::TUPVMapFIRs();

UserPicsWidget::UserPicsWidget(QWidget* pParent)
    : QWidget(pParent), ui(new Ui::UserPicsWidget),
      mZoneID(0), mUserPicID(0), mCheckSetting(false),
      mDelaySizeToContents(false), mDelayStretch(false),
      mDelayCheck(false), onDelaySave(false)
{
    ui->setupUi(this);
    m_was_selection = false;
    mHideArcColumns = true;

    // User pics
    mUserPicsModel = new UserPicsModel(this);
    mUserPicsModel->setTable(UserPics_T::tableName);
    mUserPicsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mUserPicsModelIndex.insert(UserPics_T::id, mUserPicsModel->fieldIndex(UserPics_T::id));
    mUserPicsModelIndex.insert(UserPics_T::zoneid, mUserPicsModel->fieldIndex(UserPics_T::zoneid));
    mUserPicsModelIndex.insert(UserPics_T::name, mUserPicsModel->fieldIndex(UserPics_T::name));
    mUserPicsModel->setCheckedColumn(mUserPicsModelIndex[UserPics_T::name]);

    mUserPicsModel->setRelation(mUserPicsModelIndex[UserPics_T::zoneid], QSqlRelation(Zone_T::tableName, Zone_T::id, Zone_T::name));

    mUserPicsModel->setFilter("0 = 1");
    mUserPicsModel->setSort(mUserPicsModelIndex[UserPics_T::name], Qt::AscendingOrder);
    selectAll(mUserPicsModel);

    ui->tableViewPics->setModel(mUserPicsModel);
    ui->tableViewPics->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewPics->setAlternatingRowColors(false);
    ui->tableViewPics->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableViewPics->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewPics->setSortingEnabled(false);

    QtCheckHeaderView* pHorHeader = new QtCheckHeaderView(Qt::Horizontal, this);
    pHorHeader->setClickable(true);
    ui->tableViewPics->setHorizontalHeader(pHorHeader);
    pHorHeader->addCheckable(mUserPicsModel->getCheckedColumn());

    ui->tableViewPics->horizontalHeader()->moveSection(ui->tableViewPics->horizontalHeader()->visualIndex(mUserPicsModelIndex[UserPics_T::id]), 0);
    ui->tableViewPics->horizontalHeader()->moveSection(ui->tableViewPics->horizontalHeader()->visualIndex(mUserPicsModelIndex[UserPics_T::zoneid]), 1);
    ui->tableViewPics->horizontalHeader()->moveSection(ui->tableViewPics->horizontalHeader()->visualIndex(mUserPicsModelIndex[UserPics_T::name]), 2);
    ui->tableViewPics->setColumnHidden(mUserPicsModelIndex[UserPics_T::id], true);
    ui->tableViewPics->setColumnHidden(mUserPicsModelIndex[UserPics_T::zoneid], true);
    ui->tableViewPics->horizontalHeader()->setMinimumWidth(10);
    ui->tableViewPics->horizontalHeader()->setDefaultSectionSize(70);
    ui->tableViewPics->horizontalHeader()->setCascadingSectionResizes(false);
    ui->tableViewPics->horizontalHeader()->setSortIndicatorShown(false);
    ui->tableViewPics->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableViewPics->horizontalHeader()->setMovable(false);
    ui->tableViewPics->horizontalHeader()->setClickable(true);
    ui->tableViewPics->horizontalHeader()->setHighlightSections(false);
    if ( mHideArcColumns )
    {
        ui->tableViewPics->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    }

    ui->tableViewPics->verticalHeader()->setDefaultSectionSize(20);
    ui->tableViewPics->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableViewPics->verticalHeader()->setClickable(false);
    ui->tableViewPics->verticalHeader()->hide();

    CRelModifyDelegate* modDelegateType = new CRelModifyDelegate(ui->tableViewPics);
    ui->tableViewPics->setItemDelegate(modDelegateType);

    CRelModifyDelegate::TColMap colMap;
    colMap.insert(mUserPicsModelIndex[UserPics_T::id], QVariant::UInt);
    colMap.insert(mUserPicsModelIndex[UserPics_T::name], QVariant::String);
    colMap.insert(mUserPicsModelIndex[UserPics_T::zoneid], QVariant::UInt);
    modDelegateType->SetColsType(colMap);

    connect(ui->tableViewPics->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&))
            , this, SLOT(onUserPicsChanged(const QModelIndex&)));
    connect(mUserPicsModel, SIGNAL(checkChanged(const QModelIndex&, bool)), SLOT(onUserPicsCheckChanged(const QModelIndex&, bool)));

    // User pics points
    mUserPicsPointModel = new QSqlRelationalTableModel(this);
    mUserPicsPointModel->setTable((UserPicsPoint_T::tableName));
    mUserPicsPointModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::id, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::id));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::sn, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::sn));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::type, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::type));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::lat, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::lat));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::lon, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::lon));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::arclat, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::arclat));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::arclon, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::arclon));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::arcdist, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::arcdist));
    mUserPicsPointModelIndex.insert(UserPicsPoint_T::userpicsid, mUserPicsPointModel->fieldIndex(UserPicsPoint_T::userpicsid));

    InitRelation();

    ui->tableViewPoints->setModel(mUserPicsPointModel);
    ui->tableViewPoints->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableViewPoints->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewPoints->setSortingEnabled(false);

    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::id]), 0);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::sn]), 1);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::type]), 2);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::lat]), 3);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::lon]), 4);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::arclat]), 5);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::arclon]), 6);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::arcdist]), 7);
    ui->tableViewPoints->horizontalHeader()->moveSection(ui->tableViewPoints->horizontalHeader()->visualIndex(mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid]), 8);
    ui->tableViewPoints->setColumnHidden(mUserPicsPointModelIndex[UserPicsPoint_T::id], true);
    ui->tableViewPoints->setColumnHidden(mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid], true);
    ui->tableViewPoints->horizontalHeader()->setMinimumWidth(10);
    ui->tableViewPoints->horizontalHeader()->setDefaultSectionSize(70);
    ui->tableViewPoints->horizontalHeader()->setCascadingSectionResizes(false);
    ui->tableViewPoints->horizontalHeader()->setSortIndicatorShown(false);
    ui->tableViewPoints->horizontalHeader()->setMovable(false);
    ui->tableViewPoints->horizontalHeader()->setClickable(false);
    ui->tableViewPoints->horizontalHeader()->setHighlightSections(false);

    if ( mHideArcColumns )
    {
        ui->tableViewPoints->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
        ui->tableViewPoints->horizontalHeader()->setResizeMode(2,QHeaderView::ResizeToContents);
        ui->tableViewPoints->horizontalHeader()->setResizeMode(3,QHeaderView::ResizeToContents);
        ui->tableViewPoints->horizontalHeader()->setResizeMode(4,QHeaderView::ResizeToContents);
        ui->tableViewPoints->horizontalHeader()->setResizeMode(7,QHeaderView::ResizeToContents);

        ui->tableViewPoints->setColumnHidden(mUserPicsPointModelIndex[UserPicsPoint_T::arclat], true);
        ui->tableViewPoints->setColumnHidden(mUserPicsPointModelIndex[UserPicsPoint_T::arclon], true);
    }

    ui->tableViewPoints->verticalHeader()->setDefaultSectionSize(20);
    ui->tableViewPoints->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableViewPoints->verticalHeader()->setClickable(false);
    ui->tableViewPoints->verticalHeader()->hide();

    modDelegateType = new CRelModifyDelegate(ui->tableViewPoints);
    ui->tableViewPoints->setItemDelegate(modDelegateType);

    colMap.clear();
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::id], QVariant::UInt);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::sn], QVariant::UInt);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::type], QVariant::UInt);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::lat], QVariant::Double);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::lon], QVariant::Double);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::arclat], QVariant::Double);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::arclon], QVariant::Double);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::arcdist], QVariant::UInt);
    colMap.insert(mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid], QVariant::UInt);
    modDelegateType->SetColsType(colMap);

    CoordDelegate* coordDelegateLat = new CoordDelegate(CoordEdit::Latitude, this);
    ui->tableViewPoints->setItemDelegateForColumn(mUserPicsPointModelIndex[UserPicsPoint_T::lat], coordDelegateLat);
    CoordDelegate* coordDelegateLon = new CoordDelegate(CoordEdit::Longitude, this);
    ui->tableViewPoints->setItemDelegateForColumn(mUserPicsPointModelIndex[UserPicsPoint_T::lon], coordDelegateLon);
    CoordDelegate* coordDelegateArcLat = new CoordDelegate(CoordEdit::Latitude, this);
    ui->tableViewPoints->setItemDelegateForColumn(mUserPicsPointModelIndex[UserPicsPoint_T::arclat], coordDelegateArcLat);
    CoordDelegate* coordDelegateArcLon = new CoordDelegate(CoordEdit::Longitude, this);
    ui->tableViewPoints->setItemDelegateForColumn(mUserPicsPointModelIndex[UserPicsPoint_T::arclon], coordDelegateArcLon);

    connect(mUserPicsPointModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(onDelaySizeToContents()));
    connect(mUserPicsPointModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(onUserPointDataChanged(QModelIndex, QModelIndex)));
    connect(ui->tableViewPoints->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(onTablePointsCurrentRowChanged(QModelIndex, QModelIndex)));
    ui->editFrame->hide();

    connect(ui->pushButtonAdd, SIGNAL(clicked()), SLOT(onButAdd()));
    connect(ui->pushButtonEdit, SIGNAL(clicked()), SLOT(onButEdit()));
    connect(ui->pushButtonDelete, SIGNAL(clicked()), SLOT(onButDelete()));
    connect(ui->pushButtonAddPoint, SIGNAL(clicked()), SLOT(onButAddPoint()));
    connect(ui->pushButtonAddPointPick, SIGNAL(clicked()), SLOT(onButAddPointPick()));
    connect(ui->pushButtonDeletePoint, SIGNAL(clicked()), SLOT(onButDeletePoint()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), SLOT(onButSave()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(onButCancel()));

    UpdateXFont();
    Translate();
    ui->tableViewPoints->viewport()->installEventFilter(this);
}

UserPicsWidget::~UserPicsWidget()
{
    delete ui;
}

void UserPicsWidget::SetModelData(const QString &curUser, int zoneID)
{
    mUser = curUser;
    mZoneID = zoneID;
    mUserPicsModel->setFilter(QString("%1 = %2").arg(UserPics_T::zoneid).arg(mZoneID));
    selectAll(mUserPicsModel);
    setVisability();
    onDelaySizeToContents();
}

void UserPicsWidget::SizeToContents(bool bSend/* = true*/)
{
    QList<QPushButton*> allPButtons = findChildren<QPushButton*>();
    foreach(QPushButton* but, allPButtons)
        but->adjustSize();

    if ( mHideArcColumns )
    {
//        ui->horizontalLayout->invalidate();
//        ui->horizontalLayout_2->invalidate();
    }
    else
    {
        ui->tableViewPics->horizontalHeader()->setStretchLastSection(false);
        ui->tableViewPics->resizeColumnsToContents();

        int iOldStretch = ui->gridLayout->columnStretch(0);
        int iNewStretch = 0;
        if ( ui->tableViewPics->horizontalHeader()->length() < buttonsWidthPics() )
        {
            onDelayStretch();

            if(ui->editFrame->isVisible())
                iNewStretch = 1;
        }
        if(iNewStretch != iOldStretch)
            ui->gridLayout->setColumnStretch(0, iNewStretch);

        ui->tableViewPoints->resizeColumnsToContents();
    }

    if (bSend)
        emit sizeChanged();
}

const QSize UserPicsWidget::GetVisibleSize() const
{
    int layoutSpacer = 6;
    int iWidth = 0;
    int iHeight = 0;

    if ( mHideArcColumns )
    {
        iWidth += layoutSpacer;
        iWidth += buttonsWidthPics();
        if ( ui->editFrame->isVisible() )
        {
            iWidth += layoutSpacer;
            iWidth += buttonsWidthPoints();
        }
        iWidth += layoutSpacer;

        iHeight += layoutSpacer;
        int iH1(0), iH2(0);
        iH1 += ui->tableViewPics->verticalHeader()->length();
        iH1 += ui->tableViewPics->horizontalHeader()->isVisible() ? ui->tableViewPics->horizontalHeader()->height() : 0;
        if ( ui->editFrame->isVisible() )
        {
            iH2 += ui->lineEditName->height();
            iH2 += layoutSpacer;
            iH2 += ui->tableViewPoints->verticalHeader()->length();
            iH2 += ui->tableViewPoints->horizontalHeader()->isVisible() ? ui->tableViewPoints->horizontalHeader()->height() : 0;
        }
        iHeight += qMax(iH1, iH2);
        iHeight += layoutSpacer;
        iHeight += ui->pushButtonAdd->height();
        iHeight += layoutSpacer;
        iH1 = iHeight;
        iHeight = qMax(iH1, 150);
    }
    else
    {
        iWidth += layoutSpacer;
        int iW1 = 0;
        iW1 += ui->tableViewPics->horizontalHeader()->length();
        iW1 += ui->tableViewPics->verticalHeader()->isVisible() ? ui->tableViewPics->verticalHeader()->width() : 0;
        iWidth += qMax(iW1, buttonsWidthPics());
        if ( ui->editFrame->isVisible() )
        {
            iWidth += layoutSpacer;
            iWidth += ui->tableViewPoints->horizontalHeader()->length();
            iWidth += ui->tableViewPoints->verticalHeader()->isVisible() ? ui->tableViewPoints->verticalHeader()->width() : 0;
        }
        iWidth += layoutSpacer;
        iW1 = iWidth;
        iWidth = qMax(iW1, 150);

        iHeight += layoutSpacer;
        int iH1(0), iH2(0);
        iH1 += ui->tableViewPics->verticalHeader()->length();
        iH1 += ui->tableViewPics->horizontalHeader()->isVisible() ? ui->tableViewPics->horizontalHeader()->height() : 0;
        if ( ui->editFrame->isVisible() )
        {
            iH2 += ui->lineEditName->height();
            iH2 += layoutSpacer;
            iH2 += ui->tableViewPoints->verticalHeader()->length();
            iH2 += ui->tableViewPoints->horizontalHeader()->isVisible() ? ui->tableViewPoints->horizontalHeader()->height() : 0;
            iH2 += 16;
        }
        iHeight += qMax(iH1, iH2);
        iHeight += layoutSpacer;
        iHeight += ui->pushButtonAdd->height();
        iHeight += layoutSpacer;
        iH1 = iHeight;
        iHeight = qMax(iH1, 150);
    }

#ifdef Q_OS_LINUX
    iWidth += 8;
    iHeight += 42;
#endif
    return QSize(iWidth + 4, iHeight + 4);

}

void UserPicsWidget::UpdateXFont()
{
    QFont font1 = ui->tableViewPics->font();
    font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    ui->tableViewPics->setFont(font1);
    ui->tableViewPoints->setFont(font1);

    onDelaySizeToContents();
}

void UserPicsWidget::Translate()
{
    ui->retranslateUi(this);

    mUserPicsModel->setHeaderData(mUserPicsModelIndex[UserPics_T::id], Qt::Horizontal, UserPics_T::tr_id());
    mUserPicsModel->setHeaderData(mUserPicsModelIndex[UserPics_T::name], Qt::Horizontal, UserPics_T::tr_name());
    mUserPicsModel->setHeaderData(mUserPicsModelIndex[UserPics_T::zoneid], Qt::Horizontal, UserPics_T::tr_zoneid());

    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::id], Qt::Horizontal, UserPicsPoint_T::tr_id());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::sn], Qt::Horizontal, UserPicsPoint_T::tr_sn());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::type], Qt::Horizontal, UserPicsPoint_T::tr_type());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::lat], Qt::Horizontal, UserPicsPoint_T::tr_lat());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::lon], Qt::Horizontal, UserPicsPoint_T::tr_lon());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::arclat], Qt::Horizontal, UserPicsPoint_T::tr_arclat());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::arclon], Qt::Horizontal, UserPicsPoint_T::tr_arclon());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::arcdist], Qt::Horizontal, UserPicsPoint_T::tr_arcdist());
    mUserPicsPointModel->setHeaderData(mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid], Qt::Horizontal, UserPicsPoint_T::tr_userpicsid());

    onDelaySizeToContents();
}

bool UserPicsWidget::selectAll(QSqlTableModel* model)
{
    bool bRes = false;
    if (model)
    {
        bRes = model->select();
        if (bRes)
        {
            while (model->canFetchMore())
                model->fetchMore();
        }
    }
    return bRes;
}

int UserPicsWidget::buttonsWidthPics() const
{
    int layoutSpacer = 6;
    int iWidth = 0;
    iWidth += layoutSpacer;
    iWidth += ui->pushButtonAdd->width();
    iWidth += layoutSpacer;
    iWidth += ui->pushButtonEdit->width();
    iWidth += layoutSpacer;
    iWidth += ui->pushButtonDelete->width();
    iWidth += 32;
    return iWidth;
}

int UserPicsWidget::buttonsWidthPoints() const
{
    int layoutSpacer = 6;
    int iWidth = 0;
    iWidth += ui->pushButtonAddPoint->width();
    iWidth += layoutSpacer;
    iWidth += ui->pushButtonAddPointPick->width();
    iWidth += layoutSpacer;
    iWidth += ui->pushButtonDeletePoint->width();
    iWidth += layoutSpacer;
    iWidth += ui->pushButtonSave->width();
    iWidth += layoutSpacer;
    iWidth += ui->pushButtonCancel->width();
    iWidth += 32;
    return iWidth;
}

void UserPicsWidget::onStretch()
{
    mDelayStretch = false;
    ui->tableViewPics->horizontalHeader()->setStretchLastSection(true);
}

void UserPicsWidget::onDelayStretch()
{
    if(!mDelayStretch)
        QTimer::singleShot(3, this, SLOT(onStretch()));
    mDelayStretch = true;
}

void UserPicsWidget::onSizeToContents()
{
    mDelaySizeToContents = false;
    SizeToContents();
}

void UserPicsWidget::onDelaySizeToContents()
{
    if(!mDelaySizeToContents)
        QTimer::singleShot(0, this, SLOT(onSizeToContents()));
    mDelaySizeToContents = true;
}

void UserPicsWidget::onDelaySaveSlot()
{
    QSqlRecord qRec1;
    qRec1 = mUserPicsPointModel->record(m_last_edit_row);
    onButSave();

    int row_count = mUserPicsPointModel->rowCount();
    if(m_last_left_column != mUserPicsPointModelIndex[UserPicsPoint_T::arcdist] &&
       m_last_edit_row < row_count - 1)
    {
        if(mUserPicID && row_count > 1)
        {
            QSqlRecord qRec;
            QSqlRecord qRec_next;
            double lat1;
            double lon1;
            double lat2;
            double lon2;
            CCoord coord1;
            CCoord coord2;
            qRec = mUserPicsPointModel->record(m_last_edit_row);
            QString type_line = qRec.value(mUserPicsPointModelIndex[UserPicsPoint_T::type]).toString();
            int get_int_by_string = GetCodeTypeByName(type_line);

            if(get_int_by_string == CBoundaryPoint::enLeftArc ||
               get_int_by_string == CBoundaryPoint::enRightArc)
            {
                lat1 = qRec.value(mUserPicsPointModelIndex[UserPicsPoint_T::lat]).toDouble();
                lon1 = qRec.value(mUserPicsPointModelIndex[UserPicsPoint_T::lon]).toDouble();
                coord1.SetLatitude(CAngle(lat1));
                coord1.SetLongitude(CAngle(lon1));
                qRec_next = mUserPicsPointModel->record(m_last_edit_row+1);
                lat2 = qRec_next.value(mUserPicsPointModelIndex[UserPicsPoint_T::lat]).toDouble();
                lon2 = qRec_next.value(mUserPicsPointModelIndex[UserPicsPoint_T::lon]).toDouble();
                coord2.SetLatitude(CAngle(lat2));
                coord2.SetLongitude(CAngle(lon2));
                coord1.Center(coord2);
                qRec.setValue(mUserPicsPointModelIndex[UserPicsPoint_T::arcdist], (int)GetDistance(coord1, coord2)/(2*100));
            }
            else
               qRec.setValue(mUserPicsPointModelIndex[UserPicsPoint_T::arcdist], 0);

            qRec.setValue(mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid], mUserPicID);
            qRec.setValue(mUserPicsPointModelIndex[UserPicsPoint_T::type], get_int_by_string);
            mUserPicsPointModel->setRecord(m_last_edit_row, qRec);
            onButSave();
        }
    }
    onDelaySave = false;
}

void UserPicsWidget::onUserPointDataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
    if(!onDelaySave)
    {
        m_last_left_column = topLeft.column();
        m_last_edit_row = topLeft.row();
        QTimer::singleShot(0, this, SLOT(onDelaySaveSlot()));
    }
    onDelaySave = true;
}

void UserPicsWidget::onTablePointsCurrentRowChanged(const QModelIndex & current, const QModelIndex & previous)
{
    //qDebug() << "current row:" << current.row();
    //qDebug() << "prev row" << previous.row();
    int row_user_pics_table = ui->tableViewPics->currentIndex().row();
    //qDebug() << "right table row:" << row_user_pics_table;

    if(row_user_pics_table > -1 && current.row() > -1)
    {
        if(m_selected_rows.contains(row_user_pics_table))
            m_selected_rows[row_user_pics_table] = current.row();
        else
            m_selected_rows.insert(row_user_pics_table, current.row());
    }
}

void UserPicsWidget::onUserPicsChanged(const QModelIndex& index)
{
    mUserPicID = 0;
    QString sText;
    mUserPicsLastRow = mUserPicsModel->index(index.row(), mUserPicsModelIndex[UserPics_T::id]).row();
    mUserPicID = mUserPicsModel->data(mUserPicsModel->index(index.row(), mUserPicsModelIndex[UserPics_T::id])).toUInt();
    sText = mUserPicsModel->data(mUserPicsModel->index(index.row(), mUserPicsModelIndex[UserPics_T::name])).toString();
    ui->lineEditName->setText(sText);
    mUserPicsPointModel->setFilter(QString("%1 = %2").arg(UserPicsPoint_T::userpicsid).arg(mUserPicID));
    selectAll(mUserPicsPointModel);
    onDelaySizeToContents();
    ui->pushButtonAddPoint->setEnabled(true);
    ui->pushButtonAddPointPick->setEnabled(true);
    ui->pushButtonCancel->setEnabled(true);
    ui->pushButtonDeletePoint->setEnabled(true);
    ui->pushButtonSave->setEnabled(true);
    if(index.isValid())
    {
        int row = index.row();
        //qDebug() << "pics row:" << row;
        if(m_selected_rows.contains(row))
        {
            //qDebug() << "set row to point:" << m_selected_rows[row];
            ui->tableViewPoints->selectRow(m_selected_rows[row]);
        }
    }

}

void UserPicsWidget::onUserPicsCheckChanged(const QModelIndex& index, bool bCheck)
{
    if ( !mCheckSetting )
    {
        // set in map
        if(index.isValid())
        {
            int userPicID = mUserPicsModel->data(mUserPicsModel->index(index.row(), mUserPicsModelIndex[UserPics_T::id])).toInt();
            mMapUserPicsVisibility[TFIRKey(mUser, mZoneID)][userPicID] = bCheck;
        }

        if(!mDelayCheck)
            QTimer::singleShot(0, this, SLOT(onDelayCheckChanged()));
        mDelayCheck = true;
    }
}

void UserPicsWidget::onDelayCheckChanged()
{
    mDelayCheck = false;
    emit userPicsVisabilityChanged();
}

void UserPicsWidget::setVisability()
{
    mCheckSetting = true;

    const TUPVMap& map = mMapUserPicsVisibility[TFIRKey(mUser, mZoneID)];
    for(int i = 0; i < mUserPicsModel->rowCount(); ++i)
    {
        int userPicID = mUserPicsModel->data(mUserPicsModel->index(i, mUserPicsModelIndex[UserPics_T::id])).toInt();
        bool bCheck = map.value(userPicID, true);
        QModelIndex indChecked = mUserPicsModel->index(i, mUserPicsModel->getCheckedColumn());
        mUserPicsModel->setData(indChecked, bCheck ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }

    mCheckSetting = false;
}

void UserPicsWidget::onButAdd()
{
    if(mZoneID)
    {
        int iRow = 0;
        QString sNew = "New";
        mUserPicsModel->insertRow(0);
        mUserPicsModel->setData(mUserPicsModel->index(iRow, 1), mZoneID);
        mUserPicsModel->setData(mUserPicsModel->index(iRow, 2), sNew);

        try
        {
            QtAutoTransaction dbTrans;
            if (!mUserPicsModel->submitAll())
                throw (mUserPicsModel->lastError().text());
            int iSelID = 0;
            QModelIndexList indexList = mUserPicsModel->match(mUserPicsModel->index(0, mUserPicsModelIndex[UserPics_T::name]), Qt::DisplayRole, sNew, 1, Qt::MatchExactly);
            if(indexList.count())
            {
                mUserPicsModel->setData(mUserPicsModel->index(indexList.first().row(), mUserPicsModelIndex[UserPics_T::name]), "New");
                iSelID = mUserPicsModel->data(mUserPicsModel->index(indexList.first().row(), mUserPicsModelIndex[UserPics_T::id])).toUInt();
            }
            if (!mUserPicsModel->submitAll())
                throw (mUserPicsModel->lastError().text());
            dbTrans.commit();
            indexList = mUserPicsModel->match(mUserPicsModel->index(0, mUserPicsModelIndex[UserPics_T::id]), Qt::DisplayRole, iSelID, 1, Qt::MatchExactly);
            if(indexList.count())
            {
                ui->tableViewPics->selectRow(indexList.first().row());
                ui->tableViewPics->scrollTo(indexList.first());
                onUserPicsChanged(indexList.first());
                QModelIndex indChecked = mUserPicsModel->index(indexList.first().row(), mUserPicsModel->getCheckedColumn());
                mUserPicsModel->setData(indChecked, Qt::Checked, Qt::CheckStateRole);
            }

        }
        catch (const QString &err)
        {
            qCritical() << err;
            QMessageBox::critical(this, tr("Error"), err);
            selectAll(mUserPicsModel);
            setVisability();
        }
        onDelaySizeToContents();
        InitRelation();
    }
}

bool UserPicsWidget::eventFilter ( QObject * watched, QEvent * event )
{
    if(watched == ui->tableViewPoints->viewport())
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent * e = reinterpret_cast<QMouseEvent*>(event);
            if(e->button() == Qt::RightButton)
            {
                QTimer::singleShot(1, this, SLOT(onUserPointPressRight()));
            }
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent * e = reinterpret_cast<QMouseEvent*>(event);
            if(e->button() == Qt::RightButton)
            {
                QTimer::singleShot(1, this, SLOT(onUserPointReleaseRight()));
            }
        }
        if(event->type() == QEvent::Leave)
        {
            if(m_was_selection)
            {
                m_was_selection = false;
                emit userPicsSelectionDrop();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
void UserPicsWidget::onUserPointPressRight()
{
    QModelIndexList selectList = ui->tableViewPoints->selectionModel()->selectedRows();
    if (!selectList.isEmpty())
    {
        if(ui->tableViewPoints->currentIndex().isValid())
        {
            m_was_selection = true;
            int row = ui->tableViewPoints->currentIndex().row();
            emit userPicsSelectChanged(mUserPicsLastRow, row, true);
        }
    }
}
void UserPicsWidget::onUserPointReleaseRight()
{
    QModelIndexList selectList = ui->tableViewPoints->selectionModel()->selectedRows();
    if (!selectList.isEmpty())
    {
        if(ui->tableViewPoints->currentIndex().isValid())
        {
            int row = ui->tableViewPoints->currentIndex().row();
            ui->tableViewPoints->reset();
            emit userPicsSelectChanged(mUserPicsLastRow, row, false);
        }
    }
    QModelIndex index = ui->tableViewPics->currentIndex();
    if(index.isValid())
    {
        int row = index.row();
        if(m_selected_rows.contains(row))
        {
            m_selected_rows[row] = -1;
        }
    }
}
void UserPicsWidget::onButEdit()
{
    bool bVisibly = ui->editFrame->isVisible();
    ui->editFrame->setVisible(!bVisibly);

    onDelaySizeToContents();
    QTimer::singleShot(10, this, SLOT(onDelaySizeToContents()));
}

void UserPicsWidget::onButDelete()
{
    QModelIndexList selectList = ui->tableViewPics->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->tableViewPics->currentIndex().isValid())
        selectList.append(ui->tableViewPics->currentIndex());
    int correction = 0;
    if (!selectList.isEmpty())
    {
        int iRes = QMessageBox::warning(this, tr("User pics"), tr("Delete?"), QMessageBox::Yes | QMessageBox::No);
        if ( iRes != QMessageBox::Yes )
            return;
    }
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == mUserPicsModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        mUserPicsModel->removeRow(rowForDelte);
        if(m_selected_rows.contains(rowForDelte))
        {
            m_selected_rows.remove(rowForDelte);
        }
    }
    if (!selectList.isEmpty())
    {
        try
        {
            QtAutoTransaction dbTrans;
            if (!mUserPicsModel->submitAll())
                throw (mUserPicsModel->lastError().text());
            dbTrans.commit();
        }
        catch (const QString &err)
        {
            qCritical() << err;
            QMessageBox::critical(this, tr("Error"), err);
        }
        selectAll(mUserPicsModel);
        setVisability();

        onDelaySizeToContents();

        emit userPicsChanged();
    }
    if(!mUserPicID)
    {
        ui->pushButtonAddPoint->setEnabled(false);
        ui->pushButtonAddPointPick->setEnabled(false);
        ui->pushButtonCancel->setEnabled(false);
        ui->pushButtonDeletePoint->setEnabled(false);
        ui->pushButtonSave->setEnabled(false);
    }
}

void UserPicsWidget::onButAddPoint()
{
    if (mUserPicID)
    {
        disconnect(mUserPicsPointModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)));
        int row_count = mUserPicsPointModel->rowCount();
        int current_row = ui->tableViewPoints->currentIndex().row();
        int iRow;
        int sn = 0;
        int sn_max = -1;

        if(current_row > -1)
        {
            iRow = current_row + 1;
            QSqlRecord qRec = mUserPicsPointModel->record(current_row);
            if(qRec.contains(UserPicsPoint_T::sn))
            {
                sn = qRec.value(UserPicsPoint_T::sn).toInt();
            }
            if(iRow != row_count)
            {
                qRec = mUserPicsPointModel->record(iRow);
                if(qRec.contains(UserPicsPoint_T::sn))
                {
                    sn_max = qRec.value(UserPicsPoint_T::sn).toInt();
                }
            }
        }
        else
        {
            iRow = 0;
            sn = 0;
            QSqlRecord qRec = mUserPicsPointModel->record(0);
            if(qRec.contains(UserPicsPoint_T::sn))
            {
                sn_max = qRec.value(UserPicsPoint_T::sn).toInt();
            }
        }
        if(sn_max > -1 && (sn_max - sn) <= 10 && (sn_max%10 == 0 || (sn_max/10 - sn/10) == 0))
        {
            for(int i = row_count - 1;i >= iRow; i--)
            {
                QSqlRecord qRec = mUserPicsPointModel->record(i);
                int new_sn = qRec.value(UserPicsPoint_T::sn).toInt();
                new_sn += 10;

                mUserPicsPointModel->insertRow(iRow);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 1), new_sn);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 2), CBoundaryPoint::enLine);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 3), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 4), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 5), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 6), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 7), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 8), mUserPicID);
            }
        }
        QSqlRecord qRec_copy_from;
        switch(iRow)
        {
        case 0:
        {
            qRec_copy_from = mUserPicsPointModel->record(0);
        }
        break;
        default:
        {
            qRec_copy_from = mUserPicsPointModel->record(iRow-1);
        }
        break;
        }
        mUserPicsPointModel->insertRow(iRow);
        QSqlRecord qRec = mUserPicsPointModel->record(iRow);
        sn= (sn/10)*10 + 10;

        if(row_count != 0)
        {
            QString type_line = qRec_copy_from.value(mUserPicsPointModelIndex[UserPicsPoint_T::type]).toString();
            QString lat = qRec_copy_from.value(mUserPicsPointModelIndex[UserPicsPoint_T::lat]).toString();
            QString lon = qRec_copy_from.value(mUserPicsPointModelIndex[UserPicsPoint_T::lon]).toString();
            QString arclat = qRec_copy_from.value(mUserPicsPointModelIndex[UserPicsPoint_T::arclat]).toString();
            QString arclon = qRec_copy_from.value(mUserPicsPointModelIndex[UserPicsPoint_T::arclon]).toString();
            QString arcdist = qRec_copy_from.value(mUserPicsPointModelIndex[UserPicsPoint_T::arcdist]).toString();

            mUserPicsPointModel->insertRow(iRow);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 1), sn);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 2), GetCodeTypeByName(type_line));
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 3), lat);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 4), lon);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 5), arclat);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 6), arclon);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 7), arcdist);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 8), mUserPicID);
        }
        else
        {
            mUserPicsPointModel->insertRow(iRow);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 1), sn);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 2), CBoundaryPoint::enLine);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 3), 0);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 4), 0);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 5), 0);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 6), 0);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 7), 0);
            mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 8), mUserPicID);
        }

        QModelIndex qmIndex = mUserPicsPointModel->index(iRow, mUserPicsPointModelIndex[UserPicsPoint_T::sn]);
        ui->tableViewPoints->setCurrentIndex(qmIndex);
        //ui->tableViewPoints->edit(qmIndex);

        onDelaySizeToContents();
        connect(mUserPicsPointModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(onUserPointDataChanged(QModelIndex, QModelIndex)));
    }
}

void UserPicsWidget::onButAddPointPick()
{
    if (mUserPicID)
    {
        emit getPickCoord(true);
    }
}

void UserPicsWidget::onPasteCoords()
{
    if (mUserPicID)
    {
        disconnect(mUserPicsPointModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)));
        CoordsDigitizer::TCoordList list;
        list = CoordsDigitizer::GetPointsFromClipboard(this);
        if(list.size())
        {
            int row_count = mUserPicsPointModel->rowCount();
            int current_row = ui->tableViewPoints->currentIndex().row();
            int iRow;
            int sn = 0;
            int sn_max = -1;
            if(current_row > -1)
            {
                QSqlRecord qRecCurrent = mUserPicsPointModel->record(current_row);
                if(qRecCurrent.contains(UserPicsPoint_T::sn))
                {
                    sn = qRecCurrent.value(UserPicsPoint_T::sn).toInt();
                }
                iRow = current_row + 1;
            }
            else
            {
                iRow = 0;
                sn = 0;
            }
            if(iRow == row_count)
            {
                sn_max = -1;
            }
            else
            {
                QSqlRecord qRecCurrent = mUserPicsPointModel->record(iRow);
                if(qRecCurrent.contains(UserPicsPoint_T::sn))
                {
                    sn_max = qRecCurrent.value(UserPicsPoint_T::sn).toInt();
                }

            }
            int list_size = list.size();
            int max_current_insert_count_possible = (sn_max - sn - 1)/10;
            if(!(sn_max > -1 && (sn_max - sn) <= 10 && (sn_max%10 == 0 || (sn_max/10 - sn/10) == 0)))
            {
                if(list_size == 1)
                {
                    max_current_insert_count_possible = 1;
                }
            }
            int need_insert = list_size - max_current_insert_count_possible;
            int shift = need_insert*10;
            /*
            qDebug() << "need add:" << list_size <<" insert possible:" << max_current_insert_count_possible <<
                        " need_insert:" <<  need_insert << " shilf:" << shift;
            */
            if(max_current_insert_count_possible > -1 
                    && need_insert > -1 && max_current_insert_count_possible != list_size)
            {
                for(int i = row_count-1; i >= iRow; i--)
                {
                    QSqlRecord qRec = mUserPicsPointModel->record(i);
                    int cur_sn = qRec.value(UserPicsPoint_T::sn).toInt();
                    int new_sn = cur_sn + shift;
                    QString type_line = qRec.value(mUserPicsPointModelIndex[UserPicsPoint_T::type]).toString();
                    QString latitude = qRec.value(mUserPicsPointModelIndex[UserPicsPoint_T::lat]).toString();
                    QString longitude = qRec.value(mUserPicsPointModelIndex[UserPicsPoint_T::lon]).toString();

                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 1), new_sn);
                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 2), GetCodeTypeByName(type_line));
                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 3), latitude);
                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 4), longitude);
                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 5), 0);
                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 6), 0);
                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 7), 0);
                    mUserPicsPointModel->setData(mUserPicsPointModel->index(i, 8), mUserPicID);

                }
            }
            for(int  i = 0; i < list_size; i++)
            {
                CCoord& coord = list[i];
                sn += 10;
                mUserPicsPointModel->insertRow(iRow);

                QSqlRecord qRec = mUserPicsPointModel->record(i);
                int id = qRec.value(UserPicsPoint_T::id).toInt();

                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 1), (sn/10)*10);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 2), CBoundaryPoint::enLine);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 3), coord.GetLatitude().GetAngle());
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 4), coord.GetLongitude().GetAngle());
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 5), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 6), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 7), 0);
                mUserPicsPointModel->setData(mUserPicsPointModel->index(iRow, 8), mUserPicID);

                onDelaySizeToContents();
                QModelIndex index = ui->tableViewPics->currentIndex();
                if(index.isValid())
                {
                    int row = index.row();
                    m_selected_rows[row] = iRow;
                    ui->tableViewPoints->selectRow(iRow);
                    /*if(m_selected_rows.contains(row))
                    {
                        m_selected_rows[row] = iRow-1;
                        ui->tableViewPoints->selectRow(iRow);
                    }*/
                }

                iRow++;
            }
        }
        connect(mUserPicsPointModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(onUserPointDataChanged(QModelIndex, QModelIndex)));
    }
}

int UserPicsWidget::GetCodeTypeByName(QString & name)
{
    CBoundaryPoint::Type type_line_value = CBoundaryPoint::enLine;
    if(name == tr("Line"))
    {
        type_line_value = CBoundaryPoint::enLine;
    }
    if(name == tr("LeftArc"))
    {
        type_line_value = CBoundaryPoint::enLeftArc;
    }
    if(name == tr("RightArc"))
    {
        type_line_value = CBoundaryPoint::enRightArc;
    }
    return type_line_value;
}

void UserPicsWidget::onPickCoord()
{
    onPasteCoords();
}

void UserPicsWidget::onButDeletePoint()
{
    QModelIndexList selectList = ui->tableViewPoints->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->tableViewPoints->currentIndex().isValid())
        selectList.append(ui->tableViewPoints->currentIndex());
    int correction = 0;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == mUserPicsPointModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        mUserPicsPointModel->removeRow(rowForDelte);
        QModelIndex index = ui->tableViewPics->currentIndex();
        if(index.isValid())
        {
            int row = index.row();
            if(m_selected_rows.contains(row))
            {
                m_selected_rows[row] = -1;
                ui->tableViewPoints->reset();
            }
        }
    }
    onDelaySizeToContents();
    onButSave();
}

void UserPicsWidget::onButSave()
{
    if (mUserPicID)
    {
        disconnect(ui->tableViewPoints->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onTablePointsCurrentRowChanged(QModelIndex,QModelIndex)));
        int iSelID = 0;
        QModelIndexList indexList = mUserPicsModel->match(mUserPicsModel->index(0, mUserPicsModelIndex[UserPics_T::id]), Qt::DisplayRole, mUserPicID, 1, Qt::MatchExactly);
        if(indexList.count())
        {
            iSelID = mUserPicsModel->data(mUserPicsModel->index(indexList.first().row(), mUserPicsModelIndex[UserPics_T::id])).toUInt();
        }

        try
        {
            QtAutoTransaction dbTrans;

            if (!mUserPicsPointModel->submitAll())
            {
                connect(ui->tableViewPoints->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(onTablePointsCurrentRowChanged(QModelIndex, QModelIndex)));
                throw (mUserPicsPointModel->lastError().text());
            }

            if(indexList.count())
            {
                mUserPicsModel->setData(mUserPicsModel->index(indexList.first().row(), mUserPicsModelIndex[UserPics_T::name]), ui->lineEditName->text());
            }
            if (!mUserPicsModel->submitAll())
                throw (mUserPicsModel->lastError().text());

            dbTrans.commit();

            indexList = mUserPicsModel->match(mUserPicsModel->index(0, mUserPicsModelIndex[UserPics_T::id]), Qt::DisplayRole, iSelID, 1, Qt::MatchExactly);
            if(indexList.count())
            {
                ui->tableViewPics->selectRow(indexList.first().row());
                ui->tableViewPics->scrollTo(indexList.first());

                onUserPicsChanged(indexList.first());
            }

            onDelaySizeToContents();
            emit userPicsChanged();
        }
        catch (const QString &err)
        {
            qCritical() << err;
            QMessageBox::critical(this, tr("Error"), err);
        }
        //DifferentSN();
        int user_pics_row = ui->tableViewPics->currentIndex().row();
        //qDebug() << "pics row:" << user_pics_row;
        if(m_selected_rows.contains(user_pics_row))
        {
            ui->tableViewPoints->selectRow(m_selected_rows[user_pics_row]);
        }

        connect(ui->tableViewPoints->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(onTablePointsCurrentRowChanged(QModelIndex, QModelIndex)));
    }
}

void UserPicsWidget::DifferentSN()
{
    QMap<int, bool> was_exist;
    int row_count = mUserPicsPointModel->rowCount();
    for(int i = 0; i < row_count; i++)
    {
        QSqlRecord qRec = mUserPicsPointModel->record(i);
        int value_sn = qRec.value(UserPicsPoint_T::sn).toInt();
        if(!was_exist.contains(value_sn))
        {
            was_exist.insert(value_sn, true);
        }
        else
        {
            QMessageBox::warning(this, tr("SN warning"), QString::number(value_sn) + " " + tr("is repeated"));
            return;
        }
    }
}

void UserPicsWidget::onButCancel()
{
    if (mUserPicID)
    {
        int iSelID = 0;
        QModelIndexList indexList = mUserPicsModel->match(mUserPicsModel->index(0, mUserPicsModelIndex[UserPics_T::id]), Qt::DisplayRole, mUserPicID, 1, Qt::MatchExactly);
        if(indexList.count())
        {
            iSelID = mUserPicsModel->data(mUserPicsModel->index(indexList.first().row(), mUserPicsModelIndex[UserPics_T::id])).toUInt();
        }
        mUserPicsPointModel->revertAll();
        mUserPicsModel->revertAll();
        selectAll(mUserPicsModel);
        setVisability();
        indexList = mUserPicsModel->match(mUserPicsModel->index(0, mUserPicsModelIndex[UserPics_T::id]), Qt::DisplayRole, iSelID, 1, Qt::MatchExactly);
        if(indexList.count())
        {
            ui->tableViewPics->selectRow(indexList.first().row());
            ui->tableViewPics->scrollTo(indexList.first());
            onUserPicsChanged(indexList.first());
        }
        onDelaySizeToContents();
    }
}

bool UserPicsWidget::IsUserPicstEnable(const QString& sUser, int idFir, int idUserPics)
{
    TUPVMapFIRs::const_iterator itMap = mMapUserPicsVisibility.constFind(TFIRKey(sUser, idFir));
    if(itMap != mMapUserPicsVisibility.constEnd())
    {
        return itMap->value(idUserPics, true);
    }
    return true;
}

void UserPicsWidget::Load(QDataStream &in)
{
    mMapUserPicsVisibility.clear();
    in >> mMapUserPicsVisibility;
}

void UserPicsWidget::Save(QDataStream &out)
{
    out << mMapUserPicsVisibility;
}

void UserPicsWidget::InitRelation()
{
    QSqlTableModel* pRelationModel = mUserPicsModel->relationModel(mUserPicsModelIndex[UserPics_T::zoneid]);
    if(pRelationModel)
    {
        selectAll(pRelationModel);
        mUserPicsModel->data(mUserPicsModel->index(0, mUserPicsModelIndex[UserPics_T::zoneid]));
    }
    mUserPicsPointModel->setRelation(mUserPicsPointModelIndex[UserPicsPoint_T::type], QSqlRelation(BoundaryPointType_T::tableName, BoundaryPointType_T::id, BoundaryPointType_T::name));
    mUserPicsPointModel->setRelation(mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid], QSqlRelation(UserPics_T::tableName, UserPics_T::id, UserPics_T::name));

    mUserPicsPointModel->setFilter("0 = 1");
    mUserPicsPointModel->setSort(mUserPicsPointModelIndex[UserPicsPoint_T::sn], Qt::AscendingOrder);
    selectAll(mUserPicsPointModel);
    pRelationModel = mUserPicsPointModel->relationModel(mUserPicsPointModelIndex[UserPicsPoint_T::type]);
    if(pRelationModel)
    {
        pRelationModel->setFilter(QString("%1 != %2").arg(BoundaryPointType_T::id).arg(CBoundaryPoint::enCircle));
        selectAll(pRelationModel);
        mUserPicsPointModel->data(mUserPicsPointModel->index(0, mUserPicsPointModelIndex[UserPicsPoint_T::type]));
    }
    pRelationModel = mUserPicsPointModel->relationModel((mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid]));
    if(pRelationModel)
    {
        selectAll(pRelationModel);
        mUserPicsPointModel->data(mUserPicsPointModel->index(0, mUserPicsPointModelIndex[UserPicsPoint_T::userpicsid]));
    }
}
