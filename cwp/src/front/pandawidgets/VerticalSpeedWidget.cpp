#include "VerticalSpeedWidget.h"
#include "ui_VerticalSpeedWidget.h"

#include <algorithm>

#include <QGridLayout>
#include <QToolButton>
#include <QScrollBar>
#include <QCloseEvent>
#include <QDebug>

#include "WidgetConsts.h"

#include "main.h"
#include "Dao.h"

using std::set;

QHash<int, std::set<int> > VerticalSpeedWidget::values_by_measure_;

VerticalSpeedWidget::VerticalSpeedWidget(const QString &acid, const QString &init_value, QWidget *parent, Qt::WindowFlags flags)
    : QFrame(parent, flags)
    , ui(new Ui::VerticalSpeedWidget)
    , measure_(METERS)
    , first_show_(true)
    , visible_buttons_count_(0)
    , grid_visible_(true)
{
    ui->setupUi(this);

    ui->labelACID->setText(acid);

    grid_layout_ = new QGridLayout(ui->scrollArea->widget());
    grid_layout_->setSpacing(0);
    grid_layout_->setMargin(0);

    ui->customValue->setValidator(new QIntValidator(1, 9999, this));

    initValues();
    init();
    setVisibleValue(init_value);
    setValueColor(init_value, Qt::black, Qt::yellow);

    setFrameShape(QFrame::Box);

    if (parent) // fix Qt 5.10 crash
        setAttribute(Qt::WA_WindowPropagation);
}

void VerticalSpeedWidget::setGrid(bool grid_visible)
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

bool VerticalSpeedWidget::setCountVisibleButtons(int count)
{
    if (count < 0)
        return false;

    visible_buttons_count_ = count;
    return true;
}

void VerticalSpeedWidget::setACID(const QString &acid)
{
    ui->labelACID->setText(acid);
}

bool VerticalSpeedWidget::setVisibleValue(const QString &sValue)
{
    QString visible_value = sValue.trimmed();
    if (visible_value.isEmpty())
        return false;

    Measure temp_measure = METERS;
    if (visible_value.startsWith("fm"))
        temp_measure = FEET;
    else if (visible_value.startsWith("ms"))
        temp_measure = METERS;
    else
        return false;

    int iValue = 0;
    bool bOk = false;
    iValue = visible_value.right(visible_value.size() - 2).toInt(&bOk);

    if (!bOk)
        return false;

    if (temp_measure != measure_) {
        switch (temp_measure) {
        case METERS:
            ui->mButton->click();
            break;
        case FEET:
            ui->fButton->click();
            break;
        default:
            qDebug() << "Unknown measure type! " << __FUNCTION__;
            return false;
        }
    }

    QString property_value;

    switch (measure_) {
    case METERS:
        property_value = "meters";
        break;
    case FEET:
        property_value = "feet";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return false;
    }

    bool button_found = false;
    foreach (QToolButton *button, buttons_) {
        if (button->property(property_value.toStdString().c_str()).toInt() == iValue) {
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

bool VerticalSpeedWidget::setValueColor(const QString &sValue, const QColor &foregroundColor,
                                        const QColor &backgroundColor)
{
    QString color_value = sValue.trimmed();

    if (color_value.isEmpty())
        return false;

    Measure temp_measure;

    if (color_value.startsWith("fm"))
        temp_measure = FEET;
    else if (color_value.startsWith("ms"))
        temp_measure = METERS;
    else
        return false;

    int iValue = 0;
    bool bOk(false);
    iValue = color_value.right(color_value.size() - 2).toInt(&bOk);

    if (!bOk)
        return false;

    QString sForeBackColor = QString("color: rgb(%1, %2, %3);")
                             .arg(foregroundColor.red())
                             .arg(foregroundColor.green())
                             .arg(foregroundColor.blue()) + ";" +
                             QString("background-color: rgb(%1, %2, %3);")
                             .arg(backgroundColor.red())
                             .arg(backgroundColor.green())
                             .arg(backgroundColor.blue()) + ";";

    QString property_value;

    switch (temp_measure) {
    case METERS:
        property_value = "meters";
        break;
    case FEET:
        property_value = "feet";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return false;
    }

    foreach (QToolButton *button, buttons_) {
        if (button->property(property_value.toStdString().c_str()).toInt() == iValue) {
            if (grid_visible_)
                button->setStyleSheet(sForeBackColor + kBorderStyle);
            else
                button->setStyleSheet(sForeBackColor);
            button->setProperty("forebackcolor", sForeBackColor);
            break;
        }
    }

    return true;
}

void VerticalSpeedWidget::setRangeColor(int valueMin, int valueMax, Measure measure, const QColor &foregroundColor,
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

    /*QSet<QToolButton *>::const_iterator end_it = buttons_.constEnd();
    for (QSet<QToolButton *>::const_iterator it = buttons_.constBegin(); it != end_it; ++it) {
        if ((*it)->property(propertyValue.toStdString().c_str()).toInt() >= valueMin &&
            (*it)->property(propertyValue.toStdString().c_str()).toInt() <= valueMax) {
            if (grid_visible_)
                (*it)->setStyleSheet(sForeBackColor + kBorderStyle);
            else
                (*it)->setStyleSheet(sForeBackColor);
            (*it)->setProperty("forebackcolor", sForeBackColor);
        }
    }*/
}

bool VerticalSpeedWidget::eventFilter(QObject *obj, QEvent *event)
{
    QToolButton *button = qobject_cast<QToolButton *>(obj);
    /*if (!button || !buttons_.contains(button))
        return false;*/

    switch (event->type()) {
    case QEvent::Enter:
        if (grid_visible_)
            button->setStyleSheet(QString(kOrangeBackgroundStyle) + kBorderStyle);
        else
            button->setStyleSheet(kOrangeBackgroundStyle);

        switch (measure_) {
        case FEET:
            emit valueEnter(QString("fm") + button->property("feet").toString());
            break;
        case METERS:
            emit valueEnter(QString("ms") + button->property("meters").toString());
            break;
        default:
            qDebug() << "Unknown measure type! " << __FUNCTION__;
            return false;
        }

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

void VerticalSpeedWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    if (first_show_) {
        int buttonHeight = (*buttons_.begin())->height();

        if (visible_buttons_count_ > 0) {
            int controlsHeight = height() - ui->scrollArea->height();
            resize(width(), buttonHeight * visible_buttons_count_ + controlsHeight);
        }

        foreach (QToolButton * button, buttons_) {
            button->setMaximumWidth(width() - 10);
            button->setMinimumWidth(width() - 10);
            if (1 == button->property("visiblebutton").toInt())
                ui->scrollArea->ensureWidgetVisible(button);
        }

        first_show_ = false;
    }
}

void VerticalSpeedWidget::closeEvent(QCloseEvent *event)
{
    emit rejected();
    hide();
    event->ignore();
}

void VerticalSpeedWidget::toolClicked()
{
    switch (measure_) {
    case FEET:
        emit accepted(QString("fm") + sender()->property("feet").toString());
        break;
    case METERS:
        emit accepted(QString("ms") + sender()->property("meters").toString());
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

    hide();
}

void VerticalSpeedWidget::on_fButton_clicked()
{
    if (FEET == measure_)
        return;

    updateButtons(FEET);
}

void VerticalSpeedWidget::on_mButton_clicked()
{
    if (METERS == measure_)
        return;

    updateButtons(METERS);
}

void VerticalSpeedWidget::on_tbRemove_clicked()
{
    emit accepted(QString(""));
    hide();
}

void VerticalSpeedWidget::on_customValue_editingFinished()
{
    if (!ui->customValue->hasFocus())
        return;

    if (ui->customValue->text().isEmpty())
        return;

    if (ui->customValue->text().length() < 2)
        return;

    switch (measure_) {
    case FEET:
        emit accepted(QString("fm") + ui->customValue->text());
        break;
    case METERS:
        emit accepted(QString("ms") + ui->customValue->text());
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

    hide();
}

void VerticalSpeedWidget::init()
{
    switch (measure_) {
    case METERS:
        ui->mButton->setChecked(true);
        break;
    case FEET:
        ui->fButton->setChecked(true);
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

    createButtons(METERS);
    createButtons(FEET);

    updateButtons(measure_);
}

void VerticalSpeedWidget::createButtons(Measure measure)
{
    QList<set<int> > values = values_by_measure_.values(measure);
    foreach (int value, values[0]) {
        QToolButton *button = new QToolButton(ui->scrollArea->widget());
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        button->setProperty(measure == METERS ? "meters" : "feet", value);
        button->setProperty("forebackcolor", kWhiteBackgroundStyle);
        button->setProperty("visiblebutton", 0);
        button->setAutoRaise(!grid_visible_);
        button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        if (grid_visible_)
            button->setStyleSheet(kWhiteBackgroundStyle + kBorderStyle);
        else
            button->setStyleSheet(kWhiteBackgroundStyle);

        button->setText(QString("%1").arg(value));

        connect(button, SIGNAL(clicked()), this, SLOT(toolClicked()));
        button->installEventFilter(this);
        buttons_.insert(measure, button);

        button->hide();
    }
}

void VerticalSpeedWidget::updateButtons(Measure measure)
{
    foreach (QToolButton *button, buttons_.values(measure_)) {
        grid_layout_->removeWidget(button);
        button->hide();
    }

    measure_ = measure;

    switch (measure_) {
    case METERS:
        ui->mButton->setStyleSheet(kGrayBackgroundStyle);
        ui->fButton->setStyleSheet(kWhiteBackgroundStyle);
        break;
    case FEET:
        ui->fButton->setStyleSheet(kGrayBackgroundStyle);
        ui->mButton->setStyleSheet(kWhiteBackgroundStyle);
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
    }

    int i = 0;
    foreach (QToolButton *button, buttons_.values(measure_)) {
        grid_layout_->addWidget(button, i, 0, Qt::AlignHCenter);
        button->show();
        ++i;
    }
}

void VerticalSpeedWidget::initValues()
{
    TVA_CATCHALL_TRY
    {
        if (values_by_measure_.empty())
            dao()->loadVerticalSpeedValues(values_by_measure_);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}
