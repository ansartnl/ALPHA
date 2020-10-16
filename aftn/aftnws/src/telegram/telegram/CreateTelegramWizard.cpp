#include "CreateTelegramWizard.h"

#include "main.h"
//#include "AirPlanModel.h"
#include "ArrWidget.h"
#include "ChgWidget.h"
#include "CnlWidget.h"
#include "DepWidget.h"
#include "DlaWidget.h"
#include "FplTelegramWidget.h"
#include "InternalClient.h"

#include "AtsMessages.h"
#include "requests.h"
#include "abonentdlg.h"
#include "getmapdialog.h"

#include "aftnmessageprocessor.h"
#include "qttranslator.h"

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

//const int n_pageFirst = 0;
//const int n_pageFpl = 1;
//const int n_pageEdit = 2;
////const int n_pageAftn = 3;
//const int n_pagePreview = 3;

const int n_indexFreeText = 0;
const int n_indexFpl = 1;
const int n_indexArr = 2;
const int n_indexChg = 3;
const int n_indexCnl = 4;
const int n_indexDep = 5;
const int n_indexDla = 6;
//const int n_indexFpl1 = 7;
//const int n_indexArr1 = 8;
//const int n_indexChg1 = 9;
//const int n_indexCnl1 = 10;
//const int n_indexDep1 = 11;
//const int n_indexDla1 = 12;

CreateTelegramWizard::CreateTelegramWizard(QWidget *parent) :
    QDialog(parent), client(0)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    telegramTypeComboBox->addItems(QStringList()
                                   << "Free text"
                                   << Ats::FPL
                                   << Ats::ARR
                                   << Ats::CHG
                                   << Ats::CNL
                                   << Ats::DEP
                                   << Ats::DLA);

    mProcessor = new AFTNMessageProcessor;
    mProcessor->setValidateRoute(false);
    mProcessor->setValidateRouteLite(false);
    mProcessor->setSaveOnlyFIR(false);
    mProcessor->setFIRName(QString());
    mProcessor->setStandard(networkSettings()->value("aftn_message_processor/FPL2012").toBool()
                           ? AFTNMessageProcessor::FPL_2012 : AFTNMessageProcessor::FPL_2007);

    connect(telegramTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onTelegramTypeChange(int)));

    while(stackedWidget->count())
        stackedWidget->removeWidget(stackedWidget->widget(0));
    onTelegramTypeChange(telegramTypeComboBox->currentIndex());

    loadSettings();
    Translate();

    connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

    connect(saveTemplateButton, SIGNAL(clicked()), SLOT(onSaveTemplate()));
    connect(loadTemplateButton, SIGNAL(clicked()), SLOT(onLoadTemplate()));

#ifdef DUSHANBE_BUILD
    saveTemplateButton->setVisible(false);
    loadTemplateButton->setVisible(false);
#endif
}

CreateTelegramWizard::~CreateTelegramWizard()
{
    saveSettings();
}

void CreateTelegramWizard::setInternalClient(InternalClient *c)
{
    client = c;
}

void CreateTelegramWizard::accept()
{
    switch (telegramTypeComboBox->currentIndex())
    {
    case n_indexFreeText:
        break;
    case n_indexFpl:
        if(!fplWidget()->validate())
            return;
        break;
    case n_indexArr:
        if(!arrWidget()->validate())
            return;
        break;
    case n_indexChg:
        if(!chgWidget()->validate())
            return;
        break;
    case n_indexCnl:
        if(!cnlWidget()->validate())
            return;
        break;
    case n_indexDep:
        if(!depWidget()->validate())
            return;
        break;
    case n_indexDla:
        if(!dlaWidget()->validate())
            return;
        break;
    }
    if (send())
        QDialog::accept();
}

void CreateTelegramWizard::setDictionary(const ReplyDictionary &d)
{
    if (d.version == ParseVersion::n_4444)
        dictionary = d;
    else if (d.version == ParseVersion::n_TC95)
        dictionaryTC95 = d;
}

void CreateTelegramWizard::createTelegramBasedOn(const QString &type, const FplStruct &base)
{
//    telegramTypeComboBox->setCurrentIndex(telegramTypeComboBox->findText(type));
//    updateModel();
////    if (model->rowCount())
////        fplView->selectRow(model->rowByFpl(base));
////    else
//        outerBaseFpl = new FplStruct(base);
//    stackedWidget->setCurrentIndex(n_pageEdit);
//    updateWidgets();
}

void CreateTelegramWizard::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
    {
        retranslateUi(this);
        Translate();
        break;
    }
    default:
        break;
    }
}

void CreateTelegramWizard::Translate()
{
    QPushButton* pButton = buttonBox->button(QDialogButtonBox::Ok);
    if(pButton) pButton->setText(tr("Send"));
}

void CreateTelegramWizard::loadSettings()
{
//    QSettings settings;
//    settings.beginGroup(sysCreateTelegramWizard);
//    restoreGeometry(settings.value(sysWindowGeometry).toByteArray());
//    settings.endGroup();
}

void CreateTelegramWizard::saveSettings()
{
//    QSettings settings;
//    settings.beginGroup(sysCreateTelegramWizard);
//    settings.setValue(sysWindowGeometry, saveGeometry());
//    settings.endGroup();
}

QTextEdit * CreateTelegramWizard::freeTextWidget() const
{
    return qobject_cast < QTextEdit * >(stackedWidget->widget(n_indexFreeText));
}

FplTelegramWidget * CreateTelegramWizard::fplWidget() const
{
    return qobject_cast < FplTelegramWidget * >(stackedWidget->widget(n_indexFpl));
}

ArrWidget * CreateTelegramWizard::arrWidget() const
{
    return qobject_cast < ArrWidget * >(stackedWidget->widget(n_indexArr));
}

ChgWidget * CreateTelegramWizard::chgWidget() const
{
    return qobject_cast < ChgWidget * >(stackedWidget->widget(n_indexChg));
}

CnlWidget * CreateTelegramWizard::cnlWidget() const
{
    return qobject_cast < CnlWidget * >(stackedWidget->widget(n_indexCnl));
}

DepWidget * CreateTelegramWizard::depWidget() const
{
    return qobject_cast < DepWidget * >(stackedWidget->widget(n_indexDep));
}

DlaWidget * CreateTelegramWizard::dlaWidget() const
{
    return qobject_cast < DlaWidget * >(stackedWidget->widget(n_indexDla));
}

void CreateTelegramWizard::fillDictionary()
{
//    if (telegramEdit)
//    {
//        switch (telegramTypeComboBox->currentIndex())
//        {
//        case n_indexFreeText:
//            break;
//        case n_indexFpl:
//            fplWidget()->setDictionary(dictionary);
//            break;
//        case n_indexArr:
//            arrWidget()->setDictionary(dictionary);
//            break;
//        case n_indexChg:
//            chgWidget()->setDictionary(dictionary);
//            break;
//        case n_indexCnl:
//            cnlWidget()->setDictionary(dictionary);
//            break;
//        case n_indexDep:
//            depWidget()->setDictionary(dictionary);
//            break;
//        case n_indexDla:
//            dlaWidget()->setDictionary(dictionary);
//            break;
//        case n_indexFpl1:
//            fplWidget()->setDictionary(dictionaryTC95);
//            break;
//        case n_indexArr1:
//            arrWidget()->setDictionary(dictionaryTC95);
//            break;
//        case n_indexChg1:
//            chgWidget()->setDictionary(dictionaryTC95);
//            break;
//        case n_indexCnl1:
//            cnlWidget()->setDictionary(dictionaryTC95);
//            break;
//        case n_indexDep1:
//            depWidget()->setDictionary(dictionaryTC95);
//            break;
//        case n_indexDla1:
//            dlaWidget()->setDictionary(dictionaryTC95);
//            break;
//        }
//    }
}


void CreateTelegramWizard::fillPreview()
{
//    if (telegramEdit)
//    {
//        AftnStruct st = aftnStruct();
//        st.transmissionIdentificator = "ZZZ000";
//        QDateTime ct = QDateTime::currentDateTimeUtc();
//        st.fillingDate = ct.date().day();
//        st.fillingHour = ct.time().hour();
//        st.fillingMinute = ct.time().minute();
//        st.senderAddress = "SENDYYYY";
//        AftnMessage a(st);
//        telegramPreview->setText(a.toByteArray());
//    }
}

bool CreateTelegramWizard::send()
{
    AftnStruct s = aftnStruct();
    if (s.text.contains("ZCZC") || s.text.contains("NNNN") || s.text.contains("+.+.") || s.text.contains(",,,,"))
    {
        QMessageBox::warning(this, tr("Error")
                             , tr("Message contains one of forbidden character sequences: ")
                             + "'ZCZC', '+.+.', 'NNNN', ',,,,'");
        return false;
    }
    SendTelegram msg(s);
    //client << msg;

    QVariant var;
    var.setValue(msg);
    client->send(var);
    close();
    return true;
}

AftnStruct CreateTelegramWizard::aftnStruct() const
{
    AftnStruct s;
//    s.additionServiceIndication = aftnHeaderWidget->additionalInfo();
//    s.priorityIndicator = aftnHeaderWidget->urgency();
    s.receiverAddresses = addressesLineEdit->text().split(" ", QString::SkipEmptyParts);
    QString type = telegramTypeComboBox->currentText();
    if (Ats::isFpl(type))
    {
        FplMessage mes(fplWidget()->fpl());
        s.text = mes.toAftn();
    } else if (Ats::isArr(type))
    {
        ArrMessage mes(arrWidget()->arr());
        s.text = mes.toAftn();
    } else if (Ats::isChg(type))
    {
        ChgMessage mes(chgWidget()->chg());
        s.text = mes.toAftn();
    } else if (Ats::isCnl(type))
    {
        CnlMessage mes(cnlWidget()->cnl());
        s.text = mes.toAftn();
    } else if (Ats::isDep(type))
    {
        DepMessage mes(depWidget()->dep());
        s.text = mes.toAftn();
    } else if (Ats::isDla(type))
    {
        DlaMessage mes(dlaWidget()->dla());
        s.text = mes.toAftn();
    } else
    {
        s.text = freeTextWidget()->toPlainText();
    }
    if (Ats::isTC95(type))
        s.version = ParseVersion::n_TC95;
    else
        s.version = ParseVersion::n_4444;
    return s;
}

#ifndef DUSHANBE_BUILD
void CreateTelegramWizard::autoFillAddresses()
{
    QString type;
    switch (telegramTypeComboBox->currentIndex())
    {
    case n_indexFreeText:
        type = "free_text";
        break;
    case n_indexFpl:
        type = "fpl";
        break;
    case n_indexArr:
        type = "arr";
        break;
    case n_indexChg:
        type = "chg";
        break;
    case n_indexCnl:
        type = "cnl";
        break;
    case n_indexDep:
        type = "dep";
        break;
    case n_indexDla:
        type = "dla";
        break;
    }
    QSqlQuery query;
    query.prepare(QString("SELECT a1.address FROM "
                          "aftn_abonent a1 "
                          "JOIN aftn_abonent_autotypes a2 ON a1.id = a2.aftn_abonent_id "
                          "WHERE a2.%1 = 1").arg(type));
    QStringList addresses;
    if (query.exec())
    {
        while (query.next())
            addresses << query.value(0).toString();
    } else
    {
        qWarning() << query.lastError().text();
    }
    addressesLineEdit->setText(addresses.join(" "));
}
#endif

void CreateTelegramWizard::saveFreeTextTemplate(const QString &name)
{
    QSqlQuery query;
    query.prepare("INSERT INTO template_free_text (id, name, text) VALUES (default, :name, :text)");
    query.bindValue(":name", name);
    query.bindValue(":text", freeTextWidget()->toPlainText());
    if (!query.exec())
        qWarning() << query.lastError().text();
}

void CreateTelegramWizard::saveFplTemplate(const QString &name)
{
    FplStruct fpl = fplWidget()->fpl();
    QSqlQuery query;
    query.prepare("INSERT INTO template_fpl (id, name, aircraft, taircraft, frules, tflight, num, wtc, "
                  "equipment, adep, departure_time, speed, flevel, route, ades, eet, alt1, alt2, other) "
                  "VALUES (default, :name, :aircraft, :taircraft, :frules, :tflight, :num, :wtc, "
                  ":equipment, :adep, :departure_time, :speed, :flevel, :route, :ades, :eet, :alt1, :alt2, "
                  ":other)");
    query.bindValue(":name", name);
    query.bindValue(":aircraft", fpl.ACID);
    query.bindValue(":taircraft", fpl.aircraftType);
    query.bindValue(":frules", fpl.flightRules);
    query.bindValue(":tflight", fpl.typeOfFlight);
    query.bindValue(":num", fpl.numberOfAircrafts);
    query.bindValue(":wtc", fpl.turbulence);
    query.bindValue(":equipment", fpl.equipment);
    query.bindValue(":adep", fpl.ADEP);
    query.bindValue(":departure_time", fpl.departureTime);
    query.bindValue(":speed", fpl.speed);
    query.bindValue(":flevel", fpl.flightLevel);
    query.bindValue(":route", fpl.route);
    query.bindValue(":ades", fpl.ADES);
    query.bindValue(":eet", fpl.dtADES);
    query.bindValue(":alt1", fpl.alt1);
    query.bindValue(":alt2", fpl.alt2);
    query.bindValue(":other", fpl.other);
    if (!query.exec())
        qWarning() << query.lastError().text();
}

void CreateTelegramWizard::saveArrTemplate(const QString &name)
{
    ArrStruct arr = arrWidget()->arr();
    QSqlQuery query;
    query.prepare("INSERT INTO template_arr (id, name, aircraft, adep, ades, arrival_airport, "
                  "arrival_time, arrival_airport_name, other) "
                  "VALUES (default, :name, :aircraft, :adep, :ades, :arrival_airport, :arrival_time, "
                  ":arrival_airport_name, :other)");
    query.bindValue(":name", name);
    query.bindValue(":aircraft", arr.ACID);
    query.bindValue(":adep", arr.ADEP);
    query.bindValue(":ades", arr.ADES);
    query.bindValue(":arrival_airport", arr.arrivalAirport);
    query.bindValue(":arrival_time", arr.arrivalTime);
    query.bindValue(":arrival_airport_name", arr.arrivalAirportName);
    query.bindValue(":other", "");
    if (!query.exec())
        qWarning() << query.lastError().text();
}

void CreateTelegramWizard::saveChgTemplate(const QString &name)
{
    ChgStruct chg = chgWidget()->chg();
    QSqlQuery query;
    query.prepare("INSERT INTO template_chg (id, name, aircraft, adep, ades, other, taircraft, frules, "
                  "tflight, num, wtc, equipment, chg_adep, departure_time, speed, flevel, route, chg_ades, "
                  "eet, alt1, alt2, chg_other, changes_flags) "
                  "VALUES (default, :name, :aircraft, :adep, :ades, :other, :taircraft, :frules, :tflight, "
                  ":num, :wtc, :equipment, :chg_adep, :departure_time, :speed, :flevel, :route, :chg_ades, "
                  ":eet, :alt1, :alt2, :chg_other, :changes_flags)");
    query.bindValue(":name", name);
    query.bindValue(":aircraft", chg.ACID);
    query.bindValue(":adep", chg.ADEP);
    query.bindValue(":ades", chg.ADES);
    query.bindValue(":other", "");
    query.bindValue(":taircraft", chg.aircraftType);
    query.bindValue(":frules", chg.flightRules);
    query.bindValue(":tflight", chg.typeOfFlight);
    query.bindValue(":num", chg.numberOfAircrafts);
    query.bindValue(":wtc", chg.turbulence);
    query.bindValue(":equipment", chg.equipment);
    query.bindValue(":chg_adep", chg.changedADEP);
    query.bindValue(":departure_time", chg.departureTime);
    query.bindValue(":speed", chg.speed);
    query.bindValue(":flevel", chg.flightLevel);
    query.bindValue(":chg_ades", chg.changedADES);
    query.bindValue(":route", chg.route);
    query.bindValue(":eet", chg.dtADES);
    query.bindValue(":alt1", chg.alt1);
    query.bindValue(":alt2", chg.alt2);
    query.bindValue(":chg_other", chg.other);
    query.bindValue(":changes_flags", int(chg.changesFlags));
    if (!query.exec())
        qWarning() << query.lastError().text();
}

void CreateTelegramWizard::saveCnlTemplate(const QString &name)
{
    CnlStruct cnl = cnlWidget()->cnl();
    QSqlQuery query;
    query.prepare("INSERT INTO template_cnl (id, name, aircraft, adep, ades, other) "
                  "VALUES (default, :name, :aircraft, :adep, :ades, :other)");
    query.bindValue(":name", name);
    query.bindValue(":aircraft", cnl.ACID);
    query.bindValue(":adep", cnl.ADEP);
    query.bindValue(":ades", cnl.ADES);
    query.bindValue(":other", "");
    if (!query.exec())
        qWarning() << query.lastError().text();
}

void CreateTelegramWizard::saveDepTemplate(const QString &name)
{
    DepStruct dep = depWidget()->dep();
    QSqlQuery query;
    query.prepare("INSERT INTO template_dep (id, name, aircraft, adep, departure_time, ades, other) "
                  "VALUES (default, :name, :aircraft, :adep, :departure_time, :ades, :other)");
    query.bindValue(":name", name);
    query.bindValue(":aircraft", dep.ACID);
    query.bindValue(":adep", dep.ADEP);
    query.bindValue(":departure_time", dep.departureTime);
    query.bindValue(":ades", dep.ADES);
    query.bindValue(":other", "");
    if (!query.exec())
        qWarning() << query.lastError().text();
}

void CreateTelegramWizard::saveDlaTemplate(const QString &name)
{
    DlaStruct dla = dlaWidget()->dla();
    QSqlQuery query;
    query.prepare("INSERT INTO template_dla (id, name, aircraft, adep, departure_time, ades, other) "
                  "VALUES (default, :name, :aircraft, :adep, :departure_time, :ades, :other)");
    query.bindValue(":name", name);
    query.bindValue(":aircraft", dla.ACID);
    query.bindValue(":adep", dla.ADEP);
    query.bindValue(":departure_time", dla.departureTime);
    query.bindValue(":ades", dla.ADES);
    query.bindValue(":other", "");
    if (!query.exec())
        qWarning() << query.lastError().text();
}

void CreateTelegramWizard::loadFreeTextTemplate()
{
    ValueMap vMap;

    QSqlQuery selectList;
    selectList.prepare("SELECT id, name, text FROM template_free_text");
    if (!selectList.exec())
    {
        qWarning() << selectList.lastError().text();
        return;
    }
    while (selectList.next())
        vMap.insert(selectList.value(0).toInt()
                    , Value(selectList.value(1).toString(), selectList.value(2).toString()));

    if (vMap.isEmpty())
    {
        QMessageBox::warning(this, tr("Free text templates"), tr("There are no saved templates"));
        return;
    }

    bool ok;
    int id = GetMapDialog::getIndex(this, tr("Templates"), tr("Select template:"), vMap, &ok);
    if (!ok)
        return;

    QSqlQuery select;
    select.prepare("SELECT text FROM template_free_text WHERE id = :id");
    select.bindValue(":id", id);
    if (!select.exec())
    {
        qWarning() << select.lastError().text();
        return;
    }
    if (!select.next())
    {
        qWarning() << "Could not find free text template with id " << id;
        return;
    }
    freeTextWidget()->setText(select.value(0).toString());
}

void CreateTelegramWizard::loadFplTemplate()
{
    ValueMap vMap;

    QSqlQuery selectList;
    selectList.prepare("SELECT id, name, aircraft, taircraft, frules, tflight, num, wtc, equipment, "
                       "adep, departure_time, speed, flevel, route, ades, eet, alt1, alt2, other "
                       "FROM template_fpl");
    if (!selectList.exec())
    {
        qWarning() << selectList.lastError().text();
        return;
    }
    while (selectList.next())
    {
        FplStruct fpl;
        fpl.ACID = selectList.value(2).toString();
        fpl.aircraftType = selectList.value(3).toString();
        fpl.flightRules = selectList.value(4).toString();
        fpl.typeOfFlight = selectList.value(5).toString();
        fpl.numberOfAircrafts = selectList.value(6).toInt();
        fpl.turbulence = selectList.value(7).toString();
        fpl.equipment = selectList.value(8).toString();
        fpl.ADEP = selectList.value(9).toString();
        fpl.departureTime = selectList.value(10).toDateTime();
        fpl.dtADEP = fpl.departureTime.toString("hh:mm");
        fpl.speed = selectList.value(11).toString();
        fpl.flightLevel = selectList.value(12).toString();
        fpl.route = selectList.value(13).toString();
        fpl.ADES = selectList.value(14).toString();
        fpl.dtADES = selectList.value(15).toString();
        fpl.alt1 = selectList.value(16).toString();
        fpl.alt2 = selectList.value(17).toString();
        fpl.other = selectList.value(18).toString();
        vMap.insert(selectList.value(0).toInt()
                    , Value(selectList.value(1).toString(), FplMessage(fpl).toAftn()));
    }

    if (vMap.isEmpty())
    {
        QMessageBox::warning(this, tr("FPL templates"), tr("There are no saved templates"));
        return;
    }

    bool ok;
    int id = GetMapDialog::getIndex(this, tr("Templates"), tr("Select template:"), vMap, &ok);
    if (!ok)
        return;

    QSqlQuery select;
    select.prepare("SELECT aircraft, taircraft, frules, tflight, num, wtc, equipment, adep, "
                   "departure_time, speed, flevel, route, ades, eet, alt1, alt2, other "
                   "FROM template_fpl WHERE id = :id");
    select.bindValue(":id", id);
    if (!select.exec())
    {
        qWarning() << select.lastError().text();
        return;
    }
    if (!select.next())
    {
        qWarning() << "Could not find FPL template with id " << id;
        return;
    }
    FplStruct fpl;
    fpl.ACID = select.value(0).toString();
    fpl.aircraftType = select.value(1).toString();
    fpl.flightRules = select.value(2).toString();
    fpl.typeOfFlight = select.value(3).toString();
    fpl.numberOfAircrafts = select.value(4).toInt();
    fpl.turbulence = select.value(5).toString();
    fpl.equipment = select.value(6).toString();
    fpl.ADEP = select.value(7).toString();
    fpl.departureTime = select.value(8).toDateTime();
    fpl.speed = select.value(9).toString();
    fpl.flightLevel = select.value(10).toString();
    fpl.route = select.value(11).toString();
    fpl.ADES = select.value(12).toString();
    fpl.dtADES = select.value(13).toString();
    fpl.alt1 = select.value(14).toString();
    fpl.alt2 = select.value(15).toString();
    fpl.other = select.value(16).toString();
    fplWidget()->setFpl(fpl);
}

void CreateTelegramWizard::loadArrTemplate()
{
    ValueMap vMap;

    QSqlQuery selectList;
    selectList.prepare("SELECT id, name, aircraft, adep, ades, arrival_airport, "
                       "arrival_time, arrival_airport_name, other "
                       "FROM template_arr");
    if (!selectList.exec())
    {
        qWarning() << selectList.lastError().text();
        return;
    }
    while (selectList.next())
    {
        ArrStruct arr;
        arr.ACID = selectList.value(2).toString();
        arr.ADEP = selectList.value(3).toString();
        arr.ADES = selectList.value(4).toString();
        arr.arrivalAirport = selectList.value(5).toString();
        arr.arrivalTime = selectList.value(6).toDateTime();
        arr.arrivalAirportName = selectList.value(7).toString();
        vMap.insert(selectList.value(0).toInt()
                    , Value(selectList.value(1).toString(), ArrMessage(arr).toAftn()));
    }

    if (vMap.isEmpty())
    {
        QMessageBox::warning(this, tr("ARR templates"), tr("There are no saved templates"));
        return;
    }

    bool ok;
    int id = GetMapDialog::getIndex(this, tr("Templates"), tr("Select template:"), vMap, &ok);
    if (!ok)
        return;

    QSqlQuery select;
    select.prepare("SELECT aircraft, adep, ades, arrival_airport, "
                   "arrival_time, arrival_airport_name, other "
                   "FROM template_arr WHERE id = :id");
    select.bindValue(":id", id);
    if (!select.exec())
    {
        qWarning() << select.lastError().text();
        return;
    }
    if (!select.next())
    {
        qWarning() << "Could not find ARR template with id " << id;
        return;
    }
    ArrStruct arr;
    arr.ACID = select.value(0).toString();
    arr.ADEP = select.value(1).toString();
    arr.ADES = select.value(2).toString();
    arr.arrivalAirport = select.value(3).toString();
    arr.arrivalTime = select.value(4).toDateTime();
    arr.arrivalAirportName = select.value(5).toString();
    arrWidget()->setArr(arr);
}

void CreateTelegramWizard::loadChgTemplate()
{
    ValueMap vMap;

    QSqlQuery selectList;
    selectList.prepare("SELECT id, name, aircraft, adep, ades, other, taircraft, frules, tflight, num, "
                       "wtc, equipment, chg_adep, departure_time, speed, flevel, route, chg_ades, "
                       "eet, alt1, alt2, chg_other, changes_flags "
                       "FROM template_chg");
    if (!selectList.exec())
    {
        qWarning() << selectList.lastError().text();
        return;
    }
    while (selectList.next())
    {
        ChgStruct chg;
        chg.ACID = selectList.value(2).toString();
        chg.ADEP = selectList.value(3).toString();
        chg.ADES = selectList.value(4).toString();

        chg.aircraftType = selectList.value(6).toString();
        chg.flightRules = selectList.value(7).toString();
        chg.typeOfFlight = selectList.value(8).toString();
        chg.numberOfAircrafts = selectList.value(9).toInt();
        chg.turbulence = selectList.value(10).toString();
        chg.equipment = selectList.value(11).toString();
        chg.changedADEP = selectList.value(12).toString();
        chg.departureTime = selectList.value(13).toDateTime();
        chg.dtADEP = chg.departureTime.toString("hh:mm");
        chg.speed = selectList.value(14).toString();
        chg.flightLevel = selectList.value(15).toString();
        chg.route = selectList.value(16).toString();
        chg.changedADES = selectList.value(17).toString();
        chg.dtADES = selectList.value(18).toString();
        chg.alt1 = selectList.value(19).toString();
        chg.alt2 = selectList.value(20).toString();
        chg.other = selectList.value(21).toString();
        chg.changesFlags = ChgStruct::Changes(selectList.value(22).toInt());
        vMap.insert(selectList.value(0).toInt()
                    , Value(selectList.value(1).toString(), ChgMessage(chg).toAftn()));
    }

    if (vMap.isEmpty())
    {
        QMessageBox::warning(this, tr("CHG templates"), tr("There are no saved templates"));
        return;
    }

    bool ok;
    int id = GetMapDialog::getIndex(this, tr("Templates"), tr("Select template:"), vMap, &ok);
    if (!ok)
        return;

    QSqlQuery select;
    select.prepare("SELECT aircraft, adep, ades, other, taircraft, frules, tflight, num, "
                   "wtc, equipment, chg_adep, departure_time, speed, flevel, route, chg_ades, "
                   "eet, alt1, alt2, chg_other, changes_flags "
                   "FROM template_chg WHERE id = :id");
    select.bindValue(":id", id);
    if (!select.exec())
    {
        qWarning() << select.lastError().text();
        return;
    }
    if (!select.next())
    {
        qWarning() << "Could not find CHG template with id " << id;
        return;
    }
    ChgStruct chg;
    chg.ACID = select.value(0).toString();
    chg.ADEP = select.value(1).toString();
    chg.ADES = select.value(2).toString();

    chg.aircraftType = select.value(4).toString();
    chg.flightRules = select.value(5).toString();
    chg.typeOfFlight = select.value(6).toString();
    chg.numberOfAircrafts = select.value(7).toInt();
    chg.turbulence = select.value(8).toString();
    chg.equipment = select.value(9).toString();
    chg.changedADEP = select.value(10).toString();
    chg.departureTime = select.value(11).toDateTime();
    chg.speed = select.value(12).toString();
    chg.flightLevel = select.value(13).toString();
    chg.route = select.value(14).toString();
    chg.changedADES = select.value(15).toString();
    chg.dtADES = select.value(16).toString();
    chg.alt1 = select.value(17).toString();
    chg.alt2 = select.value(18).toString();
    chg.other = select.value(19).toString();
    chg.changesFlags = ChgStruct::Changes(select.value(20).toInt());
    chgWidget()->setChg(chg);
}

void CreateTelegramWizard::loadCnlTemplate()
{
    ValueMap vMap;

    QSqlQuery selectList;
    selectList.prepare("SELECT id, name, aircraft, adep, ades, other "
                       "FROM template_cnl");
    if (!selectList.exec())
    {
        qWarning() << selectList.lastError().text();
        return;
    }
    while (selectList.next())
    {
        CnlStruct cnl;
        cnl.ACID = selectList.value(2).toString();
        cnl.ADEP = selectList.value(3).toString();
        cnl.ADES = selectList.value(4).toString();
        vMap.insert(selectList.value(0).toInt()
                    , Value(selectList.value(1).toString(), CnlMessage(cnl).toAftn()));
    }

    if (vMap.isEmpty())
    {
        QMessageBox::warning(this, tr("CNL templates"), tr("There are no saved templates"));
        return;
    }

    bool ok;
    int id = GetMapDialog::getIndex(this, tr("Templates"), tr("Select template:"), vMap, &ok);
    if (!ok)
        return;

    QSqlQuery select;
    select.prepare("SELECT aircraft, adep, ades, other "
                   "FROM template_cnl WHERE id = :id");
    select.bindValue(":id", id);
    if (!select.exec())
    {
        qWarning() << select.lastError().text();
        return;
    }
    if (!select.next())
    {
        qWarning() << "Could not find CNL template with id " << id;
        return;
    }
    CnlStruct cnl;
    cnl.ACID = select.value(0).toString();
    cnl.ADEP = select.value(1).toString();
    cnl.ADES = select.value(2).toString();
    cnlWidget()->setCnl(cnl);
}

void CreateTelegramWizard::loadDepTemplate()
{
    ValueMap vMap;

    QSqlQuery selectList;
    selectList.prepare("SELECT id, name, aircraft, adep, departure_time, ades, other "
                       "FROM template_dep");
    if (!selectList.exec())
    {
        qWarning() << selectList.lastError().text();
        return;
    }
    while (selectList.next())
    {
        DepStruct dep;
        dep.ACID = selectList.value(2).toString();
        dep.ADEP = selectList.value(3).toString();
        dep.departureTime = selectList.value(4).toDateTime();
        dep.dtADEP = dep.departureTime.toString("hh:mm");
        dep.ADES = selectList.value(5).toString();
        vMap.insert(selectList.value(0).toInt()
                    , Value(selectList.value(1).toString(), DepMessage(dep).toAftn()));
    }

    if (vMap.isEmpty())
    {
        QMessageBox::warning(this, tr("DEP templates"), tr("There are no saved templates"));
        return;
    }

    bool ok;
    int id = GetMapDialog::getIndex(this, tr("Templates"), tr("Select template:"), vMap, &ok);
    if (!ok)
        return;

    QSqlQuery select;
    select.prepare("SELECT aircraft, adep, departure_time, ades, other "
                   "FROM template_dep WHERE id = :id");
    select.bindValue(":id", id);
    if (!select.exec())
    {
        qWarning() << select.lastError().text();
        return;
    }
    if (!select.next())
    {
        qWarning() << "Could not find DEP template with id " << id;
        return;
    }
    DepStruct dep;
    dep.ACID = select.value(0).toString();
    dep.ADEP = select.value(1).toString();
    dep.departureTime = select.value(2).toDateTime();
    dep.dtADEP = dep.departureTime.toString("hh:mm");
    dep.ADES = select.value(3).toString();
    depWidget()->setDep(dep);
}

void CreateTelegramWizard::loadDlaTemplate()
{
    ValueMap vMap;

    QSqlQuery selectList;
    selectList.prepare("SELECT id, name, aircraft, adep, departure_time, ades, other "
                       "FROM template_dla");
    if (!selectList.exec())
    {
        qWarning() << selectList.lastError().text();
        return;
    }
    while (selectList.next())
    {
        DlaStruct dla;
        dla.ACID = selectList.value(2).toString();
        dla.ADEP = selectList.value(3).toString();
        dla.departureTime = selectList.value(4).toDateTime();
        dla.dtADEP = dla.departureTime.toString("hh:mm");
        dla.ADES = selectList.value(5).toString();
        vMap.insert(selectList.value(0).toInt()
                    , Value(selectList.value(1).toString(), DlaMessage(dla).toAftn()));
    }

    if (vMap.isEmpty())
    {
        QMessageBox::warning(this, tr("DLA templates"), tr("There are no saved templates"));
        return;
    }

    bool ok;
    int id = GetMapDialog::getIndex(this, tr("Templates"), tr("Select template:"), vMap, &ok);
    if (!ok)
        return;

    QSqlQuery select;
    select.prepare("SELECT aircraft, adep, departure_time, ades, other "
                   "FROM template_dla WHERE id = :id");
    select.bindValue(":id", id);
    if (!select.exec())
    {
        qWarning() << select.lastError().text();
        return;
    }
    if (!select.next())
    {
        qWarning() << "Could not find DLA template with id " << id;
        return;
    }
    DlaStruct dla;
    dla.ACID = select.value(0).toString();
    dla.ADEP = select.value(1).toString();
    dla.departureTime = select.value(2).toDateTime();
    dla.dtADEP = dla.departureTime.toString("hh:mm");
    dla.ADES = select.value(3).toString();
    dlaWidget()->setDla(dla);
}

void CreateTelegramWizard::onTelegramTypeChange(int index)
{
    if(!stackedWidget->count())
    {
        dictionary.version = ParseVersion::n_4444;

        QWidget* telegramEdit;
        telegramEdit = new QTextEdit(this);
        connect ((QTextEdit *)(telegramEdit), SIGNAL(textChanged()), SLOT(onTextChangedFreeText()));
        stackedWidget->insertWidget(n_indexFreeText, telegramEdit);

        telegramEdit = new FplTelegramWidget(mProcessor, this);
        stackedWidget->insertWidget(n_indexFpl, telegramEdit);
        fplWidget()->setVersion(ParseVersion::n_4444);

        telegramEdit = new ArrWidget(mProcessor, this);
        stackedWidget->insertWidget(n_indexArr, telegramEdit);
        arrWidget()->setVersion(ParseVersion::n_4444);

        telegramEdit = new ChgWidget(mProcessor, this);
        stackedWidget->insertWidget(n_indexChg, telegramEdit);
        chgWidget()->setVersion(ParseVersion::n_4444);

        telegramEdit = new CnlWidget(mProcessor, this);
        stackedWidget->insertWidget(n_indexCnl, telegramEdit);
        cnlWidget()->setVersion(ParseVersion::n_4444);

        telegramEdit = new DepWidget(mProcessor, this);
        stackedWidget->insertWidget(n_indexDep, telegramEdit);
        depWidget()->setVersion(ParseVersion::n_4444);

        telegramEdit = new DlaWidget(mProcessor, this);
        stackedWidget->insertWidget(n_indexDla, telegramEdit);
        dlaWidget()->setVersion(ParseVersion::n_4444);
    }
    if(index >= 0 && index < stackedWidget->count())
    {
        stackedWidget->setCurrentIndex(index);
#ifndef DUSHANBE_BUILD
        autoFillAddresses();
#endif
    }
}

void CreateTelegramWizard::onTextChangedFreeText()
{
    QTextEdit *tempTelegramEdit = freeTextWidget();
    if(tempTelegramEdit)
    {
        QString s = tempTelegramEdit->toPlainText();
        if (s == s.toUpper())
            return;
        int pos = tempTelegramEdit->textCursor().position();
        tempTelegramEdit->setText(s.toUpper());
        QTextCursor cursor = tempTelegramEdit->textCursor();
        cursor.setPosition(pos);
        tempTelegramEdit->setTextCursor(cursor);
    }
}

void CreateTelegramWizard::on_butAddAddress_clicked()
{
    AbonentDlg dlg(AbonentDlg::ALL, 0, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        QStringList list = addressesLineEdit->text().split(" ", QString::SkipEmptyParts);
        list.append(dlg.selectedAddresses());
        list.removeDuplicates();
        list.removeOne("");
        addressesLineEdit->setText(list.join(" "));
    }
}

void CreateTelegramWizard::onSaveTemplate()
{
    QString name = QInputDialog::getText(this, tr("Template name"), tr("Input template name:"));
    if (name.isEmpty())
        return;
    switch (telegramTypeComboBox->currentIndex())
    {
    case n_indexFreeText:
        saveFreeTextTemplate(name);
        break;
    case n_indexFpl:
        saveFplTemplate(name);
        break;
    case n_indexArr:
        saveArrTemplate(name);
        break;
    case n_indexChg:
        saveChgTemplate(name);
        break;
    case n_indexCnl:
        saveCnlTemplate(name);
        break;
    case n_indexDep:
        saveDepTemplate(name);
        break;
    case n_indexDla:
        saveDlaTemplate(name);
        break;
    }
}

void CreateTelegramWizard::onLoadTemplate()
{
    switch (telegramTypeComboBox->currentIndex())
    {
    case n_indexFreeText:
        loadFreeTextTemplate();
        break;
    case n_indexFpl:
        loadFplTemplate();
        break;
    case n_indexArr:
        loadArrTemplate();
        break;
    case n_indexChg:
        loadChgTemplate();
        break;
    case n_indexCnl:
        loadCnlTemplate();
        break;
    case n_indexDep:
        loadDepTemplate();
        break;
    case n_indexDla:
        loadDlaTemplate();
        break;
    }
}
