#ifndef ARRWIDGET_H
#define ARRWIDGET_H

#include "ui_ArrWidget.h"

#include "ArrMessage.h"

class ReplyDictionary;

class AFTNMessageProcessor;

class ArrWidget : public QWidget, private Ui::ArrWidget
{
    Q_OBJECT
public:
    explicit ArrWidget(AFTNMessageProcessor *p, QWidget *parent = 0);

    void setReadOnly(bool r);

    void setArr(const ArrStruct &a);

    void setVersion(int v);

    ArrStruct arr() const;

    bool validate();

protected:
    virtual void changeEvent(QEvent *e);
    void setFieldDefaultBackground();

protected slots:
    bool validateArr(bool showMessageBox = false);

    void onEditTextChangeAdep(const QString &s);
    void onEditTextChangeAdes(const QString &s);
    void onEditTextChangeArrivalAirport(const QString &s);
    void onTextChangedACID(const QString &s);
    void onTaxtChangedArrivalAirportName(const QString &s);

protected:
    int version;
    QString invalidValue;
    AFTNMessageProcessor *mProcessor;
};

#endif // ARRWIDGET_H
