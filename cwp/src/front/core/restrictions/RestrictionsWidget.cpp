#include "RestrictionsWidget.h"
#include "ui_RestrictionsWidget.h"
#include "UserRestrictionDlg.h"
#include "qtautotransaction.h"
#include "GUIView.h"
#include "Restriction.h"
#include "GUIRestriction.h"
#include "main.h"
#include "XMasterWindow.h"

#include "OLDIMessages.h"

#include <QtSql/QSqlTableModel>
#include <QSqlError>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QItemDelegate>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QInputDialog>
#include <QItemEditorFactory>
#include <QHeaderView>

#include "obelix/CmdSendUserZone.h"

//! Restriction_User table field names.
static const QString g_fieldId = "id";
static const QString g_fieldName = "name";
static const QString g_fieldType = "type";
static const QString g_fieldZoneId = "zone_id";
static const QString g_fieldHMin = "h_min";
static const QString g_fieldHMax = "h_max";
static const QString g_fieldTimeStart = "time_start";
static const QString g_fieldTimeEnd = "time_end";
static const QString g_fieldGUID = "guid";
static const QString g_tableName = "restriction_user";

static const QString g_typeTableName = "restriction_user_type";
static const QString g_typeFieldId = "id";
static const QString g_typeFieldName = "name";

RestrictionsWidget   *gWidgetRestriction = 0;
// True when picking a zone mode active.
static bool gIsPickingZoneModeActive = false;
// Restriction user dialog.
static UserRestrictionDlg *gUserResDlg = 0;


//! Class extends QItemDelegate
class ReModifyDelegate : public QItemDelegate
{
public:
    ReModifyDelegate(QObject* pObject = 0)
        : QItemDelegate(pObject), mIsUserRoleActive(false)
    {}

    //! Inherited from QSqlRelationalDelegate
    virtual QWidget* createEditor(
            QWidget* parent,
            const QStyleOptionViewItem & option,
            const QModelIndex & index ) const;

    void activateUserRole() { mIsUserRoleActive = true; }
    void resetUserRole() { mIsUserRoleActive = false; }

private:
    bool    mIsUserRoleActive;
};

QWidget* ReModifyDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem & option,
                                        const QModelIndex & index ) const
{
    if (!index.isValid())
        return 0;

    const QSqlTableModel *sqlTableModel = dynamic_cast<const QSqlTableModel *>( index.model() );
    // Does not let edit 'Obstacles' if user role is used or admin tries to edit 'TimeEnd' field.
    if ( sqlTableModel )
    {
        int column = sqlTableModel->fieldIndex(g_fieldType);
        QVariant type = sqlTableModel->data( sqlTableModel->index(index.row(), column) );
        if ( type.isValid() && CRestrictionUser::enObstacle == CRestrictionUser::Type(type.toInt()) )
        {
            if ( mIsUserRoleActive ||
                 index.column() == sqlTableModel->fieldIndex(g_fieldTimeEnd) )
                return 0;
        }
    }

    QVariant::Type t = static_cast<QVariant::Type> (index.data(Qt::EditRole).userType());

    const QItemEditorFactory *factory = QItemEditorFactory::defaultFactory();
    QWidget* pEditor = factory->createEditor(t, parent);

    if (dynamic_cast<QDoubleSpinBox*> (pEditor))
        dynamic_cast<QDoubleSpinBox*> (pEditor)->setDecimals(4);
    else if (dynamic_cast<QDateTimeEdit*> (pEditor))
        dynamic_cast<QDateTimeEdit*> (pEditor)->setCalendarPopup(true);

    return pEditor;
}


////////////////////////////////////////////////////////////////////////////////
// RestrictionsWidget implementation.

RestrictionsWidget::RestrictionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestrictionsWidget), mRole(Admin)
{
    ui->setupUi(this);
    gWidgetRestriction = this;
    gUserResDlg = 0;
    ui->toolButtonDelete->setEnabled(false);
    ui->toolButtonEditPoints->setEnabled(false);
    ui->toolButtonHighlight->setEnabled(false);
    setupUi();

    UpdateXFont();
}

RestrictionsWidget::~RestrictionsWidget()
{
    gWidgetRestriction = 0;
    gUserResDlg = 0;
    gIsPickingZoneModeActive = false;
    delete ui;
}

void RestrictionsWidget::setupUi()
{
    QSqlTableModel *restrictionsModel = new QSqlTableModel(this);
    restrictionsModel->setTable(g_tableName);
    restrictionsModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    restrictionsModel->select();

    ReModifyDelegate *itemDelegate = new ReModifyDelegate(ui->tableView);
    ui->tableView->setItemDelegate(itemDelegate);

    connect(restrictionsModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(onDataChanged(const QModelIndex &, const QModelIndex &)), Qt::QueuedConnection);

    ui->tableView->setModel(restrictionsModel);
    ui->tableView->horizontalHeader()->moveSection(
                ui->tableView->horizontalHeader()->visualIndex(restrictionsModel->fieldIndex(g_fieldId)), 0);
    ui->tableView->horizontalHeader()->moveSection(
                ui->tableView->horizontalHeader()->visualIndex(restrictionsModel->fieldIndex(g_fieldName)), 1);
    ui->tableView->horizontalHeader()->moveSection(
                ui->tableView->horizontalHeader()->visualIndex(restrictionsModel->fieldIndex(g_fieldZoneId)), 2);
    ui->tableView->setColumnHidden(restrictionsModel->fieldIndex(g_fieldId), true);
    ui->tableView->setColumnHidden(restrictionsModel->fieldIndex(g_fieldZoneId), true);
    ui->tableView->setColumnHidden(restrictionsModel->fieldIndex(g_fieldType), true);
    ui->tableView->setColumnHidden(restrictionsModel->fieldIndex(g_fieldGUID), true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex & , const QModelIndex &)),
            SLOT(onCurrentRowChanged(const QModelIndex & , const QModelIndex &)));

    // Init combo box with loaded FIR names.
    if ( gActiveView )
    {
        const CView::TSceneList &firs = gActiveView->GetScenes();
        for ( CView::TSceneList::const_iterator itc = firs.constBegin();
              itc != firs.constEnd(); ++itc )
        {
            const CView::TScenePtr &fir = *itc;
            addFir(fir->GetID(), fir->GetIDName(), fir->GetName());
        }

        int idMainFir = gActiveView->GetMainFIR() ? gActiveView->GetMainFIR()->GetID() : -1;
        // Select main FIR in the combo list of FIRs;
        for ( int i = 0; i < ui->comboBoxFIR->count(); i++ )
        {
            int firId = ui->comboBoxFIR->itemData(i).toInt();
            if ( idMainFir == firId )
            {
                ui->comboBoxFIR->setCurrentIndex(i);
                break;
            }
        }
    }

    translate();
}

void RestrictionsWidget::setUserRole(UserRole role)
{
    mRole = role;

    ReModifyDelegate *itemDelegate = static_cast<ReModifyDelegate *>( ui->tableView->itemDelegate() );
    mRole == Admin ? itemDelegate->resetUserRole() : itemDelegate->activateUserRole();
}

void RestrictionsWidget::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_ASSERT(gActiveView);

    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>(ui->tableView->model());

    quint32 restId = restrictionsModel->data( restrictionsModel->index(topLeft.row(),
                                                      restrictionsModel->fieldIndex(g_fieldId)) ).toUInt();
    quint32 zoneId = ui->comboBoxFIR->count() < 1 ? 0 :
                                                ui->comboBoxFIR->itemData(ui->comboBoxFIR->currentIndex()).toUInt();

    // Update restriction zone in the scene.
    CView::TSceneList &firs = gActiveView->GetScenes();
    for ( CView::TSceneList::iterator it = firs.begin(); it != firs.end(); ++it )
    {
        CView::TScenePtr &fir = *it;
        if ( fir->GetID() == zoneId )
        {
            for ( Scene::TRestrictionUserList::iterator itr = fir->GetRestrictionUser().begin();
                  itr != fir->GetRestrictionUser().end(); ++itr )
            {
                QSharedPointer<CRestrictionUser> &r = *itr;

                if ( restId != r->GetID() )
                    continue;

                r->SetName( restrictionsModel->data(
                               restrictionsModel->index(topLeft.row(), restrictionsModel->fieldIndex(g_fieldName))).toString() );
                r->SetHMin( restrictionsModel->data(
                               restrictionsModel->index(topLeft.row(), restrictionsModel->fieldIndex(g_fieldHMin))).toDouble() );
                r->SetHMax( restrictionsModel->data(
                               restrictionsModel->index(topLeft.row(), restrictionsModel->fieldIndex(g_fieldHMax))).toDouble() );
                r->SetTimeBegin( restrictionsModel->data(
                               restrictionsModel->index(topLeft.row(), restrictionsModel->fieldIndex(g_fieldTimeStart))).toDateTime() );
                r->SetTimeEnd( restrictionsModel->data(
                               restrictionsModel->index(topLeft.row(), restrictionsModel->fieldIndex(g_fieldTimeEnd))).toDateTime() );

                // Do this in the case begin or end time is changed.
                CGUIRestrictionUser *ru = dynamic_cast<CGUIRestrictionUser *>( r.data() );
                if ( ru )
                    ru->updateVisibility();

                sendUserZone(r, QString(OLDIMesURZ::enUpdate), fir->GetIDName());

                break;
            }
            break;
        }
    }

    SizeToContents();
}

void RestrictionsWidget::onCurrentRowChanged(const QModelIndex & current,
                                             const QModelIndex & previous)
{
    if ( !current.isValid() )
    {
        ui->toolButtonDelete->setEnabled(false);
        ui->toolButtonEditPoints->setEnabled(false);
        ui->toolButtonHighlight->setEnabled(false);
        return;
    }

    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>(ui->tableView->model());

    CRestrictionUser::Type typeRestriction = (CRestrictionUser::Type) restrictionsModel->data(
                restrictionsModel->index(current.row(), restrictionsModel->fieldIndex(g_fieldType))).toInt();

    QDateTime timeStart = restrictionsModel->data(
                restrictionsModel->index(current.row(), restrictionsModel->fieldIndex(g_fieldTimeStart))).toDateTime();
    QDateTime timeEnd = restrictionsModel->data(
                restrictionsModel->index(current.row(), restrictionsModel->fieldIndex(g_fieldTimeEnd))).toDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();

    bool isButtonEnabled = mRole == Admin ? true : (CRestrictionUser::enObstacle != typeRestriction);
    ui->toolButtonDelete->setEnabled(isButtonEnabled);
    ui->toolButtonEditPoints->setEnabled(isButtonEnabled);
    ui->toolButtonHighlight->setEnabled( (timeStart.isNull() || timeStart <= currentDateTime)
                                         && (currentDateTime <= timeEnd || timeEnd.isNull()) );
}

void RestrictionsWidget::translate()
{
    ui->retranslateUi(this);

    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>(ui->tableView->model());

    restrictionsModel->setHeaderData(restrictionsModel->fieldIndex( g_fieldName ), Qt::Horizontal, tr("Name"));
    restrictionsModel->setHeaderData(restrictionsModel->fieldIndex( g_fieldHMin ), Qt::Horizontal, tr("Alt. Min"));
    restrictionsModel->setHeaderData(restrictionsModel->fieldIndex( g_fieldHMax ), Qt::Horizontal, tr("Alt. Max"));
    restrictionsModel->setHeaderData(restrictionsModel->fieldIndex( g_fieldTimeStart ), Qt::Horizontal, tr("Time Begin"));
    restrictionsModel->setHeaderData(restrictionsModel->fieldIndex( g_fieldTimeEnd ), Qt::Horizontal, tr("Time End"));

    SizeToContents();
}

void RestrictionsWidget::addFir(uint id, const QString &idName, const QString &name)
{
    ui->comboBoxFIR->addItem(QString("%1 (%2)").arg(idName).arg(name), id);
}

void RestrictionsWidget::removeFir(uint id)
{
    for ( int i = 0; i < ui->comboBoxFIR->count(); i++ )
    {
        if ( id == ui->comboBoxFIR->itemData(i).toUInt() )
        {
            ui->comboBoxFIR->removeItem(i);
            break;
        }
    }
    if ( ui->comboBoxFIR->count() < 1 )
        on_comboBoxFIR_currentIndexChanged(-1);
    else
        on_comboBoxFIR_currentIndexChanged(ui->comboBoxFIR->currentIndex());
}

void RestrictionsWidget::removeFirs()
{
    ui->comboBoxFIR->clear();
    on_comboBoxFIR_currentIndexChanged(-1);
}

void RestrictionsWidget::on_comboBoxFIR_currentIndexChanged(int index)
{
    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>(ui->tableView->model());

    if ( index != -1 && index < ui->comboBoxFIR->count() )
    {
        quint32 zoneId = ui->comboBoxFIR->itemData(index).toUInt();
        restrictionsModel->setFilter(QString("zone_id = %1").arg(zoneId));
    }
    else
        restrictionsModel->setFilter("0 = 1");
    restrictionsModel->select();

    SizeToContents();
}

void RestrictionsWidget::rejectPickingMode()
{
    if ( gIsPickingZoneModeActive && gActiveView )
    {
        // Avoid of firing addZone slot.
        gIsPickingZoneModeActive = false;

        gActiveView->stopCoordsPicking();
    }

    gIsPickingZoneModeActive = false;
}

void RestrictionsWidget::on_toolButtonAdd_clicked()
{
    // Avoid of firing addZone() func if "Add" button is called more
    // than one time.
    gIsPickingZoneModeActive = false;

    // Call view delegate to start editing of new restriction zone in the scene.
    if ( gActiveView )
        gActiveView->startCoordsPicking();

    gIsPickingZoneModeActive = true;
}

void RestrictionsWidget::on_toolButtonDelete_clicked()
{
    rejectPickingMode();

    Q_ASSERT(gActiveView);

    quint32 zoneId = ui->comboBoxFIR->count() < 1 ? 0 :
                                                ui->comboBoxFIR->itemData(ui->comboBoxFIR->currentIndex()).toUInt();
    Q_ASSERT(zoneId >= 0);

    QModelIndexList selectList = ui->tableView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->tableView->currentIndex().isValid())
        selectList.append(ui->tableView->currentIndex());
    if ( selectList.empty() )
        return;

    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>( ui->tableView->model() );

    QString zoneName = restrictionsModel->data(
                restrictionsModel->index(selectList.at(0).row(), restrictionsModel->fieldIndex(g_fieldName))).toString();
    if ( QMessageBox::question(this, tr("Delete zone"),
                               tr("Would you like to remove '%1' user restriction zone?").arg(zoneName),
         QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes )
        return;

    // Remove restriction zone from table and database ...

    QList<quint32> delIdList;
    foreach (const QModelIndex &selected, selectList)
    {
        QModelIndex qmIndex = restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldId));
        delIdList << restrictionsModel->data(qmIndex).toUInt();
        restrictionsModel->removeRow(selected.row());
        break;
    }
    QSqlError lastError = restrictionsModel->lastError();
    if ( lastError.isValid() )
    {
        QMessageBox::warning(this, tr("Remove zone"), lastError.text());
        return;
    }

    // Remove restriction zone from scene and cache ...

    CView::TSceneList &firs = gActiveView->GetScenes();
    for ( CView::TSceneList::iterator it = firs.begin(); it != firs.end(); ++it )
    {
        CView::TScenePtr &fir = *it;
        if ( fir->GetID() == zoneId )
        {
            foreach ( quint32 restId, delIdList )
            {
                QSharedPointer<CRestrictionUser> restrictionZone = fir->DelUserRestriction(restId);
                QGraphicsItem *guiItem = dynamic_cast<QGraphicsItem *>( restrictionZone.data() );
                if ( guiItem )
                    gActiveView->scene()->removeItem(guiItem);

                sendUserZone(restrictionZone, QString(OLDIMesURZ::enDelete), fir->GetIDName());
            }
            break;
        }
    }

    SizeToContents();
}

void RestrictionsWidget::on_toolButtonEditPoints_clicked()
{
    rejectPickingMode();

    Q_ASSERT(gActiveView);

    QModelIndexList selectList = ui->tableView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->tableView->currentIndex().isValid())
        selectList.append(ui->tableView->currentIndex());

    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>(ui->tableView->model());

    foreach (const QModelIndex &selected, selectList)
    {
        QModelIndex qmIndex = restrictionsModel->index(selected.row(),
                                                       restrictionsModel->fieldIndex(g_fieldId));
        if ( !qmIndex.isValid() )
            continue;

        // Open editor dialog ...

        if ( gUserResDlg )
            gUserResDlg->activateWindow();
        else
        {
            CRestrictionUser r;
            r.SetID( restrictionsModel->data(
                         restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldId))).toUInt() );
            r.SetName( restrictionsModel->data(
                           restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldName))).toString() );
            r.SetType( (CRestrictionUser::Type) restrictionsModel->data(
                           restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldType))).toInt() );
            r.SetHMin( restrictionsModel->data(
                           restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldHMin))).toDouble() );
            r.SetHMax( restrictionsModel->data(
                           restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldHMax))).toDouble() );
            r.SetTimeBegin( restrictionsModel->data(
                           restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldTimeStart))).toDateTime() );
            r.SetTimeEnd( restrictionsModel->data(
                           restrictionsModel->index(selected.row(), restrictionsModel->fieldIndex(g_fieldTimeEnd))).toDateTime() );

            gUserResDlg = new UserRestrictionDlg(UserRestrictionDlg::UserRole(mRole), this);
            gUserResDlg->insertZone(r);
            connect(gUserResDlg, SIGNAL(finished(int)), SLOT(onUserresFinished(int)));
        }

        gUserResDlg->show();
        break;
    }
}

void RestrictionsWidget::on_toolButtonHighlight_clicked()
{
    Q_ASSERT(gActiveView);

    QModelIndexList selectList = ui->tableView->selectionModel()->selectedRows();
    if (selectList.isEmpty() && ui->tableView->currentIndex().isValid())
        selectList.append(ui->tableView->currentIndex());

    if ( selectList.empty() )
        return;

    QModelIndex index = selectList.at(0);

    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>( ui->tableView->model() );
    quint32 restrictionId = restrictionsModel->data(
                restrictionsModel->index(index.row(), restrictionsModel->fieldIndex(g_fieldId)) ).toUInt();

    quint32 zoneId = 0;
    if ( ui->comboBoxFIR->count() )
        zoneId = ui->comboBoxFIR->itemData(ui->comboBoxFIR->currentIndex()).toUInt();

    // Highlight restriction zone in the scene for 500 ms.
    CView::TSceneList &firs = gActiveView->GetScenes();
    for ( CView::TSceneList::iterator it = firs.begin(); it != firs.end(); ++it )
    {
        CView::TScenePtr &fir = *it;
        if ( fir->GetID() == zoneId )
        {
            for ( Scene::TRestrictionUserList::iterator itr = fir->GetRestrictionUser().begin();
                  itr != fir->GetRestrictionUser().end(); ++itr )
            {
                QSharedPointer<CRestrictionUser> &r = *itr;
                if ( r->GetID() == restrictionId )
                {
                    CGUIRestrictionUser *guiItem = dynamic_cast<CGUIRestrictionUser *>( r.data() );
                    if ( guiItem )
                        guiItem->highlight(true, 500);
                    break;
                }
            }
            break;
        }
    }
}

void RestrictionsWidget::addZone()
{
    // Restriction zone was not requested for creation.
    if ( !gIsPickingZoneModeActive )
        return;

    gIsPickingZoneModeActive = false;
    setEnabled(false);

    quint32 zoneId = 0;
    if ( ui->comboBoxFIR->count() )
        zoneId = ui->comboBoxFIR->itemData(ui->comboBoxFIR->currentIndex()).toUInt();

    const QString typeCircle = tr("Circle");
    const QString typePolygon = tr("Polygon");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Choose restriction zone type"),
                                         tr("Zone type:"),
                                         QStringList() << typeCircle << typePolygon,
                                         0, false, &ok);
    if ( ok && !item.isEmpty() )
    {
        if ( gUserResDlg )
            gUserResDlg->activateWindow();
        else
        {
            gUserResDlg = new UserRestrictionDlg(UserRestrictionDlg::UserRole(mRole), this);
            gUserResDlg->setFirId(zoneId);
            gUserResDlg->insertPointsFromClipboard(
                        item == typeCircle ? UserRestrictionDlg::Circle : UserRestrictionDlg::Polygon);
            connect(gUserResDlg, SIGNAL(finished(int)), SLOT(onUserresFinished(int)));
        }

        gUserResDlg->show();
        return;
    }

    setEnabled(true);
}

void RestrictionsWidget::onUserresFinished(int result)
{
    setEnabled(true);

    quint32 restrictionId = gUserResDlg->getId();
    // Destroy editor dialog.
    gUserResDlg->close();
    gUserResDlg = 0;

    if ( result != QDialog::Accepted )
        return;

    // Update table model ...

    QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>( ui->tableView->model() );
    restrictionsModel->select();

    SizeToContents();

    // Add restriction zone to the scene ...

    quint32 zoneId = ui->comboBoxFIR->count() < 1 ? 0 :
                                                ui->comboBoxFIR->itemData(ui->comboBoxFIR->currentIndex()).toUInt();

    TVA_CATCHALL_TRY
    {
        CView::TSceneList &firs = gActiveView->GetScenes();
        for ( CView::TSceneList::iterator it = firs.begin(); it != firs.end(); ++it )
        {
            CView::TScenePtr &fir = *it;
            if ( fir->GetID() == zoneId )
            {
                // First - remove restriction zone from scene and cache (if it is there).
                QSharedPointer<CRestrictionUser> restrictionZone = fir->DelUserRestriction(restrictionId);
                QGraphicsItem *guiItem = dynamic_cast<QGraphicsItem *>( restrictionZone.data() );
                if ( guiItem )
                    gActiveView->scene()->removeItem(guiItem);

                // Load from database.
                dao()->LoadUserRestriction(*fir.data(), restrictionId);

                // Add modified(created) restriction zone to scene.
                for ( Scene::TRestrictionUserList::iterator itr = fir->GetRestrictionUser().begin();
                      itr != fir->GetRestrictionUser().end(); ++itr )
                {
                    QSharedPointer<CRestrictionUser> &r = *itr;
                    if ( r->GetID() == restrictionId )
                    {
                        CGUIRestrictionUser *guiItem = dynamic_cast<CGUIRestrictionUser *>( r.data() );
                        if ( guiItem )
                        {
                            gActiveView->scene()->addItem( guiItem );

                            qint64 qllXMin, qllYMax;
                            double dScale;
                            CCoord centralCoord;
                            gActiveView->GetCoordSystem(qllXMin, qllYMax, dScale, centralCoord);
                            guiItem->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
                        }

                        sendUserZone(r, QString(OLDIMesURZ::enInsert), fir->GetIDName());

                        break;
                    }
                }
                break;
            }
        }
    }
    TVA_CATCHALL(ex)
    {
        QMessageBox::warning(this, tr("Inserting zone to scene"), ex.reason());
    }
}

void RestrictionsWidget::showEvent(QShowEvent * event)
{
    UpdateXFont();
    SizeToContents();
    QWidget::showEvent(event);
}

void RestrictionsWidget::UpdateXFont()
{
    if(ui->tableView)
    {
        QFont font1 = ui->tableView->font();
        font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        ui->tableView->setFont(font1);
        SizeToContents();
    }
}

void RestrictionsWidget::closeEvent(QCloseEvent* event)
{
    // Close doc widget - this is necessary to open widget after it is closed
    // by the user.
    QWidget *mainWindow = getMainWindow();
    if ( mainWindow && qobject_cast<XMasterWindow *>(mainWindow) )
    {
        rejectPickingMode();

        XMasterWindow *masterWindow = qobject_cast<XMasterWindow *>(mainWindow);
        masterWindow->CloseDoc(mRole == User ? XDocWidget::RestrictionsUserEditor :
                                               XDocWidget::RestrictionsAdminEditor);
    }

    QWidget::closeEvent(event);
}

void RestrictionsWidget::changeEvent(QEvent* event)
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

void RestrictionsWidget::sendUserZone(const QSharedPointer<CRestrictionUser>& pZone, const QString& sType, const QString& sFirIdName)
{
    if(pZone && !sType.isEmpty() && !sFirIdName.isEmpty() && master())
    {
        QByteArray arr;
        QDataStream strm(&arr, QIODevice::WriteOnly);
        strm << *pZone;

        QSharedPointer<obelix::CCmdSendUserZone> pCmd(new obelix::CCmdSendUserZone(sType, sFirIdName, arr.toHex()));
        master()->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());

//        OLDIMesDataURZ mesData;
//        mesData.iMesNum = master()->GetOLDIProc()->GetNextMesNum();
//        mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
//        mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
//        mesData.sDBType = sType;
//        mesData.sFirIdName = sFirIdName;
//        QByteArray arr;
//        QDataStream strm(&arr, QIODevice::WriteOnly);
//        strm << *pZone;
//        mesData.sHexData = arr.toHex();
//        QSharedPointer<OLDIMessage> pMes(new OLDIMesURZ(mesData));
//        //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
//        //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
//        master()->GetOLDIProc()->SendMessage(pMes, false);
    }
}

void RestrictionsWidget::Refresh()
{
    int ind = ui->comboBoxFIR->currentIndex();
    QList<QDialog*> lstMsgBox = findChildren<QDialog*>();
    for(QList<QDialog*>::iterator it = lstMsgBox.begin(); it != lstMsgBox.end(); ++it)
        (*it)->close();
    gUserResDlg = 0;
    QMetaObject::invokeMethod(this, "on_comboBoxFIR_currentIndexChanged", Qt::QueuedConnection, Q_ARG(int, ind));
}

void RestrictionsWidget::on_toolButtonRequest_clicked()
{
    rejectPickingMode();

    if(gActiveView)
    {
        quint32 zoneId = ui->comboBoxFIR->count() < 1 ? 0 :
                                                    ui->comboBoxFIR->itemData(ui->comboBoxFIR->currentIndex()).toUInt();

        for ( CView::TSceneList::const_iterator it = gActiveView->GetScenes().constBegin(); it != gActiveView->GetScenes().constEnd(); ++it )
        {
            if ( (*it)->GetID() == zoneId )
            {
                if ( QMessageBox::question(this, tr("Request zones"),
                                           tr("Would you like to request users restriction zones for '%1' from FDP?\n(current zones will be deleted)")
                                            .arg((*it)->GetIDName()),
                     QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes )
                    return;

                // Remove restriction zones from table and database ...
                QSqlTableModel *restrictionsModel = static_cast<QSqlTableModel *>( ui->tableView->model() );
                QList<quint32> delIdList;
                for (int i = 0; i < restrictionsModel->rowCount(); ++i)
                {
                    QModelIndex qmIndex = restrictionsModel->index(i, restrictionsModel->fieldIndex(g_fieldId));
                    delIdList << restrictionsModel->data(qmIndex).toUInt();
                }
                restrictionsModel->removeRows(0, restrictionsModel->rowCount());
                QSqlError lastError = restrictionsModel->lastError();
                if ( lastError.isValid() )
                {
                    QMessageBox::warning(this, tr("Remove zone"), lastError.text());
                    return;
                }
                SizeToContents();

                // Remove restriction zones from scene and cache ...
                foreach ( quint32 restId, delIdList )
                {
                    QSharedPointer<CRestrictionUser> restrictionZone = (*it)->DelUserRestriction(restId);
                    QGraphicsItem *guiItem = dynamic_cast<QGraphicsItem *>( restrictionZone.data() );
                    if ( guiItem )
                        gActiveView->scene()->removeItem(guiItem);
                }

                QSharedPointer<CRestrictionUser> pRestr(new CRestrictionUser);
                sendUserZone(pRestr, QString(OLDIMesURZ::enRequest), (*it)->GetIDName());

                break;
            }
        }
    }
}

const QSize RestrictionsWidget::GetVisibleSize() const
{
    int layoutSpacing = qobject_cast<QGridLayout*>( layout() ) ?
                qobject_cast<QGridLayout*>( layout() )->spacing() : 0;
    int iWidth = qMax(ui->tableView->horizontalHeader()->length(), 400),
        iHeight = ui->tableView->verticalHeader()->length();
    iHeight += ui->tableView->horizontalHeader()->isVisible() ? ui->tableView->horizontalHeader()->height() : 0;
    iHeight += ui->comboBoxFIR->height() + layoutSpacing;
    iHeight += layoutSpacing;
#ifdef Q_OS_LINUX
    iWidth += 8;
    iHeight += 42;
#endif
    return QSize(iWidth + 4, iHeight + 4);
}

void RestrictionsWidget::SizeToContents(bool bSend/* = true*/)
{
    ui->tableView->resizeColumnsToContents();
    if (bSend)
        emit sizeChanged();
}
