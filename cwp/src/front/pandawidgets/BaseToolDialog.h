#ifndef BASETOOLDIALOG_H
#define BASETOOLDIALOG_H

#include <QWidget>

class BaseToolDialog : public QWidget
{
    Q_OBJECT

public:
    explicit BaseToolDialog(bool modal, bool close_button, QWidget *parent = 0);

public Q_SLOTS:
    virtual void setVisible(bool visible);

Q_SIGNALS:
    void hidden();

protected:
    virtual void hideEvent(QHideEvent *event);

};

#endif // BASETOOLDIALOG_H
