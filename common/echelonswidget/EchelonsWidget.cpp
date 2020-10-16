#include "EchelonsWidget.h"

#include <QtDebug>

#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

#include <QGridLayout>
#include <QToolButton>
#include <QScrollBar>

EchelonsWidget::EchelonsWidget(Measure measure, int levelMetersMax, const QSqlDatabase& database, QWidget *parent,
    Qt::WindowFlags flags) :
    QFrame(parent, flags), model(0), mValue(0), mMeasure(measure), firstShow(true)
{
    setupUi(this);

    setFrameShape(QFrame::Box);

    if (parent) // fix Qt 5.10 crash
        setAttribute(Qt::WA_WindowPropagation);

    glayout = new QGridLayout(scrollArea->widget());

    glayout->setSpacing(0);
    glayout->setMargin(0);

    model = new QSqlQueryModel(this);

    mDatabase = database;
    mlevelMetersMax = levelMetersMax;
    mFirstMeasure = measure;

    if (measure == Meters)
        mButton->setChecked(true);
    else
        fButton->setChecked(true);

    landComboBox->hide();

    init(measure);

    //customValue->selectAll();
    QMetaObject::invokeMethod(customValue, "selectAll", Qt::QueuedConnection);
}

EchelonsWidget::~EchelonsWidget()
{
}

void EchelonsWidget::init(Measure measure)
{
    QSqlQuery query(mDatabase);
    mMeasure = measure;
//    if (mMeasure == Meters)
//    {
//        query.prepare("select distinct azimuth, meters, round(meters/"+QString::number(METER_DIVIDER)+") as rounded_meters"
//            " from echelons where vfr_ifr = 1 and meters <= ? and meters is not null order by "
//            " rounded_meters desc");
//    }
//    else if (mMeasure == Feet)
    {
        query.prepare("select echelon, azimuth, meters, feet, round(meters/"+QString::number(METER_DIVIDER)+") as rounded_meters"
            " from echelons where vfr_ifr = 1 and meters <= ? order by "
            " echelon desc");
    }
    query.addBindValue(QVariant(mlevelMetersMax));
    if (!query.exec())
    {
        qWarning() << query.lastError().text();
    }

    model->setQuery(query);

    qDeleteAll(buttons);
    buttons.clear();

    QToolButton *toolButton;

    int last = 0;
    int k = 0;
    for (int i = 0; i < model->rowCount(); ++i)
    {
        int column = 0;
        if (model->record(i).value("azimuth").toInt() == 179)
        {
            column = 0;
            if (last == 179)
            {
                ++k;
            }
            last = 179;
        }
        else if (model->record(i).value("azimuth").toInt() == 359)
        {
            column = 1;
            if (last == 359)
            {
                ++k;
            }
            last = 359;
        }
        else
        {
            last = 0;
            continue;
        }
        toolButton = new QToolButton(scrollArea->widget());

        if (mMeasure == Feet)
        {
            toolButton->setText(QString("%1").arg(model->record(i).value("echelon").toInt(), 3, 'f', 0, QChar('0')));
        }
        else if (mMeasure == Meters)
        {
            toolButton->setText(QString("%1").arg(model->record(i).value("rounded_meters").toInt(), METER_CAPACITY, 'f', 0, QChar('0')));
        }

        toolButton->setProperty("azimuth", model->record(i).value("azimuth"));
        toolButton->setProperty("echelon", model->record(i).value("echelon"));
        toolButton->setProperty("meters", model->record(i).value("meters"));
        toolButton->setProperty("feet", model->record(i).value("feet"));
        toolButton->setProperty("rounded_meters", model->record(i).value("rounded_meters"));
        toolButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
        connect(toolButton, SIGNAL(clicked()), this, SLOT(toolClicked()));

        glayout->addWidget(toolButton, i + k, column, 2, 1);
        buttons.append(toolButton);
    }
}

void EchelonsWidget::changeEvent(QEvent* event)
{
    //LanguageChange events should not be confused with LocaleChange events.
    if (event->type() == QEvent::LocaleChange)
    {
    }
    else if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }

    QFrame::changeEvent(event);
}

void EchelonsWidget::showEvent(QShowEvent* event)
{
    QFrame::showEvent(event);
    if (firstShow)
    {
        setValue(mValue);
        firstShow = false;
    }
}

void EchelonsWidget::exec(const QPoint& position, int value)
{
    move(position);
    setValue(value);
    show();
}

double EchelonsWidget::value() const
{
    return mValue;
}

void EchelonsWidget::setValue(double value)
{
    if (mMeasure == Meters)
    {
        value = qRound(value);
        foreach (QToolButton *child, findChildren<QToolButton*>())
            {
                if (qFuzzyCompare(child->property("meters").toDouble(), value))
                {
                    scrollArea->ensureWidgetVisible(child);
                    QPalette pal(child->palette());
                    pal.setBrush(QPalette::ButtonText, QBrush(Qt::blue));
                    child->setPalette(pal);
                    child->setFocus();
                }
            }
        customValue->setValue(qRound(value / METER_DIVIDER));
    }
    else if (mMeasure == Feet)
    {
        value = qRound(value);
        foreach (QToolButton *child, findChildren<QToolButton*>())
            {
                if (qFuzzyCompare(child->property("echelon").toDouble(), value))
                {
                    scrollArea->ensureWidgetVisible(child);
                    QPalette pal(child->palette());
                    pal.setBrush(QPalette::ButtonText, QBrush(Qt::blue));
                    child->setPalette(pal);
                    child->setFocus();
                }
            }
        customValue->setValue(value);
    }

    mValue = value;
}

void EchelonsWidget::setValue(QString sValue)
{
    if(!sValue.isEmpty())
    {
        Measure tempMeasure = mMeasure;
        quint32 uiResult = 0;
        bool bOk(false);
        uiResult = sValue.toUInt(&bOk);
        if(!bOk)
        {
            uiResult = sValue.right(sValue.size()-1).toUInt(&bOk);
            if(bOk)
            {
                if (sValue[0] == 'F')
                {
                    // feets
                    tempMeasure = Feet;
                }
                else if (sValue[0] == 'S')
                {
                    // meters
                    tempMeasure = Meters;
                    uiResult *= 10;
                }
            }
        }
        if(tempMeasure != mMeasure)
        {
            if(tempMeasure == Feet)
                fButton->click();
            else
                mButton->click();
        }
        setValue(double(uiResult));
    }
}

void EchelonsWidget::setLandValue(QString sValue, QStringList sList)
{
    landComboBox->clear();
    landComboBox->addItems(sList);
    int ind = landComboBox->findText(sValue);
    if(ind == -1)
        ind = 0;
    if(ind < landComboBox->count())
        landComboBox->setCurrentIndex(ind);

    if(landComboBox->count())
        landComboBox->show();
    else
        landComboBox->hide();
}

QSize EchelonsWidget::sizeHint() const
{
    return QFrame::sizeHint() + QSize(30, 30);
}

bool EchelonsWidget::isRemainder() const
{
    return reminder->isChecked();
}

void EchelonsWidget::setRemainderShown(bool shown)
{
    reminder->setVisible(shown);
}

void EchelonsWidget::toolClicked()
{
    /*Pasha
    if (mFirstMeasure == Meters)
    {
        mValue = sender()->property("meters").toDouble();
    }
    else if (mFirstMeasure == Feet)
    {
        mValue = sender()->property("echelon").toDouble();
    }

    emit
    valueChanged(sender()->property("echelon").toDouble(), sender()->property("meters").toDouble(), mFirstMeasure);
    */
    double valF(0), valM(0);
    if (mMeasure == Meters)
    {
        mValue = sender()->property("meters").toDouble();
        valM = mValue;
        valF = valM / 30.48;
    }
    else if (mMeasure == Feet)
    {
        mValue = sender()->property("echelon").toDouble();
        valF = mValue;
        valM = valF * 30.48;
    }

#ifdef CWP_PRJ
    emit
    valueChanged(sender()->property("echelon").toDouble(), sender()->property("meters").toDouble(), mMeasure);
#else
    emit
    valueChanged(valF, valM, mMeasure);
#endif

    close();
}

void EchelonsWidget::on_fButton_clicked(bool checked)
{
    if (checked)
        init(Feet);
}

void EchelonsWidget::on_mButton_clicked(bool checked)
{
    if (checked)
        init(Meters);
}

void EchelonsWidget::on_setCustom_clicked()
{
    double valF(0), valM(0);
    if (mMeasure == Meters)
    {
        valF = customValue->value()*METER_DIVIDER / 30.48;
        valM = customValue->value()*METER_DIVIDER;
    }
    else if (mMeasure == Feet)
    {
        valF = customValue->value();
        valM = customValue->value() * 30.48;
    }
#ifdef CWP_PRJ
    foreach (QToolButton *child, findChildren<QToolButton*>())
    {
        if (mMeasure == Meters)
        {
            if (qFuzzyCompare(child->property("meters").toDouble(), valM))
            {
                valF = child->property("echelon").toDouble();
            }
        }
        else if (mMeasure == Feet)
        {
            if (qFuzzyCompare(child->property("echelon").toDouble(), valF))
            {
                valM = child->property("meters").toDouble();
            }
        }
    }
#endif
    if (mMeasure == Meters)
    {
        mValue = valM;
    }
    else if (mMeasure == Feet)
    {
        mValue = valF;
    }

    emit
    valueChanged(valF, valM, mMeasure);
    close();
}

void EchelonsWidget::setVisibleButtons(bool bVisible)
{
//    if(fButton->isVisible() != bVisible)
    {
        fButton->setVisible(bVisible);
        mButton->setVisible(bVisible);
    }
}

EchelonsWidget::Measure EchelonsWidget::measure() const
{
    return mMeasure;
}

void EchelonsWidget::on_landButton_clicked()
{
    QString sValue = landComboBox->currentText();
    if(!sValue.isEmpty())
    {
        emit valueLandChanged(sValue);
    }
    else
    {
        mMeasure = Feet;
        mValue = 0;
        emit valueChanged(mValue, mValue, mMeasure);
    }
    close();
}
