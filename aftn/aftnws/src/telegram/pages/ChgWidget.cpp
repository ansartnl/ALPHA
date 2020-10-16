#include "ChgWidget.h"

#include "aftnmessageprocessor.h"
#include "main.h"

#include <QtGui/QInputDialog>
#include <QMessageBox>

ChgWidget::ChgWidget(AFTNMessageProcessor *p, QWidget *parent) :
    QWidget(parent), mProcessor(p)
{
    setupUi(this);

    selectRoute->setVisible(false);

    setVersion(ParseVersion::n_4444);
    fillCombo();

    setReadOnlyField(true);

    invalidValue = "background-color: #FF637F;";

    connect(field8CheckBox, SIGNAL(toggled(bool)), SLOT(onField8CheckBox(bool)));
    connect(field9CheckBox, SIGNAL(toggled(bool)), SLOT(onField9CheckBox(bool)));
    connect(field10CheckBox, SIGNAL(toggled(bool)), SLOT(onField10CheckBox(bool)));
    connect(field13CheckBox, SIGNAL(toggled(bool)), SLOT(onField13CheckBox(bool)));
    connect(field15CheckBox, SIGNAL(toggled(bool)), SLOT(onField15CheckBox(bool)));
    connect(field16CheckBox, SIGNAL(toggled(bool)), SLOT(onField16CheckBox(bool)));
    connect(field18CheckBox, SIGNAL(toggled(bool)), SLOT(onField18CheckBox(bool)));

    connect(aircraftType, SIGNAL(textChanged(QString)), SLOT(onEditTextAircraftType(QString)));
    connect(equipment, SIGNAL(textChanged(QString)), SLOT(onEditTextEquipment(QString)));
    connect(changedAdep, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdep(QString)));
    connect(route, SIGNAL(textChanged()), SLOT(onEditTextRoute()));
    connect(changedAdes, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAdes(QString)));
    connect(eet, SIGNAL(textChanged(QString)), SLOT(onTextChangedEet(QString)));
    connect(alt1, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAlt1(QString)));
    connect(alt2, SIGNAL(textChanged(QString)), SLOT(onEditTextChangeAlt2(QString)));
    connect(other, SIGNAL(textChanged()), SLOT(onEditTextOther()));
    connect(acid, SIGNAL(textChanged(QString)), SLOT(onEditTextACID(QString)));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(onEditTextAdep(QString)));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(onEditTextAdes(QString)));

#ifndef DUSHANBE_BUILD
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
    connect(changedAdep, SIGNAL(textChanged(QString)), SLOT(process13Field()));
    connect(departureTime, SIGNAL(timeChanged(QTime)), SLOT(process13Field()));
    //15
    connect(speedType, SIGNAL(currentIndexChanged(QString)), SLOT(process15Field()));
    connect(speed, SIGNAL(valueChanged(QString)), SLOT(process15Field()));
    connect(flightLevelType, SIGNAL(currentIndexChanged(QString)), SLOT(process15Field()));
    connect(flightLevel, SIGNAL(valueChanged(QString)), SLOT(process15Field()));
    connect(route, SIGNAL(textChanged()), SLOT(process15Field()));
    //16
    connect(changedAdes, SIGNAL(textChanged(QString)), SLOT(process16Field()));
    connect(eet, SIGNAL(editingFinished()), SLOT(process16Field()));
    connect(alt1, SIGNAL(textChanged(QString)), SLOT(process16Field()));
    connect(alt2, SIGNAL(textChanged(QString)), SLOT(process16Field()));
    //18
    connect(other, SIGNAL(textChanged()), SLOT(process18Field()));

    connect(acid, SIGNAL(textChanged(QString)), SLOT(process7FieldH()));
    connect(adep, SIGNAL(textChanged(QString)), SLOT(process13FieldH()));
    connect(ades, SIGNAL(textChanged(QString)), SLOT(process16FieldH()));

    process7FieldH();
    process13FieldH();
    process16FieldH();
#endif
}

void ChgWidget::setReadOnly(bool r)
{
    setReadOnlyField(r);

    acid->setEnabled(!r);
    adep->setEnabled(!r);
    ades->setEnabled(!r);

    field8CheckBox->setEnabled(!r);
    field9CheckBox->setEnabled(!r);
    field10CheckBox->setEnabled(!r);
    field13CheckBox->setEnabled(!r);
    field15CheckBox->setEnabled(!r);
    field16CheckBox->setEnabled(!r);
    field18CheckBox->setEnabled(!r);
}

void ChgWidget::setReadOnlyField(bool r)
{
    changedAdep->setEnabled(!r);
    changedAdes->setEnabled(!r);

    flightRules->setEnabled(!r);
    flightType->setEnabled(!r);
    numberOfAircrafts->setEnabled(!r);
    aircraftType->setEnabled(!r);
    wtc->setEnabled(!r);
    equipment->setEnabled(!r);
    departureTime->setEnabled(!r);
    speedType->setEnabled(!r);
    speed->setEnabled(!r);
    flightLevelType->setEnabled(!r);
    flightLevel->setEnabled(!r);
    selectRoute->setEnabled(!r);
    route->setEnabled(!r);
    eet->setEnabled(!r);
    alt1->setEnabled(!r);
    alt2->setEnabled(!r);
    other->setEnabled(!r);
}

void ChgWidget::setChg(const ChgStruct &c)
{
    setVersion(c.version);

    fillCombo();

    acid->setText(c.ACID);
    adep->setText(c.ADEP);
    ades->setText(c.ADES);

    if (c.changesFlags.testFlag(ChgStruct::FLIGHT_RULES)
            || c.changesFlags.testFlag(ChgStruct::TYPE_OF_FLIGHT))
    {
        field8CheckBox->setChecked(true);
        flightRules->setCurrentIndex(flightRules->findText(c.flightRules));
        flightType->setCurrentIndex(flightRules->findText(c.typeOfFlight));
    }
    if (c.changesFlags.testFlag(ChgStruct::NUMBER_OF_AIRCRAFTS)
            || c.changesFlags.testFlag(ChgStruct::AIRCRAFT_TYPE)
            || c.changesFlags.testFlag(ChgStruct::TURBULENCE))
    {
        field9CheckBox->setChecked(true);
        numberOfAircrafts->setValue(c.numberOfAircrafts);
        aircraftType->setText(c.aircraftType);
        wtc->setCurrentIndex(wtc->findText(c.turbulence));
    }
    if (c.changesFlags.testFlag(ChgStruct::EQUIPMENT))
    {
        field10CheckBox->setChecked(true);
        equipment->setText(c.equipment);
    }
    if (c.changesFlags.testFlag(ChgStruct::CHANGE_ADEP)
            || c.changesFlags.testFlag(ChgStruct::DEPARTURE_TIME))
    {
        field13CheckBox->setChecked(true);
        changedAdep->setText(c.changedADEP);
        departureTime->setTime(c.departureTime.time());
    }
    if (c.changesFlags.testFlag(ChgStruct::SPEED)
            || c.changesFlags.testFlag(ChgStruct::FLIGHT_LEVEL)
            || c.changesFlags.testFlag(ChgStruct::ROUTE))
    {
        field15CheckBox->setChecked(true);
        QString s = c.speed;
        speedType->setCurrentIndex(speedType->findText(s.mid(0, 1)));
        speed->setValue(s.mid(1).toUInt());
        QString fl = c.flightLevel;
        flightLevelType->setCurrentIndex(speedType->findText(fl.mid(0, 1)));
        flightLevel->setValue(fl.mid(1).toUInt());
        route->setText(c.route);
    }
    if (c.changesFlags.testFlag(ChgStruct::CHANGE_ADES)
            || c.changesFlags.testFlag(ChgStruct::EET)
            || c.changesFlags.testFlag(ChgStruct::ALT1)
            || c.changesFlags.testFlag(ChgStruct::ALT2))
    {
        field16CheckBox->setChecked(true);
        changedAdes->setText(c.changedADES);
        eet->setText(c.dtADES);
        alt1->setText(c.alt1);
        alt2->setText(c.alt2);
    }
    if (c.changesFlags.testFlag(ChgStruct::OTHER))
    {
        field18CheckBox->setChecked(true);
        other->setText(c.other);
    }
}

void ChgWidget::setVersion(int v)
{
    version = v;
    if (v == ParseVersion::n_TC95)
        telegramType->setText(Ats::CHG1);
    else
        telegramType->setText(Ats::CHG);
}

ChgStruct ChgWidget::chg() const
{
    ChgStruct ret;

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
    ret.changedADEP = changedAdep->text();
    ret.changedADES = changedAdes->text();

    ret.changesFlags = 0;

    if (field8CheckBox->isChecked())
    {
        ret.changesFlags |= ChgStruct::FLIGHT_RULES;
        ret.changesFlags |= ChgStruct::TYPE_OF_FLIGHT;
    }
    if (field9CheckBox->isChecked())
    {
        ret.changesFlags |= ChgStruct::NUMBER_OF_AIRCRAFTS;
        ret.changesFlags |= ChgStruct::AIRCRAFT_TYPE;
        ret.changesFlags |= ChgStruct::TURBULENCE;
    }
    if (field10CheckBox->isChecked())
    {
        ret.changesFlags |= ChgStruct::EQUIPMENT;
    }
    if (field13CheckBox->isChecked())
    {
        ret.changesFlags |= ChgStruct::CHANGE_ADEP;
        ret.changesFlags |= ChgStruct::DEPARTURE_TIME;
    }
    if (field15CheckBox->isChecked())
    {
        ret.changesFlags |= ChgStruct::SPEED;
        ret.changesFlags |= ChgStruct::FLIGHT_LEVEL;
        ret.changesFlags |= ChgStruct::ROUTE;
    }
    if (field16CheckBox->isChecked())
    {
        ret.changesFlags |= ChgStruct::CHANGE_ADES;
        ret.changesFlags |= ChgStruct::EET;
        ret.changesFlags |= ChgStruct::ALT1;
        ret.changesFlags |= ChgStruct::ALT2;
    }
    if (field18CheckBox->isChecked())
    {
        ret.changesFlags |= ChgStruct::OTHER;
    }

    return ret;
}

void ChgWidget::fillCombo()
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

void ChgWidget::changeEvent(QEvent *e)
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

void ChgWidget::onSelectRoute()
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

void ChgWidget::onField8CheckBox(bool r)
{
    flightRules->setEnabled(r);
    flightType->setEnabled(r);
#ifndef DUSHANBE_BUILD
    if (r)
    {
        process8Field();
    } else
    {
        flightRules->setStyleSheet("");
        flightType->setStyleSheet("");
    }
#endif
}
void ChgWidget::onField9CheckBox(bool r)
{
    numberOfAircrafts->setEnabled(r);
    aircraftType->setEnabled(r);
    wtc->setEnabled(r);
#ifndef DUSHANBE_BUILD
    if (r)
    {
        process9Field();
    } else
    {
        numberOfAircrafts->setStyleSheet("");
        aircraftType->setStyleSheet("");
        wtc->setStyleSheet("");
    }
#endif
}
void ChgWidget::onField10CheckBox(bool r)
{
    equipment->setEnabled(r);
#ifndef DUSHANBE_BUILD
    if (r)
    {
        process10Field();
    } else
    {
        equipment->setStyleSheet("");
    }
#endif
}
void ChgWidget::onField13CheckBox(bool r)
{
    changedAdep->setEnabled(r);
    departureTime->setEnabled(r);
#ifndef DUSHANBE_BUILD
    if (r)
    {
        process13Field();
    } else
    {
        changedAdep->setStyleSheet("");
        departureTime->setStyleSheet("");
    }
#endif
}
void ChgWidget::onField15CheckBox(bool r)
{
    speedType->setEnabled(r);
    speed->setEnabled(r);
    flightLevelType->setEnabled(r);
    flightLevel->setEnabled(r);
    selectRoute->setEnabled(r);
    route->setEnabled(r);
#ifndef DUSHANBE_BUILD
    if (r)
    {
        process15Field();
    } else
    {
        speedType->setStyleSheet("");
        speed->setStyleSheet("");
        flightLevelType->setStyleSheet("");
        flightLevel->setStyleSheet("");
        route->setStyleSheet("");
    }
#endif
}
void ChgWidget::onField16CheckBox(bool r)
{
    changedAdes->setEnabled(r);
    eet->setEnabled(r);
    alt1->setEnabled(r);
    alt2->setEnabled(r);
#ifndef DUSHANBE_BUILD
    if (r)
    {
        process16Field();
    } else
    {
        changedAdes->setStyleSheet("");
        eet->setStyleSheet("");
        alt1->setStyleSheet("");
        alt2->setStyleSheet("");
    }
#endif
}
void ChgWidget::onField18CheckBox(bool r)
{
    other->setEnabled(r);
#ifndef DUSHANBE_BUILD
    if (r)
    {
        process18Field();
    } else
    {
        other->setStyleSheet("");
    }
#endif
}

bool ChgWidget::process7FieldH(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    mProcessor->processType7(chg().ACID, mMap, errorCodes);
    mProcessor->validateType7(mMap, errorCodes);

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

bool ChgWidget::process13FieldH(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field13 = chgStruct.ADEP;
    mProcessor->processType13(field13, mMap, errorCodes);
    mProcessor->validateType13(mMap, errorCodes);

    adep->setStyleSheet("");

    if (adep->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField13Format, QString()));

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField13Format)
        {
#ifndef DUSHANBE_BUILD
            adep->setStyleSheet(invalidValue);
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

bool ChgWidget::process16FieldH(bool showMessageBox)
{
    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field16 = chgStruct.ADES;
    mProcessor->processType16(field16, mMap, errorCodes);
    mProcessor->validateType16(mMap, errorCodes);

    ades->setStyleSheet("");

    if (ades->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField16Format, QString()));

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField16Format)
        {
#ifndef DUSHANBE_BUILD
            ades->setStyleSheet(invalidValue);
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

bool ChgWidget::process8Field(bool showMessageBox)
{
    if (!field8CheckBox->isChecked())
        return true;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field8 = chgStruct.flightRules + chgStruct.typeOfFlight;
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

bool ChgWidget::process9Field(bool showMessageBox)
{
    if (!field9CheckBox->isChecked())
        return true;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString sTemp;
    if(chgStruct.numberOfAircrafts > 1)
        sTemp = QString::number(chgStruct.numberOfAircrafts);
    QString field9 = sTemp
            + chgStruct.aircraftType + "/" + chgStruct.turbulence;
    mProcessor->processType9(field9, mMap, errorCodes);
    mProcessor->validateType9(mMap,errorCodes);

    numberOfAircrafts->setStyleSheet("");
    aircraftType->setStyleSheet("");
    wtc->setStyleSheet("");

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

bool ChgWidget::process10Field(bool showMessageBox)
{
    if (!field10CheckBox->isChecked())
        return true;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field10 = chgStruct.equipment;
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

bool ChgWidget::process13Field(bool showMessageBox)
{
    if (!field13CheckBox->isChecked())
        return true;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field13 = chgStruct.changedADEP + chgStruct.departureTime.toString("hhmm");
    mProcessor->processType13(field13, mMap, errorCodes);
    mProcessor->validateType13(mMap, errorCodes);

    changedAdep->setStyleSheet("");
    departureTime->setStyleSheet("");

    if (changedAdep->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField13Format, QString()));

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField13Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidDepartureTime)
        {
#ifndef DUSHANBE_BUILD
            changedAdep->setStyleSheet(invalidValue);
            departureTime->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                changedAdep->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool ChgWidget::process15Field(bool showMessageBox)
{
    if (!field15CheckBox->isChecked())
        return true;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field15 = chgStruct.speed + chgStruct.flightLevel
            + " " + chgStruct.route;

    mProcessor->processType15(field15, mMap, errorCodes);
    mProcessor->validateType15(mMap, errorCodes);

    speedType->setStyleSheet("");
    speed->setStyleSheet("");
    flightLevel->setStyleSheet("");
    flightLevelType->setStyleSheet("");
    route->setStyleSheet("");

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
                speedType->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool ChgWidget::process16Field(bool showMessageBox)
{
    if (!field16CheckBox->isChecked())
        return true;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field16 = chgStruct.ADES + removeColonFromTime(chgStruct.dtADES) + " "
            + chgStruct.alt1 + " " + chgStruct.alt2;
    mProcessor->processType16(field16, mMap, errorCodes);
    mProcessor->validateType16(mMap, errorCodes);

    changedAdes->setStyleSheet("");
    eet->setStyleSheet("");
    alt1->setStyleSheet("");
    alt2->setStyleSheet("");

    if (changedAdes->text().isEmpty())
        errorCodes.append(AFTNErrorInfo(AFTNMessageProcessor::InvalidField16Format, QString()));

    foreach (const AFTNErrorInfo &error, errorCodes)
    {
        if (AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidField16Format
                || AFTNMessageProcessor::ProcessResult(error.first) == AFTNMessageProcessor::InvalidArrivalTime)
        {
#ifndef DUSHANBE_BUILD
            changedAdes->setStyleSheet(invalidValue);
            eet->setStyleSheet(invalidValue);
            alt1->setStyleSheet(invalidValue);
            alt2->setStyleSheet(invalidValue);
#endif
            if (showMessageBox)
            {
                changedAdes->setFocus();
                QMessageBox::warning(this, tr("Error"), mProcessor->errorString(error));
            }
            return false;
        }
    }
    return true;
}

bool ChgWidget::process18Field(bool showMessageBox)
{
    if (!field18CheckBox->isChecked())
        return true;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;

    ChgStruct chgStruct;
    chgStruct = chg();
    QString field18 = chgStruct.other;
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

void ChgWidget::onEditTextChangeAdep(const QString &s)
{
    int pos = changedAdep->cursorPosition();
    changedAdep->setText(s.toUpper());
    changedAdep->setCursorPosition(pos);
}

void ChgWidget::onEditTextChangeAdes(const QString &s)
{
    int pos = changedAdes->cursorPosition();
    changedAdes->setText(s.toUpper());
    changedAdes->setCursorPosition(pos);
}

void ChgWidget::onEditTextChangeAlt1(const QString &s)
{
    int pos = alt1->cursorPosition();
    alt1->setText(s.toUpper());
    alt1->setCursorPosition(pos);
}

void ChgWidget::onEditTextChangeAlt2(const QString &s)
{
    int pos = alt2->cursorPosition();
    alt2->setText(s.toUpper());
    alt2->setCursorPosition(pos);
}

void ChgWidget::onEditTextEquipment(const QString &s)
{
    int pos = equipment->cursorPosition();
    equipment->setText(s.toUpper());
    equipment->setCursorPosition(pos);
}

void ChgWidget::onEditTextAircraftType(const QString &s)
{
    int pos = aircraftType->cursorPosition();
    aircraftType->setText(s.toUpper());
    aircraftType->setCursorPosition(pos);
}

void ChgWidget::onEditTextACID(const QString &s)
{
    int pos = acid->cursorPosition();
    acid->setText(s.toUpper());
    acid->setCursorPosition(pos);
}

void ChgWidget::onEditTextAdep(const QString &s)
{
    int pos = adep->cursorPosition();
    adep->setText(s.toUpper());
    adep->setCursorPosition(pos);
}

void ChgWidget::onEditTextAdes(const QString &s)
{
    int pos = ades->cursorPosition();
    ades->setText(s.toUpper());
    ades->setCursorPosition(pos);
}

void ChgWidget::onEditTextRoute()
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

void ChgWidget::onEditTextOther()
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

void ChgWidget::onTextChangedEet(const QString &s)
{
    int pos = eet->cursorPosition();
    if ((s.right(s.indexOf(":"))).toInt() > 59)
        eet->setText(s.left(s.indexOf(":")) + ":00");
    eet->setCursorPosition(pos);
}

bool ChgWidget::validate()
{
    if(!process7FieldH(true))
        return false;
    if(!process13FieldH(true))
        return false;
    if(!process16FieldH(true))
        return false;
    if(flightRules->isEnabled() && flightType->isEnabled() && !process8Field(true))
        return false;
    if(numberOfAircrafts->isEnabled() && aircraftType->isEnabled() && !process9Field(true))
        return false;
    if(equipment->isEnabled() && !process10Field(true))
        return false;
    if(changedAdep->isEnabled() && departureTime->isEnabled() && !process13Field(true))
        return false;
    if(speedType->isEnabled() && speed->isEnabled() && flightLevelType->isEnabled()
            && flightLevel->isEnabled() && selectRoute->isEnabled() && route->isEnabled()
            && !process15Field(true))
        return false;
    if(changedAdes->isEnabled() && eet->isEnabled() && alt1->isEnabled() && alt2->isEnabled()
            && !process16Field(true))
        return false;
    if(other->isEnabled() && !process18Field(true))
        return false;
    return true;
}
