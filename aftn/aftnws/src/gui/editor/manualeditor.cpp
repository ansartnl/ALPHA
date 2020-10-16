#include "manualeditor.h"

#include "fpleditwidget.h"

#include "qtheaderview.h"

#include "aftnmessageprocessor.h"

#include "translations.h"

#include "main.h"

#include "ocitablemodel.h"

#include <QtDebug>

#include <QtGui/QStringListModel>
#include <QtGui/QMessageBox>
#include <QtGui/QDataWidgetMapper>
#include <QtGui/QTextEdit>
#include <QtGui/QItemDelegate>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>

#define MANUALEDITOR "manualEditor"

#if defined(DUSHANBE_BUILD)
#include "fpltan.h"
#endif

class AftnMessageDelegate : public QItemDelegate
{
public:
    AftnMessageDelegate(QObject* parent = 0):QItemDelegate(parent){}

    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        if (qobject_cast<QTextEdit*>(editor))
            model->setData(index, editor->property("plainText").toString().remove(QRegExp("(\\r|\\n")));
        else
            QItemDelegate::setModelData(editor, model, index);
    }

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        if (qobject_cast<QTextEdit*>(editor))
            editor->setProperty("plainText", index.data().toString().replace(QRegExp("([^\\r\\n])-"), "\\1\n-"));
        else
            QItemDelegate::setEditorData(editor, index);
    }
};

ManualEditor::ManualEditor(QWidget* pParent) :
    AbstractBrowser(pParent)
{
    setupUi(this);

    messageModel = new OciTableModel(this);
    messageModel->setTable(TBL_MAN_AFTN);
    messageModel->sort(messageModel->fieldIndex(TBL_MAN_AFTN_CREATED),
                       Qt::AscendingOrder);
    messageModel->showInsertingRow(false);

    mapper = new QDataWidgetMapper(this);

    mapper->setModel(messageModel);
    mapper->setItemDelegate(new AftnMessageDelegate(this));
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    mapper->addMapping(messageCreated, messageModel->fieldIndex(
                           TBL_MAN_AFTN_CREATED), "dateTime");
    mapper->addMapping(messageEdit, messageModel->fieldIndex(TBL_MAN_AFTN_TEXT));

    fplWidget = new FplEditWidget(this);
    connect(applyFPL, SIGNAL(clicked()), fplWidget, SLOT(onApply()));
    connect(fplWidget, SIGNAL(apply(FPL)), this, SLOT(onApplyFpl(FPL)));
    connect(fplWidget, SIGNAL(maybeDushanbe(bool&, const FPL&)), this, SLOT(onMaybeDushanbe(bool&, const FPL&)));
    scrollArea->setWidget(fplWidget);

    errorsModel = new QStringListModel(this);
    errorsView->setModel(errorsModel);

    connect(mapper, SIGNAL(currentIndexChanged(int)), this,
            SLOT(currentMessageIndexChanged(int)));

    updateModel();

    restoreState(staticMetaObject.className());
}


ManualEditor::~ManualEditor()
{
    saveState(staticMetaObject.className());
}

void ManualEditor::updateModel()
{
    mapper->submit();
    int oldRowCount = messageModel->rowCount();
    int currentIndex = mapper->currentIndex();
    messageModel->refresh();

    if (messageModel->rowCount()) {
        currentIndex = currentIndex == -1 ? 0 : currentIndex;
        if(currentIndex >= messageModel->rowCount())
            currentIndex = messageModel->rowCount()-1;
        mapper->setCurrentIndex(currentIndex);
    } else {
        currentMessageIndexChanged(-1);
    }

    if (oldRowCount != messageModel->rowCount())
        emit countChanged();
}

void ManualEditor::currentMessageIndexChanged(int index)
{
    // clear errors
    errorsModel->setStringList(QStringList());

    if (index == -1) {
        messageEdit->clear();
        messageCreated->clear();
        fplWidget->clear();
        setEnabled(false);
        return;
    }

    setEnabled(true);

    bool messageAvailable = messageModel->rowCount();

    next->setEnabled(index + 1 != messageModel->rowCount());
    previous->setEnabled(index);

    deleteMessage->setEnabled(messageAvailable);
    applyMessage->setEnabled(messageAvailable);
    messageEdit->setEnabled(messageAvailable);

    tryParseAndInsertCurrentMessage();
}

void ManualEditor::onApplyFpl(const FPL &fpl)
{
    fplWidget->clear();
    deleteCurrentMessage();
}

void ManualEditor::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
        tryParseAndInsertCurrentMessage();
    }

    QWidget::changeEvent(event);
}

bool ManualEditor::tryParseAndInsertCurrentMessage()
{
    QString message = messageEdit->toPlainText();

    AFTNMessageProcessor::AftnMessageErrors errorCodes;
//    QVariantMap messageMap;
    bool first = false;
    if (messageMap.isEmpty())
        first = true;

    // проанализировать сообщение и если есть индекс судна, аэропорт вылета/посадки, показываем пользователю подсказку
    messageMap = aftnMessageProcessor->processMessage(message, errorCodes);
//    onMessageChanged(messageMap);
    if (first)
        onMessageChanged();

    // Если парсинг прошел успешно, пробуем вставить
    if (errorCodes.isEmpty())
    {
#if defined(DUSHANBE_BUILD)
        QString sHeader;
        int currentIndex = mapper->currentIndex();
        if(currentIndex != -1)
        {
            sHeader = messageModel->index(currentIndex, messageModel->fieldIndex(TBL_MAN_AFTN_HEADER)).data().toString();
        }
        messageToTAN(sHeader, message, messageMap, false);
#elif defined(BAN_BUILD)
        // nothing
#else
        aftnMessageProcessor->insertMessage(messageMap, errorCodes);
#endif
    }

    // Если вставка прошла успешно - удаляем и выходим
    if (errorCodes.isEmpty()) {
        fplWidget->clear();
        deleteCurrentMessage();
        return true;
    }

    // Устанавливаем ошибки
    errorsModel->setStringList(aftnMessageProcessor->errorStrings(errorCodes));

    QList<int> errorLines = aftnMessageProcessor->getErrorLines();

    QString src = messageEdit->toPlainText();
    QList<QTextEdit::ExtraSelection> list;

    int lastMinusSection = 0;
    int curSection = 0;
    while (lastMinusSection != -1)
    {
        int tmp = lastMinusSection;
        lastMinusSection = src.indexOf("-", lastMinusSection);
        if (lastMinusSection == -1) {
            lastMinusSection = src.length();
        }

        if (errorLines.contains(curSection))
        {
            QTextEdit::ExtraSelection extra;
            extra.cursor = QTextCursor(messageEdit->textCursor());
            extra.cursor.setPosition(tmp);
            extra.cursor.setPosition(lastMinusSection, QTextCursor::KeepAnchor);
            extra.format = QTextCharFormat();
            extra.format.setFontUnderline(true);
            list.append(extra);
        }
        if (lastMinusSection == src.length())
            break;
        ++lastMinusSection;
        ++curSection;
    }
    messageEdit->QTextEdit::setExtraSelections(list);

    return false;
}

void ManualEditor::deleteCurrentMessage()
{
    messageMap.clear();

    int currentIdx  = mapper->currentIndex();
    if (!messageModel->removeRow(mapper->currentIndex())) {
        qWarning() << Q_FUNC_INFO;
        qWarning() << messageModel->lastError().text();
        return;
    }

    if (messageModel->rowCount())
    {
        if(currentIdx >= messageModel->rowCount())
            currentIdx = messageModel->rowCount()-1;
        mapper->setCurrentIndex(currentIdx);
        onMessageChanged();
    }
    else
        currentMessageIndexChanged(-1);

    emit countChanged();
}

void ManualEditor::on_next_clicked()
{
    mapper->toNext();
    onMessageChanged();
}

void ManualEditor::on_previous_clicked()
{
    mapper->toPrevious();
    onMessageChanged();
}

void ManualEditor::on_deleteMessage_clicked()
{
    bool answer = QMessageBox::question(this, this->windowTitle(), tr(
                                            "Do you really want to delete message?"), QMessageBox::Ok
                                        | QMessageBox::Cancel) == QMessageBox::Ok;

    if (!answer)
        return;

    deleteCurrentMessage();
}

void ManualEditor::on_applyMessage_clicked()
{
    if (!tryParseAndInsertCurrentMessage())
        onMessageChanged();
}

//void ManualEditor::onMessageChanged(const QVariantMap &message)
void ManualEditor::onMessageChanged()
{
    QString type = messageMap.value(AFTN_MESSAGE_TYPE).toString();
    if (type == "FPL")
    {
        applyFPL->setEnabled(true);
        fplWidget->setEnabled(true);
        fplWidget->setFpl(messageMap);
    } else
    {
        fplWidget->clear();
        fplWidget->setEnabled(false);
        applyFPL->setEnabled(false);
    }
}
void ManualEditor::closeEvent(QCloseEvent *event)
{
    AbstractBrowser::closeEvent(event);
}

void ManualEditor::on_findButton_clicked()
{
    if (findEdit->text().isEmpty())
        return;
    int lastIdx = mapper->currentIndex() == -1?0:mapper->currentIndex();
    for (int i = mapper->currentIndex() + 1; i < messageModel->rowCount(); ++i)
    {
        if (messageModel->index(i, messageModel->fieldIndex(TBL_MAN_AFTN_TEXT)).data().toString().contains(findEdit->text(), Qt::CaseInsensitive))
        {
            mapper->setCurrentIndex(i);
            onMessageChanged();
            return;
        }
    }
    for (int i = 0; i < lastIdx; ++i)
    {
        if (messageModel->index(i, messageModel->fieldIndex(TBL_MAN_AFTN_TEXT)).data().toString().contains(findEdit->text(), Qt::CaseInsensitive))
        {
            mapper->setCurrentIndex(i);
            onMessageChanged();
            return;
        }
    }
    QMessageBox::information(this, tr("Information"), tr("Not found"));
}

void ManualEditor::onMaybeDushanbe(bool& bRes, const FPL& fpl)
{
#if defined(DUSHANBE_BUILD)
    bRes = true;
    messageToTAN(fpl);
#endif
}

void ManualEditor::messageToTAN(const QString& sHeader, const QString& sMessage, const QVariantMap& map, bool bError)
{
#if defined(DUSHANBE_BUILD)
    QSqlQuery mesAftnQuery;

    FPLfromFDP Fpl(FPLfromFDP::TBL_MSG_FROM_FDP);
    Fpl.setId(0);
    Fpl.setFirName(aftnMessageProcessor->FIRName().split(",").at(0));
    Fpl.setAircraftId(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString());
    Fpl.setFlightRules(map.value(AFTN_MESSAGE_FLIGHT_RULES).toString());
    Fpl.setFlightType(map.value(AFTN_MESSAGE_FLIGHT_TYPE).toString());
    Fpl.setAircraftNumber(map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt());
    Fpl.setAircraftType(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString());
    Fpl.setTurbulenceCategory(map.value(AFTN_MESSAGE_TURBULENCE).toString());
    Fpl.setEquipment(map.value(AFTN_MESSAGE_EQUIPMENT).toString());
    Fpl.setDepartureAerodrome(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString());
    Fpl.setDepartureDateTime(map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime());
    Fpl.setSpeed(map.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString());
    Fpl.setLevel(map.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString());
    Fpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
    Fpl.setDestAerodrome(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString());
    Fpl.setEET(map.value(AFTN_MESSAGE_ELAPSED_TIME).toString());
    Fpl.setAltDestinationAerodrome(map.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString());
    Fpl.setAltDestinationAerodrome2(map.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString());
    Fpl.setOtherInformation(map.value(AFTN_MESSAGE_OTHER_INFO).toString());
    QVariantMap firEET = map.value(AFTN_MESSAGE_OTHER_INFO_FIR_EET).toMap();
    //Fpl.setEntryFIREET(firEET.value(aftnMessageProcessor->FIRName().split(",").at(0)/*, "00:00"*/).toString());
    Fpl.setEntryFIREET(aftnMessageProcessor->FIRName().split(","), firEET);

    //apply CHG message
    {
        QVariantMap changedMap = map.value(AFTN_MESSAGE_CHANGED_FIELDS).toMap();
        if(changedMap.size())
        {
            if (changedMap.contains(AFTN_MESSAGE_AIRCRAFT_ID))
            {
                Fpl.setAircraftId(changedMap.value(AFTN_MESSAGE_AIRCRAFT_ID).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_FLIGHT_RULES))
            {
                Fpl.setFlightRules(changedMap.value(AFTN_MESSAGE_FLIGHT_RULES).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_FLIGHT_TYPE))
            {
                Fpl.setFlightType(changedMap.value(AFTN_MESSAGE_FLIGHT_TYPE).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_AIRCRAFT_COUNT))
            {
                Fpl.setAircraftNumber(changedMap.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt());
                Fpl.setAircraftType(changedMap.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString());
                Fpl.setTurbulenceCategory(changedMap.value(AFTN_MESSAGE_TURBULENCE).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_EQUIPMENT))
            {
                Fpl.setEquipment(changedMap.value(AFTN_MESSAGE_EQUIPMENT).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_DEPARTURE_AERODROME))
            {
                Fpl.setDepartureAerodrome(changedMap.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString());
                Fpl.setDepartureDateTime(changedMap.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime());
            }
            else
            {
                if (!map.contains(AFTN_MESSAGE_DEPARTURE_TIME_ORIG))
                {
                    Fpl.setDepartureDateTime(QDateTime::fromString("1900-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss"));
                }
                else
                {
                    if (changedMap.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
                    {
                        QDate date = changedMap.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
                        Fpl.setDateDOF(date);
                    }

                }
            }

            if (changedMap.contains(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER))
            {
                Fpl.setSpeed(changedMap.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString());
                Fpl.setLevel(changedMap.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString());
                Fpl.setRoute(changedMap.value(AFTN_MESSAGE_ROUTE).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_DESTINATION_AERODROME))
            {
                Fpl.setDestAerodrome(changedMap.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString());
                Fpl.setEET(changedMap.value(AFTN_MESSAGE_ELAPSED_TIME).toString());

                if (changedMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD))
                {
                    Fpl.setAltDestinationAerodrome(changedMap.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString());
                }

                if (changedMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD_2))
                {
                    Fpl.setAltDestinationAerodrome2(changedMap.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString());
                }
            }

            if (changedMap.contains(AFTN_MESSAGE_OTHER_INFO))
            {
                Fpl.setOtherInformation(changedMap.value(AFTN_MESSAGE_OTHER_INFO).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_OTHER_INFO_FIR_EET))
            {
                QVariantMap firEET = changedMap.value(AFTN_MESSAGE_OTHER_INFO_FIR_EET).toMap();
                //Fpl.setEntryFIREET(firEET.value(aftnMessageProcessor->FIRName().split(",").at(0)/*, "00:00"*/).toString());
                Fpl.setEntryFIREET(aftnMessageProcessor->FIRName().split(","), firEET);
            }
        }
    }

    Fpl.setOper(bError ? "E" : "I");
    Fpl.setOperTime(QDateTime::currentDateTime()); // may be QDateTime::currentDateTimeUtc()
    Fpl.setMessageText(QString(sHeader+sMessage).simplified());

    Fpl.prepareInsert(mesAftnQuery);
    if(!mesAftnQuery.exec())
    {
        qWarning() << mesAftnQuery.lastError().text();
    }
#endif
}

void ManualEditor::messageToTAN(const FPL& fpl)
{
#if defined(DUSHANBE_BUILD)
    QSqlQuery mesAftnQuery;
    FPLfromFDP Fpl = FPLfromFDP(fpl).clone(FPLfromFDP::TBL_MSG_FROM_FDP);
    Fpl.setId(0);
    Fpl.setOper("I");
    Fpl.setOperTime(QDateTime::currentDateTime()); // may be QDateTime::currentDateTimeUtc()
    Fpl.setMessageText(messageEdit->toPlainText());

    Fpl.prepareInsert(mesAftnQuery);
    if(!mesAftnQuery.exec())
    {
        qWarning() << mesAftnQuery.lastError().text();
    }
#endif
}

// --------------------------------------------------------------------
ManualEditorHelper::ManualEditorHelper(QObject* parent) :
    ObjectsManagerHelper(parent), mAction(0), manualMessagesCount(0)
{
    mAction = new QAction(icon(), name(), this);
    mAction->setObjectName(id());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
    updateState();
    timerId = startTimer(5000);
}

QString ManualEditorHelper::id() const
{
    return ManualEditor::staticMetaObject.className();
}

QString ManualEditorHelper::path() const
{
    return tr("/View/Manual editor");
}

QString ManualEditorHelper::name() const
{
    return tr("Manual Editor");
}

QString ManualEditorHelper::description() const
{
    return tr("Manual edditing of wrong AFTN messages");
}

QWidget* ManualEditorHelper::widget(QWidget* parent) const
{
    ManualEditor* editor = new ManualEditor(parent);

    connect(editor, SIGNAL(countChanged()), this, SLOT(updateState()));

    connect(this, SIGNAL(changeCount()), editor, SLOT(updateModel()));

    return editor;
}

void ManualEditorHelper::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timerId) {
        updateState();
        return;
    }

    ObjectsManagerHelper::timerEvent(event);
}

QAction* ManualEditorHelper::action() const
{
    return mAction;
}

void ManualEditorHelper::updateState()
{
    int newManualMessagesCount = 0;

    QSqlQuery query("SELECT COUNT(*) FROM " TBL_MAN_AFTN);

    if (query.lastError().isValid()) {
        qWarning() << Q_FUNC_INFO;
        qWarning() << query.lastError().text();
    }

    if (query.next())
        newManualMessagesCount = query.value(0).toInt();

    bool enabled = newManualMessagesCount > 0;

    mAction->setEnabled(enabled);
    mAction->setText(tr("%1 for manual processing").arg(newManualMessagesCount));
    mAction->setProperty("color", enabled ? 0xFF0000 : 0x000000);

    if (!manualMessagesCount && newManualMessagesCount)
        emit changeCount();
    else if(manualMessagesCount != newManualMessagesCount)
    {
        if(manualMessagesCount == 1)
            emit changeCount();
        else
        {
            // for Dushanbe
            emit changeCount();
        }
    }

    manualMessagesCount = newManualMessagesCount;
}

void ManualEditorHelper::retranslateUi()
{
    mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
    updateState();
}

// --------------------------------------------------------------------

ManualTelegramEditorHelper::ManualTelegramEditorHelper(QObject *parent) :
    ObjectsManagerHelper(parent), mAction(0), manualMessagesCount(0)
{
    mAction = new QAction(icon(), name(), this);
    mAction->setObjectName(id());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
    updateState();
    timerId = startTimer(5000);
}

QString ManualTelegramEditorHelper::id() const
{
    return ManualEditor::staticMetaObject.className();
}

QString ManualTelegramEditorHelper::path() const
{
    return tr("/Telegrams/Manual editor");
}

QString ManualTelegramEditorHelper::name() const
{
    return tr("Manual Editor");
}

QString ManualTelegramEditorHelper::description() const
{
    return tr("Manual edditing of wrong AFTN messages");
}

QWidget *ManualTelegramEditorHelper::widget(QWidget *parent) const
{
    ManualEditor* editor = new ManualEditor(parent);

    connect(editor, SIGNAL(countChanged()), this, SLOT(updateState()));

    connect(this, SIGNAL(changeCount()), editor, SLOT(updateModel()));

    return editor;
}

void ManualTelegramEditorHelper::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {
        updateState();
        return;
    }

    ObjectsManagerHelper::timerEvent(event);
}

QAction *ManualTelegramEditorHelper::action() const
{
    return mAction;
}

void ManualTelegramEditorHelper::retranslateUi()
{
    mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
    updateState();
}

void ManualTelegramEditorHelper::updateState()
{
    int newManualMessagesCount = 0;

    QSqlQuery query("SELECT COUNT(*) FROM " TBL_MAN_AFTN);

    if (query.lastError().isValid()) {
        qWarning() << Q_FUNC_INFO;
        qWarning() << query.lastError().text();
    }

    if (query.next())
        newManualMessagesCount = query.value(0).toInt();

    bool enabled = newManualMessagesCount > 0;

    mAction->setEnabled(enabled);
    mAction->setText(tr("%1 for manual processing").arg(newManualMessagesCount));
    mAction->setProperty("color", enabled ? 0xFF0000 : 0x000000);

    if (!manualMessagesCount && newManualMessagesCount)
        emit changeCount();
    else if(manualMessagesCount != newManualMessagesCount)
    {
        if(manualMessagesCount == 1)
            emit changeCount();
        else
        {
            // for Dushanbe
            emit changeCount();
        }
    }

    manualMessagesCount = newManualMessagesCount;
}
