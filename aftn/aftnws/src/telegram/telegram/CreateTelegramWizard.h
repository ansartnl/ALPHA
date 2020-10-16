#ifndef CREATETELEGRAMWIZARD_H
#define CREATETELEGRAMWIZARD_H

#include "ui_CreateTelegramWizard.h"

#include "replies.h"
#include "templates.h"

//class AirPlanModel;
class FplTelegramWidget;
class ArrWidget;
class ChgWidget;
class CnlWidget;
class DepWidget;
class DlaWidget;
class InternalClient;

class AFTNMessageProcessor;

class QTextEdit;

class CreateTelegramWizard : public QDialog, private Ui::CreateTelegramWizard
{
    Q_OBJECT
    
public:
    explicit CreateTelegramWizard(QWidget *parent = 0);
    virtual ~CreateTelegramWizard();

    void setInternalClient(InternalClient *c);
    void setDictionary(const ReplyDictionary &d);

    void createTelegramBasedOn(const QString &type, const FplStruct &base);

protected:
    void changeEvent(QEvent *e);
    void accept();
    void Translate();

    void loadSettings();
    void saveSettings();

    QTextEdit * freeTextWidget() const;
    FplTelegramWidget * fplWidget() const;
    ArrWidget * arrWidget() const;
    ChgWidget * chgWidget() const;
    CnlWidget * cnlWidget() const;
    DepWidget * depWidget() const;
    DlaWidget * dlaWidget() const;

    void setEditWidget();
    void fillDictionary();

    void fillPreview();

    bool send();

    AftnStruct aftnStruct() const;

#ifndef DUSHANBE_BUILD
    void autoFillAddresses();
#endif

    void saveFreeTextTemplate(const QString &name);
    void saveFplTemplate(const QString &name);
    void saveArrTemplate(const QString &name);
    void saveChgTemplate(const QString &name);
    void saveCnlTemplate(const QString &name);
    void saveDepTemplate(const QString &name);
    void saveDlaTemplate(const QString &name);

    void loadFreeTextTemplate();
    void loadFplTemplate();
    void loadArrTemplate();
    void loadChgTemplate();
    void loadCnlTemplate();
    void loadDepTemplate();
    void loadDlaTemplate();

protected slots:
    void onTelegramTypeChange(int index);
    void onTextChangedFreeText();
    void on_butAddAddress_clicked();

    void onSaveTemplate();
    void onLoadTemplate();

protected:
    InternalClient *client;
    ReplyDictionary dictionary;
    ReplyDictionary dictionaryTC95;
    QList < ReplyFPL > depFplList;
    QList < ReplyFPL > arrFplList;
    AFTNMessageProcessor *mProcessor;
};

#endif // CREATETELEGRAMWIZARD_H
