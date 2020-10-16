#ifndef FPLTELEGRAMWIDGET_H
#define FPLTELEGRAMWIDGET_H

#include "ui_FplTelegramWidget.h"

#include "replies.h"

class AFTNMessageProcessor;

class FplTelegramWidget : public QWidget, private Ui::FplTelegramWidget
{
    Q_OBJECT
public:
    explicit FplTelegramWidget(AFTNMessageProcessor *p, QWidget *parent = 0);

    void setReadOnly(bool r);

    void setFpl(const FplStruct &f, bool pln = false);

    void setVersion(int v);

    FplStruct fpl() const;

    bool validate();

protected:
    void fillCombo();
    virtual void changeEvent(QEvent *e);
    QVariantMap mMap;

protected slots:
    void onSelectRoute();

    bool process7Field(bool showMessageBox = false);
    bool process8Field(bool showMessageBox = false);
    bool process9Field(bool showMessageBox = false);
    bool process10Field(bool showMessageBox = false);
    bool process13Field(bool showMessageBox = false);
    bool process15Field(bool showMessageBox = false);
    bool process16Field(bool showMessageBox = false);
    bool process18Field(bool showMessageBox = false);

    void onEditTextChangeAdep(const QString &s);
    void onEditTextChangeAdes(const QString &s);
    void onEditTextChangeAlt1(const QString &s);
    void onEditTextChangeAlt2(const QString &s);
    void onEditTextEquipment(const QString &s);
    void onEditTextACID(const QString &s);
    void onEditTextAircraftType(const QString &s);
    void onEditTextRoute();
    void onEditTextOther();
    void onTextChangedEet(const QString &s);

protected:
    AFTNMessageProcessor *mProcessor;

    RouteRecords mRoutes;
    QString invalidValue;
    int version;
    bool isPln;
    FplStruct mFpl;
    bool existDictionary;
};

#endif // FPLTELEGRAMWIDGET_H
