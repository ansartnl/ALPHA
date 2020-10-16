#include "FplTelegramWidget.h"

#include "main.h"

#include "aftnmessageprocessor.h"

#include <QString>
#include <QInputDialog>
#include <QMessageBox>

FplTelegramWidget::FplTelegramWidget(AFTNMessageProcessor *p, QWidget *parent) :
    QWidget(parent), mProcessor(p)
{
    setupUi(this);

    selectRoute->setVisible(false);

    setVersion(ParseVersion::n_4444);
    fillCombo();

    invalidValue = "background-color: #FF637F;";

    existDictionary = false;

    connect(selectRoute, SIGNAL(clicked()), SLOT(onSelectRoute()));
    connect(acid, SIGNAL(textChanged(QString)), SLOT(onEditTextACID(QString)));
    connect(aircraftType, SIGNAL(textChanged(QString)), SLOT(onEditTextAircraftType(QString)));
    connect(equipment, SIGNAL(textChanged(QString)), SLOT(onEditTextEquipment(QString)));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdep(QString)));
    connect(route, SIGNAL(textChanged()), SLOT(onEditTextRoute()));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdes(QString)));
    connect(eet, SIGNAL(textChanged(QString)), SLOT(onTextChangedEet(QString)));
    connect(alt1, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAlt1(QString)));
    connect(alt2, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAlt2(QString)));
    connect(other, SIGNAL(textChanged()), SLOT(onEditTextOther()));

#ifndef DUSHANBE_BUILD
    //7
    connect(acid, SIGNAL(textChanged(QString)), SLOT(process7Field()));
    //8
    connect(flightRules, SIGNAL(currentIndexChanged(QString)), SLOT(process8Field()));
    connect(flightType, SIGNAL(currentIndexChanged(QString)), SLOT(process8Field()));
    //9
    connect(numberOfAircrafts, SIGNAL(valueChanged(QString)), SLOT(process9Field()));
    connect(aircraftType, SIGNAL(textChanged(QString)), SLOT(process9Field()));
    connect(wtc, SIGNAL(currentIndexChanged(QString)), SLOT(process9Field()));
    //10
    connect(equipment, SIGNAL(textChanged(QString)), SLOT(process10Field()));
    //13
    connect(adep, SIGNAL(textChanged(QString)), SLOT(process13Field()));
    connect(departureTime, SIGNAL(timeChanged(QTime)), SLOT(process13Field()));
    //15
    connect(speedType, SIGNAL(currentIndexChanged(QString)), SLOT(process15Field()));
    connect(speed, SIGNAL(valueChanged(QString)), SLOT(process15Field()));
    connect(flightLevelType, SIGNAL(currentIndexChanged(QString)), SLOT(process15Field()));
    connect(flightLevel, SIGNAL(valueChanged(QString)), SLOT(process15Field()));
    connect(route, SIGNAL(textChanged()), SLOT(process15Field()));
    //16
    connect(ades, SIGNAL(textChanged(QString)), SLOT(process16Field()));
    connect(eet, SIGNAL(textChanged(QString)), SLOT(process16Field()));
    connect(alt1, SIGNAL(textChanged(QString)), SLOT(process16Field()));
    connect(alt2, SIGNAL(textChanged(QString)), SLOT(process16Field()));
    //18
    connect(other, SIGNAL(textChanged()), SLOT(process18Field()));

    process7Field();
    process8Field();
    process9Field();
    process10Field();
    process13Field();
    process15Field();
    process16Field();
    process18Field();
#endif
}

void FplTelegramWidget::setReadOnly(bool r)
{
    acid->setEnabled(!r);
    flightRules->setEnabled(!r);
    flightType->setEnabled(!r);
    numberOfAircrafts->setEnabled(!r);
    aircraftType->setEnabled(!r);
    wtc->setEnabled(!r);
    equipment->setEnabled(!r);
    adep->setEnabled(!r);
    departureTime->setEnabled(!r);
    speedType->setEnabled(!r);
    speed->setEnabled(!r);
    flightLevelType->setEnabled(!r);
    flightLevel->setEnabled(!r);
    selectRoute->setEnabled(!r);
    route->setEnabled(!r);
    ades->setEnabled(!r);
    eet->setEnabled(!r);
    alt1->setEnabled(!r);
    alt2->setEnabled(!r);
    other->setEnabled(!r);
}

void FplTelegramWidget::setFpl(const FplStruct &f, bool pln)
{
    isPln = pln;
    mFpl = f;

    setVersion(f.version);

    fillCombo();

    acid->setText(f.ACID);
    flightRules->setCurrentIndex(flightRules->findText(f.flightRules));
    flightType->setCurrentIndex(flightType->findText(f.typeOfFlight));
    numberOfAircrafts->setValue(f.numberOfAircrafts);
    aircraftType->setText(f.aircraftType);
    wtc->setCurrentIndex(wtc->findText(f.turbulence));
    equipment->setText(f.equipment);
    adep->setText(f.ADEP);
    departureTime->setTime(f.departureTime.time());
    QString s = f.speed;
    speedType->setCurrentIndex(speedType->findText(s.mid(0, 1)));
    speed->setValue(s.mid(1).toUInt());
    QString fl = f.flightLevel;
    flightLevelType->setCurrentIndex(flightLevelType->findText(fl.mid(0, 1)));
    flightLevel->setValue(fl.mid(1).toUInt());
    route->setText(f.route);
    ades->setText(f.ADES);
    eet->setText(f.dtADES);
    alt1->setText(f.alt1);
    alt2->setText(f.alt2);
    other->setText(f.other);
}

void FplTelegramWidget::setVersion(int v)
{
    version = v;
    if (v == ParseVersion::n_TC95)
    {
        if (isPln)
            telegramType->setText(Ats::PLN1);
        else
            telegramType->setText(Ats::FPL1);
    } else
    {
        telegramType->setText(Ats::FPL);
    }
}

FplStruct FplTelegramWidget::fpl() const
{
    FplStruct ret;

    ret.version = version;
    ret.ACID = acid->text();
    ret.flightRules = flightRules->currentText();
    ret.typeOfFlight = flightType->currentText();
    ret.numberOfAircrafts = numberOfAircrafts->value();
    ret.aircraftType = aircraftType->text();
    ret.turbulence = wtc->currentText();
    ret.equipment = equipment->text();
    ret.ADEP = adep->text();
    ret.departureTime = QDateTime(QDateTime::currentDateTimeUtc().date(), departureTime->time(), Qt::UTC);
    ret.dtADEP = departureTime->time().toString("hh:mm");
    int speedDigitCount = 4;
    if ((speedType->currentText() == "M") || (speedType->currentText() == "М"))
        speedDigitCount = 3;
    ret.speed = QString("%1%2").arg(speedType->currentText())
            .arg(speed->value(), speedDigitCount, 10, QChar('0'));
    int levelDigitCount = 3;
    if ((flightLevelType->currentText() == "S") || (flightLevelType->currentText() == "M")
            || (flightLevelType->currentText() == "С") || (flightLevelType->currentText() == "М"))
        levelDigitCount = 4;
    ret.flightLevel = QString("%1%2").arg(flightLevelType->currentText())
            .arg(flightLevel->value(), levelDigitCount, 10, QChar('0'));
    ret.route = route->toPlainText();
    ret.ADES = ades->text();
    ret.dtADES = eet->text();
    ret.alt1 = alt1->text();
    ret.alt2 = alt2->text();
    ret.other = other->toPlainText();

    return ret;
}

void FplTelegramWidget::fillCombo()
{
    speedType->clear();
    if (version == ParseVersion::n_4444)
    {
        speedType->addItems(QStringList() << "K" << "N" << "M");
    } else if (version == ParseVersion::n_TC95)
    {
        speedType->addItems(QStringList() << "К" << "Н" << "М");
    }

    flightLevelType->clear();
    if (version == ParseVersion::n_4444)
    {
        flightLevelType->addItems(QStringList() << "F" << "S" << "A" << "M");
    } else if (version == ParseVersion::n_TC95)
    {
        flightLevelType->addItems(QStringList() << "Ф" << "С" << "А" << "М");
    }

    flightRules->clear();
    flightRules->addItems(QStringList() << "I" << "V" << "Y" << "Z");
    flightType->clear();
    flightType->addItems(QStringList() << "G" << "M" << "N" << "S" << "X");
    wtc->clear();
    wtc->addItems(QStringList() << "H" << "L" << "M");
}

void FplTelegramWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void FplTelegramWidget::onSelectRoute()
{
    bool ok;
    QString r = QInputDialog::getItem(this, tr("Select route"), tr("Route:")
                                      , mRoutes.routesByAirports(adep->text()
                                                                 , ades->text())
                                      , 0, 0, &ok);
    if (!ok)
        return;
    route->setText(r);
}

bool FplTelegramWidget::process7Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    mProcessor->processType7(fpl().ACID, mMap, errorCodes);
    mProcessor->validateType7(mMap,errorCodes);

    acid->setStyleSheet("");

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first)
                == AFTNMessageProcessor::InvalidField7Format)
        {
#ifndef DUSHANBE_BUILD
            acid->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                acid->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool FplTelegramWidget::process8Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    FplStruct fplStruct;
    fplStruct = fpl();
    QString field8 = fplStruct.flightRules + fplStruct.typeOfFlight;
    mProcessor->processType8(field8, mMap, errorCodes);
    mProcessor->validateType8(mMap,errorCodes);

    flightRules->setStyleSheet("");
    flightType->setStyleSheet("");

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField8Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidFlightRules
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidFlightType)
        {
#ifndef DUSHANBE_BUILD
            flightRules->setStyleSheet(invalidValue);
            flightType->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                flightRules->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool FplTelegramWidget::process9Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    FplStruct fplStruct;
    fplStruct = fpl();
    QString sNum;
    if(fplStruct.numberOfAircrafts > 1)
        sNum = QString::number(fplStruct.numberOfAircrafts);
    QString field9 = sNum
            + fplStruct.aircraftType + "/" + fplStruct.turbulence;
    mProcessor->processType9(field9, mMap, errorCodes);
    mProcessor->validateType9(mMap,errorCodes);

#ifndef DUSHANBE_BUILD
    numberOfAircrafts->setStyleSheet("");
    aircraftType->setStyleSheet("");
    wtc->setStyleSheet("");
#endif

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField9Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidTurbulenceCategory)
        {
#ifndef DUSHANBE_BUILD
            numberOfAircrafts->setStyleSheet(invalidValue);
            aircraftType->setStyleSheet(invalidValue);
            wtc->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                aircraftType->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool FplTelegramWidget::process10Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    FplStruct fplStruct;
    fplStruct = fpl();
    QString field10 = fplStruct.equipment;
    mProcessor->processType10(field10, mMap, errorCodes);
    mProcessor->validateType10(mMap, errorCodes);

    equipment->setStyleSheet("");

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField10Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidEquipment)
        {
#ifndef DUSHANBE_BUILD
            equipment->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                equipment->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool FplTelegramWidget::process13Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    FplStruct fplStruct;
    fplStruct = fpl();
    QString field13 = fplStruct.ADEP + fplStruct.departureTime.toString("hhmm");
    mProcessor->processType13(field13, mMap, errorCodes);
    mProcessor->validateType13(mMap, errorCodes);

    adep->setStyleSheet("");
    departureTime->setStyleSheet("");

    if (adep->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField13Format, QString()));

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField13Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidDepartureTime)
        {
#ifndef DUSHANBE_BUILD
            adep->setStyleSheet(invalidValue);
            departureTime->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                adep->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool FplTelegramWidget::process15Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    FplStruct fplStruct;
    fplStruct = fpl();
    QString field15 = fplStruct.speed + fplStruct.flightLevel
            + " " + fplStruct.route;

    mProcessor->processType15(field15, mMap, errorCodes);
    mProcessor->validateType15(mMap, errorCodes);

#ifndef DUSHANBE_BUILD
    speedType->setStyleSheet("");
    speed->setStyleSheet("");
    flightLevel->setStyleSheet("");
    flightLevelType->setStyleSheet("");
    route->setStyleSheet("");
#endif

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField15Format
                /*|| AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidRoute
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidRouteItem
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidRouteItemSequence
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidRoutePointPair
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::NeededRouteNotExists*/)
        {
#ifndef DUSHANBE_BUILD
            speedType->setStyleSheet(invalidValue);
            speed->setStyleSheet(invalidValue);
            flightLevel->setStyleSheet(invalidValue);
            flightLevelType->setStyleSheet(invalidValue);
            route->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                speed->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool FplTelegramWidget::process16Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    FplStruct fplStruct;
    fplStruct = fpl();
    QString field16 = fplStruct.ADES + removeColonFromTime(fplStruct.dtADES) + " "
            + fplStruct.alt1 + " " + fplStruct.alt2;
    mProcessor->processType16(field16, mMap, errorCodes);
    mProcessor->validateType16(mMap, errorCodes);

#ifndef DUSHANBE_BUILD
    ades->setStyleSheet("");
    eet->setStyleSheet("");
    alt1->setStyleSheet("");
    alt2->setStyleSheet("");
#endif

    if (ades->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField16Format, QString()));

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField16Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidArrivalTime)
        {

#ifndef DUSHANBE_BUILD
            ades->setStyleSheet(invalidValue);
            eet->setStyleSheet(invalidValue);
            alt1->setStyleSheet(invalidValue);
            alt2->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                ades->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool FplTelegramWidget::process18Field(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    FplStruct fplStruct;
    fplStruct = fpl();
    QString field18 = fplStruct.other;
    mProcessor->processType18(field18, mMap, errorCodes);
    mProcessor->validateType18(mMap, errorCodes);

    QTextCharFormat format;
    format.setBackground(QColor());
    QTextCursor cursor;
    cursor.select(QTextCursor::LineUnderCursor);
    QTextEdit::ExtraSelection selection;
    selection.cursor = cursor;
    selection.format = format;
    other->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField18Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidDateOfFLight
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidBorderCrossTime
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidBorderCrossTerm
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidFirEet
                )
        {
            QTextCursor cursor = other->textCursor();

            int numberDof = other->toPlainText().indexOf(QRegExp("DOF"));
            if (numberDof == -1)
                break;

            cursor.setPosition(numberDof,QTextCursor::MoveAnchor);
            cursor.setPosition(numberDof+3,QTextCursor::KeepAnchor);

            QTextCharFormat format;
#ifndef DUSHANBE_BUILD
            format.setBackground(QColor(255, 99, 127));
#endif

            QTextEdit::ExtraSelection selection;
            selection.cursor = cursor;
            selection.format = format;
            other->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

            if (showMessageBox)
            {
                other->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

void FplTelegramWidget::onEditTextChangeAdep(const QString &s)
{
    int pos = adep->cursorPosition();
    adep->setText(s.toUpper());
    adep->setCursorPosition(pos);
}

void FplTelegramWidget::onEditTextChangeAdes(const QString &s)
{
    int pos = ades->cursorPosition();
    ades->setText(s.toUpper());
    ades->setCursorPosition(pos);
}

void FplTelegramWidget::onEditTextChangeAlt1(const QString &s)
{
    int pos = alt1->cursorPosition();
    alt1->setText(s.toUpper());
    alt1->setCursorPosition(pos);
}

void FplTelegramWidget::onEditTextChangeAlt2(const QString &s)
{
    int pos = alt2->cursorPosition();
    alt2->setText(s.toUpper());
    alt2->setCursorPosition(pos);
}
void FplTelegramWidget::onEditTextEquipment(const QString &s)
{
    int pos = equipment->cursorPosition();
    equipment->setText(s.toUpper());
    equipment->setCursorPosition(pos);
}
void FplTelegramWidget::onEditTextACID(const QString &s)
{
    int pos = acid->cursorPosition();
    acid->setText(s.toUpper());
    acid->setCursorPosition(pos);
}
void FplTelegramWidget::onEditTextAircraftType(const QString &s)
{
    int pos = aircraftType->cursorPosition();
    aircraftType->setText(s.toUpper());
    aircraftType->setCursorPosition(pos);
}
void FplTelegramWidget::onEditTextRoute()
{
    QString s = route->toPlainText();
    if (s == s.toUpper())
        return;
    int pos = route->textCursor().position();
    route->setText(s.toUpper());
    QTextCursor cursor = route->textCursor();
    cursor.setPosition(pos);
    route->setTextCursor(cursor);
}

void FplTelegramWidget::onEditTextOther()
{
    QString s = other->toPlainText();
    if (s == s.toUpper())
        return;
    int pos = other->textCursor().position();
    other->setText(s.toUpper());
    QTextCursor cursor = other->textCursor();
    cursor.setPosition(pos);
    other->setTextCursor(cursor);
}

void FplTelegramWidget::onTextChangedEet(const QString &s)
{
    int pos = eet->cursorPosition();
    if ((s.right(s.indexOf(":"))).toInt() > 59)
        eet->setText(s.left(s.indexOf(":")) + ":00");
    eet->setCursorPosition(pos);
}

bool FplTelegramWidget::validate()
{
    if(process7Field(true) &&
        process8Field(true) &&
        process9Field(true) &&
        process10Field(true) &&
        process13Field(true) &&
        process15Field(true) &&
        process16Field(true) &&
        process18Field(true))
            return true;
    return false;
}
