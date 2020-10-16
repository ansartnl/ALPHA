#ifndef DLAWIDGET_H
#define DLAWIDGET_H

#include "ui_DlaWidget.h"

#include "DlaMessage.h"

class ReplyDictionary;

class AFTNMessageProcessor;

class DlaWidget : public QWidget, private Ui::DlaWidget
{
    Q_OBJECT
public:
    explicit DlaWidget(AFTNMessageProcessor *p, QWidget *parent = 0);

    void setReadOnly(bool r);

    void setDla(const DlaStruct &d);

    void setVersion(int v);

    DlaStruct dla() const;

    bool validate();

protected:
    virtual void changeEvent(QEvent *e);

    void setFieldDefaultBackground();

protected slots:
    bool validateDla(bool showMessageBox = false);

    void onEditTextChangeAdep(const QString &s);
    void onEditTextChangeAdes(const QString &s);
    void onTextChangedACID(const QString &s);

protected:
    int version;
    QString invalidValue;
    AFTNMessageProcessor *mProcessor;
};

#endif // DLAWIDGET_H
