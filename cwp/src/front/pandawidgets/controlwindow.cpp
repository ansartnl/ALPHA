#include "controlwindow.h"
#include "ui_controlwindow.h"
#include "main.h"
#include "back/utils/ProfileSettings.h"

#include <QDateTime>
#include <QFocusEvent>
#include <QMenu>
#include <QSlider>
#include <QSqlTableModel>
#include <QStackedLayout>
#include <QtSql/QSqlRecord>

#include <qxtspansliderrestricted.h>

ControlWindow::ControlWindow(QWidget *parent) :
    BaseToolDialog(false, false, parent),
    ui(new Ui::ControlWindow),
    height_slider_(new QxtSpanSliderRestricted(Qt::Horizontal, this)),
    speed_mapper_(new QSignalMapper(this)),
    distance_mapper_(new QSignalMapper(this)),
    angle_mapper_(new QSignalMapper(this)),
    angle_(135)
{
    qRegisterMetaType<HeightMeasure>("HeightMeasure");

    ui->setupUi(this);

    qobject_cast < QStackedLayout * >(ui->stack->layout())->setStackingMode(QStackedLayout::StackAll);

    updateLanguage();

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    connect(ui->pbAutoAntioverlap, SIGNAL(clicked(bool)), SIGNAL(autoAntiOverlapClicked(bool)));

    //1st section
    connect(ui->cbSector, SIGNAL(currentIndexChanged(int)), SIGNAL(sectorComboIndexChanged(int)));
    connect(ui->bQuit, SIGNAL(clicked()), SIGNAL(logOffButtonClicked()));
    connect(ui->pbRB, SIGNAL(clicked(bool)), SIGNAL(vectorMeasurementButtonClicked(bool)));
    connect(ui->pbShort, SIGNAL(clicked(bool)), SLOT(_OnShortCW(bool)));

    //2nd section
    connect(ui->bMaps, SIGNAL(clicked()), SIGNAL(layersTriggered()));
    connect(ui->bRestrAreas, SIGNAL(clicked(bool)), SIGNAL(restrictionsButtonClicked(bool)));
    connect(ui->pbFreq, SIGNAL(clicked(bool)), SIGNAL(freqButtonClicked(bool)));

    connect(ui->cbRoutes, SIGNAL(clicked(bool)), SIGNAL(routesClicked(bool)));
    connect(ui->cbPoints, SIGNAL(clicked(bool)), SIGNAL(pointsClicked(bool)));
    connect(ui->cbBPoints, SIGNAL(clicked(bool)), SIGNAL(BpointsClicked(bool)));
    connect(ui->cbSectors, SIGNAL(clicked(bool)), SIGNAL(sectorsClicked(bool)));
    connect(ui->cbReminders, SIGNAL(clicked(bool)), SIGNAL(remindersClicked(bool)));
    connect(ui->cbMeteo, SIGNAL(clicked(bool)), SIGNAL(meteoClicked(bool)));

    connect(ui->pbNormal, SIGNAL(clicked()), SIGNAL(normalButtonClicked()));

    connect(ui->bAdfOn, SIGNAL(toggled(bool)), SIGNAL(bearingIdChanged()));
    connect(ui->cbAdfChannel, SIGNAL(checkedItemsChanged(QStringList)), SLOT(onBearingIdChanged(QStringList)));

    height_slider_->setTickPosition(QSlider::TicksAbove);
    height_slider_->setHandleMovementMode(QxtSpanSlider::NoOverlapping);
    height_slider_->setSingleStep(10);
    height_slider_->setPageStep(100);
    height_slider_->setMinimum(0);
    height_slider_->setMaximum(680);
    connect(height_slider_, SIGNAL(lowerPositionChanged(int)), SIGNAL(heightLowerPositionChanged(int)));
    connect(height_slider_, SIGNAL(upperPositionChanged(int)), SIGNAL(heightUpperPositionChanged(int)));
    connect(height_slider_, SIGNAL(spanChanged(int, int)), SIGNAL(heightSpanChanged(int, int)));
    ui->horizontalLayout_3->addWidget(height_slider_);

    const QString sheet = " QToolButton { "
        "padding-right: 20px; } "
        " QToolButton::menu-indicator { "
        "subcontrol-origin: padding; "
        "subcontrol-position: center right; }";
    ui->tbPrefSet->setStyleSheet(sheet);

    /*const QString sheet2 = " QToolButton { "
        "border: 1px solid yellow; "
        "border-radius: 2px; "
        "background-color: yellow; "
        "padding-right: 10px; } "
        " QToolButton::menu-indicator { "
        "subcontrol-origin: padding; "
        "subcontrol-position: center right; }";
    ui->tbTL->setStyleSheet(sheet2);
    ui->tbPressure->setStyleSheet(sheet2);*/

    ui->cbHeight->addItem(tr("meters"), 0);
    ui->cbHeight->addItem(tr("feet"), 1);
    connect(ui->cbHeight, SIGNAL(currentIndexChanged(int)), SIGNAL(heightCurrentIndexChanged(int)));
    ui->cbSpeed->addItem(tr("km/h"), 0);
    ui->cbSpeed->addItem(tr("knots"), 1);
    connect(ui->cbSpeed, SIGNAL(currentIndexChanged(int)), SIGNAL(speedCurrentIndexChanged(int)));

    ui->bLabelBottomLeft->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->bLabelTopLeft->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->bLabelBottomRight->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->bLabelBottomLeft->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->bLabelTopRight->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->pbAutoAntioverlap->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->pbShort->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->pbRB->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
    ui->bAdfOn->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");


    /*ui->frameMain->setVisible(false);
    ui->frameScale->setVisible(false);
    ui->frameHeightFilter->setVisible(false);
    ui->bSector->setVisible(false);
    ui->bRestrAreas->setVisible(false);
    ui->frameAirplaneLength->setVisible(false);*/
    //ui->framePressureLevel->setVisible(false);

    adjustSize();

    connect(speed_mapper_, SIGNAL(mapped(int)), this, SIGNAL(speedClicked(int)));
    connect(distance_mapper_, SIGNAL(mapped(int)), this, SIGNAL(distanceClicked(int)));

    connect(ui->bLabelTopLeft, SIGNAL(clicked()), angle_mapper_, SLOT(map()));
    angle_mapper_->setMapping(ui->bLabelTopLeft, 90);
    connect(ui->bLabelBottomLeft, SIGNAL(clicked()), angle_mapper_, SLOT(map()));
    angle_mapper_->setMapping(ui->bLabelBottomLeft, 135);
    connect(ui->bLabelBottomRight, SIGNAL(clicked()), angle_mapper_, SLOT(map()));
    angle_mapper_->setMapping(ui->bLabelBottomRight, 225);
    connect(ui->bLabelTopRight, SIGNAL(clicked()), angle_mapper_, SLOT(map()));
    angle_mapper_->setMapping(ui->bLabelTopRight, 270);

    connect(angle_mapper_, SIGNAL(mapped(int)), this, SLOT(onAngleClicked(int)));
    connect(ui->cbHidePlains, SIGNAL(clicked(bool)), SIGNAL(HidePlainsClicked(bool)));//

    ui->pbShort->setChecked(true);
    setOrientation(horizontal);

    QString feet_meters = networkSettings()->value("users/feet_meters").toString();

    if (!feet_meters.isEmpty()){
        if (feet_meters == "false")
            ui->cbHeight->setEnabled(false);
    }
    else ui->cbHeight->setEnabled(false);


}

ControlWindow::~ControlWindow()
{
    delete ui;
}

bool ControlWindow::isRestrictionButtonChecked() const
{
    return ui->bRestrAreas->isChecked();
}

void ControlWindow::CreateOldButtons()
{
    QLayout * layout = ui->horizontalLayout_6->layout();
    if ( layout != NULL )
    {
        int count = 0;
        QLayoutItem* item;
        while ( ( item = layout->takeAt( 0 ) ) != NULL )
        {
            disconectButtonFromMapper(speed_buttons_, speed_mapper_, reinterpret_cast<QPushButton*>(item->widget()), count+1);
            delete item->widget();
            delete item;
            count++;
        }
    }
    layout = ui->horizontalLayout_8->layout();
    if ( layout != NULL )
    {
        int count = 0;
        QLayoutItem* item;
        while ( ( item = layout->takeAt( 0 ) ) != NULL )
        {
            disconectButtonFromMapper(distance_buttons_, distance_mapper_, reinterpret_cast<QPushButton*>(item->widget()), (count+1)*10);
            delete item->widget();
            delete item;
            count++;
        }
    }
    for(int i = 0; i < 6; i++)
    {
        QString name = QString::number(i*5);
        QPushButton *push_speed = new QPushButton(name, this);
        QPushButton *push_distance = new QPushButton(this);
        connectButtonToMapper(speed_buttons_, speed_mapper_, push_speed, i*5);
        if(i < 2)
        {
            push_distance->setText(name);
            connectButtonToMapper(distance_buttons_, distance_mapper_, push_distance, i*5);
        }
        else
        {
            int value = (i-1)*10;
            push_distance->setText(QString::number(value));
            connectButtonToMapper(distance_buttons_, distance_mapper_, push_distance, value);
        }

        ui->horizontalLayout_6->addWidget(push_speed);
        ui->horizontalLayout_8->addWidget(push_distance);
        push_speed->setMaximumSize(33, 27);
        push_speed->setMinimumSize(33,27);
        push_distance->setMaximumSize(33, 27);
        push_distance->setMinimumSize(33,27);
        push_distance->setCheckable(true);
        push_speed->setCheckable(true);
        push_distance->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
        push_speed->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");

    }
}

void ControlWindow::CreateNewButtons()
{
    QLayout * layout = ui->horizontalLayout_6->layout();
    if ( layout != NULL )
    {
        int count = 0;
        QLayoutItem* item;
        while ( ( item = layout->takeAt( 0 ) ) != NULL )
        {
            disconectButtonFromMapper(speed_buttons_, speed_mapper_, reinterpret_cast<QPushButton*>(item->widget()), count*5);
            delete item->widget();
            delete item;
            count++;
        }
    }
    layout = ui->horizontalLayout_8->layout();
    if ( layout != NULL )
    {
        int count = 0;
        QLayoutItem* item;
        while ( ( item = layout->takeAt( 0 ) ) != NULL )
        {
            int value;
            if(count < 2)
            {
                value = count*5;
            }
            else
            {
                value = (count-1)*10;
            }
            disconectButtonFromMapper(distance_buttons_, distance_mapper_, reinterpret_cast<QPushButton*>(item->widget()), value);
            delete item->widget();
            delete item;
            count++;
        }
    }
    for(int i = 1; i < 6; i++)
    {
        QPushButton *push_speed = new QPushButton(QString::number(i), this);
        ui->horizontalLayout_6->addWidget(push_speed);
        connectButtonToMapper(speed_buttons_, speed_mapper_, push_speed, i);
        if(i < 5)
        {
            QPushButton *push_distance = new QPushButton(this);
            push_distance->setText(QString::number(i*10));
            connectButtonToMapper(distance_buttons_, distance_mapper_, push_distance, i*10);
            ui->horizontalLayout_8->addWidget(push_distance);
            push_distance->setMaximumSize(33, 27);
            push_distance->setMinimumSize(33,27);
            push_distance->setCheckable(true);
            push_distance->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");
        }
        push_speed->setMaximumSize(33, 27);
        push_speed->setMinimumSize(33,27);

        push_speed->setCheckable(true);
        push_speed->setStyleSheet("*:checked { background-color: rgb(140, 140, 140); }");

    }
}

int ControlWindow::zoom() const
{
    return ui->slZoom->value();
}

int ControlWindow::angle() const
{
    return angle_;
}

int ControlWindow::sectorCurrentIndex()
{
    return ui->cbSector->currentIndex();
}

QString ControlWindow::sectorCurrentText()
{
    return ui->cbSector->currentText();
}

void ControlWindow::sectorBlockSignals(bool block)
{
    ui->cbSector->blockSignals(block);
}

void ControlWindow::setCurrentDate(const QString &date)
{
    ui->lDate->setText(date);
}

void ControlWindow::setCurrentTime(const QString &time)
{
    ui->lTime->setText(time);
}

void ControlWindow::setCurrentUser(const QString &user)
{
    ui->lUser->setText(user);
}

void ControlWindow::setSpeedVectorValue(int value)
{
    foreach(QPushButton * b, speed_buttons_)
    {
        b->setChecked(false);
        //b->setStyleSheet("");
    }
    foreach(QPushButton * b, distance_buttons_)
    {
        b->setChecked(false);
        //b->setStyleSheet("");
    }
    QHash<int, QPushButton *>::iterator it = speed_buttons_.find(value);
    if (it != speed_buttons_.end())
    {
        it.value()->setChecked(true);
        /*it.value()->setStyleSheet(
        QString("background-color: rgb(182, 182, 182);"));*/
        //it.value()->setStyleSheet("*:checked { background-color: rgb(70, 70, 70); }");
    }
}

void ControlWindow::setDistanceVectorValue(int value)
{
    foreach(QPushButton * b, speed_buttons_)
    {
        b->setChecked(false);
        //b->setStyleSheet("");
    }
    foreach(QPushButton * b, distance_buttons_)
    {
        b->setChecked(false);
        //b->setStyleSheet("");
    }
    QHash<int, QPushButton *>::iterator it = distance_buttons_.find(value);
    if (it != distance_buttons_.end())
    {
        it.value()->setChecked(true);
        /*it.value()->setStyleSheet(
        QString("background-color: rgb(182, 182, 182);"));*/
        //it.value()->setStyleSheet("*:checked { background-color: rgb(70, 70, 70); }");

    }
}



void ControlWindow::setMainFrameVisible(bool visible)
{
    //ui->frameButtons->setVisible(visible);
    ui->frameScale->setVisible(visible);
    adjustSize();
}

void ControlWindow::setHeightFilterFrameVisible(bool visible)
{
    ui->frameHeightFilter->setVisible(visible);
    adjustSize();
}

/*void ControlWindow::setPressureLevelFrameVisible(bool visible)
{
    ui->framePressureLevel->setVisible(visible);
    adjustSize();
}*/

void ControlWindow::setAirplaneLengthFrameVisible(bool visible)
{
    if(ui->pbShort->isChecked()) {
        ui->frameAirplaneLength->setVisible(visible);
        adjustSize();
    }
}

void ControlWindow::setRestrictionsEditorVisible(bool visible)
{
    ui->bRestrAreas->setVisible(visible);
    adjustSize();
}

/*void ControlWindow::setFlightLevel(const QString &text)
{
    ui->tbTL->setText(text);
}

void ControlWindow::setPressure(const QString &text)
{
    ui->tbPressure->setText(text);
}*/

void ControlWindow::setAutoAntiOverlapChecked(bool checked)
{
    ui->bAutoAntiOverlap->setChecked(checked);
}

void ControlWindow::setRestrictionButtonChecked(bool checked)
{
    ui->bRestrAreas->setChecked(checked);
}

void ControlWindow::setSectorModel(QSqlTableModel *model, int column)
{
    ui->cbSector->setModel(model);
    ui->cbSector->setModelColumn(column);
}

void ControlWindow::setSectorCurrentIndex(int index)
{
    ui->cbSector->setCurrentIndex(index);
}

void ControlWindow::setSettingsMenu(QMenu *menu)
{
    ui->tbPrefSet->setMenu(menu);
    //ui->tbPrefSet->addAction(menu->menuAction());
    //ui->tbPrefSet->setDefaultAction(menu->menuAction());
}

/*void ControlWindow::setFlightLevelMenu(QMenu *menu)
{
    ui->tbTL->setMenu(menu);
    ui->tbTL->addAction(menu->menuAction());
    ui->tbTL->setDefaultAction(menu->menuAction());
}

void ControlWindow::setPressureMenu(QMenu *menu)
{
    ui->tbPressure->setMenu(menu);
    ui->tbPressure->addAction(menu->menuAction());
    ui->tbPressure->setDefaultAction(menu->menuAction());
}*/

void ControlWindow::setZoomRange(int min, int max)
{
    Q_ASSERT(max > min);

    ui->slZoom->setEnabled(true);
    ui->slZoom->setRange(min, max);
    //ui->slZoom->setSingleStep((max - min) / 20);
    ui->slZoom->setPageStep(5);

    ui->lZoomMin->setText(QString::number(min));
    ui->lZoomMax->setText(QString::number(max));
}

void ControlWindow::setZoom(int current)
{
    //ui->slZoom->blockSignals(true);
    ui->slZoom->setValue(current);
    //ui->slZoom->blockSignals(false);
    ui->lZoomCurrent->setText(QString::number(current));
}

void ControlWindow::setOrientation(Orientation orientation)
{
    QLayout *old_main_layout = layout();
    old_main_layout->removeItem(ui->fixedHeightSpacer);
    old_main_layout->removeWidget(ui->frameInfo);
    //old_main_layout->removeWidget(ui->framePressureLevel);
    old_main_layout->removeWidget(ui->frameButtons);
    old_main_layout->removeWidget(ui->frameDisplay);
    old_main_layout->removeWidget(ui->frameHeightFilterScale);
    old_main_layout->removeWidget(ui->frameLabelsOverlap);
    old_main_layout->removeWidget(ui->frameAirplaneLength);
    old_main_layout->removeWidget(ui->frameAdf);
    old_main_layout->removeWidget(ui->frameAflSpeed);
    delete old_main_layout;

    QBoxLayout *main_layout = NULL;
    if (horizontal == orientation) {
        main_layout = new QHBoxLayout(this);
        main_layout->setSpacing(2);
        main_layout->setContentsMargins(2, 2, 2, 2);
        main_layout->setObjectName(QString::fromUtf8("horizontalLayout"));
    } else {
        main_layout = new QVBoxLayout(this);
        main_layout->setSpacing(2);
        main_layout->setContentsMargins(2, 2, 2, 2);
        main_layout->setObjectName(QString::fromUtf8("verticalLayout"));
    }
    main_layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(main_layout);

    if (orientation == horizontal)
        main_layout->addItem(ui->fixedHeightSpacer);

     QBoxLayout *frameHeightFilterScale_layout = NULL;
     QLayout *old_frameHeightFilterScale_layout = ui->frameHeightFilterScale->layout();
     old_frameHeightFilterScale_layout->removeWidget(ui->frameHeightFilter);
     old_frameHeightFilterScale_layout->removeWidget(ui->frameScale);
     delete old_frameHeightFilterScale_layout;

      QBoxLayout *frameHeightFilter_layout = NULL;
      QBoxLayout *frameHeightFilter_layout_1 = NULL;
      QBoxLayout *frameHeightFilter_layout_2 = NULL;
      QLayout *old_frameHeightFilter_layout = ui->frameHeightFilter->layout();
      old_frameHeightFilter_layout->removeWidget(ui->lHeightMin);
      old_frameHeightFilter_layout->removeWidget(ui->lHeightMax);
      old_frameHeightFilter_layout->removeWidget(ui->lHightFilter);
      old_frameHeightFilter_layout->removeWidget(ui->cbHidePlains);
      delete frameHeightFilter_layout_1;
      delete frameHeightFilter_layout_2;
      delete old_frameHeightFilter_layout;

      QBoxLayout *frameScale_layout = NULL;
      QBoxLayout *frameScale_layout_1 = NULL;
      QBoxLayout *frameScale_layout_2 = NULL;
      QBoxLayout *frameScale_layout_3 = NULL;
      QLayout *old_frameScale_layout = ui->frameScale->layout();

      old_frameScale_layout->removeWidget(ui->lZoomMin);
      old_frameScale_layout->removeWidget(ui->lZoomCurrent);
      old_frameScale_layout->removeWidget(ui->lZoomMax);
      old_frameScale_layout->removeWidget(ui->slZoom);


      ui->stack->removeWidget(ui->lScale);
      ui->stack->removeWidget(ui->pbNormal);
      old_frameScale_layout->removeWidget(ui->stack);
      delete frameScale_layout_1;
      delete frameScale_layout_2;
      delete frameScale_layout_3;
      delete old_frameScale_layout;


    if (orientation == horizontal) {

        frameHeightFilterScale_layout = new QVBoxLayout();
        frameHeightFilterScale_layout->setSpacing(2);
        frameHeightFilterScale_layout->setContentsMargins(2, 2, 2, 2);
        frameHeightFilterScale_layout->setObjectName(QString::fromUtf8("frameHeightFilterScale_layout"));

        frameHeightFilter_layout = new QVBoxLayout();
        frameHeightFilter_layout->setSpacing(2);
        frameHeightFilter_layout->setContentsMargins(2, 2, 2, 2);
        frameHeightFilter_layout->setObjectName(QString::fromUtf8(""));

        height_slider_->setOrientation(Qt::Horizontal);
        ui->lHeightMin->setAlignment(Qt::AlignLeft);
        ui->lHeightMax->setAlignment(Qt::AlignRight);

        frameHeightFilter_layout->addWidget(ui->lHightFilter);
        frameHeightFilter_layout->addWidget(ui->cbHidePlains);

        frameHeightFilter_layout_1 = new QHBoxLayout();
        frameHeightFilter_layout_1->addWidget(ui->lHeightMin);
        frameHeightFilter_layout_1->addWidget(ui->lHeightMax);
        frameHeightFilter_layout->addLayout(frameHeightFilter_layout_1);
        frameHeightFilter_layout->addWidget(height_slider_);

        frameScale_layout = new QVBoxLayout();
        frameScale_layout->setSpacing(2);
        frameScale_layout->setContentsMargins(2, 2, 2, 2);
        frameScale_layout->setObjectName(QString::fromUtf8(""));

        ui->slZoom->setOrientation(Qt::Horizontal);

        frameScale_layout_1 = new QHBoxLayout();
        frameScale_layout_1->addWidget(ui->lScale);
        frameScale_layout_1->addWidget(ui->pbNormal);

        frameScale_layout->addLayout(frameScale_layout_1);

        ui->lZoomMin->setAlignment(Qt::AlignLeft);
        ui->lZoomCurrent->setAlignment(Qt::AlignHCenter);
        ui->lZoomMax->setAlignment(Qt::AlignRight );
        frameScale_layout_2 = new QHBoxLayout();
        frameScale_layout_2->addWidget(ui->lZoomMin);
        frameScale_layout_2->addWidget(ui->lZoomCurrent);
        frameScale_layout_2->addWidget(ui->lZoomMax);
        frameScale_layout->addLayout(frameScale_layout_2);
        frameScale_layout->addWidget(ui->slZoom);

        ui->frameHeightFilterScale->setMinimumSize(322,0);

    }
    else {
        frameHeightFilterScale_layout= new QHBoxLayout();
        frameHeightFilterScale_layout->setSpacing(2);
        frameHeightFilterScale_layout->setContentsMargins(2, 2, 2, 2);
        frameHeightFilterScale_layout->setObjectName(QString::fromUtf8(""));

        frameHeightFilter_layout = new QVBoxLayout();
        frameHeightFilter_layout->setSpacing(2);
        frameHeightFilter_layout->setContentsMargins(2, 2, 2, 2);
        frameHeightFilter_layout->setObjectName(QString::fromUtf8(""));

        height_slider_->setOrientation(Qt::Vertical);
        ui->lHeightMax->setAlignment(Qt::AlignCenter);
        ui->lHeightMin->setAlignment(Qt::AlignCenter);
        frameHeightFilter_layout->addWidget(ui->lHightFilter);
        frameHeightFilter_layout->addWidget(ui->cbHidePlains);

        frameHeightFilter_layout->addWidget(ui->lHeightMax);
        frameHeightFilter_layout_2 = new QHBoxLayout();

        frameHeightFilter_layout_2->addWidget(height_slider_);

        frameHeightFilter_layout->addLayout(frameHeightFilter_layout_2);
        frameHeightFilter_layout->addWidget(ui->lHeightMin);

        frameScale_layout = new QVBoxLayout();
        frameScale_layout->setSpacing(2);
        frameScale_layout->setContentsMargins(2, 2, 2, 2);
        frameScale_layout->setObjectName(QString::fromUtf8(""));

        ui->slZoom->setOrientation(Qt::Vertical);
        frameScale_layout->addWidget(ui->lScale);
        frameScale_layout->addWidget(ui->pbNormal);

        ui->lZoomMax->setAlignment(Qt::AlignCenter);
        frameScale_layout->addWidget(ui->lZoomMax);
        frameScale_layout_3 = new QHBoxLayout();
        frameScale_layout->addLayout(frameScale_layout_3);
        ui->lZoomCurrent->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

        frameScale_layout_3->addWidget(ui->lZoomCurrent);
        frameScale_layout_3->addWidget(ui->slZoom);

        frameScale_layout_3->addSpacing(44);
        ui->lZoomMin->setAlignment(Qt::AlignCenter);
        frameScale_layout->addWidget(ui->lZoomMin);

        ui->frameHeightFilterScale->setMinimumSize(0,160);

    }

    ui->frameHeightFilter->setLayout(frameHeightFilter_layout);
    ui->frameScale->setLayout(frameScale_layout);
    ui->frameHeightFilterScale->setLayout(frameHeightFilterScale_layout);

    frameHeightFilterScale_layout->addWidget(ui->frameHeightFilter);
    frameHeightFilterScale_layout->addWidget(ui->frameScale);

    QBoxLayout *main_layout_1 = new QHBoxLayout();
    QBoxLayout *main_layout_2 = new QHBoxLayout();
    main_layout_1->addWidget(ui->frameButtons);
    main_layout_1->addWidget(ui->frameDisplay);
    main_layout_2->addWidget(ui->frameAdf);
    main_layout_2->addWidget(ui->frameAflSpeed);
    main_layout->addWidget(ui->frameInfo);
    main_layout->addLayout(main_layout_1);
    main_layout->addWidget(ui->frameHeightFilterScale);
    main_layout->addWidget(ui->frameLabelsOverlap);
    main_layout->addWidget(ui->frameAirplaneLength);
    main_layout->addLayout(main_layout_2);

    adjustSize();
}

void ControlWindow::onPointsClicked(bool checked)
{
    ui->cbPoints->setChecked(checked);
}

void ControlWindow::onBPointsClicked(bool checked)
{
    ui->cbBPoints->setChecked(checked);
}

void ControlWindow::onRoutesClicked(bool checked)
{
    ui->cbRoutes->setChecked(checked);
}

void ControlWindow::onSectorsClicked(bool checked)
{
    ui->cbSectors->setChecked(checked);
}

void ControlWindow::setHeightCurrentIndex(int index)
{
    ui->cbHeight->setCurrentIndex(index);
}

void ControlWindow::setSpeedCurrentIndex(int index)
{
    ui->cbSpeed->setCurrentIndex(index);
}

void ControlWindow::on_slZoom_valueChanged(int value)
{
    ui->lZoomCurrent->setText(QString::number(value));
    emit zoomChanged(value);
}

void ControlWindow::changeEvent(QEvent *event)
{
    if (QEvent::ActivationChange == event->type()) {
        setWindowOpacity(isActiveWindow() ? 1 : 0.5);
    } else if (QEvent::FontChange == event->type()) {
        QFont f = font();
        f.setFamily("Courier New");
        f.setBold(false);
        ui->lDisplay->setFont(f);
        ui->lHightFilter->setFont(f);
        ui->lScale->setFont(f);
        ui->lAdf->setFont(f);
        f.setBold(true);
        ui->lAdfLastPeleng->setFont(f);
    }
}

void ControlWindow::updateLanguage()
{
    ui->bQuit->setText(tr("Quit"));
    ui->bRestrAreas->setText(tr("R.Areas"));
    ui->tbPrefSet->setText(tr("Pref.Set"));
    ui->bMaps->setText(tr("Maps"));
    ui->lHightFilter->setText(tr("Height Filter"));
    ui->lScale->setText(tr("ZOOM"));
    ui->pbFreq->setText(tr("User pics"));
}

void ControlWindow::onAngleClicked(int value)
{
    angle_ = value;

    emit angleClicked(value);
}

void ControlWindow::onBearingIdChanged(const QStringList &items)
{
    if (items.size() > 1)
    {
        ui->cbAdfChannel->setEditText(tr("%1 freq").arg(items.size()));
    }

    emit bearingIdChanged();
}

void ControlWindow::connectButtonToMapper(QHash<int, QPushButton *> &bindings, QSignalMapper *mapper, QPushButton *button, int value)
{
    connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(button, value);
    bindings.insert(value, button);
}

void ControlWindow::disconectButtonFromMapper(QHash<int, QPushButton *> &bindings, QSignalMapper *mapper, QPushButton *button, int value)
{
    disconnect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->removeMappings(button);
    if(bindings.contains(value))
    {
        bindings.remove(value);
    }
}

void ControlWindow::onLowerPositionChanged(int value)
{
    Q_ASSERT(height_slider_);
    emit heightSpanChanged(value, height_slider_->upperValue());
}

void ControlWindow::onUpperPositionChanged(int value)
{
    Q_ASSERT(height_slider_);
    emit heightSpanChanged(height_slider_->lowerValue(), value);
}

int ControlWindow::heightLowerValue() const
{
    return height_slider_->lowerValue();
}

void ControlWindow::setHeightLowerValue(int value)
{
    height_slider_->setLowerValue(value);
}

int ControlWindow::heightUpperValue() const
{
    return height_slider_->upperValue();
}

void ControlWindow::setHeightUpperValue(int value)
{
    height_slider_->setUpperValue(value);
}

void ControlWindow::setHeightRange(int min, int max)
{
    Q_ASSERT(max > min);

    height_slider_->setMinimum(min);
    height_slider_->setMaximum(max);
    height_slider_->setSingleStep((max - min) / 10);
    height_slider_->setPageStep((max - min) / 5);
}

void ControlWindow::setSpanRange(int min, int max)
{
    Q_ASSERT(max > min);
    height_slider_->setSpan(min, max);

    setHeightMinText(QString("%1").arg(min));
    setHeightMaxText(QString("%1").arg(max));
}

void ControlWindow::setRestrictedSpanRange(int min, int max)
{
    Q_ASSERT(max > min);
    height_slider_->setRestrictedSpan(min, max);
}

void ControlWindow::setHeightMinText(const QString &text)
{
    ui->lHeightMin->setText(text);
}

void ControlWindow::setHeightMaxText(const QString &text)
{
    ui->lHeightMax->setText(text);
}
void ControlWindow::_OnShortCW(bool check)
{
    if (check)
    {
        ui->fixedHeightSpacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);

        ui->frameButtons->show();
        ui->frameDisplay->show();
        ui->frameHeightFilterScale->show();
        ui->frameLabelsOverlap->show();
        ui->frameAirplaneLength->show();
        ui->frameAdf->show();
    }
    else
    {
        ui->fixedHeightSpacer->changeSize(0, ui->fixedHeightSpacer->geometry().height(), QSizePolicy::Fixed, QSizePolicy::Fixed);

        ui->frameButtons->hide();
        ui->frameDisplay->hide();
        ui->frameHeightFilterScale->hide();
        ui->frameLabelsOverlap->hide();
        ui->frameAirplaneLength->hide();
        ui->frameAdf->hide();
    }
}

void ControlWindow::MeteoVisible(bool visible)
{
    ui->cbMeteo->setVisible(visible);
    //ui->cbMeteo->click();
}
void ControlWindow::MeteoCheck()
{
    ui->cbMeteo->setChecked(true);
}

void ControlWindow::setBearingPoints(const QList<CBearingPoint> &points)
{
    ui->cbAdfChannel->clear();

    int length = 0;
    foreach (const CBearingPoint &point, points)
    {
        ui->cbAdfChannel->addItem(point.GetName(), point.GetID());
        length = qMax(point.GetName().size(), length);
    }
    ui->cbAdfChannel->setMinimumContentsLength(length);
}

void ControlWindow::setBearingId(const QVariant &id)
{   
    QSqlTableModel* sectorBearingModel = new QSqlTableModel(this);
    sectorBearingModel->setTable("sectoring_bearing");
    sectorBearingModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sectorBearingModel->select();

    for (int i = 0; i < ui->cbAdfChannel->count(); ++i){
         ui->cbAdfChannel->setItemCheckState(i, Qt::Unchecked);
    }
    for (int iRow = 0; iRow < sectorBearingModel->rowCount(); iRow++){
        if (sectorBearingModel->record(iRow).value("sect_id").toUInt() == id){
            int bID = sectorBearingModel->record(iRow).value("bearing_id").toInt();
            ui->cbAdfChannel->setItemCheckState(ui->cbAdfChannel->findData(bID), Qt::Checked);
       }
    }

}

TBearingIdSet ControlWindow::getBearingId() const
{
    TBearingIdSet result;

    if (ui->bAdfOn->isChecked())
    {
        for (int i = 0; i < ui->cbAdfChannel->count(); ++i)
        {
            if (ui->cbAdfChannel->itemCheckState(i) == Qt::Checked)
                result.insert(ui->cbAdfChannel->itemData(i).toUInt());
        }
    }

    return result;
}

void ControlWindow::onLastBearing(const QString &lastBearing)
{
    ui->lAdfLastPeleng->setText(lastBearing);
}




