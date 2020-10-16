#include "rpleditor.h"
#include "rpl.h"
#include "AFTNDao.h"

#include "uppercasevalidator.h"

#include "main.h"

#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>

#include <QtSql/QSqlTableModel>

RplEditor::RplEditor(QWidget* pParent) :
  AbstractDialog(pParent), mRpl()
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

  printButton = new QPushButton(tr("Print..."), this);
  buttonBox->addButton(printButton, QDialogButtonBox::ActionRole);

  ValidFromEdit->setDate(QDateTime::currentDateTime().toUTC().date());
  ValidToEdit->setDate(QDateTime::currentDateTime().toUTC().date().addDays(1));

  //restoreState(staticMetaObject.className());

  QMetaObject::invokeMethod(this, "OnFixedSize", Qt::QueuedConnection);
}

RplEditor::~RplEditor()
{
  saveState(staticMetaObject.className());
}

void RplEditor::on_buttonBox_clicked(QAbstractButton* button)
{
  if (buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    doSavePlan();
  else if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    doApplyPlan();
  else if (button == printButton)
    printWidget(frame);
}

/** Update controls values from database */
void RplEditor::init()
{
  // FIR names
  firComboModel = new QSqlQueryModel(FIRCombo);
  firComboModel->setQuery(
    "SELECT id, id_name FROM zone WHERE type = 1 ORDER BY name");
  FIRCombo->setModel(firComboModel);
  FIRCombo->setModelColumn(1);
  FIRCombo->setCurrentIndex(0);

  // AiRplanes
  aircrafttypesModel = new QSqlTableModel(this);
  aircrafttypesModel->setTable("airplane_type");
  aircrafttypesModel->sort(0, Qt::AscendingOrder);
  aircrafttypesModel->select();
  AirplaneCombo->setModel(aircrafttypesModel);
  AirplaneCombo->setCurrentIndex(aircrafttypesModel->fieldIndex("name"));

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
}

/** Save changes (update old plan or save as new) */
void RplEditor::doSavePlan()
{
  // load RPL
  RPL Rpl = mRpl;

  if (!rpl(Rpl))
    return;
  // save RPL to database
  QSqlError error = AFTNDao::saveRPL(Rpl);
  if (!error.isValid()) {
    // close dialog
    accept();
  } else {
    QMessageBox::critical(this, tr("Error when saving RPL"), tr(
      "Error '%1' occurs when saving RPL").arg(error.text()));
  }
}

/** Save plan without closing form */
void RplEditor::doApplyPlan()
{
  // load RPL
  RPL Rpl = mRpl;

  if (!rpl(Rpl))
    return;

  // save RPL to database
  QSqlError error = AFTNDao::saveRPL(Rpl);

  if (!error.isValid()) {
    if (mRpl.isNew()) {
      mRpl = Rpl;
      setWindowTitle(tr("Editing Flight Plane #%1").arg(mRpl.getId()));
    }
  } else {
    QMessageBox::critical(this, tr("Error when saving RPL"), tr(
      "Exception '%1' occurs when saving RPL").arg(error.text()));
  }
}

void RplEditor::setRpl(const RPL& Rpl)
{
  mRpl = Rpl;
  if (mRpl.isNew())
    setWindowTitle(tr("Creating Flight Plane"));
  else
    setWindowTitle(tr("Editing Flight Plane #%1").arg(mRpl.getId()));

  ValidFromEdit->setDate(mRpl.validFrom());

  ValidToEdit->setDate(mRpl.validTo());

  TimeDepEdit->setTime(mRpl.departureTime());

  FIRCombo->setCurrentIndex(FIRCombo->findText(mRpl.firName()));

  ACIDEdit->setText(mRpl.aircarftId());

  TurbulenceCombo->setCurrentIndex(TurbulenceCombo->findText(
    mRpl.turbulenceCategory()));

  RulesCombo->setCurrentIndex(RulesCombo->findText(mRpl.flightRules()));

  FlightTypeCombo->setCurrentIndex(FlightTypeCombo->findText(mRpl.flightType()));

  NumberEdit->setValue(mRpl.aircraftNumber());

  AirplaneCombo->setCurrentIndex(AirplaneCombo->findText(mRpl.aircraftType()));
  if (AirplaneCombo->currentIndex() == -1)
    AirplaneCombo->setEditText(mRpl.aircraftType());
  AirplaneCombo->setStyleSheet(validValue);

  EquipmentEdit->setText(mRpl.equipment());
  EquipmentEdit->setStyleSheet(validValue);

  AerodromeDepEdit->setText(mRpl.departureAerodrome());
  AerodromeDepEdit->setStyleSheet(validValue);

  TimeDepEdit->setTime(mRpl.departureTime());

  entryFIREET->setText(mRpl.entryFIREET());
  entryFIREET->setStyleSheet(validValue);

  EET->setText(mRpl.EET());
  EET->setStyleSheet(validValue);

  RouteEdit->setText(mRpl.route());
  RouteEdit->setStyleSheet(validValue);

  AerodromeDestEdit->setText(mRpl.destinationAerodrome());
  AerodromeDestEdit->setStyleSheet(validValue);

  OtherEdit->setText(mRpl.otherInfo());
  OtherEdit->setStyleSheet(validValue);

  ALTNEdit->setText(mRpl.altDestinationAerodrome());
  ALTNEdit->setStyleSheet(validValue);

  ALTN2Edit->setText(mRpl.altdestinationAerodrome2());
  ALTN2Edit->setStyleSheet(validValue);

  SpeedCombo->setCurrentIndex(SpeedCombo->findText(mRpl.speedUnit()));
  SpeedCombo->setStyleSheet(validValue);

  SpeedEdit->setText(mRpl.speedValue());
  SpeedEdit->setStyleSheet(validValue);

  LevelCombo->setCurrentIndex(LevelCombo->findText(mRpl.levelUnit()));
  LevelCombo->setStyleSheet(validValue);

  LevelEdit->setText(mRpl.levelValue());
  LevelEdit->setStyleSheet(validValue);

  QString sDays = mRpl.days();
  MondayCheck->setChecked(0 < sDays.length() && '0' != sDays[0]);
  TuesdayCheck->setChecked(1 < sDays.length() && '0' != sDays[1]);
  WednesdayCheck->setChecked(2 < sDays.length() && '0' != sDays[2]);
  ThursdayCheck->setChecked(3 < sDays.length() && '0' != sDays[3]);
  FridayCheck->setChecked(4 < sDays.length() && '0' != sDays[4]);
  SaturdayCheck->setChecked(5 < sDays.length() && '0' != sDays[5]);
  SundayCheck->setChecked(6 < sDays.length() && '0' != sDays[6]);
  FIRCombo->setFocus();
}

bool RplEditor::rpl(RPL& Rpl)
{
  bool isValid = true;

  Rpl.setValidFrom(ValidFromEdit->date());

  Rpl.setValidTo(ValidToEdit->date());

  Rpl.setFirName(FIRCombo->currentText());

  if (!Rpl.setAircraftId(ACIDEdit->text())) {
    ACIDEdit->setStyleSheet(invalidValue);
    ACIDEdit->setFocus();
    isValid = false;
  } else {
    ACIDEdit->setStyleSheet(validValue);
  }

  Rpl.setFlightRules(RulesCombo->currentText());
  Rpl.setFlightType(FlightTypeCombo->currentText());
  Rpl.setAircraftNumber(NumberEdit->value());

  if (!Rpl.setAircraftType(AirplaneCombo->currentText())) {
    AirplaneCombo->setStyleSheet(invalidValue);
    AirplaneCombo->setFocus();
    isValid = false;
  } else {
    AirplaneCombo->setStyleSheet(validValue);
  }

  Rpl.setTurbulenceCategory(TurbulenceCombo->currentText());

  if (!Rpl.setEquipment(EquipmentEdit->text())) {
    EquipmentEdit->setStyleSheet(invalidValue);
    EquipmentEdit->setFocus();
    isValid = false;
  } else {
    EquipmentEdit->setStyleSheet(validValue);
  }

  if (!Rpl.setDepartureAerodrome(AerodromeDepEdit->text())) {
    AerodromeDepEdit->setStyleSheet(invalidValue);
    AerodromeDepEdit->setFocus();
    isValid = false;
  } else {
    AerodromeDepEdit->setStyleSheet(validValue);
  }

  Rpl.setDepartureTime(TimeDepEdit->time());

  if (!Rpl.setRoute(RouteEdit->toPlainText())) {
    RouteEdit->setStyleSheet(invalidValue);
    RouteEdit->setFocus();
    isValid = false;
  } else {
    RouteEdit->setStyleSheet(validValue);
  }

  if (!Rpl.setDestinationAerodrome(AerodromeDestEdit->text())) {
    AerodromeDestEdit->setStyleSheet(invalidValue);
    AerodromeDestEdit->setFocus();
    isValid = false;
  } else {
    AerodromeDestEdit->setStyleSheet(validValue);
  }

  if (!Rpl.setEntryFIREET(entryFIREET->text())) {
    entryFIREET->setStyleSheet(invalidValue);
    entryFIREET->setFocus();
    isValid = false;
  } else {
    entryFIREET->setStyleSheet(validValue);
  }

  if (!Rpl.setEET(EET->text())) {
    EET->setStyleSheet(invalidValue);
    EET->setFocus();
    isValid = false;
  } else {
    EET->setStyleSheet(validValue);
  }

  if (!Rpl.setAltDestinationAerodrome(ALTNEdit->text())) {
    ALTNEdit->setStyleSheet(invalidValue);
    ALTNEdit->setFocus();
    isValid = false;
  } else {
    ALTNEdit->setStyleSheet(validValue);
  }

  if (!Rpl.setAltDestinationAerodrome2(ALTN2Edit->text())) {
    ALTN2Edit->setStyleSheet(invalidValue);
    ALTN2Edit->setFocus();
    isValid = false;
  } else {
    ALTN2Edit->setStyleSheet(validValue);
  }

  if (!Rpl.setOtherInfo(OtherEdit->toPlainText())) {
    OtherEdit->setStyleSheet(invalidValue);
    OtherEdit->setFocus();
    isValid = false;
  } else {
    OtherEdit->setStyleSheet(validValue);
  }

  QString unit = SpeedCombo->currentText();
  QString value = SpeedEdit->text();

  if (!Rpl.setSpeed(unit, value)) {
    SpeedCombo->setStyleSheet(invalidValue);
    SpeedEdit->setStyleSheet(invalidValue);
    SpeedEdit->setFocus();
    isValid = false;
  } else {
    SpeedCombo->setStyleSheet(validValue);
    SpeedEdit->setStyleSheet(validValue);
  }

  unit = LevelCombo->currentText();
  value = LevelEdit->text();

  if (!Rpl.setLevel(unit, value)) {
    LevelCombo->setStyleSheet(invalidValue);
    LevelEdit->setStyleSheet(invalidValue);
    isValid = false;
    LevelEdit->setFocus();
  } else {
    LevelCombo->setStyleSheet(validValue);
    LevelEdit->setStyleSheet(validValue);
  }

  // Logic route control
  QVariantMap map;
  AFTNMessageProcessor::AftnMessageErrors errorCodes;
  map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
  aftnMessageProcessor->validateType15(map, errorCodes);
  if (!errorCodes.isEmpty()) {
    isValid = false;
    RouteEdit->setStyleSheet(invalidValue);
    QStringList errors = aftnMessageProcessor->errorStrings(errorCodes);
    //QMessageBox::critical(this, tr("Error"), tr("Route error. %1").arg(
    //  errors.join("\n")));
  } else {
    RouteEdit->setStyleSheet(validValue);
  }

  //other (field 18) as FPL2012
  if(isValid)
  {
      QVariantMap map;
      AFTNMessageProcessor::AftnMessageErrors errorCodes;
      bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
      if(bEmptyFIR)
          aftnMessageProcessor->setFIRName(Rpl.firName());

      aftnMessageProcessor->processType10(Rpl.equipment(), map, errorCodes);
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
      if(isValid)
      {
          aftnMessageProcessor->processType18(Rpl.otherInfo(), map, errorCodes);
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
          }
      }
      if(bEmptyFIR)
          aftnMessageProcessor->setFIRName("");
  }

  QString days;

  days.append(MondayCheck->isChecked() ? '1' : '0');
  days.append(TuesdayCheck->isChecked() ? '2' : '0');
  days.append(WednesdayCheck->isChecked() ? '3' : '0');
  days.append(ThursdayCheck->isChecked() ? '4' : '0');
  days.append(FridayCheck->isChecked() ? '5' : '0');
  days.append(SaturdayCheck->isChecked() ? '6' : '0');
  days.append(SundayCheck->isChecked() ? '7' : '0');

  Rpl.setDays(days);

  return isValid;
}

void RplEditor::on_ACIDEdit_editingFinished()
{
  if (mRpl.setAircraftId(ACIDEdit->text()))
    ACIDEdit->setStyleSheet(validValue);
  else
    ACIDEdit->setStyleSheet(invalidValue);
}

void RplEditor::on_EquipmentEdit_editingFinished()
{
  if (mRpl.setEquipment(EquipmentEdit->text()))
    ;//EquipmentEdit->setStyleSheet(validValue);
  else
    ;//EquipmentEdit->setStyleSheet(invalidValue);
}

void RplEditor::on_AerodromeDepEdit_editingFinished()
{
  if (mRpl.setDepartureAerodrome(AerodromeDepEdit->text()))
    AerodromeDepEdit->setStyleSheet(validValue);
  else
    AerodromeDepEdit->setStyleSheet(invalidValue);
}

void RplEditor::on_SpeedEdit_editingFinished()
{
  QString unit = SpeedCombo->currentText();
  QString value = SpeedEdit->text();

  if (mRpl.setSpeed(unit, value)) {
    SpeedCombo->setStyleSheet(validValue);
    SpeedEdit->setStyleSheet(validValue);
  } else {
    SpeedCombo->setStyleSheet(invalidValue);
    SpeedEdit->setStyleSheet(invalidValue);
  }
}

void RplEditor::on_LevelEdit_editingFinished()
{
  QString unit = LevelCombo->currentText();
  QString value = LevelEdit->text();

  if (mRpl.setLevel(unit, value)) {
    LevelCombo->setStyleSheet(validValue);
    LevelEdit->setStyleSheet(validValue);
  } else {
    LevelCombo->setStyleSheet(invalidValue);
    LevelEdit->setStyleSheet(invalidValue);
  }
}

void RplEditor::on_RouteEdit_editingFinished()
{
  if (mRpl.setRoute(RouteEdit->toPlainText()))
    RouteEdit->setStyleSheet(validValue);
  else
    RouteEdit->setStyleSheet(invalidValue);

  // Logic route control
  // WORKAROUND FOR ROUTE VAIDATING
  QVariantMap map;
  AFTNMessageProcessor::AftnMessageErrors errorCodes;
  map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
  aftnMessageProcessor->validateType15(map, errorCodes);
  if (!errorCodes.isEmpty()) {
    RouteEdit->setStyleSheet(invalidValue);
    QStringList errors = aftnMessageProcessor->errorStrings(errorCodes);
  } else {
    RouteEdit->setStyleSheet(validValue);
  }
}

void RplEditor::on_AerodromeDestEdit_editingFinished()
{
  if (mRpl.setDestinationAerodrome(AerodromeDestEdit->text()))
    AerodromeDestEdit->setStyleSheet(validValue);
  else
    AerodromeDestEdit->setStyleSheet(invalidValue);
}

void RplEditor::on_ALTNEdit_editingFinished()
{
  if (mRpl.setAltDestinationAerodrome(ALTNEdit->text()))
    ALTNEdit->setStyleSheet(validValue);
  else
    ALTNEdit->setStyleSheet(invalidValue);
}

void RplEditor::on_ALTN2Edit_editingFinished()
{
  if (mRpl.setAltDestinationAerodrome2(ALTN2Edit->text()))
    ALTN2Edit->setStyleSheet(validValue);
  else
    ALTN2Edit->setStyleSheet(invalidValue);
}

void RplEditor::onTextChanged()
{
  if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(sender())) {
    QString original = textEdit->toPlainText();
    QString uppered = original.toUpper();

    if (original != uppered) {
      QTextCursor textCursor = textEdit->textCursor();
      int position = textCursor.position();
      textEdit->setPlainText(uppered);
      textEdit->moveCursor(QTextCursor::Start);

      for (; position > 0; --position)
        textEdit->moveCursor(QTextCursor::NextCharacter);
    }
  }
}

void RplEditor::changeEvent(QEvent* event)
{
  if (event->type() == QEvent::LanguageChange) {
    retranslateUi(this);
    printButton->setText(tr("Print..."));
  }
  QDialog::changeEvent(event);
}

void RplEditor::on_ValidFromEdit_dateChanged(const QDate& date)
{
  if (ValidToEdit->date() < date)
    ValidToEdit->setDate(date.addDays(1));

  ValidToEdit->setMinimumDate(date.addDays(1));
}

void RplEditor::OnFixedSize()
{
    setFixedSize(width(), height());
}
