#ifndef DEPWIDGET_H
#define DEPWIDGET_H

#include "ui_DepWidget.h"

#include "DepMessage.h"

class ReplyDictionary;

class AFTNMessageProcessor;

class DepWidget : public QWidget, private Ui::DepWidget
{
    Q_OBJECT
    
public:
    explicit DepWidget(AFTNMessageProcessor *p, QWidget *parent = 0);

    void setReadOnly(bool r);

    void setDep(const DepStruct &d);

    void setVersion(int v);

    DepStruct dep() const;

    bool validate();

protected:
    virtual void changeEvent(QEvent *e);

    void setFieldDefaultBackground();

protected slots:
    bool validateDep(bool showMessageBox = false);

    void onEditTextChangeAdep(const QString &s);
    void onEditTextChangeAdes(const QString &s);
    void onTextChangedACID(const QString &s);

protected:
    int version;
    QString invalidValue;
    AFTNMessageProcessor *mProcessor;
};

#endif // DEPWIDGET_H
