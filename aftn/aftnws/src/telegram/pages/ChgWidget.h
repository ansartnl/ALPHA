#ifndef CHGWIDGET_H
#define CHGWIDGET_H

#include "ui_ChgWidget.h"

#include "ChgMessage.h"
#include "replies.h"

class AFTNMessageProcessor;

class ChgWidget : public QWidget, private Ui::ChgWidget
{
    Q_OBJECT
public:
    explicit ChgWidget(AFTNMessageProcessor *p, QWidget *parent = 0);

    void setReadOnly(bool r);
    void setReadOnlyField(bool r);

    void setChg(const ChgStruct &c);

    void setVersion(int v);

    ChgStruct chg() const;

    bool validate();

protected:
    void fillCombo();
    virtual void changeEvent(QEvent *e);

protected slots:
    void onSelectRoute();

    void onField8CheckBox(bool r);
    void onField9CheckBox(bool r);
    void onField10CheckBox(bool r);
    void onField13CheckBox(bool r);
    void onField15CheckBox(bool r);
    void onField16CheckBox(bool r);
    void onField18CheckBox(bool r);

protected:
    RouteRecords mRoutes;
    QVariantMap mMap;
    int version;
    QString invalidValue;
    AFTNMessageProcessor *mProcessor;

protected slots:
    bool process7FieldH(bool showMessageBox = false);
    bool process13FieldH(bool showMessageBox = false);
    bool process16FieldH(bool showMessageBox = false);

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
    void onEditTextAircraftType(const QString &s);
    void onEditTextRoute();
    void onEditTextOther();

    void onEditTextACID(const QString &s);
    void onEditTextAdep(const QString &s);
    void onEditTextAdes(const QString &s);
    void onTextChangedEet(const QString &s);
};

#endif // CHGWIDGET_H
