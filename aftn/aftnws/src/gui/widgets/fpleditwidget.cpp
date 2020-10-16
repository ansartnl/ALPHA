#include "fpleditwidget.h"
#include "fpl.h"
#include "AFTNDao.h"

#include "aftnmessageprocessor.h"

#include "printutils.h"

#include "main.h"

#include "uppercasevalidator.h"

#include <QtGui/QMessageBox>

#include <QtSql/QSqlTableModel>

FplEditWidget::FplEditWidget(QWidget* pParent) :
    QWidget(pParent)
{
    setupUi(this);
    init();

    validValue = "";
    invalidValue = "background-color: #FF637F;";

    networkSettings()->beginGroup("aftn_message_processor");
    QStringList listFirName = networkSettings()->value("fir_name").toString().split(",");
    if(!listFirName.isEmpty())
        firName = listFirName.first();
    networkSettings()->endGroup();

    uppercaseValidator = new UppercaseValidator(this);
    ACIDEdit->setValidator(uppercaseValidator);
    AerodromeDepEdit->setValidator(uppercaseValidator);
    AerodromeDestEdit->setValidator(uppercaseValidator);
    EquipmentEdit->setValidator(uppercaseValidator);
    ALTNEdit->setValidator(uppercaseValidator);
    ALTN2Edit->setValidator(uppercaseValidator);
    AirplaneCombo->setValidator(uppercaseValidator);
    LevelEdit->setValidator(uppercaseValidator);
    AirplaneCombo->lineEdit()->setMaxLength(4);

    connect(RouteEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(OtherEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    connect(ACIDEdit, SIGNAL(textChanged(QString)), this, SLOT(onACIDEditChanged()));
    connect(AirplaneCombo, SIGNAL(editTextChanged(QString)), this, SLOT(onAirplaneComboChanged()));
    connect(EquipmentEdit, SIGNAL(textChanged(QString)), this, SLOT(onEquipmentEditChanged()));
    connect(AerodromeDepEdit, SIGNAL(textChanged(QString)), this, SLOT(onAerodromeDepEditChanged()));
    connect(SpeedEdit, SIGNAL(textChanged(QString)), this, SLOT(onSpeedEditChanged()));
    connect(LevelEdit, SIGNAL(textChanged(QString)), this, SLOT(onLevelEditChanged()));
    connect(RouteEdit, SIGNAL(textChanged()), this, SLOT(onRouteEditChanged()));
    connect(AerodromeDestEdit, SIGNAL(textChanged(QString)), this, SLOT(onAerodromeDestEditChanged()));
    connect(entryFIREET, SIGNAL(textChanged(QString)), this, SLOT(onEntryFIREETChanged()));
    connect(EET, SIGNAL(textChanged(QString)), this, SLOT(onEETChanged()));
    connect(ALTNEdit, SIGNAL(textChanged(QString)), this, SLOT(onALTNEditChanged()));
    connect(ALTN2Edit, SIGNAL(textChanged(QString)), this, SLOT(onALTN2EditChanged()));

    connect(applyButton, SIGNAL(clicked()), this, SLOT(onApply()));
    applyButton->setVisible(false);
}

void FplEditWidget::setFpl(const QVariantMap &message)
{
    clear();

    QDateTime zeroDateTime = QDateTime::currentDateTime().toUTC();
    zeroDateTime.setTime(QTime(0, 0, 0, 0));

    TimeDepEdit->setDateTime(zeroDateTime);
    entryFIREET->setText("00:00");

    FIRCombo->setCurrentIndex(FIRCombo->findText(firName));
    if (FIRCombo->currentIndex() == -1)
        FIRCombo->setEditText(firName);

    ACIDEdit->setText(message.value(AFTN_MESSAGE_AIRCRAFT_ID).toString());

    TurbulenceCombo->setCurrentIndex(TurbulenceCombo->findText(message.value(AFTN_MESSAGE_TURBULENCE).toString()));

    RulesCombo->setCurrentIndex(RulesCombo->findText(message.value(AFTN_MESSAGE_FLIGHT_RULES).toString()));

    FlightTypeCombo->setCurrentIndex(FlightTypeCombo->findText(message.value(AFTN_MESSAGE_FLIGHT_TYPE).toString()));

    NumberEdit->setValue(message.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt());

    AirplaneCombo->setCurrentIndex(AirplaneCombo->findText(message.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString()));
    if (AirplaneCombo->currentIndex() == -1)
        AirplaneCombo->setEditText(message.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString());

    EquipmentEdit->setText(message.value(AFTN_MESSAGE_EQUIPMENT).toString());

    AerodromeDepEdit->setText(message.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString());

    TimeDepEdit->setTime(message.value(AFTN_MESSAGE_DEPARTURE_TIME).toTime());

    QVariantMap firEET = message.value(AFTN_MESSAGE_OTHER_INFO_FIR_EET).toMap();
    entryFIREET->setText(firEET.value(firName).toString());

    EET->setText(message.value(AFTN_MESSAGE_ELAPSED_TIME).toString());

    RouteEdit->setText(message.value(AFTN_MESSAGE_ROUTE).toString());

    AerodromeDestEdit->setText(message.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString());

    OtherEdit->setText(message.value(AFTN_MESSAGE_OTHER_INFO).toString());

    ALTNEdit->setText(message.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString());

    ALTN2Edit->setText(message.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString());

    QString speed = message.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString();
    QRegExp speedRegExp("([KNM])(\\d{3,4})");
    if (speedRegExp.exactMatch(speed))
    {
        SpeedCombo->setCurrentIndex(SpeedCombo->findText(speedRegExp.cap(1)));
        if (SpeedCombo->currentIndex() == -1)
            SpeedCombo->setEditText(speedRegExp.cap(1));
        SpeedEdit->setText(speedRegExp.cap(2));
    }

    QString level = message.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString();
    QRegExp levelRegExp("([FASM])(\\d{3,4})|VFR");
    if (levelRegExp.exactMatch(level))
    {
        LevelCombo->setCurrentIndex(LevelCombo->findText(levelRegExp.cap(1)));
        if (LevelCombo->currentIndex() == -1)
            LevelCombo->setEditText(levelRegExp.cap(1));
        LevelEdit->setText(levelRegExp.cap(2));
    }

//    status->setCurrentIndex(status->findText(mFpl.status()));

    checkFpl();
}

void FplEditWidget::checkFpl()
{
    FPL fpl;
    fpl.setFirName(FIRCombo->currentText());
    if (!fpl.setAircraftId(ACIDEdit->text())) {
        ACIDEdit->setStyleSheet(invalidValue);
    } else {
        ACIDEdit->setStyleSheet(validValue);
    }

    if (!fpl.setAircraftType(AirplaneCombo->currentText())) {
        AirplaneCombo->setStyleSheet(invalidValue);
    } else {
        AirplaneCombo->setStyleSheet(validValue);
    }

    if (!fpl.setEquipment(EquipmentEdit->text())) {
        EquipmentEdit->setStyleSheet(invalidValue);
    } else {
        EquipmentEdit->setStyleSheet(validValue);
    }

    if (!fpl.setDepartureAerodrome(AerodromeDepEdit->text())) {
        AerodromeDepEdit->setStyleSheet(invalidValue);
    } else {
        AerodromeDepEdit->setStyleSheet(validValue);
    }

    if (!fpl.setRoute(RouteEdit->toPlainText())) {
        RouteEdit->setStyleSheet(invalidValue);
    } else {
        RouteEdit->setStyleSheet(validValue);
    }

    if (!fpl.setDestAerodrome(AerodromeDestEdit->text())) {
        AerodromeDestEdit->setStyleSheet(invalidValue);
    } else {
        AerodromeDestEdit->setStyleSheet(validValue);
    }

    if (!fpl.setEntryFIREET(entryFIREET->text())) {
        entryFIREET->setStyleSheet(invalidValue);
    } else {
        entryFIREET->setStyleSheet(validValue);
    }

    if (!fpl.setEET(EET->text())) {
        EET->setStyleSheet(invalidValue);
    } else {
        EET->setStyleSheet(validValue);
    }

    if (!fpl.setAltDestinationAerodrome(ALTNEdit->text())) {
        ALTNEdit->setStyleSheet(invalidValue);
    } else {
        ALTNEdit->setStyleSheet(validValue);
    }

    if (!fpl.setAltDestinationAerodrome2(ALTN2Edit->text())) {
        ALTN2Edit->setStyleSheet(invalidValue);
    } else {
        ALTN2Edit->setStyleSheet(validValue);
    }

    if (!fpl.setOtherInformation(OtherEdit->toPlainText())) {
        OtherEdit->setStyleSheet(invalidValue);
    } else {
        OtherEdit->setStyleSheet(validValue);
    }

    QString unit = SpeedCombo->currentText();

    QString value = SpeedEdit->text();

    if (!fpl.setSpeed(unit, value)) {
        SpeedCombo->setStyleSheet(invalidValue);
        SpeedEdit->setStyleSheet(invalidValue);
    } else {
        SpeedCombo->setStyleSheet(validValue);
        SpeedEdit->setStyleSheet(validValue);
    }

    unit = LevelCombo->currentText();
    value = LevelEdit->text();

    if (!fpl.setLevel(unit, value)) {
        LevelCombo->setStyleSheet(invalidValue);
        LevelEdit->setStyleSheet(invalidValue);
    } else {
        LevelCombo->setStyleSheet(validValue);
        LevelEdit->setStyleSheet(validValue);
    }

    // Logic route control
    {
        QVariantMap map;
        AFTNMessageProcessor::AftnMessageErrors errorCodes;
        bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName(fpl.firName());

        map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
        aftnMessageProcessor->validateType15(map, errorCodes);
        if (!errorCodes.isEmpty()) {
            RouteEdit->setStyleSheet(invalidValue);
        } else {
            fpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
            RouteEdit->setStyleSheet(validValue);
        }

        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName("");
    }
    //other (field 18) as FPL2012
    {
        QVariantMap map;
        AFTNMessageProcessor::AftnMessageErrors errorCodes;
        bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName(fpl.firName());

        aftnMessageProcessor->processType10(fpl.equipment(), map, errorCodes);
        aftnMessageProcessor->validateType10(map, errorCodes);
        if(!errorCodes.isEmpty())
        {
            EquipmentEdit->setStyleSheet(invalidValue);
        }
        else
        {
            EquipmentEdit->setStyleSheet(validValue);
        }
        errorCodes.clear();
        aftnMessageProcessor->processType18(fpl.otherInformation(), map, errorCodes);
        aftnMessageProcessor->validateType18(map, errorCodes);
        if(!errorCodes.isEmpty())
        {
            OtherEdit->setStyleSheet(invalidValue);
        }
        else
        {
            OtherEdit->setStyleSheet(validValue);
        }
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName("");
    }
}

void FplEditWidget::init()
{
    // select zones where type = zone
    QSqlQueryModel *firComboModel = new QSqlQueryModel(FIRCombo);
    firComboModel->setQuery(
                "SELECT id, id_name FROM zone WHERE type = 1 ORDER BY name");
    FIRCombo->setModel(firComboModel);
    FIRCombo->setModelColumn(1);
    FIRCombo->setCurrentIndex(0);

    QSqlTableModel *aircrafttypesModel = new QSqlTableModel(this);
    aircrafttypesModel->setTable("airplane_type");
    aircrafttypesModel->sort(0, Qt::AscendingOrder);
    aircrafttypesModel->select();
    AirplaneCombo->setModel(aircrafttypesModel);
    AirplaneCombo->setModelColumn(aircrafttypesModel->fieldIndex("name"));

    // Flight types
    QSqlTableModel *flightTypesModel = new QSqlTableModel(this);
    flightTypesModel->setTable("flight_types");
    flightTypesModel->sort(0, Qt::AscendingOrder);
    flightTypesModel->select();
    FlightTypeCombo->setModel(flightTypesModel);
    FlightTypeCombo->setModelColumn(flightTypesModel->fieldIndex("name"));

    // Turbulence category
    QSqlTableModel *turbulenceModel = new QSqlTableModel(this);
    turbulenceModel->setTable("turbulence_category");
    turbulenceModel->sort(0, Qt::AscendingOrder);
    turbulenceModel->select();
    TurbulenceCombo->setModel(turbulenceModel);
    TurbulenceCombo->setModelColumn(turbulenceModel->fieldIndex("name"));

    // Flight rules
    QSqlTableModel *flightRulesModel = new QSqlTableModel(this);
    flightRulesModel->setTable("flight_rules");
    flightRulesModel->sort(0, Qt::AscendingOrder);
    flightRulesModel->select();
    RulesCombo->setModel(flightRulesModel);
    RulesCombo->setModelColumn(flightRulesModel->fieldIndex("name"));

    // Level parameters
    QSqlTableModel *levelTypesModel = new QSqlTableModel(this);
    levelTypesModel->setTable("level_types");
    levelTypesModel->select();
    LevelCombo->setModel(levelTypesModel);
    LevelCombo->setModelColumn(levelTypesModel->fieldIndex("name"));

    // Speed parameters
    QSqlTableModel *speedTypesModel = new QSqlTableModel(this);
    speedTypesModel->setTable("speed_types");
    speedTypesModel->select();
    SpeedCombo->setModel(speedTypesModel);
    SpeedCombo->setModelColumn(speedTypesModel->fieldIndex("name"));
}

void FplEditWidget::clear()
{
    TimeDepEdit->clear();
    entryFIREET->clear();

    FIRCombo->clear();
    FIRCombo->setCurrentIndex(-1);

    ACIDEdit->clear();
    ACIDEdit->setStyleSheet(validValue);

    TurbulenceCombo->setCurrentIndex(-1);

    RulesCombo->setCurrentIndex(-1);

    FlightTypeCombo->setCurrentIndex(-1);

    NumberEdit->clear();

    AirplaneCombo->setCurrentIndex(-1);
    AirplaneCombo->clearEditText();
    AirplaneCombo->setStyleSheet(validValue);

    EquipmentEdit->clear();
    EquipmentEdit->setStyleSheet(validValue);

    AerodromeDepEdit->clear();
    AerodromeDepEdit->setStyleSheet(validValue);

    TimeDepEdit->clear();

    entryFIREET->clear();
    entryFIREET->setStyleSheet(validValue);

    EET->clear();
    EET->setStyleSheet(validValue);

    RouteEdit->clear();
    RouteEdit->setStyleSheet(validValue);

    AerodromeDestEdit->clear();
    AerodromeDestEdit->setStyleSheet(validValue);

    OtherEdit->clear();
    OtherEdit->setStyleSheet(validValue);

    ALTNEdit->clear();
    ALTNEdit->setStyleSheet(validValue);

    ALTN2Edit->clear();
    ALTN2Edit->setStyleSheet(validValue);

    SpeedCombo->setCurrentIndex(-1);
    SpeedCombo->setStyleSheet(validValue);

    SpeedEdit->clear();
    SpeedEdit->setStyleSheet(validValue);

    LevelCombo->setCurrentIndex(-1);
    LevelCombo->setStyleSheet(validValue);

    LevelEdit->clear();
    LevelEdit->setStyleSheet(validValue);
}

void FplEditWidget::onTextChanged()
{
    if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(sender())) {
        QString original = textEdit->toPlainText();
        QString uppered = original.toUpper();

        if (original != uppered) {
            QTextCursor textCursor = textEdit->textCursor();
            int iPosition = textCursor.position();
            textEdit->setPlainText(uppered);
            textEdit->moveCursor(QTextCursor::Start);

            for (; iPosition > 0; --iPosition)
                textEdit->moveCursor(QTextCursor::NextCharacter);
        }
    }
}

void FplEditWidget::onAirplaneComboChanged()
{
    FPL mFpl;
    if (mFpl.setAircraftType(AirplaneCombo->currentText()))
        AirplaneCombo->setStyleSheet(validValue);
    else
        AirplaneCombo->setStyleSheet(invalidValue);
}

void FplEditWidget::onACIDEditChanged()
{
    FPL mFpl;
    if (mFpl.setAircraftId(ACIDEdit->text()))
        ACIDEdit->setStyleSheet(validValue);
    else
        ACIDEdit->setStyleSheet(invalidValue);
}

void FplEditWidget::onEquipmentEditChanged()
{
    FPL mFpl;
    if (mFpl.setEquipment(EquipmentEdit->text()))
        ;//EquipmentEdit->setStyleSheet(validValue);
    else
        ;//EquipmentEdit->setStyleSheet(invalidValue);
}

void FplEditWidget::onAerodromeDepEditChanged()
{
    FPL mFpl;
    if (mFpl.setDepartureAerodrome(AerodromeDepEdit->text()))
        AerodromeDepEdit->setStyleSheet(validValue);
    else
        AerodromeDepEdit->setStyleSheet(invalidValue);
}

void FplEditWidget::onSpeedEditChanged()
{
    QString unit = SpeedCombo->currentText();
    QString value = SpeedEdit->text();

    FPL mFpl;
    if (mFpl.setSpeed(unit, value)) {
        SpeedCombo->setStyleSheet(validValue);
        SpeedEdit->setStyleSheet(validValue);
    } else {
        SpeedCombo->setStyleSheet(invalidValue);
        SpeedEdit->setStyleSheet(invalidValue);
    }
}

void FplEditWidget::onLevelEditChanged()
{
    QString unit = LevelCombo->currentText();
    QString value = LevelEdit->text();

    FPL mFpl;
    if (mFpl.setLevel(unit, value)) {
        LevelCombo->setStyleSheet(validValue);
        LevelEdit->setStyleSheet(validValue);
    } else {
        LevelCombo->setStyleSheet(invalidValue);
        LevelEdit->setStyleSheet(invalidValue);
    }
}

void FplEditWidget::onRouteEditChanged()
{
    FPL mFpl;
    if (mFpl.setRoute(RouteEdit->toPlainText()))
        RouteEdit->setStyleSheet(validValue);
    else
        RouteEdit->setStyleSheet(invalidValue);

    // Logic route control
    QVariantMap map;
    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
    aftnMessageProcessor->validateType15(map, errorCodes);
    if (!errorCodes.isEmpty()) {
        RouteEdit->setStyleSheet(invalidValue);
    } else {
        mFpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
        RouteEdit->setStyleSheet(validValue);
    }
}

void FplEditWidget::onAerodromeDestEditChanged()
{
    FPL mFpl;
    if (mFpl.setDestAerodrome(AerodromeDestEdit->text()))
        AerodromeDestEdit->setStyleSheet(validValue);
    else
        AerodromeDestEdit->setStyleSheet(invalidValue);
}

void FplEditWidget::onEntryFIREETChanged()
{
    FPL mFpl;
    if (mFpl.setEntryFIREET(entryFIREET->text()))
        entryFIREET->setStyleSheet(validValue);
    else
        entryFIREET->setStyleSheet(invalidValue);
}

void FplEditWidget::onEETChanged()
{
    FPL mFpl;
    if (mFpl.setEET(EET->text()))
        EET->setStyleSheet(validValue);
    else
        EET->setStyleSheet(invalidValue);
}

void FplEditWidget::onALTNEditChanged()
{
    FPL mFpl;
    if (mFpl.setAltDestinationAerodrome(ALTNEdit->text()))
        ALTNEdit->setStyleSheet(validValue);
    else
        ALTNEdit->setStyleSheet(invalidValue);
}

void FplEditWidget::onALTN2EditChanged()
{
    FPL mFpl;
    if (mFpl.setAltDestinationAerodrome2(ALTN2Edit->text()))
        ALTN2Edit->setStyleSheet(validValue);
    else
        ALTN2Edit->setStyleSheet(invalidValue);
}

void FplEditWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);

    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
    }
}

void FplEditWidget::onApply()
{
    FPL fpl;
    bool isValid = true;

    fpl.setFirName(FIRCombo->currentText());

    if (!fpl.setAircraftId(ACIDEdit->text())) {
        isValid = false;
        ACIDEdit->setStyleSheet(invalidValue);
    } else {
        ACIDEdit->setStyleSheet(validValue);
    }

    if (!fpl.setFlightRules(RulesCombo->currentText())) {
        isValid = false;
        RulesCombo->setStyleSheet(invalidValue);
    } else {
        RulesCombo->setStyleSheet(validValue);
    }

    if (!fpl.setFlightType(FlightTypeCombo->currentText())) {
        isValid = false;
        FlightTypeCombo->setStyleSheet(invalidValue);
    } else {
        FlightTypeCombo->setStyleSheet(validValue);
    }

    if (!fpl.setAircraftNumber(NumberEdit->value())) {
        isValid = false;
        NumberEdit->setStyleSheet(invalidValue);
    } else {
        NumberEdit->setStyleSheet(validValue);
    }

    if (!fpl.setAircraftType(AirplaneCombo->currentText())) {
        isValid = false;
        AirplaneCombo->setStyleSheet(invalidValue);
    } else {
        AirplaneCombo->setStyleSheet(validValue);
    }

    if (!fpl.setTurbulenceCategory(TurbulenceCombo->currentText())) {
        isValid = false;
        TurbulenceCombo->setStyleSheet(invalidValue);
    } else {
        TurbulenceCombo->setStyleSheet(validValue);
    }

    if (!fpl.setEquipment(EquipmentEdit->text())) {
        isValid = false;
        EquipmentEdit->setStyleSheet(invalidValue);
    } else {
        EquipmentEdit->setStyleSheet(validValue);
    }

    if (!fpl.setDepartureAerodrome(AerodromeDepEdit->text())) {
        isValid = false;
        AerodromeDepEdit->setStyleSheet(invalidValue);
    } else {
        AerodromeDepEdit->setStyleSheet(validValue);
    }

    if (!fpl.setDepartureTime(TimeDepEdit->time())) {
        isValid = false;
        TimeDepEdit->setStyleSheet(invalidValue);
    } else {
        TimeDepEdit->setStyleSheet(validValue);
    }

    if (!fpl.setRoute(RouteEdit->toPlainText())) {
        isValid = false;
        RouteEdit->setStyleSheet(invalidValue);
    } else {
        RouteEdit->setStyleSheet(validValue);
    }

    if (!fpl.setDestAerodrome(AerodromeDestEdit->text())) {
        isValid = false;
        AerodromeDestEdit->setStyleSheet(invalidValue);
    } else {
        AerodromeDestEdit->setStyleSheet(validValue);
    }

    if (!fpl.setEntryFIREET(entryFIREET->text())) {
        isValid = false;
        entryFIREET->setStyleSheet(invalidValue);
    } else {
        entryFIREET->setStyleSheet(validValue);
    }

    if (!fpl.setEET(EET->text())) {
        isValid = false;
        EET->setStyleSheet(invalidValue);
    } else {
        EET->setStyleSheet(validValue);
    }

    if (!fpl.setAltDestinationAerodrome(ALTNEdit->text())) {
        isValid = false;
        ALTNEdit->setStyleSheet(invalidValue);
    } else {
        ALTNEdit->setStyleSheet(validValue);
    }

    if (!fpl.setAltDestinationAerodrome2(ALTN2Edit->text())) {
        isValid = false;
        ALTN2Edit->setStyleSheet(invalidValue);
    } else {
        ALTN2Edit->setStyleSheet(validValue);
    }

    if (!fpl.setOtherInformation(OtherEdit->toPlainText())) {
        isValid = false;
        OtherEdit->setStyleSheet(invalidValue);
    } else {
        OtherEdit->setStyleSheet(validValue);
    }

    QString unit = SpeedCombo->currentText();

    QString value = SpeedEdit->text();

    if (!fpl.setSpeed(unit, value)) {
        isValid = false;
        SpeedCombo->setStyleSheet(invalidValue);
        SpeedEdit->setStyleSheet(invalidValue);
    } else {
        SpeedCombo->setStyleSheet(validValue);
        SpeedEdit->setStyleSheet(validValue);
    }

    unit = LevelCombo->currentText();
    value = LevelEdit->text();

    if (!fpl.setLevel(unit, value)) {
        isValid = false;
        LevelCombo->setStyleSheet(invalidValue);
        LevelEdit->setStyleSheet(invalidValue);
    } else {
        LevelCombo->setStyleSheet(validValue);
        LevelEdit->setStyleSheet(validValue);
    }

    if(isValid)
    { // Logic route control
        QVariantMap map;
        AFTNMessageProcessor::AftnMessageErrors errorCodes;
        bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName(fpl.firName());

        map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
        aftnMessageProcessor->validateType15(map, errorCodes);

        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName("");

        if (!errorCodes.isEmpty()) {
            isValid = false;
            RouteEdit->setStyleSheet(invalidValue);
            QStringList errors = aftnMessageProcessor->errorStrings(errorCodes);
            QMessageBox::critical(this, tr("Error"), tr("Route error. %1").arg(
                                      errors.join("\n")));
        } else {
            fpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
            RouteEdit->setStyleSheet(validValue);
        }
    }
    //other (field 18) as FPL2012
    if(isValid)
    {
        QVariantMap map;
        AFTNMessageProcessor::AftnMessageErrors errorCodes;
        bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName(fpl.firName());

        aftnMessageProcessor->processType10(fpl.equipment(), map, errorCodes);
        aftnMessageProcessor->validateType10(map, errorCodes);
        if(!errorCodes.isEmpty())
        {
            EquipmentEdit->setStyleSheet(invalidValue);
            isValid = false;
        }
        else
        {
            EquipmentEdit->setStyleSheet(validValue);
        }
        if(isValid)
        {
            aftnMessageProcessor->processType18(fpl.otherInformation(), map, errorCodes);
            aftnMessageProcessor->validateType18(map, errorCodes);
            if(!errorCodes.isEmpty())
            {
                OtherEdit->setStyleSheet(invalidValue);
                isValid = false;
            }
            else
            {
                OtherEdit->setStyleSheet(validValue);
                { // DOF podpora
                    if(map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
                    {
                        QDate dateOfFlight = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
                        if(dateOfFlight.isValid())
                            fpl.setDateDOF(dateOfFlight);
                    }
                }
            }
        }
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName("");
    }

    if (isValid)
    {
        QSqlError error;
        bool bFlag = false;
        emit maybeDushanbe(bFlag, fpl);
        if(!bFlag)
            error = AFTNDao::saveFPL(fpl);

        if (error.isValid())
            QMessageBox::critical(this, tr("Error when saving FPL"), tr(
                                      "Exception '%1' occurs when saving FPL").arg(error.text()));
        else
            emit apply(fpl);
    }
}
