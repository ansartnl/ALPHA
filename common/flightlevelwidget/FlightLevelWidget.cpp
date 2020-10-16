#include <QGridLayout>
#include <QToolButton>
#include <QScrollBar>
#include <QCloseEvent>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "FlightLevelWidget.h"
#include "ui_FlightLevelWidget.h"
#include "../dbconnectionsmanager/dbconnectionsmanager.h"

const QString FlightLevelWidget::kWhiteBackgroundStyle = "background-color: rgb(255, 255, 255);";
const QString FlightLevelWidget::kOrangeBackgroundStyle = "background-color: rgb(255, 127, 0);";
const QString FlightLevelWidget::kGrayBackgroundStyle = "background-color: rgb(147, 147, 147);";
const QString FlightLevelWidget::kBorderStyle = "border: 1px solid #8f8f91";


FlightLevelWidget::FlightLevelWidget(const QString &acid, const QString &type, 
                                     QWidget *parent, Qt::WindowFlags flags)
    : QFrame(parent, flags)
    , ui(new Ui::FlightLevelWidget)
    , first_show_(true)
    , verticalSpacer1(new QSpacerItem(20, 10, QSizePolicy::Preferred, QSizePolicy::Expanding))
    , verticalSpacer2(new QSpacerItem(20, 10, QSizePolicy::Preferred, QSizePolicy::Expanding))
    , visible_buttons_count_(0)
    , grid_visible_(true)
    , value_in_meters_(kInvalidValue)
    , value_in_feets_(kInvalidValue)
{
    ui->setupUi(this);

    ui->labelACID->setText(acid);
    ui->labelType->setText(type);

    ui->customValue->setValidator(new QIntValidator(1, 9999, this));

    setValues(loadFlightLevels(), FEET);

    setFrameShape(QFrame::Box);

    if (parent) // fix Qt 5.10 crash
        setAttribute(Qt::WA_WindowPropagation);

    connect(ui->cbTflMarker, SIGNAL(stateChanged(int)), this, SLOT(onMarkerStateChanged(int)));
}

void FlightLevelWidget::setGrid(bool grid_visible)
{
    grid_visible_ = grid_visible;

	foreach (QToolButton *button, buttons_) {
		button->setAutoRaise(!grid_visible_);
		if (grid_visible_)
			button->setStyleSheet(button->property("forebackcolor").toString() + kBorderStyle);
		else
			button->setStyleSheet(button->property("forebackcolor").toString());
	}
}

void FlightLevelWidget::setTflMarkerVisible(bool visible)
{
    ui->cbTflMarker->setVisible(visible);
}

void FlightLevelWidget::setTflMarkerChecked(bool checked)
{
    ui->cbTflMarker->setChecked(checked);
}

bool FlightLevelWidget::isTflMarkerChecked() const
{
    return ui->cbTflMarker->isChecked();
}

bool FlightLevelWidget::setCountVisibleButtons(int count)
{
    if (count < 0)
        return false;

    visible_buttons_count_ = count;
    return true;
}

void FlightLevelWidget::setACID(const QString &acid)
{
    ui->labelACID->setText(acid);
}

void FlightLevelWidget::setType(const QString &type)
{
    ui->labelType->setText(type);
}

bool FlightLevelWidget::setVisibleValue(const QString &sValue)
{
    QString visible_value(sValue.trimmed());
    if (visible_value.isEmpty())
        return false;

    Measure temp_measure;
    if (visible_value[0] == 'F')
        temp_measure = FEET;
    else if (visible_value[0] == 'C')
        temp_measure = METERS;
    else
        return false;

    int iValue = 0;
    bool bOk(false);
    iValue = visible_value.right(visible_value.size() - 1).toInt(&bOk);

    if (!bOk)
        return false;

    bool button_found = false;
    QString propertyValue;

    switch (temp_measure) {
    case METERS:
        propertyValue = "meters";
        break;
    case FEET:
        propertyValue = "feet";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return false;
    }

	foreach(QToolButton * button, buttons_) {
        if (button->property(propertyValue.toLatin1().constData()).toInt() == iValue) {
			ui->scrollArea->ensureWidgetVisible(button);
			button->setProperty("visiblebutton", 1);
			button_found = true;
		} else {
			button->setProperty("visiblebutton", 0);
		}
	}

    if (!button_found)
        ui->customValue->setText(QString("%1").arg(iValue));
    else
        ui->customValue->clear();

    return true;
}

QPoint FlightLevelWidget::buttonPosition(const QString &button_value)
{
    QString visible_value(button_value.trimmed());
    if (visible_value.isEmpty())
        return QPoint();

    Measure temp_measure;
    if (visible_value[0] == 'F')
        temp_measure = FEET;
    else if (visible_value[0] == 'C')
        temp_measure = METERS;
    else
        return QPoint();

    int iValue = 0;
    bool bOk(false);
    iValue = visible_value.right(visible_value.size() - 1).toInt(&bOk);

    if (!bOk)
        return QPoint();

    QString propertyValue;
    switch (temp_measure) {
    case METERS:
        propertyValue = "meters";
        break;
    case FEET:
        propertyValue = "feet";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return QPoint();
    }

	foreach(QToolButton * button, buttons_) {
        if (button->property(propertyValue.toLatin1().constData()).toInt() == iValue) {
            ui->scrollArea->ensureWidgetVisible(button);
            button->setProperty("visiblebutton", 1);
            QPoint content_point(button->mapTo(ui->scrollAreaWidgetContents, button->rect().center()));
            QPoint area_point(ui->scrollAreaWidgetContents->mapTo(ui->scrollArea, content_point));
            return ui->scrollArea->mapTo(this, area_point);
        }
    }

    return QPoint();
}

bool FlightLevelWidget::setValueColor(const QString &value, const QColor &foreground_color, const QColor &background_color)
{
    QString color_value(value.trimmed());
    if (color_value.isEmpty())
        return false;

    Measure temp_measure;

    if (color_value[0] == 'F')
        temp_measure = FEET;
    else if (color_value[0] == 'C')
        temp_measure = METERS;
    else
        return false;

    int iValue = 0;
    bool bOk(false);
    iValue = color_value.right(color_value.size() - 1).toInt(&bOk);

    if (!bOk)
        return false;

    QString sForeBackColor = QString("color: rgb(%1, %2, %3);")
                             .arg(foreground_color.red())
                             .arg(foreground_color.green())
                             .arg(foreground_color.blue()) + ";" +
                             QString("background-color: rgb(%1, %2, %3);")
                             .arg(background_color.red())
                             .arg(background_color.green())
                             .arg(background_color.blue()) + ";";

    QString propertyValue;

    switch (temp_measure) {
    case METERS:
        propertyValue = "meters";
        break;
    case FEET:
        propertyValue = "feet";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return false;
    }
    
	QSet<QToolButton *>::iterator end_it = buttons_.end();
	for (QSet<QToolButton *>::iterator it = buttons_.begin(); it != end_it; ++it) {
		if ((*it)->property(propertyValue.toStdString().c_str()).toInt() == iValue) {
			if (grid_visible_)
				(*it)->setStyleSheet(sForeBackColor + kBorderStyle);
			else
				(*it)->setStyleSheet(sForeBackColor);
			(*it)->setProperty("forebackcolor", sForeBackColor);
			break;
		}
	}

    return true;
}

void FlightLevelWidget::setRangeColor(int valueMin, int valueMax, Measure measure, const QColor &foregroundColor,
                                      const QColor &backgroundColor)
{
    QString sForeBackColor = QString("color: rgb(%1, %2, %3);")
                             .arg(foregroundColor.red())
                             .arg(foregroundColor.green())
                             .arg(foregroundColor.blue()) + ";" +
                             QString("background-color: rgb(%1, %2, %3);")
                             .arg(backgroundColor.red())
                             .arg(backgroundColor.green())
                             .arg(backgroundColor.blue()) + ";";

    QString propertyValue;

    switch (measure) {
    case METERS:
        propertyValue = "meters";
        break;
    case FEET:
        propertyValue = "feet";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

	QSet<QToolButton *>::iterator end_it = buttons_.end();
	for (QSet<QToolButton *>::iterator it = buttons_.begin(); it != end_it; ++it) {
		if ((*it)->property(propertyValue.toStdString().c_str()).toInt() >= valueMin &&
			(*it)->property(propertyValue.toStdString().c_str()).toInt() <= valueMax) {
			if (grid_visible_)
				(*it)->setStyleSheet(sForeBackColor + kBorderStyle);
			else
				(*it)->setStyleSheet(sForeBackColor);
			(*it)->setProperty("forebackcolor", sForeBackColor);
		}
	}
}

void FlightLevelWidget::setValues(const QList<FlightLevel> &values, Measure /*measure*/)
{
    first_show_ = true;
    flight_levels_ = values;

    init();
}

int FlightLevelWidget::value(Measure measure) const
{
    return measure == METERS ? value_in_meters_ : value_in_feets_;
}

bool FlightLevelWidget::eventFilter(QObject *obj, QEvent *event)
{
    QToolButton *button = qobject_cast<QToolButton *>(obj);
    if (!button || !buttons_.contains(button))
        return false;

    switch (event->type()) {
    case QEvent::Enter:
        if (grid_visible_)
            button->setStyleSheet(QString(kOrangeBackgroundStyle) + kBorderStyle);
        else
            button->setStyleSheet(kOrangeBackgroundStyle);

        emit valueEnter(QString("F") + button->property("feet").toString());
        break;
    case QEvent::Leave:
        if (grid_visible_)
            button->setStyleSheet(button->property("forebackcolor").toString() + kBorderStyle);
        else
            button->setStyleSheet(button->property("forebackcolor").toString());
        break;
    default:
        return false;
    }

    return true;
}

void FlightLevelWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    if (first_show_) {
        int button_width = 0;
        int button_height = 0;

		foreach (QToolButton * button, buttons_) {
            if (button->width() > button_width)
                button_width = button->width();
            button_height = button->height();
        }

        if (visible_buttons_count_ > 0) {
            int controls_height = height() - ui->scrollArea->height();
			resize(width(), button_height * visible_buttons_count_ + controls_height);
        }

		foreach(QToolButton * button, buttons_) {
			button->setMaximumWidth(button_width);
			button->setMinimumWidth(button_width);
            if (1 == button->property("visiblebutton").toInt())
                ui->scrollArea->ensureWidgetVisible(button);
        }

        first_show_ = false;
    }
}

void FlightLevelWidget::closeEvent(QCloseEvent *event)
{
    emit rejected();
    hide();
    event->ignore();
}

void FlightLevelWidget::toolClicked()
{
    value_in_meters_ = sender()->property("meters").toInt();
    value_in_feets_ = sender()->property("feet").toInt();
    emit accepted(sender()->property("feet").toDouble(), sender()->property("meters").toDouble(), FEET);

    hide();
}

void FlightLevelWidget::on_tbRemove_clicked()
{
    value_in_meters_ = kInvalidValue;
    value_in_feets_ = kInvalidValue;
    emit accepted(value_in_feets_, value_in_meters_, FEET);
    hide();
}

void FlightLevelWidget::on_customValue_editingFinished()
{
    if (!ui->customValue->hasFocus())
        return;

    if (ui->customValue->text().isEmpty())
        return;

    if (ui->customValue->text().length() < 2)
        return;

    value_in_feets_ = ui->customValue->text().toInt();
    emit accepted(ui->customValue->text().toDouble(), 0., FEET);

    hide();
}

void FlightLevelWidget::onMarkerStateChanged(int state)
{
    if (Qt::Checked == state) {
        emit tflMarkerChecked();
        hide();
    }
}

QList<FlightLevelWidget::FlightLevel> FlightLevelWidget::loadFlightLevels()
{
    static QList<FlightLevel> flightLevels;

    if (flightLevels.isEmpty()) {
        QSqlDatabase db = QSqlDatabase::database();
        if(DBConnectionsManager::instance().getConnectionStatus() == 1)
        //if (db.isOpen())
        {
            QSqlQuery query(db);
            if (query.exec("SELECT name, echelon, meters, feet FROM echelons ORDER BY ID DESC"))
            {
                while (query.next())
                {
                    FlightLevel fl;
                    fl.name    = query.value(0).toString();
                    fl.echelon = query.value(1).toInt();
                    fl.meters  = query.value(2).toInt();
                    fl.feet    = query.value(3).toInt();

                    if (!fl.name.isEmpty()) {
                        flightLevels.append(fl);
                    }
                }
            }
        }
    }

    return flightLevels;
}

void FlightLevelWidget::init()
{
    ui->gridLayout->removeItem(verticalSpacer1);
    ui->gridLayout->removeItem(verticalSpacer2);

	foreach (QToolButton * button, buttons_) {
        ui->gridLayout->removeWidget(button);
        delete button;
    }
    buttons_.clear();

    bool left_column = true;
    int i = 0;
    foreach (const FlightLevel &fl, flight_levels_) {
        QToolButton *button = new QToolButton(ui->scrollArea->widget());
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        button->setProperty("meters", QString("%1").arg(fl.meters, 4, 10 , QChar('0')));
        button->setProperty("feet", QString("%1").arg(fl.echelon, 3, 10 , QChar('0')));
        button->setProperty("forebackcolor", kWhiteBackgroundStyle);
        button->setProperty("visiblebutton", 0);
        button->setAutoRaise(!grid_visible_);
        button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

        if (grid_visible_)
            button->setStyleSheet(kWhiteBackgroundStyle + kBorderStyle);
        else
            button->setStyleSheet(kWhiteBackgroundStyle);

        button->setText(fl.name);

        connect(button, SIGNAL(clicked()), this, SLOT(toolClicked()));
        button->installEventFilter(this);

        buttons_.insert(button);

        ui->gridLayout->addWidget(button, i, int(left_column), 2, 1, Qt::AlignTop | (left_column ? Qt::AlignLeft : Qt::AlignRight));
        left_column = !left_column;

        ++i;
    }

    ui->gridLayout->addItem(verticalSpacer1, flight_levels_.size(), 0, 1, 1, Qt::AlignTop);
    ui->gridLayout->addItem(verticalSpacer2, flight_levels_.size() + 1, 1, 1, 1, Qt::AlignTop);
}

void FlightLevelWidget::setFocusOnCustomEdit() const
{
    QMetaObject::invokeMethod(ui->customValue, "setFocus", Qt::QueuedConnection);
}
