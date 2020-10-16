#ifndef CNLWIDGET_H
#define CNLWIDGET_H

#include "ui_CnlWidget.h"

#include "CnlMessage.h"

class ReplyDictionary;

class AFTNMessageProcessor;

class CnlWidget : public QWidget, private Ui::CnlWidget
{
    Q_OBJECT
public:
    explicit CnlWidget(AFTNMessageProcessor *p, QWidget *parent = 0);

    void setReadOnly(bool r);

    void setCnl(const CnlStruct &c);

    void setVersion(int v);

    CnlStruct cnl() const;

    bool validate();

protected:
    virtual void changeEvent(QEvent *e);

    bool isConnectSignals;
    void setFieldDefaultBackground();

protected slots:
    bool validateCnl(bool showMessageBox = false);

    void onTextChangedACID(const QString &);
    void onEditTextChangeAdep(const QString &s);
    void onEditTextChangeAdes(const QString &s);

protected:
    int version;
    QString invalidValue;
    AFTNMessageProcessor *mProcessor;
};

#endif // CNLWIDGET_H
