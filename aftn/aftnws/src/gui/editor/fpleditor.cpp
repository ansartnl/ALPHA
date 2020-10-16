#include "fpleditor.h"
#include "fpl.h"
#include "AFTNDao.h"

#include "aftnmessageprocessor.h"

#include "printutils.h"

#include "main.h"

#include "uppercasevalidator.h"

#include <QPushButton>
#include <QMessageBox>

#include <QtSql/QSqlTableModel>

FplEditor::FplEditor(QWidget* pParent) :
    AbstractDialog(pParent), mFpl(), canOtherInfoEmpty(false)
{
    setupUi(this);
    init();

    validValue = "";
    invalidValue = "background-color: #FF637F;";

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

    // Print engine
    printButton = new QPushButton(tr("Print..."), this);
    buttonBox->addButton(printButton, QDialogButtonBox::ActionRole);
    printButton->show();

    //restoreState(staticMetaObject.className());

    QMetaObject::invokeMethod(this, "OnFixedSize", Qt::QueuedConnection);
}


FplEditor::~FplEditor()
{
    saveState(staticMetaObject.className());
}

void FplEditor::on_buttonBox_clicked(QAbstractButton* button)
{
    if (buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
        doSavePlan();
    else if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
        doApplyPlan();
    else if (button == printButton)
        printWidget(frame, QPrinter::Landscape);
}

/** Update controls values from database */
void FplEditor::init()
{
    // select zones where type = zone
    firComboModel = new QSqlQueryModel(FIRCombo);
    firComboModel->setQuery(
                "SELECT id, id_name FROM zone WHERE type = 1 ORDER BY name");
    FIRCombo->setModel(firComboModel);
    FIRCombo->setModelColumn(1);
    FIRCombo->setCurrentIndex(0);

    aircrafttypesModel = new QSqlTableModel(this);
    aircrafttypesModel->setTable("airplane_type");
    aircrafttypesModel->sort(0, Qt::AscendingOrder);
    aircrafttypesModel->select();
    AirplaneCombo->setModel(aircrafttypesModel);
    AirplaneCombo->setModelColumn(aircrafttypesModel->fieldIndex("name"));

    // Flight types
    flightTypesModel = new QSqlTableModel(this);
    flightTypesModel->setTable("flight_types");
    flightTypesModel->sort(0, Qt::AscendingOrder);
    flightTypesModel->select();
    FlightTypeCombo->setModel(flightTypesModel);
    FlightTypeCombo->setModelColumn(flightTypesModel->fieldIndex("name"));

    // Turbulence category
    turbulenceModel = new QSqlTableModel(this);
    turbulenceModel->setTable("turbulence_category");
    turbulenceModel->sort(0, Qt::AscendingOrder);
    turbulenceModel->select();
    TurbulenceCombo->setModel(turbulenceModel);
    TurbulenceCombo->setModelColumn(turbulenceModel->fieldIndex("name"));

    // Flight rules
    flightRulesModel = new QSqlTableModel(this);
    flightRulesModel->setTable("flight_rules");
    flightRulesModel->sort(0, Qt::AscendingOrder);
    flightRulesModel->select();
    RulesCombo->setModel(flightRulesModel);
    RulesCombo->setModelColumn(flightRulesModel->fieldIndex("name"));

    // Level parameters
    levelTypesModel = new QSqlTableModel(this);
    levelTypesModel->setTable("level_types");
    levelTypesModel->select();
    LevelCombo->setModel(levelTypesModel);
    LevelCombo->setModelColumn(levelTypesModel->fieldIndex("name"));

    // Speed parameters
    speedTypesModel = new QSqlTableModel(this);
    speedTypesModel->setTable("speed_types");
    speedTypesModel->select();
    SpeedCombo->setModel(speedTypesModel);
    SpeedCombo->setModelColumn(speedTypesModel->fieldIndex("name"));

    statusTypesModel = new QSqlTableModel(this);
    statusTypesModel->setTable("fpl_status_types");
    statusTypesModel->select();
    status->setModel(statusTypesModel);
    status->setModelColumn(statusTypesModel->fieldIndex("name"));
}

/** Save changes (update old plan or save as new) */
void FplEditor::doSavePlan()
{
    // load FPL
    FPL Fpl = mFpl;

    // If fpl not valid
    if (!fpl(Fpl))
        return;

    // save FPL to database
    QSqlError error = AFTNDao::saveFPL(Fpl);

    if (!error.isValid())
        // close dialog
        accept();
    else
        QMessageBox::critical(this, tr("Error when saving FPL"), tr(
                                  "Exception '%1' occurs when saving FPL").arg(error.text()));
}

/** Save plan without closing form */
void FplEditor::doApplyPlan(void)
{
    // load FPL
    FPL Fpl = mFpl;

    if (!fpl(Fpl))
        return;

    // save FPL to database
    QSqlError error = AFTNDao::saveFPL(Fpl);

    if (!error.isValid()) {
        if (mFpl.isNew()) {
            mFpl = Fpl;
            setWindowTitle(tr("Editing Flight Plane #%1").arg(mFpl.id()));
        }
    } else {
        QMessageBox::critical(this, tr("Error when saving FPL"), tr(
                                  "Exception '%1' occurs when saving FPL").arg(error.text()));
    }
}

/** Copy information from FPL to dialog and over */
void FplEditor::setFpl(const FPL& Fpl)
{
    mFpl = Fpl;

    if (mFpl.isNew())
        setWindowTitle(tr("Creating Flight Plane"));
    else
        setWindowTitle(tr("Editing Flight Plane #%1").arg(mFpl.id()));

    QDateTime zeroDateTime = QDateTime::currentDateTime().toUTC();
    zeroDateTime.setTime(QTime(0, 0, 0, 0));

    TimeDepEdit->setDateTime(zeroDateTime);
    entryFIREET->setText("00:00");

    FIRCombo->setEditText(mFpl.firName());
    FIRCombo->setCurrentIndex(FIRCombo->findText(mFpl.firName()));

    ACIDEdit->setText(mFpl.aircarftId());
    ACIDEdit->setStyleSheet(validValue);

    TurbulenceCombo->setCurrentIndex(TurbulenceCombo->findText(
                                         mFpl.turbulenceCategory()));

    RulesCombo->setCurrentIndex(RulesCombo->findText(mFpl.flightRules()));

    FlightTypeCombo->setCurrentIndex(FlightTypeCombo->findText(mFpl.flightType()));

    NumberEdit->setValue(mFpl.aircraftNumber());

    AirplaneCombo->setStyleSheet(validValue);
    AirplaneCombo->setCurrentIndex(AirplaneCombo->findText(mFpl.aircraftType()));
    if (AirplaneCombo->currentIndex() == -1)
        AirplaneCombo->setEditText(mFpl.aircraftType());

    EquipmentEdit->setText(mFpl.equipment());
    EquipmentEdit->setStyleSheet(validValue);

    AerodromeDepEdit->setText(mFpl.departureAerodrome());
    AerodromeDepEdit->setStyleSheet(validValue);

    TimeDepEdit->setTime(mFpl.departureTime());

    entryFIREET->setText(mFpl.entryFIREET());
    entryFIREET->setStyleSheet(validValue);

    EET->setText(mFpl.EET());
    EET->setStyleSheet(validValue);

    RouteEdit->setText(mFpl.route());
    RouteEdit->setStyleSheet(validValue);

    AerodromeDestEdit->setText(mFpl.destinationAerodrome());
    AerodromeDestEdit->setStyleSheet(validValue);

    OtherEdit->setText(mFpl.otherInformation());
    OtherEdit->setStyleSheet(validValue);

    ALTNEdit->setText(mFpl.altDestinationAerodrome());
    ALTNEdit->setStyleSheet(validValue);

    ALTN2Edit->setText(mFpl.altDestinationAero2());
    ALTN2Edit->setStyleSheet(validValue);

    SpeedCombo->setCurrentIndex(SpeedCombo->findText(mFpl.speedUnit()));
    SpeedCombo->setStyleSheet(validValue);

    SpeedEdit->setText(mFpl.speedValue());
    SpeedEdit->setStyleSheet(validValue);

    LevelCombo->setCurrentIndex(LevelCombo->findText(mFpl.levelUnit()));
    LevelCombo->setStyleSheet(validValue);

    LevelEdit->setText(mFpl.levelValue());
    LevelEdit->setStyleSheet(validValue);

    status->setCurrentIndex(status->findText(mFpl.exFPL()));

    FIRCombo->setFocus();
}

bool FplEditor::fpl(FPL& Fpl)
{
    bool isValid = true;

    Fpl.setFirName(FIRCombo->currentText());
    if (!Fpl.setAircraftId(ACIDEdit->text())) {
        ACIDEdit->setStyleSheet(invalidValue);
        ACIDEdit->setFocus();
        isValid = false;
    } else {
        ACIDEdit->setStyleSheet(validValue);
    }

    Fpl.setFlightRules(RulesCombo->currentText());
    Fpl.setFlightType(FlightTypeCombo->currentText());
    Fpl.setAircraftNumber(NumberEdit->value());

    if (!Fpl.setAircraftType(AirplaneCombo->currentText())) {
        AirplaneCombo->setStyleSheet(invalidValue);
        AirplaneCombo->setFocus();
        isValid = false;
    } else {
        AirplaneCombo->setStyleSheet(validValue);
    }

    Fpl.setTurbulenceCategory(TurbulenceCombo->currentText());

    if (!Fpl.setEquipment(EquipmentEdit->text())) {
        EquipmentEdit->setStyleSheet(invalidValue);
        EquipmentEdit->setFocus();
        isValid = false;
    } else {
        EquipmentEdit->setStyleSheet(validValue);
    }

    if (!Fpl.setDepartureAerodrome(AerodromeDepEdit->text())) {
        AerodromeDepEdit->setStyleSheet(invalidValue);
        AerodromeDepEdit->setFocus();
        isValid = false;
    } else {
        AerodromeDepEdit->setStyleSheet(validValue);
    }

    Fpl.setDepartureTime(TimeDepEdit->time());

    if (!Fpl.setRoute(RouteEdit->toPlainText())) {
        RouteEdit->setStyleSheet(invalidValue);
        RouteEdit->setFocus();
        isValid = false;
    } else {
        RouteEdit->setStyleSheet(validValue);
    }

    if (!Fpl.setDestAerodrome(AerodromeDestEdit->text())) {
        AerodromeDestEdit->setStyleSheet(invalidValue);
        AerodromeDestEdit->setFocus();
        isValid = false;
    } else {
        AerodromeDestEdit->setStyleSheet(validValue);
    }

    if (!Fpl.setEntryFIREET(entryFIREET->text())) {
        entryFIREET->setStyleSheet(invalidValue);
        entryFIREET->setFocus();
        isValid = false;
    } else {
        entryFIREET->setStyleSheet(validValue);
    }

    if (!Fpl.setEET(EET->text())) {
        EET->setStyleSheet(invalidValue);
        EET->setFocus();
        isValid = false;
    } else {
        EET->setStyleSheet(validValue);
    }

    if (!Fpl.setAltDestinationAerodrome(ALTNEdit->text())) {
        ALTNEdit->setStyleSheet(invalidValue);
        ALTNEdit->setFocus();
        isValid = false;
    } else {
        ALTNEdit->setStyleSheet(validValue);
    }

    if (!Fpl.setAltDestinationAerodrome2(ALTN2Edit->text())) {
        ALTN2Edit->setStyleSheet(invalidValue);
        ALTN2Edit->setFocus();
        isValid = false;
    } else {
        ALTN2Edit->setStyleSheet(validValue);
    }

    if (!Fpl.setOtherInformation(OtherEdit->toPlainText())) {
        OtherEdit->setStyleSheet(invalidValue);
        OtherEdit->setFocus();
        isValid = false;
    } else {
        OtherEdit->setStyleSheet(validValue);
    }

    QString unit = SpeedCombo->currentText();

    QString value = SpeedEdit->text();

    if (!Fpl.setSpeed(unit, value)) {
        SpeedCombo->setStyleSheet(invalidValue);
        SpeedEdit->setStyleSheet(invalidValue);
        isValid = false;
        SpeedEdit->setFocus();
    } else {
        SpeedCombo->setStyleSheet(validValue);
        SpeedEdit->setStyleSheet(validValue);
    }

    unit = LevelCombo->currentText();
    value = LevelEdit->text();

    if (!Fpl.setLevel(unit, value)) {
        LevelCombo->setStyleSheet(invalidValue);
        LevelEdit->setStyleSheet(invalidValue);
        isValid = false;
        LevelEdit->setFocus();
    } else {
        LevelCombo->setStyleSheet(validValue);
        LevelEdit->setStyleSheet(validValue);
    }

    Fpl.setExFPL(status->currentText());

    // Logic route control
    if(isValid)
    {
        QVariantMap map;
        AFTNMessageProcessor::AftnMessageErrors errorCodes;
        map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
        bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName(Fpl.firName());
        aftnMessageProcessor->validateType15(map, errorCodes);
        if (!errorCodes.isEmpty()) {
            isValid = false;
            RouteEdit->setStyleSheet(invalidValue);
            QStringList errors = aftnMessageProcessor->errorStrings(errorCodes);
            QMessageBox::critical(this, tr("Error"), tr("Route error. %1").arg(
                                      errors.join("\n")));
        } else {
            Fpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
            RouteEdit->setStyleSheet(validValue);
        }
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName("");
    }
    //other (field 18) as FPL2012
    if(isValid)
    {
        QVariantMap map;
        AFTNMessageProcessor::AftnMessageErrors errorCodes;
        bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName(Fpl.firName());

        aftnMessageProcessor->processType10(Fpl.equipment(), map, errorCodes);
        aftnMessageProcessor->validateType10(map, errorCodes);
        if(!errorCodes.isEmpty())
        {
            EquipmentEdit->setStyleSheet(invalidValue);
            EquipmentEdit->setFocus();
            isValid = false;
        }
        else
        {
            EquipmentEdit->setStyleSheet(validValue);
        }
        if(isValid && (!canOtherInfoEmpty || !Fpl.otherInformation().isEmpty()))
        {
            aftnMessageProcessor->processType18(Fpl.otherInformation(), map, errorCodes);
            aftnMessageProcessor->validateType18(map, errorCodes);
            if(!errorCodes.isEmpty())
            {
                OtherEdit->setStyleSheet(invalidValue);
                OtherEdit->setFocus();
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
                            Fpl.setDateDOF(dateOfFlight);
                    }
                }
            }
        }
        if(bEmptyFIR)
            aftnMessageProcessor->setFIRName("");
    }

    return isValid;
}

void FplEditor::onTextChanged()
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

void FplEditor::on_ACIDEdit_editingFinished()
{
    if (mFpl.setAircraftId(ACIDEdit->text()))
        ACIDEdit->setStyleSheet(validValue);
    else
        ACIDEdit->setStyleSheet(invalidValue);
}

void FplEditor::on_EquipmentEdit_editingFinished()
{
    if (mFpl.setEquipment(EquipmentEdit->text()))
        ;//EquipmentEdit->setStyleSheet(validValue);
    else
        ;//EquipmentEdit->setStyleSheet(invalidValue);
}

void FplEditor::on_AerodromeDepEdit_editingFinished()
{
    if (mFpl.setDepartureAerodrome(AerodromeDepEdit->text()))
        AerodromeDepEdit->setStyleSheet(validValue);
    else
        AerodromeDepEdit->setStyleSheet(invalidValue);
}

void FplEditor::on_SpeedEdit_editingFinished()
{
    QString unit = SpeedCombo->currentText();
    QString value = SpeedEdit->text();

    if (mFpl.setSpeed(unit, value)) {
        SpeedCombo->setStyleSheet(validValue);
        SpeedEdit->setStyleSheet(validValue);
    } else {
        SpeedCombo->setStyleSheet(invalidValue);
        SpeedEdit->setStyleSheet(invalidValue);
    }
}

void FplEditor::on_LevelEdit_editingFinished()
{
    QString unit = LevelCombo->currentText();
    QString value = LevelEdit->text();

    if (mFpl.setLevel(unit, value)) {
        LevelCombo->setStyleSheet(validValue);
        LevelEdit->setStyleSheet(validValue);
    } else {
        LevelCombo->setStyleSheet(invalidValue);
        LevelEdit->setStyleSheet(invalidValue);
    }
}

void FplEditor::on_RouteEdit_editingFinished()
{
    if (mFpl.setRoute(RouteEdit->toPlainText()))
        RouteEdit->setStyleSheet(validValue);
    else
        RouteEdit->setStyleSheet(invalidValue);

    // Logic route control
    /*
    QVariantMap map;
    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
    bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
    if(bEmptyFIR)
        aftnMessageProcessor->setFIRName(mFpl.firName());
    aftnMessageProcessor->validateType15(map, errorCodes);
    if (!errorCodes.isEmpty()) {
        RouteEdit->setStyleSheet(invalidValue);
    } else {
        mFpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
        RouteEdit->setStyleSheet(validValue);
    }
    if(bEmptyFIR)
        aftnMessageProcessor->setFIRName("");
    */
}

void FplEditor::on_AerodromeDestEdit_editingFinished()
{
    if (mFpl.setDestAerodrome(AerodromeDestEdit->text()))
        AerodromeDestEdit->setStyleSheet(validValue);
    else
        AerodromeDestEdit->setStyleSheet(invalidValue);
}

void FplEditor::on_ALTNEdit_editingFinished()
{
    if (mFpl.setAltDestinationAerodrome(ALTNEdit->text()))
        ALTNEdit->setStyleSheet(validValue);
    else
        ALTNEdit->setStyleSheet(invalidValue);
}

void FplEditor::on_ALTN2Edit_editingFinished()
{
    if (mFpl.setAltDestinationAerodrome2(ALTN2Edit->text()))
        ALTN2Edit->setStyleSheet(validValue);
    else
        ALTN2Edit->setStyleSheet(invalidValue);
}

void FplEditor::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
        printButton->setText(tr("Print..."));
    }
}

void FplEditor::OnFixedSize()
{
    setFixedSize(width(), height());
}

void FplEditor::setCanOtherInfoEmpty(bool bCan)
{
    canOtherInfoEmpty = bCan;
}
