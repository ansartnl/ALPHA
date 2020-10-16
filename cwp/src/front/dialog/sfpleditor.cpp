#include "sfpleditor.h"

#include "aftnmessageprocessor.h"

#include "printutils.h"

#include "main.h"
#include "Master.h"

#include "uppercasevalidator.h"

#include <QPushButton>
#include <QMessageBox>

#include <QtSql/QSqlTableModel>

SfplEditor::SfplEditor(QWidget* pParent) :
    AbstractDialog(pParent), canOtherInfoEmpty(false)
{
    setupUi(this);
    init();

    validValue = "";
    invalidValue = "background-color: #FF637F;";

//    uppercaseValidator = new UppercaseValidator(this);
//    ACIDEdit->setValidator(uppercaseValidator);
//    AerodromeDepEdit->setValidator(uppercaseValidator);
//    AerodromeDestEdit->setValidator(uppercaseValidator);
//    EquipmentEdit->setValidator(uppercaseValidator);
//    ALTNEdit->setValidator(uppercaseValidator);
//    ALTN2Edit->setValidator(uppercaseValidator);
//    AirplaneCombo->setValidator(uppercaseValidator);
//    LevelEdit->setValidator(uppercaseValidator);
//    AirplaneCombo->lineEdit()->setMaxLength(4);

    // Print engine
//    printButton = new QPushButton(tr("Print..."), this);
//    buttonBox->addButton(printButton, QDialogButtonBox::ActionRole);
//    printButton->show();

    //restoreState(staticMetaObject.className());

    QMetaObject::invokeMethod(this, "OnFixedSize", Qt::QueuedConnection);
}


SfplEditor::~SfplEditor()
{
    saveState(staticMetaObject.className());
}

void SfplEditor::on_buttonBox_clicked(QAbstractButton* button)
{
    if (buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
        doSavePlan();
    else if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
        doApplyPlan();
    else if (button == printButton)
        printWidget(frame, QPrinter::Landscape);
}

/** Update controls values from database */
void SfplEditor::init()
{
    /*
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
    */
}

/** Save changes (update old plan or save as new) */
void SfplEditor::doSavePlan()
{
    // load FPL
    QSharedPointer<CSFPL> Sfpl(new CSFPL(mSfpl));

    // If fpl not valid
    if (!sfpl(*Sfpl))
        return;

    if (master() && master()->getRedisDB())
    {
        bool ok = master()->getRedisDB()->saveSFPL(Sfpl);

        if (ok)
            accept();
        else
            QMessageBox::critical(this, tr("Error when saving FPL"), tr("Error when saving FPL"));
    }
}

/** Save plan without closing form */
void SfplEditor::doApplyPlan(void)
{
//    // load FPL
//    FPL Fpl = mFpl;

//    if (!fpl(Fpl))
//        return;

//    // save FPL to database
//    QSqlError error = AFTNDao::saveFPL(Fpl);

//    if (!error.isValid()) {
//        if (mFpl.isNew()) {
//            mFpl = Fpl;
//            setWindowTitle(tr("Editing Flight Plane #%1").arg(mFpl.id()));
//        }
//    } else {
//        QMessageBox::critical(this, tr("Error when saving FPL"), tr(
//                                  "Exception '%1' occurs when saving FPL").arg(error.text()));
//    }
}

/** Copy information from FPL to dialog and over */
void SfplEditor::setSfpl(const CSFPL &Sfpl)
{
    mSfpl = Sfpl;

    if (mSfpl.GetUid().isNull())
        setWindowTitle(tr("Creating Flight Plane"));
    else
        setWindowTitle(tr("Editing Flight Plane #%1").arg(mSfpl.GetID()));

    ACIDEdit->setText(mSfpl.GetACID());   
    CodeEdit->setText(mSfpl.GetCODEtoString());
    CFLEdit->setText(mSfpl.GetCFL().toString());
    TFLEdit->setText(mSfpl.GetCOPinFL().toString());
    CSEdit->setText(mSfpl.GetCurSector());
    NSEdit->setText(mSfpl.GetNextSector());
    NPEdit->setText(mSfpl.GetNextPoint());
    HdgEdit->setText(mSfpl.GetHdg());
    SpdEdit->setText(mSfpl.GetSpd());
    VrcEdit->setText(mSfpl.GetVrc());
    EquipmentEdit->setText(mSfpl.GetEquipment());
    AerodromeDepEdit->setText(mSfpl.GetDepAerodrome());
    AerodromeDestEdit->setText(mSfpl.GetDestAerodrome());
}

bool SfplEditor::sfpl(CSFPL& Sfpl)
{
    bool isValid = true;

    Sfpl.SetACID(ACIDEdit->text());  
    Sfpl.SetCODE(CodeEdit->text().toInt(0, 8));
    Sfpl.SetCFL(FLVal(CFLEdit->text()));
    Sfpl.SetCOPinFL(FLVal(TFLEdit->text()));
    Sfpl.SetCurSector(CSEdit->text());
    Sfpl.SetNextSector(NSEdit->text());
    Sfpl.SetNextPoint(NPEdit->text());
    Sfpl.SetHdg(HdgEdit->text());
    Sfpl.SetSpd(SpdEdit->text());
    Sfpl.SetVrc(VrcEdit->text());
    Sfpl.SetEquipment(EquipmentEdit->text());
    Sfpl.SetDepAerodrome(AerodromeDepEdit->text());
    Sfpl.SetDestAerodrome(AerodromeDestEdit->text());

    return isValid;
}

void SfplEditor::onTextChanged()
{
//    if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(sender())) {
//        QString original = textEdit->toPlainText();
//        QString uppered = original.toUpper();

//        if (original != uppered) {
//            QTextCursor textCursor = textEdit->textCursor();
//            int iPosition = textCursor.position();
//            textEdit->setPlainText(uppered);
//            textEdit->moveCursor(QTextCursor::Start);

//            for (; iPosition > 0; --iPosition)
//                textEdit->moveCursor(QTextCursor::NextCharacter);
//        }
//    }
}

/*
void SfplEditor::on_ACIDEdit_editingFinished()
{
//    if (mFpl.setAircraftId(ACIDEdit->text()))
//        ACIDEdit->setStyleSheet(validValue);
//    else
//        ACIDEdit->setStyleSheet(invalidValue);
}

void SfplEditor::on_EquipmentEdit_editingFinished()
{
//    if (mFpl.setEquipment(EquipmentEdit->text()))
        ;//EquipmentEdit->setStyleSheet(validValue);
//    else
        ;//EquipmentEdit->setStyleSheet(invalidValue);
}

void SfplEditor::on_AerodromeDepEdit_editingFinished()
{
//    if (mFpl.setDepartureAerodrome(AerodromeDepEdit->text()))
//        AerodromeDepEdit->setStyleSheet(validValue);
//    else
//        AerodromeDepEdit->setStyleSheet(invalidValue);
}

void SfplEditor::on_SpeedEdit_editingFinished()
{
//    QString unit = SpeedCombo->currentText();
//    QString value = SpeedEdit->text();

//    if (mFpl.setSpeed(unit, value)) {
//        SpeedCombo->setStyleSheet(validValue);
//        SpeedEdit->setStyleSheet(validValue);
//    } else {
//        SpeedCombo->setStyleSheet(invalidValue);
//        SpeedEdit->setStyleSheet(invalidValue);
//    }
}

void SfplEditor::on_LevelEdit_editingFinished()
{
//    QString unit = LevelCombo->currentText();
//    QString value = LevelEdit->text();

//    if (mFpl.setLevel(unit, value)) {
//        LevelCombo->setStyleSheet(validValue);
//        LevelEdit->setStyleSheet(validValue);
//    } else {
//        LevelCombo->setStyleSheet(invalidValue);
//        LevelEdit->setStyleSheet(invalidValue);
//    }
}

void SfplEditor::on_RouteEdit_editingFinished()
{
//    if (mFpl.setRoute(RouteEdit->toPlainText()))
//        RouteEdit->setStyleSheet(validValue);
//    else
//        RouteEdit->setStyleSheet(invalidValue);

    // Logic route control

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

}

void SfplEditor::on_AerodromeDestEdit_editingFinished()
{
//    if (mFpl.setDestAerodrome(AerodromeDestEdit->text()))
//        AerodromeDestEdit->setStyleSheet(validValue);
//    else
//        AerodromeDestEdit->setStyleSheet(invalidValue);
}

void SfplEditor::on_ALTNEdit_editingFinished()
{
//    if (mFpl.setAltDestinationAerodrome(ALTNEdit->text()))
//        ALTNEdit->setStyleSheet(validValue);
//    else
//        ALTNEdit->setStyleSheet(invalidValue);
}

void SfplEditor::on_ALTN2Edit_editingFinished()
{
//    if (mFpl.setAltDestinationAerodrome2(ALTN2Edit->text()))
//        ALTN2Edit->setStyleSheet(validValue);
//    else
//        ALTN2Edit->setStyleSheet(invalidValue);
}
*/

void SfplEditor::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
        printButton->setText(tr("Print..."));
    }
}

void SfplEditor::OnFixedSize()
{
    setFixedSize(width(), height());
}

void SfplEditor::setCanOtherInfoEmpty(bool bCan)
{
    canOtherInfoEmpty = bCan;
}
