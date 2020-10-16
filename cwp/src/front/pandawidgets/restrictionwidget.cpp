#include <QCompleter>

#include "restrictionwidget.h"
#include "ui_restrictionwidget.h"

#include "restrictionlistwidgetitem.h"
#include "RestrRouteItem.h"
#include "RestRouteCurrentItem.h"

#include "listwidgetdelegate.h"
#include "coordedit.h"
#include "ListAC.h"

#include "main.h"

extern QString currentUserName;

//const QRegExp regExpGeo = QRegExp(QString::fromUtf8("(\\d{2}|\\d{4})([NS\u0421\u042E])(\\d{3}|\\d{5})([WE\u0417\u0412])"));
const QRegExp regExpName("[\\d,\\w]{0,5}");

RestrictionWidget::RestrictionWidget(QWidget *parent) :
    BaseToolDialog(false, true, parent),
    ui(new Ui::RestrictionWidget),
    restriction_(0),
    active_point_(100, false),
    pick_mode_(false),
    mouse_press_(false),
    completer_(new QCompleter(this))
{
    ui->setupUi(this);
    updateLanguage();
    new_restriction_ = false;

    completer_->setCompletionMode(QCompleter::PopupCompletion);

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    for (auto it = CRestrictionUser::TYPE_DESCRIPTION.constBegin(); it != CRestrictionUser::TYPE_DESCRIPTION.constEnd(); ++it)
    {
        ui->cbRestrictionType->addItem(it.value(), it.key());
    }

    ui->cbRestrictionShape->addItem(RestrListWidgetItem::IDTypeToText(CBoundaryPoint::enLine), CBoundaryPoint::enLine);
    ui->cbRestrictionShape->addItem(RestrListWidgetItem::IDTypeToText(CBoundaryPoint::enCircle), CBoundaryPoint::enCircle);

    //ui->lwRestrictions->setItemDelegate(new ListWidgetDelegate(ui->lwRestrictions));

    /*for (int i = 1; i < 11; ++i)//???
        ui->cbPagePoly->addItem(QString(tr("Page %1")).arg(i));*/

    for (int i = 1; i < 26; ++i)
        ui->cbPageLine->addItem(QString(tr("Page %1")).arg(i));

    QIntValidator *int_validator = new QIntValidator(1, 99999, this);
    ui->leRestrictionLowerHeight->setValidator(int_validator);
    ui->leRestrictionUpperHeight->setValidator(int_validator);
    ui->leFirstLineLowerHeight->setValidator(int_validator);
    ui->leFirstLineUpperHeight->setValidator(int_validator);
    ui->leSecondLineLowerHeight->setValidator(int_validator);
    ui->leSecondLineUpperHeight->setValidator(int_validator);
    ui->leCircleRadius->setValidator(int_validator);
    ui->leBigAxis->setValidator(int_validator);
    ui->leSmallAxis->setValidator(int_validator);
    ui->leAngle->setValidator(int_validator);

    showControls(false);

    ui->leCircleCenter->setValidator(new QRegExpValidator(regExpName, this));
    ui->leEllipseCenter->setValidator(new QRegExpValidator(regExpName, this));

    ui->leFirstLineStartPoint->setValidator(new QRegExpValidator(regExpName, this));
    ui->leFirstLineEndPoint->setValidator(new QRegExpValidator(regExpName, this));
    ui->leSecondLineStartPoint->setValidator(new QRegExpValidator(regExpName, this));
    ui->leSecondLineEndPoint->setValidator(new QRegExpValidator(regExpName, this));

    ui->leCircleCenter->setCompleter(completer_);

    ui->tableWidgetRestrictionPoint->setShowGrid(true);
    ui->tableWidgetRestrictionPoint->setColumnCount(3);
    ui->tableWidgetRestrictionPoint->horizontalHeader()->hide();
    ui->tableWidgetRestrictionPoint->verticalHeader()->hide();

    ui->tableWidgetRestrictionPoint->setColumnWidth(0, 30);
    ui->tableWidgetRestrictionPoint->setColumnWidth(1, 90);

    ui->tableWidgetRestrictionPoint->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidgetRestrictionPoint->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetRestrictionPoint->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetRestrictionPoint->setSortingEnabled(false);
    connect(ui->tableWidgetRestrictionPoint->selectionModel(),SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(OnChangeSelect(const QItemSelection &, const QItemSelection &)));

    ui->frame->hide();
    ui->showButton->setDisabled(false);
    ui->hideButton->setDisabled(true);
    ui->upButton->setDisabled(true);
    ui->downButton->setDisabled(true);
    ui->pbDel->setEnabled(false);
    ui->pbCreate->setHidden(true);
    ui->pbDelete->setHidden(true);

    connect(ui->pbCreate, SIGNAL(clicked()), SLOT(createRestriction()));
    connect(ui->pbUpdate, SIGNAL(clicked()), SLOT(updateRestriction()));
    connect(ui->pbDelete, SIGNAL(clicked()), SLOT(deleteRestriction()));

    connect(ui->cbRestrictionType, SIGNAL(currentIndexChanged(int)), SLOT(onRestrictionTypeChanged(int)));
    connect(ui->cbRestrictionShape, SIGNAL(currentIndexChanged(int)), SLOT(onRestrictionShapeChanged(int)));

    connect(ui->cbCircleCenterPointType, SIGNAL(currentIndexChanged(int)), SLOT(circleCenterPointTypeChanged(int)));

    connect(ui->cbFirstLineStartType, SIGNAL(currentIndexChanged(int)), SLOT(firstLineStartTypeChanged(int)));
    connect(ui->cbFirstLineEndType, SIGNAL(currentIndexChanged(int)), SLOT(firstLineEndTypeChanged(int)));
    connect(ui->cbSecondLineStartType, SIGNAL(currentIndexChanged(int)), SLOT(secondLineStartTypeChanged(int)));
    connect(ui->cbSecondLineEndType, SIGNAL(currentIndexChanged(int)), SLOT(secondLineEndTypeChanged(int)));


    connect(ui->pbAdd, SIGNAL(clicked()), SLOT(addClicked()));
    connect(ui->pbDel, SIGNAL(clicked()), SLOT(deleteClicked()));

    connect(ui->upButton, SIGNAL(clicked()), SLOT(upButtonClicked()));
    connect(ui->downButton, SIGNAL(clicked()), SLOT(downButtonClicked()));
    connect(ui->showButton, SIGNAL(clicked()), SLOT(showButtonClicked()));
    connect(ui->hideButton, SIGNAL(clicked()), SLOT(hideButtonClicked()));


    // Line
    connect(ui->leFirstLineStartPoint, SIGNAL(editingFinished()), SLOT(onleLineStartPointEditingFinished01()));
    connect(ui->leFirstLineEndPoint, SIGNAL(editingFinished()), SLOT(onleLineEndPointEditingFinished01()));
    connect(ui->leSecondLineStartPoint, SIGNAL(editingFinished()), SLOT(onleLineStartPointEditingFinished02()));
    connect(ui->leSecondLineEndPoint, SIGNAL(editingFinished()), SLOT(onleLineEndPointEditingFinished02()));

    connect(ui->leFirstLineLowerHeight, SIGNAL(editingFinished()), SLOT(onleFirstLineLowerHeightEditingFinished()));
    connect(ui->leFirstLineUpperHeight, SIGNAL(editingFinished()), SLOT(onleFirstLineUpperHeightEditingFinished()));

    connect(ui->leSecondLineLowerHeight, SIGNAL(editingFinished()), SLOT(onleSecondLineLowerHeightEditingFinished()));
    connect(ui->leSecondLineUpperHeight, SIGNAL(editingFinished()), SLOT(onleSecondLineUpperHeightEditingFinished()));

    // Ellipse
    connect(ui->cbEllipceCenter, SIGNAL(currentIndexChanged(int)), SLOT(ellipseCenterPointTypeChanged(int)));

    // Global Date
    connect(ui->deRestrictionStartDate, SIGNAL(dateChanged(QDate)), SLOT(onGlobalStartDateChange(QDate)));
    connect(ui->deRestrictionEndDate, SIGNAL(dateChanged(QDate)), SLOT(onGlobalEndDateChange(QDate)));

    // Global Time
    //connect(ui->teRestrictionStart, SIGNAL(timeChanged(QTime)), SLOT(onGlobalStartTimeChange(QTime)));
    //connect(ui->teRestrictionEnd, SIGNAL(timeChanged(QTime)), SLOT(onGlobalEndTimeChange(QTime)));

    // Line time
    //connect(ui->teFirstLineStartTime, SIGNAL(timeChanged(QTime)), SLOT(onFirstLineStartTime(QTime)));
    //connect(ui->teFirstLineEndTime, SIGNAL(timeChanged(QTime)), SLOT(onFirstLineEndTime(QTime)));
    //connect(ui->teSecondLineStartTime, SIGNAL(timeChanged(QTime)), SLOT(onSecondLineStartTime(QTime)));
    //connect(ui->teSecondLineEndTime, SIGNAL(timeChanged(QTime)), SLOT(onSecondLineEndTime(QTime)));


    connect(ui->lwRestrictions, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            SLOT(currentItemChanged(QListWidgetItem *, QListWidgetItem *)));

    // Picker
    connect(ui->pbPickPoly, SIGNAL(toggled(bool)), SLOT(pbPickToggled(bool)));
    connect(ui->pbPickLine, SIGNAL(toggled(bool)), SLOT(pbPickToggled(bool)));
    connect(ui->pbPickCircle, SIGNAL(toggled(bool)), SLOT(pbPickToggled(bool)));
    connect(ui->pbPickEllipse, SIGNAL(toggled(bool)), SLOT(pbPickToggled(bool)));

    ui->pbPickPoly->hide();
    ui->pbPickLine->hide();
    ui->pbPickCircle->hide();
    ui->pbPickEllipse->hide();

    // Circle page

    connect(ui->leCircleCenter, SIGNAL(editingFinished()), SLOT(onleCircleCenterEditingFinished()));
    connect(ui->leCircleRadius, SIGNAL(editingFinished()), SLOT(onleCircleRadiusEditingFinished()));


    // Ellipse page
    connect(ui->leEllipseCenter, SIGNAL(editingFinished()), SLOT(onEllipseCenterEditingFinished()));
    connect(ui->leBigAxis, SIGNAL(editingFinished()), SLOT(onEllipseBigAxisEditingFinished()));
    connect(ui->leSmallAxis, SIGNAL(editingFinished()), SLOT(onEllipseSmallAxisEditingFinished()));
    connect(ui->leAngle, SIGNAL(editingFinished()), SLOT(onEllipseAngleEditingFinished()));

    // Route page
    connect(ui->pbAddRouteRest, SIGNAL(clicked()), SLOT(onAddRouteRestriction()));
    connect(ui->pbDeleteRouteRest, SIGNAL(clicked()), SLOT(onDeleteRouteRestriction()));

    connect(ui->lwRouteList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            SLOT(currentRouteItemChanged(QListWidgetItem *, QListWidgetItem *)));

    connect(ui->leRouteFilter, SIGNAL(textChanged(QString)), SLOT(routeFilterChanged(QString)));


    setConnectionsToEnableUpdate();
}

RestrictionWidget::~RestrictionWidget()
{
    delete ui;
}

/*void RestrictionWidget::itemRouteListReset(const Routes &routes)
{
    routes_ = routes;
    routeFilterChanged("");
}

void RestrictionWidget::itemOrpListReset(const Orps &orps)
{
    orps_ = orps;
}*/

void RestrictionWidget::itemListReset(const QList<QSharedPointer<CRestrictionUser> > &restrictions)
{
    if (restriction_) {
        hideGraphics();
        restriction_.clear();
        new_restriction_ = false;
    }
    showControls(false);
    ui->lwRestrictions->clear();

    QSettings &settings = *globalSettings();
    settings.beginGroup(currentUserName);
    QDataStream dataStream(settings.value("Restriction/Favorites").toByteArray());
    settings.endGroup();
    QSet<quint32> favorites;
    dataStream >> favorites;

    foreach (const QSharedPointer<CRestrictionUser> &restriction, restrictions) {
        RestrListWidgetItem *item = new RestrListWidgetItem(restriction);
        if (favorites.contains(restriction->GetID()))
            item->setFavorites(true);

        ui->lwRestrictions->addItem(item);
    }

    QListWidgetItem *separator = new SeparatorItem();
    separator->setSizeHint(QSize(0, 10));
    separator->setFlags(Qt::NoItemFlags);
    separator->setText("   -----   ");

    ui->lwRestrictions->addItem(separator);
}

void RestrictionWidget::itemAdded(const QSharedPointer<CRestrictionUser> &restriction)
{
    if (updateRestrictionInList(restriction))
        return;

    RestrListWidgetItem *list_item = new RestrListWidgetItem(restriction, true);
    ui->lwRestrictions->addItem(list_item);

    if (restriction_) {
        if (restriction->GetID() == restriction_->GetID())
            currentItemChanged(list_item, 0);
    }
}

void RestrictionWidget::itemModified(const QSharedPointer<CRestrictionUser> &restriction)
{
    updateRestrictionInList(restriction);
}

void RestrictionWidget::itemDeleted(const QUuid &uid)
{
    for (int i = 0; i < ui->lwRestrictions->count(); ++i) {
        if (RestrListWidgetItem *lw_item = dynamic_cast<RestrListWidgetItem *>(ui->lwRestrictions->item(i))) {
            QSharedPointer<CRestrictionUser> restriction = lw_item->restrictionPointer();
            if (restriction->GetGUID() == uid) {
                if (restriction_ && (restriction_->GetGUID() == uid)) {
                    hideGraphics();
                    restriction_.clear();
                }
                QListWidgetItem *item_to_delete = ui->lwRestrictions->takeItem(i);
                delete item_to_delete;
                break;
            }
        }
    }

    if (0 == ui->lwRestrictions->count()) {
        showControls(false);
        ui->pbDelete->setEnabled(false);
    }
}

bool RestrictionWidget::updateRestrictionInList(const QSharedPointer<CRestrictionUser> &restriction)
{
    for (int i = 0; i < ui->lwRestrictions->count(); ++i) {
        if (RestrListWidgetItem *list_item = dynamic_cast<RestrListWidgetItem *>(ui->lwRestrictions->item(i))) {
            QSharedPointer<CRestrictionUser> list_restriction = list_item->restrictionPointer();
            if (list_restriction->GetID() == restriction->GetID()) {
                list_item->setRestriction(restriction);
                if (restriction_) {
                    if (restriction->GetID() == restriction_->GetID())
                        currentItemChanged(list_item, 0);
                }
                return true;
            }
        }
    }

    return false;
}

void RestrictionWidget::internalPikerModeOff()
{
    ui->pbPickPoly->setChecked(false);
    ui->pbPickLine->setChecked(false);
    ui->pbPickCircle->setChecked(false);
    ui->pbPickEllipse->setChecked(false);
}

void RestrictionWidget::currentRouteItemChanged(QListWidgetItem *current, QListWidgetItem *prev)
{
    Q_UNUSED(prev);
    /*if (RestrRouteItem *listItem = dynamic_cast<RestrRouteItem *>(current)) {
        Route *item = listItem->route();
        ui->cbRoutePointStart->clear();
        ui->cbRoutePointEnd->clear();
        foreach (Route::point_t point, item->points) {
            ui->cbRoutePointStart->addItem(QString::fromStdString(point.name));
            ui->cbRoutePointEnd->addItem(QString::fromStdString(point.name));
        }
    }*/
}

void RestrictionWidget::routeFilterChanged(const QString &str)
{
    //QRegExp regExp(str);
    ui->lwRouteList->clear();

    /*foreach (Route object, routes_) {
        if (regExp.indexIn(QString::fromStdString(object.name)) != -1) {
            ui->lwRouteList->addItem(new RestrRouteItem(object));
        }
    }*/

}

void RestrictionWidget::createRestriction()
{
    restriction_ = QSharedPointer<CRestrictionUser>(new CRestrictionUser);
    new_restriction_ = true;

    clearAllForm();
    clearDataItemPair(restriction_.data(), active_point_);

    ui->stackedWidget->setCurrentIndex(FILLED_PAGE);
    ui->leRestrictionTitle->clear();
    ui->cbRestrictionFavorites->setChecked(false);
    ui->lwRestrictions->clearSelection();
    ui->cbRestrictionActual->setCheckState(Qt::Unchecked);
    ui->teRestrictionStart->setTime(RestrListWidgetItem::defaultStartTime());
    ui->teRestrictionEnd->setTime(RestrListWidgetItem::defaultEndTime());
    ui->deRestrictionStartDate->setDate(RestrListWidgetItem::defaultStartDate());
    ui->deRestrictionEndDate->setDate(RestrListWidgetItem::defaultEndDate());
    ui->leRestrictionLowerHeight->clear();
    ui->leRestrictionUpperHeight->clear();

    ui->cbRestrictionType->setCurrentIndex(ui->cbRestrictionType->findData(restriction_->GetType()));

    onRestrictionShapeChanged(ui->cbRestrictionShape->currentIndex());

    showControls(true);
    ui->leRestrictionTitle->setFocus();

    ui->cbRestrictionFavorites->hide();

    ui->pbUpdate->setEnabled(true);
    ui->pbDelete->setEnabled(false);
}

void RestrictionWidget:: updateRestriction()
{
    setGeneralParamForRestriction();

    ui->cbRestrictionFavorites->show();
    ui->pbUpdate->setEnabled(false);
    ui->pbDel->setEnabled(false);

    if (new_restriction_) {
        hideGraphics();
        emit createRestriction(restriction_);

        for (int i = 0; i < ui->lwRestrictions->count(); ++i) {
            if (RestrListWidgetItem *list_item = dynamic_cast<RestrListWidgetItem *>(ui->lwRestrictions->item(i))) {
                QSharedPointer<CRestrictionUser> list_restriction = list_item->restrictionPointer();
                if (list_restriction->GetID() == restriction_->GetID()) {
                    currentItemChanged(list_item, 0);
                    return;
                }
            }
        }
        restriction_.clear();
        new_restriction_ = false;
    } else {
        emit updateRestriction(restriction_);
    }
}

void RestrictionWidget::deleteRestriction()
{
    if ( QMessageBox::question(this, tr("Delete restriction"),
                           tr("Delete restriction '%1' permanently?").arg(ui->leRestrictionTitle->text()),
        QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes )
        return;
	emit deleteRestriction(restriction_);
}

void RestrictionWidget::nextLinePageClicked()
{
    int current_index = ui->cbPageLine->currentIndex();

    ui->cbPageLine->setCurrentIndex(++current_index);

    if (24 == current_index)
        ui->pbNextLinePage->setEnabled(false);

    ui->pbPrevLinePage->setEnabled(true);
}

void RestrictionWidget::prevLinePageClicked()
{
    int current_index = ui->cbPageLine->currentIndex();

    ui->cbPageLine->setCurrentIndex(--current_index);

    if (0 == current_index)
        ui->pbPrevLinePage->setEnabled(false);

    ui->pbNextLinePage->setEnabled(true);
}

void RestrictionWidget::firstLinePointClicked(int checked)
{
    ui->cbFirstLineStartType->setEnabled(checked);
    ui->cbFirstLineEndType->setEnabled(checked);

    ui->leFirstLineStartPoint->setEnabled(checked);
    ui->leFirstLineEndPoint->setEnabled(checked);

    ui->teFirstLineStartTime->setEnabled(checked);
    ui->teFirstLineEndTime->setEnabled(checked);

    ui->leFirstLineLowerHeight->setEnabled(checked);
    ui->leFirstLineUpperHeight->setEnabled(checked);

    int index = 1 + 4 * ui->cbPageLine->currentIndex();
    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    if (points.size() <= index) {
        points.resize(index + 1);
        active_point_.resize(index + 1);
    }

    if ((index < restriction_->GetPoints().size()))
        active_point_[index] = checked;

    index = 0 + 4 * ui->cbPageLine->currentIndex();
    if ((index < restriction_->GetPoints().size()))
        active_point_[index] = checked;

    updateGraphics();
    if (checked)
        ui->leFirstLineStartPoint->setFocus();
}

void RestrictionWidget::secondLinePointClicked(int checked)
{
    ui->cbSecondLineStartType->setEnabled(checked);
    ui->cbSecondLineEndType->setEnabled(checked);

    ui->leSecondLineStartPoint->setEnabled(checked);
    ui->leSecondLineEndPoint->setEnabled(checked);

    ui->teSecondLineStartTime->setEnabled(checked);
    ui->teSecondLineEndTime->setEnabled(checked);

    ui->leSecondLineLowerHeight->setEnabled(checked);
    ui->leSecondLineUpperHeight->setEnabled(checked);

    int index = 3 + 4 * ui->cbPageLine->currentIndex();

    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    if (index >= points.size()) {
        points.resize(index + 1);
        active_point_.resize(index + 1);
    }

    if ((index < points.size()))
        active_point_[index] = checked;

    index = 2 + 4 * ui->cbPageLine->currentIndex();
    if ((index < points.size()))
        active_point_[index] = checked;

    updateGraphics();

    if (checked)
        ui->leSecondLineStartPoint->setFocus();
}

void RestrictionWidget::firstLineStartTypeChanged(int current_index)
{
    setTextForLine(current_index, 0, ui->leFirstLineStartPoint);
}

void RestrictionWidget::firstLineEndTypeChanged(int current_index)
{
    setTextForLine(current_index, 1, ui->leFirstLineEndPoint);
}

void RestrictionWidget::secondLineStartTypeChanged(int current_index)
{
    setTextForLine(current_index, 2, ui->leSecondLineStartPoint);
}

void RestrictionWidget::secondLineEndTypeChanged(int current_index)
{
    setTextForLine(current_index, 3, ui->leSecondLineEndPoint);
}

void RestrictionWidget::setTextForLine(int type, int index, QLineEdit *line)
{
    if (0 == type)
        line->setValidator(new QRegExpValidator(regExpName, this));
    else
        line->setValidator(new CoordValidator(CoordEdit::LatitudeLongitude, this));
    line->clear();

    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    if (points.empty())
        return;

    int point_index = ui->cbPageLine->currentIndex() * 4 + index;
    if ((point_index >= points.size()) || (point_index < 0))
        return;

    pointTypeChanged(type, points[point_index]->GetCoord(), line);
}

void RestrictionWidget::addClicked()
{
    CRestrictionUser::TPointList points = restriction_->GetPoints();
    int rowCount = ui->tableWidgetRestrictionPoint->rowCount();

    CRestrictionUser::TPoint point(new CRestrictPoint);
    points.insert((rowCount + 1)*10,point);
    restriction_->SetPoints(points);

    initRestrictionUserPoint(ui->tableWidgetRestrictionPoint->rowCount(), GEOTYPE, point);

    updateGraphics();
    ui->pbUpdate->setEnabled(true);


}

void RestrictionWidget::deleteClicked()
{
    CRestrictionUser::TPointList points = restriction_->GetPoints();
    CRestrictionUser::TPointList points_test;
    int current_row = ui->tableWidgetRestrictionPoint->currentRow();
    int key = current_row * 10 + 10;

    if (points.contains(key)) {
        points.remove(key);
    }

    int row_count = ui->tableWidgetRestrictionPoint->rowCount();
    for (int i = row_count - 1 ; i >= 0; --i) {
        ui->tableWidgetRestrictionPoint->removeRow(i);
    }

    int k = 0;
    for(int i = 0; i < points.size(); ++i){
        int point_test_index = (i) * 10 + 10;
        for(int j = k; j < points.size() + 1; ++j){
            int point_index = (j) * 10 + 10;

            if(point_index != key){
                CRestrictionUser::TPoint point(new CRestrictPoint);
                point = points[point_index];
                point->SetSN(point_test_index);
                points_test.insert(point_test_index, point);
                initRestrictionUserPoint(i, GEOTYPE, points_test[point_test_index]);
                k = j + 1;
                break;
            }
        }
    }

    restriction_->SetPoints(points_test);
    updateGraphics();
    ui->pbUpdate->setEnabled(true);
    ui->pbDel->setEnabled(false);
}

void RestrictionWidget::upButtonClicked()
{
    int current_row = ui->tableWidgetRestrictionPoint->currentRow();
    if(current_row == 0)
        return;

    CRestrictionUser::TPointList points = restriction_->GetPoints();
    CRestrictionUser::TPointList points_test;

    int key = current_row * 10 + 10;
    int row_count = ui->tableWidgetRestrictionPoint->rowCount();
    for (int i = row_count - 1 ; i >= 0; --i) {
        ui->tableWidgetRestrictionPoint->removeRow(i);
    }

    for(int i = 0; i < points.size(); ++i){
        int point_index = (i) * 10 + 10;
        CRestrictionUser::TPoint point(new CRestrictPoint);

        if(points.contains(point_index) && (point_index != key - 10 && point_index != key))
           points_test.insert(point_index, points[point_index]);

        if(points.contains(point_index) && point_index == key - 10){
           point = points[point_index + 10];
           point->SetSN(point_index);
           points_test.insert(point_index, point);
        }
        if(points.contains(point_index) && point_index == key){
           point = points[point_index - 10];
           point->SetSN(point_index);
           points_test.insert(point_index, point);
        }
        initRestrictionUserPoint(i, GEOTYPE, points_test[point_index]);

    }

    restriction_->SetPoints(points_test);
    updateGraphics();
    ui->pbUpdate->setEnabled(true);
    ui->upButton->setDisabled(true);
    ui->downButton->setDisabled(true);
    ui->tableWidgetRestrictionPoint->setCurrentCell(current_row - 1,0);
}

void RestrictionWidget::downButtonClicked()
{
    int current_row = ui->tableWidgetRestrictionPoint->currentRow();
    int row_count = ui->tableWidgetRestrictionPoint->rowCount();
    if(current_row == row_count - 1)
        return;

    CRestrictionUser::TPointList points = restriction_->GetPoints();
    CRestrictionUser::TPointList points_test;

    int key = current_row * 10 + 10;

    for (int i = row_count - 1 ; i >= 0; --i) {
        ui->tableWidgetRestrictionPoint->removeRow(i);
    }

    for(int i = 0; i < points.size(); ++i){
        int point_index = (i) * 10 + 10;
        CRestrictionUser::TPoint point(new CRestrictPoint);

        if(points.contains(point_index) && (point_index != key + 10 && point_index != key))
           points_test.insert(point_index, points[point_index]);

        if(points.contains(point_index) && point_index == key + 10){
           point = points[point_index - 10];
           point->SetSN(point_index);
           points_test.insert(point_index, point);
        }
        if(points.contains(point_index) && point_index == key){
            point = points[point_index + 10];
            point->SetSN(point_index);
            points_test.insert(point_index, point);
        }
        initRestrictionUserPoint(i, GEOTYPE, points_test[point_index]);

    }

    restriction_->SetPoints(points_test);
    updateGraphics();
    ui->pbUpdate->setEnabled(true);
    ui->upButton->setDisabled(true);
    ui->downButton->setDisabled(true);
    ui->tableWidgetRestrictionPoint->setCurrentCell(current_row + 1,0);
}

void RestrictionWidget::showButtonClicked()
{
    ui->frame->show();
    ui->showButton->setDisabled(true);
    ui->hideButton->setDisabled(false);
    ui->pbCreate->setHidden(false);
    ui->pbDelete->setHidden(false);
}

void RestrictionWidget::hideButtonClicked()
{
    ui->frame->hide();
    ui->showButton->setDisabled(false);
    ui->hideButton->setDisabled(true);
    ui->pbCreate->setHidden(true);
    ui->pbDelete->setHidden(true);
}

void RestrictionWidget::pointTypeChanged(int current_index, const CCoord &coord, QLineEdit *line_edit)
{
    line_edit->blockSignals(true);
    if (NAMETYPE == current_index) {
        line_edit->setValidator(new QRegExpValidator(regExpName, this));
        line_edit->setInputMask(QString());
        line_edit->setText(CListAC::getPointNameByCoords(coord));
    } else {
        line_edit->setValidator(new CoordValidator(CoordEdit::LatitudeLongitude, this));
        static const QString mask = "00" + coordcommon::getDegree() + "00'00'' 000" + coordcommon::getDegree() + "00'00'';_";
        line_edit->setInputMask(mask);

        QString coord_str = CoordEdit::toString(CoordEdit::Latitude, coord.GetLatitude().GetAngle()) + " " +
                CoordEdit::toString(CoordEdit::Longitude, coord.GetLongitude().GetAngle());
        line_edit->setText(coord_str.trimmed());
    }
    line_edit->blockSignals(false);
}

void RestrictionWidget::polyPointTypeChanged(int type_index, int point_index, QLineEdit *line_edit)
{
    line_edit->blockSignals(true);
    if (NAMETYPE == type_index) {
        line_edit->setValidator(new QRegExpValidator(regExpName, this));
        line_edit->setInputMask(QString());
    } else {
        line_edit->setValidator(new CoordValidator(CoordEdit::LatitudeLongitude, this));
        line_edit->setInputMask("00" + coordcommon::getDegree() + "00'00'' 000" + coordcommon::getDegree() + "00'00'';_");
    }
    line_edit->blockSignals(false);

    int key = point_index * 10 + 10;
    CRestrictionUser::TPointList &points = restriction_->GetPoints();
    if (points.contains(key)) {
        pointTypeChanged(type_index, points[key]->GetCoord(), line_edit);
    }
}

void RestrictionWidget::lineStartFirstPointTypeChanged(int type_index)
{
    const QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    if (points.empty())
        return;

    const int point_index = 0;
    if (point_index >= points.size())
        return;

    pointTypeChanged(type_index, points[point_index]->GetCoord(), ui->leFirstLineStartPoint);
}

void RestrictionWidget::lineEndFirstPointTypeChanged(int type_index)
{
    const QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    if (points.empty())
        return;

    const int point_index = 1;
    if (point_index >= points.size())
        return;

    pointTypeChanged(type_index, points[point_index]->GetCoord(), ui->leFirstLineEndPoint);
}

void RestrictionWidget::lineStartSecondPointTypeChanged(int type_index)
{
    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    if (points.empty())
        return;

    const int point_index = 2;
    if (point_index >= points.size())
        return;

    pointTypeChanged(type_index, points[point_index]->GetCoord(), ui->leSecondLineStartPoint);
}

void RestrictionWidget::lineEndSecondPointTypeChanged(int type_index)
{
    const QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    if (points.empty())
        return;

    const int point_index = 3;
    if (point_index >= points.size())
        return;

    pointTypeChanged(type_index, points[point_index]->GetCoord(), ui->leSecondLineEndPoint);
}

void RestrictionWidget::circleCenterPointTypeChanged(int current_index)
{
    QList<CRestrictionUser::TPoint> points = restriction_->GetPoints().values();
    if (points.isEmpty())
        pointTypeChanged(current_index, CCoord(), ui->leCircleCenter);
    else
        pointTypeChanged(current_index, points[0]->GetArcCoord(), ui->leCircleCenter);
}

void RestrictionWidget::ellipseCenterPointTypeChanged(int current_index)
{
    //pointTypeChanged(current_index, restr_item_->center, ui->leEllipseCenter);
}

void RestrictionWidget::pointValueEditingFinished(int current_index, CCoord &coord, QLineEdit *line_edit)
{
    if (NAMETYPE == current_index)
        findCoordsByPointName(line_edit->text(), coord);
    else coord = stringToCoord(line_edit->text());
}

void RestrictionWidget::on_leFirstPolyEditEditingFinished()
{
    if (!restriction_)
        return;
    QLineEdit *line_edit = qobject_cast<QLineEdit *>(sender());
    if (!line_edit)
        return;
    int nRow = line_edit->property("row").toInt();
    int nCol = line_edit->property("col").toInt();

    CRestrictionUser::TPointList &points = restriction_->GetPoints();
    const int point_index = (nRow) * 10 + 10;
    CRestrictionUser::TPoint point(new CRestrictPoint);
    QMap<int, CRestrictionUser::TPoint>::iterator it = points.find(point_index);
    point->SetSN(point_index);

    QComboBox *combo_widget = new QComboBox();
    combo_widget = qobject_cast<QComboBox *>(ui->tableWidgetRestrictionPoint->cellWidget(nRow, nCol - 1));
    int index = combo_widget->currentIndex();
    pointValueEditingFinished(index, point->GetCoord(), line_edit);

    if (it != points.end())
        points.erase(it);
    points.insert(point_index, point);
    restriction_->SetPoints(points);

    updateGraphics();
}

void RestrictionWidget::polylineEditEditingFinished(int index, int polyline_point_type_index, QLineEdit *line_edit)
{
    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    while (points.size() <= index) {
        CRestrictionUser::TPoint point_t;
        points.push_back(point_t);
        active_point_.push_back(true);
    }
    /*auto geo_point = points[index].pos;
    pointValueEditingFinished(polyline_point_type_index, geo_point, line_edit);
    points[index].pos = geo_point;*/

    updateGraphics();
}

void RestrictionWidget::onleLineStartPointEditingFinished01()
{
    int index = 0 + 4 * ui->cbPageLine->currentIndex();
    polylineEditEditingFinished(index, ui->cbFirstLineStartType->currentIndex(), ui->leFirstLineStartPoint);
}

void RestrictionWidget::onleLineEndPointEditingFinished01()
{
    int index = 1 + 4 * ui->cbPageLine->currentIndex();
    polylineEditEditingFinished(index, ui->cbFirstLineEndType->currentIndex(), ui->leFirstLineEndPoint);
}

void RestrictionWidget::onleLineStartPointEditingFinished02()
{
    int index = 2 + 4 * ui->cbPageLine->currentIndex();
    polylineEditEditingFinished(index, ui->cbSecondLineStartType->currentIndex(), ui->leSecondLineStartPoint);
}

void RestrictionWidget::onleLineEndPointEditingFinished02()
{
    int index = 3 + 4 * ui->cbPageLine->currentIndex();
    polylineEditEditingFinished(index, ui->cbSecondLineEndType->currentIndex(), ui->leSecondLineEndPoint);
}

void RestrictionWidget::lineSetLowerHeight(int index, QLineEdit *lineEdit)
{
    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    while (points.size() <= index) {
        CRestrictionUser::TPoint point_t;
        points.push_back(point_t);
        active_point_.push_back(true);
    }
    //points[index].height_range.lo = lineEdit->text().toDouble() * 100.;
}

void RestrictionWidget::lineSetUpperHeight(int index, QLineEdit *line_edit)
{
    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    while (points.size() <= index) {
        CRestrictionUser::TPoint point_t;
        points.push_back(point_t);
        active_point_.push_back(true);
    }
    //points[index].height_range.hi = line_edit->text().toDouble() * 100.;
}

void RestrictionWidget::lineSetStartTime(int index, const QTime &time_edit)
{
    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    while (points.size() <= index) {
        CRestrictionUser::TPoint point_t;
        points.push_back(point_t);
        active_point_.push_back(true);
    }
    //points[index].time_range.lo = RestrListWidgetItem::GetDateTime(ui->deRestrictionStartDate->date(), time_edit);
}

void RestrictionWidget::lineSetEndTime(int index, const QTime &time_edit)
{
    QVector<CRestrictionUser::TPoint> points = restriction_->GetPoints().values().toVector();
    while (points.size() <= index) {
        CRestrictionUser::TPoint point_t;
        points.push_back(point_t);
        active_point_.push_back(true);
    }
    //points[index].time_range.hi = RestrListWidgetItem::GetDateTime(ui->deRestrictionStartDate->date(), time_edit);
}

void RestrictionWidget::onleFirstLineLowerHeightEditingFinished()
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetLowerHeight(index, ui->leFirstLineLowerHeight);
    lineSetLowerHeight(index + 1, ui->leFirstLineLowerHeight);
}

void RestrictionWidget::onleFirstLineUpperHeightEditingFinished()
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetUpperHeight(index, ui->leFirstLineUpperHeight);
    lineSetUpperHeight(index + 1, ui->leFirstLineUpperHeight);
}

void RestrictionWidget::onleSecondLineLowerHeightEditingFinished()
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetLowerHeight(index + 2, ui->leSecondLineLowerHeight);
    lineSetLowerHeight(index + 3, ui->leSecondLineLowerHeight);
}

void RestrictionWidget::onleSecondLineUpperHeightEditingFinished()
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetUpperHeight(index + 2, ui->leSecondLineUpperHeight);
    lineSetUpperHeight(index + 3, ui->leSecondLineUpperHeight);
}

void RestrictionWidget::onGlobalStartDateChange(const QDate &date)
{
    if (ui->deRestrictionEndDate->date() < date)
        ui->deRestrictionEndDate->setDate(date);
}

void RestrictionWidget::onGlobalEndDateChange(const QDate &date)
{
    if (ui->deRestrictionStartDate->date() > date)
        ui->deRestrictionStartDate->setDate(date);
}

void RestrictionWidget::onGlobalStartTimeChange(const QTime &time)
{
    if (ui->teRestrictionEnd->time() < time)
        ui->teRestrictionEnd->setTime(time);
}

void RestrictionWidget::onGlobalEndTimeChange(const QTime &time)
{
    if (ui->teRestrictionStart->time() > time)
        ui->teRestrictionStart->setTime(time);
}

void RestrictionWidget::onFirstLineStartTime(const QTime &time)
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetStartTime(index, time);
    lineSetStartTime(index + 1, time);
    if (ui->teFirstLineEndTime->time() < time)
        ui->teFirstLineEndTime->setTime(time);
}

void RestrictionWidget::onFirstLineEndTime(const QTime &time)
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetEndTime(index, time);
    lineSetEndTime(index + 1, time);
    if (ui->teFirstLineStartTime->time() > time)
        ui->teFirstLineStartTime->setTime(time);
}

void RestrictionWidget::onSecondLineStartTime(const QTime &time)
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetStartTime(index + 2, time);
    lineSetStartTime(index + 3, time);
    if (ui->teSecondLineEndTime->time() < time)
        ui->teSecondLineEndTime->setTime(time);
}

void RestrictionWidget::onSecondLineEndTime(const QTime &time)
{
    int index = ui->cbPageLine->currentIndex() * 4;
    lineSetEndTime(index + 2, time);
    lineSetEndTime(index + 3, time);
    if (ui->teSecondLineStartTime->time() > time)
        ui->teSecondLineStartTime->setTime(time);
}

void RestrictionWidget::onleCircleCenterEditingFinished()
{
    CRestrictionUser::TPointList &points = restriction_->GetPoints();
    CRestrictionUser::TPoint point;
    if (!points.empty())
        point = *points.begin();
    else
        point = CRestrictionUser::TPoint(new CRestrictPoint);

    point->SetType(CBoundaryPoint::enCircle);
    point->SetSN(10);

    pointValueEditingFinished(ui->cbCircleCenterPointType->currentIndex(), point->GetArcCoord(), ui->leCircleCenter);

    points.clear();
    points.insert(10, point);
}

void RestrictionWidget::onleCircleRadiusEditingFinished()
{
    if (ui->leCircleRadius->text().isEmpty()) {
        RestrListWidgetItem::SetRadius(restriction_.data(), 0.);
        hideGraphics();
    } else {
        RestrListWidgetItem::SetRadius(restriction_.data(), ui->leCircleRadius->text().toDouble());
        updateGraphics();
    }
}

void RestrictionWidget::onEllipseCenterEditingFinished()
{
    //pointValueEditingFinished(ui->cbEllipceCenter->currentIndex(), restr_item_->center, ui->leEllipseCenter);
}

void RestrictionWidget::onEllipseBigAxisEditingFinished()
{
    if (ui->leBigAxis->text().isEmpty())
        RestrListWidgetItem::SetBigAxis(restriction_.data(), 0.);
    else
        RestrListWidgetItem::SetBigAxis(restriction_.data(), ui->leBigAxis->text().toDouble());

    updateGraphics();
}

void RestrictionWidget::onEllipseSmallAxisEditingFinished()
{
    if (ui->leSmallAxis->text().isEmpty())
        RestrListWidgetItem::SetSmallAxis(restriction_.data(), 0.);
    else
        RestrListWidgetItem::SetSmallAxis(restriction_.data(), ui->leSmallAxis->text().toDouble());

    updateGraphics();
}

void RestrictionWidget::onEllipseAngleEditingFinished()
{
    if (ui->leAngle->text().isEmpty())
        RestrListWidgetItem::SetAngle(restriction_.data(), 0.);
    else
        RestrListWidgetItem::SetAngle(restriction_.data(), ui->leAngle->text().toDouble());

    updateGraphics();
}

void RestrictionWidget::pbPickToggled(bool checked)
{
    pick_mode_ = checked;
    emit pickerMode(checked);
}

void RestrictionWidget::on_leRestrictionTitle_editingFinished()
{
    RestrListWidgetItem::SetNameText(restriction_.data(), ui->leRestrictionTitle->text());
}

void RestrictionWidget::on_cbRestrictionActual_stateChanged(int state)
{
    RestrListWidgetItem::setActual(restriction_.data(), state);
}

void RestrictionWidget::on_cbRestrictionFavorites_stateChanged(int state)
{
    if (new_restriction_)
        return;

    RestrListWidgetItem *item = dynamic_cast<RestrListWidgetItem *>(ui->lwRestrictions->currentItem());
    if (item) {
        item->setFavorites(state);

        // save favorites
        QSet<quint32> favorites;
        for (int i = 0; i < ui->lwRestrictions->count(); ++i) {
            RestrListWidgetItem *list_item = dynamic_cast<RestrListWidgetItem *>(ui->lwRestrictions->item(i));
            if (list_item && list_item->favorites())
                favorites.insert(list_item->restrictionPointer()->GetID());
        }

        QByteArray byteArray;
        QDataStream dataStream(&byteArray, QIODevice::ReadWrite);
        dataStream << favorites;

        QSettings &settings = *globalSettings();
        settings.beginGroup(currentUserName);
        settings.setValue("Restriction/Favorites", byteArray);
        settings.endGroup();
    }
}

void RestrictionWidget::currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (restriction_) {
        hideGraphics();
        restriction_.clear();
    }

    if (RestrListWidgetItem *list_item = dynamic_cast<RestrListWidgetItem *>(current)) {
        restriction_ = list_item->restriction();
        new_restriction_ = false;
        clearAllForm();

        ui->cbRestrictionFavorites->setChecked(list_item->favorites());
        ui->stackedWidget->setCurrentIndex(FILLED_PAGE);
        ui->pbUpdate->setEnabled(false);
        ui->pbDelete->setEnabled(true);

        showControls(true);

        ui->leRestrictionTitle->setText(RestrListWidgetItem::NameText(restriction_.data()));
        ui->cbRestrictionActual->setCheckState(RestrListWidgetItem::actual(restriction_.data()));
        //ui->cbRestrictionFixed->setChecked(rp.is_permanent);
        ui->leRestrictionLowerHeight->setText(RestrListWidgetItem::heightRangeLo(restriction_.data()));
        //ui->leRestrictionLowerHeight->setText(QString("%1").arg(rp.height_range.lo));
        ui->leRestrictionUpperHeight->setText(RestrListWidgetItem::heightRangeHi(restriction_.data()));
        //ui->leRestrictionUpperHeight->setText(QString("%1").arg(rp.height_range.hi));
        ui->deRestrictionStartDate->blockSignals(true);
        ui->deRestrictionStartDate->setDate(RestrListWidgetItem::StartTime(restriction_.data()).date());
        ui->deRestrictionStartDate->blockSignals(false);
        ui->deRestrictionEndDate->blockSignals(true);
        ui->deRestrictionEndDate->setDate(RestrListWidgetItem::EndTime(restriction_.data()).date());
        ui->deRestrictionEndDate->blockSignals(false);
        //ui->deRestrictionDate->setDate(lo.date());
        ui->teRestrictionStart->blockSignals(true);
        ui->teRestrictionStart->setTime(RestrListWidgetItem::StartTime(restriction_.data()).time());
        ui->teRestrictionStart->blockSignals(false);
        //ui->teRestrictionStart->setTime(lo.time());
        ui->teRestrictionEnd->blockSignals(true);
        ui->teRestrictionEnd->setTime(RestrListWidgetItem::EndTime(restriction_.data()).time());
        ui->teRestrictionEnd->blockSignals(false);
        //ui->teRestrictionEnd->setTime(hi.time());
        ui->cbRestrictionType->setCurrentIndex(ui->cbRestrictionType->findData(restriction_->GetType()));
        ui->cbRestrictionShape->setCurrentIndex(RestrListWidgetItem::TypeRestrictionID(restriction_.data()));

        onRestrictionShapeChanged(RestrListWidgetItem::TypeRestrictionID(restriction_.data()));

        switch (RestrListWidgetItem::TypeRestrictionID(restriction_.data())) {
        case POLYGON:
        {
            for (int i = 0; i < ui->tableWidgetRestrictionPoint->rowCount(); ++i)
                    ui->tableWidgetRestrictionPoint->removeRow(i);
            ui->tableWidgetRestrictionPoint->setRowCount(0);

            const CRestrictionUser::TPointList &points = restriction_->GetPoints();
            active_point_.fill(true, points.size());
            active_point_.insert(active_point_.size(), 100 - points.size(), false);

            int i = 0;
            while (i < points.size()) {
                int point_index = (i) * 10 + 10;
                CRestrictionUser::TPoint point;
                QMap<int, CRestrictionUser::TPoint>::const_iterator it = points.find(point_index);
                if (it != points.end())
                    point = *it;

                initRestrictionUserPoint(i, GEOTYPE, point);
                ++i;
            }
        }

            break;

        case CIRCLE:
            ui->leCircleRadius->setText(QString("%1").arg(RestrListWidgetItem::Radius(restriction_.data())));
            circleCenterPointTypeChanged(ui->cbCircleCenterPointType->currentIndex());
            break;

        default:
            showControls(false);
            ui->pbDelete->setEnabled(false);
            break;
        }
        updateGraphics();
    }
    ui->upButton->setDisabled(true);
    ui->downButton->setDisabled(true);
}

void RestrictionWidget::onMouseMove(const Qt::MouseButton &button, const PandaPoint &point, const QSet<PandaItemBase *> &items_list)
{
    Q_UNUSED(button);
    Q_UNUSED(items_list);

    if (!mouse_press_)
        return;

    //int current_type = RestrListWidgetItem::TypeRestrictionID(restriction_.data());

    /*switch (current_type) {
    case Restriction::polygon:
    case Restriction::polyline:
    case Restriction::route_section:

        break;
    case Restriction::circle: {
        auto distanceA = AbstractProjection::distance(point, PandaPoint(restr_item_->center.lat, restr_item_->center.lon));
        if (distanceA > 99999)
            distanceA = 99999;

        restr_item_->small_axis = distanceA;
        restr_item_->big_axis = distanceA;
        ui->leCircleRadius->setText(QString("%1").arg(distanceA));
    }
    break;
    case Restriction::ellipse: {
        auto point1 = point;
        point1.setLongitude(restr_item_->center.lon);
        auto distanceA = AbstractProjection::distance(point1, PandaPoint(restr_item_->center.lat, restr_item_->center.lon));
        if (distanceA > 99999)
            distanceA = 99999;
        point1 = point;
        point1.setLatitude(restr_item_->center.lat);
        auto distanceB = AbstractProjection::distance(point1, PandaPoint(restr_item_->center.lat, restr_item_->center.lon));
        if (distanceB > 99999)
            distanceB = 99999;
        restr_item_->small_axis = distanceB;
        restr_item_->big_axis = distanceA;
        ui->leSmallAxis->setText(QString("%1").arg(distanceB));
        ui->leBigAxis->setText(QString("%1").arg(distanceA));
    }
    break;
    }*/
    updateGraphics();
}

void RestrictionWidget::onMousePress(const Qt::MouseButton &button, const PandaPoint &point, const QSet<PandaItemBase *> &items_list)
{
    Q_UNUSED(items_list);

    if (!pick_mode_)
        return;

    if (!restriction_)
        return;

    if (!(Qt::LeftButton & button))
        return;

    mouse_press_ = true;

    //int current_type = RestrListWidgetItem::TypeRestrictionID(restriction_.data());

    /*switch (current_type) {
    case Restriction::polygon: {
        if (100 == restr_item_->items.size())
            break;

        Restriction::item point_t;
        point_t.pos.lat = point.latitude();
        point_t.pos.lon = point.longitude();
        restr_item_->items.push_back(point_t);
        active_point_.push_back(true);

        int index = static_cast<int>(restr_item_->items.size());
        ui->cbPagePoly->setCurrentIndex((index - 1) / 10);

        if (index > 10)
            index -= 10 * ui->cbPagePoly->currentIndex();

        initPolyPointControls(index, point_t);
    }
    break;

    case Restriction::polyline: {
        if (100 == restr_item_->items.size())
            break;

        if (current_line_index_ >= restr_item_->items.size()) {
            restr_item_->items.resize(current_line_index_ + 1);
            active_point_.resize(current_line_index_ + 1);
        }

        Restriction::item point_t;
        point_t.pos.lat = point.latitude();
        point_t.pos.lon = point.longitude();
        point_t.height_range.lo = ui->leRestrictionLowerHeight->text().toDouble();
        point_t.height_range.hi = ui->leRestrictionUpperHeight->text().toDouble();
        point_t.time_range.lo = RestrListWidgetItem::GetDateTime(ui->deRestrictionStartDate->date(), ui->teRestrictionStart->time());
        point_t.time_range.hi = RestrListWidgetItem::GetDateTime(ui->deRestrictionEndDate->date(), ui->teRestrictionEnd->time());

        restr_item_->items[current_line_index_] = point_t;
        active_point_[current_line_index_] = true;

        int index = static_cast<int>(restr_item_->items.size());
        ui->cbPageLine->setCurrentIndex(current_line_index_ / 4);
        index = current_line_index_ - 4 * ui->cbPageLine->currentIndex();

        initLinePointControls(index, point_t, true);
        ++current_line_index_;
    }
    break;

    case Restriction::circle:
        ui->leCircleRadius->clear();
        restr_item_->center.lat = point.latitude();
        restr_item_->center.lon = point.longitude();
        ui->cbCircleCenterPointType->setCurrentIndex(GEOTYPE);
        ui->leCircleCenter->setText(PandaPoint::coordToString(point.latitude(), point.longitude()));
        break;
    case Restriction::ellipse:
        ui->leEllipseCenter->clear();
        restr_item_->center.lat = point.latitude();
        restr_item_->center.lon = point.longitude();
        ui->cbEllipceCenter->setCurrentIndex(GEOTYPE);
        ui->leEllipseCenter->setText(PandaPoint::coordToString(point.latitude(), point.longitude()));

        break;
    case Restriction::route_section:
        break;
    }*/
    updateGraphics();
}

void RestrictionWidget::onMouseRelease(const Qt::MouseButton &button, const PandaPoint &point, const QSet<PandaItemBase *> &items_list)
{
    Q_UNUSED(items_list);

    if (!restriction_)
        return;

    if (!(Qt::LeftButton & button))
        return;

    mouse_press_ = false;

    //int current_type = RestrListWidgetItem::TypeRestrictionID(restriction_.data());

    /*switch (current_type) {
    case Restriction::polygon:
    case Restriction::polyline:
        break;
    case Restriction::circle: {
        auto distanceA = AbstractProjection::distance(point, PandaPoint(restr_item_->center.lat, restr_item_->center.lon));

        if (distanceA > 99999)
            distanceA = 99999;
        restr_item_->small_axis = distanceA;
        restr_item_->big_axis = distanceA;
        ui->leCircleRadius->setText(QString("%1").arg(distanceA));
    }
    break;
    case Restriction::ellipse: {
        auto point1 = point;
        point1.setLongitude(restr_item_->center.lon);
        auto distanceA = AbstractProjection::distance(point1, PandaPoint(restr_item_->center.lat, restr_item_->center.lon));
        if (distanceA > 99999)
            distanceA = 99999;
        point1 = point;
        point1.setLatitude(restr_item_->center.lat);
        auto distanceB = AbstractProjection::distance(point1, PandaPoint(restr_item_->center.lat, restr_item_->center.lon));
        if (distanceB > 99999)
            distanceB = 99999;
        restr_item_->small_axis = distanceB;
        restr_item_->big_axis = distanceA;
        ui->leSmallAxis->setText(QString("%1").arg(distanceB));
        ui->leBigAxis->setText(QString("%1").arg(distanceA));
    }
    break;
    case Restriction::route_section:
        break;
    }*/
    updateGraphics();

}

void RestrictionWidget::onDbConnectionChanged(bool connected)
{
    ui->pbCreate->setEnabled(connected);
    if(!connected)
    {
        ui->cbRestrictionFavorites->setVisible(connected);
        ui->cbRestrictionActual->setVisible(connected);
        ui->cbRestrictionType->setVisible(connected);
        ui->cbRestrictionShape->setVisible(connected);
        ui->leRestrictionTitle->setVisible(connected);
        //ui->labelTitle->setVisible(connected);
        ui->stackedWidget->setCurrentIndex(EMPTY_PAGE);
        ui->pbDelete->setEnabled(connected);
    }
}

void RestrictionWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if (pick_mode_)
        emit pickerMode(false);
    hideGraphics();
}

void RestrictionWidget::onRestrictionTypeChanged(int current_index)
{
    restriction_->SetType((CRestrictionUser::Type)ui->cbRestrictionType->itemData(current_index).toInt());
}

void RestrictionWidget::onRestrictionShapeChanged(int current_index)
{
    switch (ui->cbRestrictionShape->itemData(current_index).toInt()) {
    case CBoundaryPoint::enLine:
    ui->stackedWidgetType->setCurrentIndex(POLYGON_PAGE);
    break;
    case CBoundaryPoint::enCircle:
    ui->stackedWidgetType->setCurrentIndex(CIRCLE_PAGE);
    break;
    default:
    break;
    }

    if (!restriction_)
        return;

    int current_restriction_type = RestrListWidgetItem::TypeRestrictionID(restriction_.data());
    if (current_index == current_restriction_type)
        return;

    if (new_restriction_)
        restriction_ = QSharedPointer<CRestrictionUser>(new CRestrictionUser);

    clearAllForm();

    clearDataItemPair(restriction_.data(), active_point_);

    updateGraphics();
}

void RestrictionWidget::onAddRouteRestriction()
{
    if (!restriction_)
        return;

    QList<QListWidgetItem *> list_selected = ui->lwRouteList->selectedItems();
    if (list_selected.empty())
        return;

    /*RestrRouteItem *list_item = dynamic_cast<RestrRouteItem *>(list_selected.first());
    Restriction::item item;
    item.route_name = list_item->route()->name;

    foreach (const Route & arg, routes_.values()) {
        if (arg.name == item.route_name) {
            auto &route = arg;

            auto first_point(ui->cbRoutePointStart->currentText().toStdString());
            auto second_point(ui->cbRoutePointEnd->currentText().toStdString());
            auto itStart = std::find_if(route.points.begin(), route.points.end(), [first_point](const concept_object::route::point_t & p) {return p.name == first_point; });
            auto itEnd = std::find_if(route.points.begin(), route.points.end(), [second_point](const concept_object::route::point_t & p) {return p.name == second_point; });

            if (route.points.end() == itStart)
                continue;
            if (route.points.end() == itEnd)
                continue;
            if (itStart > itEnd) {
                auto itTemp = itStart;
                itStart = itEnd;
                itEnd = itTemp;
            }

            for (auto i = itStart; i != itEnd; ++i) {
                cg::geo_point_2 firstPointCoord;
                cg::geo_point_2 secondPointCoord;

                item.segment = std::pair<std::string, std::string>(i->name, (i + 1)->name);

                findCoordsByPointNameGeo(QString::fromStdString(i->name), firstPointCoord);
                findCoordsByPointNameGeo(QString::fromStdString((i + 1)->name), secondPointCoord);

                item.coord = std::pair<cg::geo_point_2, cg::geo_point_2>(firstPointCoord, secondPointCoord);
                restr_item_->items.push_back(item);

            }
            break;
        }
    }*/
    updateRouteRestrictionList();
    updateGraphics();
}

void RestrictionWidget::onDeleteRouteRestriction()
{
    if (!restriction_)
        return;

    QList<QListWidgetItem*> list_selected = ui->lwSelectedRoute->selectedItems();
    if (list_selected.empty())
        return;
    /*RestRouteCurrentItem *listItem = dynamic_cast<RestRouteCurrentItem *>(list_selected.first());

    restr_item_->GetPoints().erase(restr_item_->GetPoints().begin() + listItem->index());*/
    updateRouteRestrictionList();
    updateGraphics();
}

void RestrictionWidget::updateRouteRestrictionList()
{
    ui->lwSelectedRoute->clear();
    if (!restriction_)
        return;

    /*int i = 0;
    std::for_each(restr_item_->GetPoints().begin(), restr_item_->GetPoints().end(), [this, &i](CRestrictionUser::TPoint & item) {
        QString first_name = findPointNameByCoords(item.coord.first.lat, restr_item_->items[i].coord.first.lon);
        QString second_name = findPointNameByCoords(item.coord.second.lat, restr_item_->items[i].coord.second.lon);

        item.segment = std::pair<std::string, std::string>(first_name.toStdString(), second_name.toStdString());
        ui->lwSelectedRoute->addItem(new RestRouteCurrentItem(&item, i++));
    });*/
}

void RestrictionWidget::updateLanguage()
{
    //ui->labelTitle->setText(tr("Title:"));
    ui->gbParameters->setTitle(tr("Parameters"));
    ui->gbLayout->setTitle(tr("Layout"));
    ui->cbRestrictionActual->setText(tr("Actual"));


    ui->lStartEndTime->setText(tr("Start time - end time"));
    ui->lHhMmHhMm->setText(tr("(hh:mm - hh:mm):"));
    ui->lLowerHeight->setText(tr("Lower height -"));
    ui->lUppeHeight->setText(tr("upper height"));
    ui->lHundredsofmeters->setText(tr("in feet"));
    ui->lNnn->setText(tr("(nnn - nnn):"));

    ui->pbCreate->setText(tr("Create"));
    ui->pbDelete->setText(tr("Delete"));
    ui->pbUpdate->setText(tr("Transmit to work places"));

    // Poly
    ui->pbPickPoly->setText(tr("Pick"));

    // Ellipse
    ui->pbPickEllipse->setText(tr("Pick"));
    ui->lCenter2->setText(tr("Center:"));
    ui->lAxisX->setText(tr("Axisx:"));
    ui->lAxisY->setText(tr("Axisy:"));
    ui->lAngle->setText(tr("Angle:"));

    // Line
    ui->pbPickLine->setText(tr("Pick"));
    ui->lStartTime1->setText(tr("Start time -"));
    ui->lStartTime2->setText(tr("Start time -"));
    ui->lEndTime1->setText(tr("end time"));
    ui->lEndTime2->setText(tr("end time"));

    ui->lLowerHeight1->setText(tr("Lower height -"));
    ui->lLowerHeight2->setText(tr("Lower height -"));
    ui->lUppeHeight1->setText(tr("upper height"));
    ui->lUppeHeight2->setText(tr("upper height"));

    // Circle
    ui->pbPickCircle->setText(tr("Pick"));
    ui->lCenter1->setText(tr("Center:"));
    ui->lRadiusinmeters->setText(tr("Radius in meters:"));

    // Route
    ui->pbAddRouteRest->setText(tr("Add"));
    ui->pbDeleteRouteRest->setText(tr("Delete"));
    ui->lRoute->setText(tr("Route"));
    ui->lPoints->setText(tr("Points"));
    ui->lSelectedroute->setText(tr("Selected route:"));
}

void RestrictionWidget::showControls(bool visible)
{
    ui->cbRestrictionFavorites->setVisible(visible);
    ui->cbRestrictionActual->setVisible(visible);
    ui->cbRestrictionType->setVisible(visible);
    ui->cbRestrictionShape->setVisible(visible);
    ui->leRestrictionTitle->setVisible(visible);
    //ui->labelTitle->setVisible(visible);

    if (!visible)
        ui->stackedWidget->setCurrentIndex(EMPTY_PAGE);
}

void RestrictionWidget::polyPointClicked(int checked, int index, QComboBox *combo_box, QLineEdit *line_edit)
{
    combo_box->setEnabled(checked);

    line_edit->setEnabled(checked);

    active_point_[index] = checked;

    updateGraphics();

    //if (checked)
    //    line_edit->setFocus();
}

void RestrictionWidget::initRestrictionUserPoint(int i, int type, const CRestrictionUser::TPoint &point)
{
    ui->tableWidgetRestrictionPoint->insertRow(i);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(QString::number(i+1));
    ui->tableWidgetRestrictionPoint->setItem(i,0,item);
    QComboBox* combo = new QComboBox();
    combo->addItem("Name");
    combo->addItem("Geo");

    combo->setCurrentIndex(type);
    combo->setProperty("row", i);
    combo->setProperty("col", 1);
    connect(combo, SIGNAL(currentIndexChanged(int)),this, SLOT(changed(int)));
    ui->tableWidgetRestrictionPoint->setCellWidget(i,1,combo);

    RestrictionLineEdit *leFirstPolyEdit = new RestrictionLineEdit();
    leFirstPolyEdit->setCompleter(completer_);
    connect(leFirstPolyEdit, SIGNAL(textEdited(const QString &)), SLOT(toUpper(const QString &)));
    connect(leFirstPolyEdit, SIGNAL(editingFinished()), SLOT(on_leFirstPolyEditEditingFinished()));
    connect(leFirstPolyEdit, SIGNAL(textEdited(const QString &)), SLOT(enableUpdate()));

    if(point)
        pointTypeChanged(combo->currentIndex(), point.data()->GetCoord(), leFirstPolyEdit);
    else{
        CCoord coord(0,0);
        pointTypeChanged(combo->currentIndex(), coord, leFirstPolyEdit);
    }

    leFirstPolyEdit->setProperty("row", i);
    leFirstPolyEdit->setProperty("col", 2);
    ui->tableWidgetRestrictionPoint->setCellWidget(i,2,leFirstPolyEdit);
}
void RestrictionWidget::initLinePointControls(int i, CRestrictionUser::TPoint &point, bool is_active)
{
    disconnect(ui->leFirstLineStartPoint, SIGNAL(editingFinished()), this, SLOT(onleLineStartPointEditingFinished01()));
    disconnect(ui->leFirstLineEndPoint, SIGNAL(editingFinished()), this, SLOT(onleLineEndP ointEditingFinished01()));
    disconnect(ui->leSecondLineStartPoint, SIGNAL(editingFinished()), this, SLOT(onleLineStartPointEditingFinished02()));
    disconnect(ui->leSecondLineEndPoint, SIGNAL(editingFinished()), this, SLOT(onleLineEndPointEditingFinished02()));

    /*switch (i) {
    case 0:
        if ((0 == point.pos.lat) || (0 == point.pos.lon)) {
            ui->cbFirstLinePoint->setChecked(false);
            ui->cbFirstLineStartType->setCurrentIndex(NAMETYPE);
            ui->leFirstLineStartPoint->clear();
            break;
        }

        ui->cbFirstLinePoint->setChecked(is_active);
        ui->cbFirstLineStartType->setCurrentIndex(GEOTYPE);
        ui->leFirstLineStartPoint->setText(PandaPoint::coordToString(point.pos.lat, point.pos.lon));

        ui->leFirstLineLowerHeight->setText(QString("%1").arg(point.height_range.lo));
        ui->leFirstLineUpperHeight->setText(QString("%1").arg(point.height_range.hi));

        ui->teFirstLineStartTime->setTime(RestrListWidgetItem::GetTime(point.time_range.lo));
        ui->teFirstLineEndTime->setTime(RestrListWidgetItem::GetTime(point.time_range.hi));

        break;
    case 1:

        if ((0 == point.pos.lat) || (0 == point.pos.lon)) {
            ui->cbFirstLineEndType->setCurrentIndex(NAMETYPE);
            ui->leFirstLineEndPoint->clear();
            break;
        }

        ui->cbFirstLinePoint->setChecked(is_active);
        ui->cbFirstLineEndType->setCurrentIndex(GEOTYPE);
        ui->leFirstLineEndPoint->setText(PandaPoint::coordToString(point.pos.lat, point.pos.lon));

        ui->leFirstLineLowerHeight->setText(QString("%1").arg(point.height_range.lo));
        ui->leFirstLineUpperHeight->setText(QString("%1").arg(point.height_range.hi));

        ui->teFirstLineStartTime->setTime(RestrListWidgetItem::GetTime(point.time_range.lo));
        ui->teFirstLineEndTime->setTime(RestrListWidgetItem::GetTime(point.time_range.hi));

        break;

    case 2:
        //if (!point || point->isNull())
        if ((0 == point.pos.lat) || (0 == point.pos.lon)) {
            ui->cbSecondLinePoint->setChecked(false);
            ui->cbSecondLineStartType->setCurrentIndex(NAMETYPE);
            ui->leSecondLineStartPoint->clear();
            break;
        }

        ui->cbSecondLinePoint->setChecked(is_active);
        ui->cbSecondLineStartType->setCurrentIndex(GEOTYPE);
        ui->leSecondLineStartPoint->setText(PandaPoint::coordToString(point.pos.lat, point.pos.lon));

        ui->leSecondLineLowerHeight->setText(QString("%1").arg(point.height_range.lo));
        ui->leSecondLineUpperHeight->setText(QString("%1").arg(point.height_range.hi));

        ui->teSecondLineStartTime->setTime(RestrListWidgetItem::GetTime(point.time_range.lo));
        ui->teSecondLineEndTime->setTime(RestrListWidgetItem::GetTime(point.time_range.hi));
        break;

    case 3:
        if ((0 == point.pos.lat) || (0 == point.pos.lon)) {
            ui->cbSecondLineEndType->setCurrentIndex(NAMETYPE);
            ui->leSecondLineEndPoint->clear();
            break;
        }
        ui->cbSecondLinePoint->setChecked(is_active);
        ui->cbSecondLineEndType->setCurrentIndex(GEOTYPE);
        ui->leSecondLineEndPoint->setText(PandaPoint::coordToString(point.pos.lat, point.pos.lon));

        ui->leSecondLineLowerHeight->setText(QString("%1").arg(point.height_range.lo));
        ui->leSecondLineUpperHeight->setText(QString("%1").arg(point.height_range.hi));

        ui->teSecondLineStartTime->setTime(RestrListWidgetItem::GetTime(point.time_range.lo));
        ui->teSecondLineEndTime->setTime(RestrListWidgetItem::GetTime(point.time_range.hi));
        break;
    }*/
    connect(ui->leFirstLineStartPoint, SIGNAL(editingFinished()), this, SLOT(onleLineStartPointEditingFinished01()));
    connect(ui->leFirstLineEndPoint, SIGNAL(editingFinished()), this, SLOT(onleLineEndPointEditingFinished01()));
    connect(ui->leSecondLineStartPoint, SIGNAL(editingFinished()), this, SLOT(onleLineStartPointEditingFinished02()));
    connect(ui->leSecondLineEndPoint, SIGNAL(editingFinished()), this, SLOT(onleLineEndPointEditingFinished02()));
}

void RestrictionWidget::findCoordsByPointName(const QString &name, CCoord &coord)
{
    coord = CListAC::getCoordsByPointName(name);
}

void RestrictionWidget::updateGraphics()
{
    CRestrictionUser *restriction = new CRestrictionUser(*restriction_);
    emit hideGraphicsObject(*restriction);

    /*switch (restriction->type) {
    case Restriction::polygon: {
        QVector<CRestrictionUser::TPoint> items;
        for (int i = 0; i < active_point_.size(); ++i) {
            if (active_point_[i])
                items.push_back(restriction->items[i]);
        }
        restriction->items = items;
    }
    break;
    case Restriction::polyline: {
        QVector<CRestrictionUser::TPoint> items;
        for (int i = 0; i < active_point_.size(); ++i) {
            if (active_point_[i])
                items.push_back(restriction->items[i]);
        }
        restriction->items.clear();
        auto count = items.size() - items.size() % 2;
        for (int i = 0; i < count; i += 2) {
            if ((0.0 == item[i].pos.lat) || (0.0 == item[i].pos.lon) ||
                    (0.0 == item[i + 1].pos.lat) || (0.0 == item[i + 1].pos.lon))
                continue;
            restriction->items.push_back(item[i]);
            restriction->items.push_back(item[i + 1]);
        }
    }
    break;
    case Restriction::circle:
    case Restriction::ellipse:
    case Restriction::route_section:

        break;
    }*/
    emit updateGraphicsObject(*restriction);
    delete restriction;
}

void RestrictionWidget::hideGraphics()
{
    if (restriction_) {
        CRestrictionUser *restriction = new CRestrictionUser(*restriction_);
        emit hideGraphicsObject(*restriction);
        delete restriction;
    }
}

void RestrictionWidget::clearAllForm()
{
    clearLineRestrForm();
    clearCircleRestrForm();
    clearEllipseRestrForm();
    clearRouteSectionRestrForm();
    internalPikerModeOff();
}

void RestrictionWidget::clearLineRestrForm()
{
    current_line_index_ = 0;
    ui->cbPageLine->setCurrentIndex(0);
    for (int i = 0; i < 4; ++i) {
        CRestrictionUser::TPoint point;
        initLinePointControls(i, point, false);
    }
}

void RestrictionWidget::clearCircleRestrForm()
{
    //ui->cbCircleCenterPointType->setCurrentIndex(NAMETYPE);
    ui->cbCircleCenterPointType->setCurrentIndex(GEOTYPE);
    ui->leCircleCenter->clear();
    ui->leCircleRadius->clear();
}

void RestrictionWidget::clearEllipseRestrForm()
{
    ui->leBigAxis->clear();
    ui->leSmallAxis->clear();
    ui->leAngle->clear();
}

void RestrictionWidget::clearRouteSectionRestrForm()
{
    ui->leRouteFilter->clear();
    ui->lwSelectedRoute->clear();
    ui->cbRoutePointStart->clear();
    ui->cbRoutePointEnd->clear();
    routeFilterChanged("");
}

void RestrictionWidget::clearDataItemPair(CRestrictionUser *restriction, QVector<bool> &active)
{
    restriction->SetPoints(CRestrictionUser::TPointList());
    active.fill(false, 100);
}

void RestrictionWidget::setGeneralParamForRestriction()
{
    RestrListWidgetItem::SetNameText(restriction_.data(), ui->leRestrictionTitle->text());
    RestrListWidgetItem::setHeightRangeLo(restriction_.data(), ui->leRestrictionLowerHeight->text().toDouble());
    RestrListWidgetItem::setHeightRangeHi(restriction_.data(), ui->leRestrictionUpperHeight->text().toDouble());
    RestrListWidgetItem::setActual(restriction_.data(), ui->cbRestrictionActual->checkState());
    QDateTime temp;
    temp.setDate(ui->deRestrictionStartDate->date());
    temp.setTime(ui->teRestrictionStart->time());
    RestrListWidgetItem::SetStartTime(restriction_.data(), temp);
    temp.setDate(ui->deRestrictionEndDate->date());
    temp.setTime(ui->teRestrictionEnd->time());
    RestrListWidgetItem::SetEndTime(restriction_.data(), temp);

    switch (ui->cbRestrictionShape->currentIndex()) {
    case POLYGON: {

    }
    break;

    case CIRCLE: {
        onleCircleRadiusEditingFinished();
        onleCircleCenterEditingFinished();
    }
    break;

    default:
        break;
    }
}

CCoord RestrictionWidget::stringToCoord(const QString &str)
{
    const QString lon_str = str.right(11).trimmed();
    const QString lat_str = str.left(11).trimmed();
    return CCoord(CoordEdit::fromString(CoordEdit::Longitude, lon_str).toDouble(), CoordEdit::fromString(CoordEdit::Latitude, lat_str).toDouble());
}

void RestrictionWidget::setCompleter(const QStringList &completions)
{
    completer_->setModel(new QStringListModel(completions, this));
}

void RestrictionWidget::toUpper(const QString &text)
{
    QLineEdit *line_edit = qobject_cast<QLineEdit *>(sender());
    if (!line_edit)
        return;
    int pos = line_edit->cursorPosition();
    line_edit->setText(text.toUpper());
    line_edit->setCursorPosition(pos);
}

void RestrictionWidget::setConnectionsToEnableUpdate()
{
    connect(ui->leRestrictionTitle, SIGNAL(textEdited(const QString &)), SLOT(enableUpdate()));

    connect(ui->deRestrictionStartDate, SIGNAL(dateChanged(const QDate &)), SLOT(enableUpdate()));
    connect(ui->deRestrictionEndDate, SIGNAL(dateChanged(const QDate &)), SLOT(enableUpdate()));

    connect(ui->teRestrictionStart, SIGNAL(timeChanged(const QTime &)), SLOT(enableUpdate()));
    connect(ui->teRestrictionEnd, SIGNAL(timeChanged(const QTime &)), SLOT(enableUpdate()));

    connect(ui->leRestrictionLowerHeight, SIGNAL(textEdited(const QString &)), SLOT(enableUpdate()));
    connect(ui->leRestrictionUpperHeight, SIGNAL(textEdited(const QString &)), SLOT(enableUpdate()));

    connect(ui->leCircleCenter, SIGNAL(textEdited(const QString &)), SLOT(enableUpdate()));
    connect(ui->leCircleRadius, SIGNAL(textEdited(const QString &)), SLOT(enableUpdate()));

    connect(ui->cbRestrictionType, SIGNAL(currentIndexChanged(int)), SLOT(enableUpdate()));
    connect(ui->cbRestrictionActual, SIGNAL(clicked(bool)), SLOT(enableUpdate()));
}

void RestrictionWidget::enableUpdate()
{
    ui->pbUpdate->setEnabled(true);
}

void RestrictionWidget::changed(int i)
{
    QComboBox *combo_widget = qobject_cast<QComboBox *>(sender());
    if (!combo_widget)
        return;
    int nRow = combo_widget->property("row").toInt();
    int nCol = combo_widget->property("col").toInt();

    QLineEdit *line_widget = new QLineEdit();
    line_widget = qobject_cast<QLineEdit *>(ui->tableWidgetRestrictionPoint->cellWidget(nRow, nCol+1));
    if (!line_widget)
        return;
    CRestrictionUser::TPointList &points = restriction_->GetPoints();
    const int point_index = (nRow) * 10 + 10;
    CRestrictionUser::TPoint point(new CRestrictPoint);
    QMap<int, CRestrictionUser::TPoint>::iterator it = points.find(point_index);
    if (it != points.end())
        point = *it;

    pointTypeChanged(i, point->GetCoord(), line_widget);
}

void RestrictionWidget::OnChangeSelect(const QItemSelection &itsnew, const QItemSelection &its)
{
    QModelIndexList selection = itsnew.indexes();
    if(selection.isEmpty())
        return;
    QModelIndex index= selection.at(0);
    if(index.column() == 0){
        ui->upButton->setDisabled(false);
        ui->downButton->setDisabled(false);
        ui->pbDel->setEnabled(true);
    }
    else{
        ui->upButton->setDisabled(true);
        ui->downButton->setDisabled(true);
        ui->pbDel->setEnabled(false);
    }
}
