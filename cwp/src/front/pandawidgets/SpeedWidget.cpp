#include "SpeedWidget.h"
#include "ui_SpeedWidget.h"

#include <algorithm>
#include <cmath>

#include <QGridLayout>
#include <QToolButton>
#include <QScrollBar>
#include <QCloseEvent>
#include <QDebug>

#include "WidgetConsts.h"

#include "main.h"
#include "Dao.h"

using std::set;

QHash<int, set<float> > SpeedWidget::values_by_measure_;

SpeedWidget::SpeedWidget(const QString &acid, const QString &init_value, QWidget *parent, Qt::WindowFlags flags)
    : QFrame(parent, flags)
    , ui(new Ui::SpeedWidget)
    , measure_(KMPH)
    , first_show_(true)
    , visible_buttons_count_(0)
    , grid_visible_(true)
{
    ui->setupUi(this);
    ui->kmButton->hide();

    ui->labelACID->setText(acid);

    knot_validator_ = new QRegExpValidator(QRegExp("\\d{1,3}"), this);
    mach_validator_ = new QRegExpValidator(QRegExp("0[\\.|,]\\d{1,2}"), this);

    grid_layout_ = new QGridLayout(ui->scrollArea->widget());
    grid_layout_->setSpacing(0);
    grid_layout_->setMargin(0);

    initValues();
    init();
    setVisibleValue(init_value);
    setValueColor(init_value, Qt::black, Qt::yellow);

    setFrameShape(QFrame::Box);

    if (parent) // fix Qt 5.10 crash
        setAttribute(Qt::WA_WindowPropagation);
}

void SpeedWidget::setGrid(bool grid_visible)
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

bool SpeedWidget::setCountVisibleButtons(int count)
{
    if (count < 0)
        return false;

    visible_buttons_count_ = count;
    return true;
}

void SpeedWidget::setACID(const QString &acid)
{
    ui->labelACID->setText(acid);
}

bool SpeedWidget::setVisibleValue(const QString &sValue)
{
    QString visible_value = sValue.trimmed();
    if (visible_value.isEmpty())
        return false;

    Measure temp_measure = KNOT;
    float fValue = 0;
    bool bOk = valueFromString(visible_value, temp_measure, fValue);

    //if (!bOk)
    //    return false;

    if (temp_measure != measure_) {
        if (KNOT == temp_measure)
            ui->ktButton->click();
        else if (MACH == temp_measure)
            ui->mButton->click();
        else
            ui->kmButton->click();
    }

    if (!bOk)
       return false;//
    QString property_value;

    switch (measure_) {
    case MACH:
        property_value = "mach";
        break;
    case KNOT:
        property_value = "knot";
        break;
    case KMPH:
        property_value = "kmph";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return false;
    }
    
    bool buttonFound = false;
    foreach (QToolButton * button, buttons_) {
        if (qFuzzyCompare(button->property(property_value.toStdString().c_str()).toFloat(), fValue)) {
            ui->scrollArea->ensureWidgetVisible(button);
            button->setProperty("visiblebutton", 1);
            buttonFound = true;
        } else {
            button->setProperty("visiblebutton", 0);
        }
    }

    if (!buttonFound)
        ui->customValue->setText(QString("%1").arg(fValue));
    else
        ui->customValue->clear();

    return true;
}

bool SpeedWidget::setValueColor(const QString &sValue, const QColor &foregroundColor, const QColor &backgroundColor)
{
    QString color_value = sValue.trimmed();
    if (color_value.isEmpty())
        return false;

    Measure tempMeasure = KNOT;// MACH;
    float fValue = 0;
    bool bOk = valueFromString(color_value, tempMeasure, fValue);

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

    switch (tempMeasure) {
    case MACH:
        property_value = "mach";
        break;
    case KNOT:
        property_value = "knot";
        break;
    case KMPH:
        property_value = "kmph";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return false;
    }

    foreach (QToolButton *button, buttons_) {
        if (qFuzzyCompare(button->property(property_value.toStdString().c_str()).toFloat(), fValue)) {
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

void SpeedWidget::setRangeColor(float valueMin, float valueMax, Measure measure, const QColor &foregroundColor, const QColor &backgroundColor)
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
    case MACH:
        propertyValue = "mach";
        break;
    case KNOT:
        propertyValue = "knot";
        break;
    case KMPH:
        propertyValue = "kmph";
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

    /*QSet<QToolButton *>::const_iterator end_it = buttons_.constEnd();
    for (QSet<QToolButton *>::const_iterator it = buttons_.constBegin(); it != end_it; ++it) {
        if ((*it)->property(propertyValue.toStdString().c_str()).toFloat() >= valueMin &&
            (*it)->property(propertyValue.toStdString().c_str()).toFloat() <= valueMax) {
            if (grid_visible_)
                (*it)->setStyleSheet(sForeBackColor + kBorderStyle);
            else
                (*it)->setStyleSheet(sForeBackColor);
            (*it)->setProperty("forebackcolor", sForeBackColor);
        }
    }*/
}

void SpeedWidget::closeEvent(QCloseEvent *event)
{
    emit rejected();
    hide();
    event->ignore();
}

bool SpeedWidget::eventFilter(QObject *obj, QEvent *event)
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
        case MACH:
            emit valueEnter(machToString(button->property("mach").toString()));
            break;
        case KNOT:
            emit valueEnter(knotToString(button->property("knot").toString()));
            break;
        case KMPH:
            emit valueEnter(QString("Km") + button->property("kmph").toString());
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

void SpeedWidget::showEvent(QShowEvent *event)
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

void SpeedWidget::toolClicked()
{
    switch (measure_) {
    case MACH:
        emit accepted(machToString(sender()->property("mach").toString()));
        break;
    case KNOT:
        emit accepted(knotToString(sender()->property("knot").toString()));
        break;
    case KMPH:
        emit accepted(QString("Km") + sender()->property("kmph").toString());
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

    hide();
}

void SpeedWidget::on_kmButton_clicked()
{
    if (KMPH == measure_)
        return;

    updateButtons(KMPH);
}

void SpeedWidget::on_ktButton_clicked()
{
    if (KNOT == measure_)
        return;

    updateButtons(KNOT);
}

void SpeedWidget::on_mButton_clicked()
{
    if (MACH == measure_)
        return;

    updateButtons(MACH);
}

void SpeedWidget::on_tbRemove_clicked()
{
    emit accepted(QString(""));
    hide();
}

void SpeedWidget::on_customValue_editingFinished()
{
    if (!ui->customValue->hasFocus())
        return;

    if (ui->customValue->text().isEmpty())
        return;

    if (ui->customValue->text().length() < 3)
        return;

    switch (measure_) {
    case MACH:
        emit accepted(machToString(ui->customValue->text()));
        break;
    case KNOT:
        emit accepted(knotToString(ui->customValue->text()));
        break;
    case KMPH:
        emit accepted(QString("Km") + ui->customValue->text());
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

    hide();
}

void SpeedWidget::init()
{
    switch (measure_) {
    case MACH:
        ui->mButton->setChecked(true);
        break;
    case KNOT:
        ui->ktButton->setChecked(true);
        break;
    case KMPH:
        ui->kmButton->setChecked(true);
        break;
    default:
        qDebug() << "Unknown measure type! " << __FUNCTION__;
        return;
    }

    createButtons(KMPH);
    createButtons(MACH);
    createButtons(KNOT);

    updateButtons(measure_);
}

void SpeedWidget::createButtons(Measure measure)
{
    QList<set<float> > values = values_by_measure_.values(measure);
    foreach (float value, values[0]) {
        QToolButton *button = new QToolButton(ui->scrollArea->widget());
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        switch (measure) {
        case MACH:
            button->setProperty("mach", QString("%1").arg(value, 3, 'f', 2, QChar('0')));
            break;
        case KNOT:
            button->setProperty("knot", QString("%1").arg(value, 3, 'f', 2, QChar('0')));
            break;
        case KMPH:
            button->setProperty("kmph", QString("%1").arg(value, 3, 'f', 2, QChar('0')));
            break;
        default:
            qDebug() << "Unknown measure type! " << __FUNCTION__;
            return;
        }
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

void SpeedWidget::updateButtons(Measure measure)
{
    foreach (QToolButton *button, buttons_.values(measure_)) {
        grid_layout_->removeWidget(button);
        button->hide();
    }

    measure_ = measure;

    switch (measure_) {
    case MACH:
        ui->customValue->setValidator(mach_validator_);
        ui->mButton->setStyleSheet(kGrayBackgroundStyle);
        ui->kmButton->setStyleSheet(kWhiteBackgroundStyle);
        ui->ktButton->setStyleSheet(kWhiteBackgroundStyle);
        break;
    case KNOT:
        ui->customValue->setValidator(knot_validator_);
        ui->ktButton->setStyleSheet(kGrayBackgroundStyle);
        ui->kmButton->setStyleSheet(kWhiteBackgroundStyle);
        ui->mButton->setStyleSheet(kWhiteBackgroundStyle);
        break;
    case KMPH:
        ui->kmButton->setStyleSheet(kGrayBackgroundStyle);
        ui->ktButton->setStyleSheet(kWhiteBackgroundStyle);
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

void SpeedWidget::initValues()
{
    TVA_CATCHALL_TRY
    {
        if (values_by_measure_.empty())
            dao()->loadSpeedValues(values_by_measure_);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

bool SpeedWidget::valueFromString(const QString &input, Measure &measure, float &value)
{
    bool ok = false;

    if (input.startsWith("K")) {
        measure = KNOT;
        value = input.mid(1, 2).toFloat(&ok) * 10;
    } else if (input.startsWith("M")) {
        measure = MACH;
        value = input.mid(1, 2).toFloat(&ok) * 0.01;
    } else if (input.startsWith("Km")) {
        measure = KMPH;
        value = input.right(input.size() - 2).toFloat(&ok);
    }

    return ok;
}

QString SpeedWidget::knotToString(const QString &value)
{
    int number = qRound(value.toDouble() / 10);
    return QString("K%1").arg(number, 2, 10, QChar('0'));
}

QString SpeedWidget::machToString(const QString &value)
{
    double i;
    int number = qRound(modf(value.toDouble(), &i) * 100);
    return QString("M%1").arg(number, 2, 10, QChar('0'));
}
