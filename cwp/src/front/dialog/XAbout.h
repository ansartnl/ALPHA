#ifndef XABOUT_H
#define XABOUT_H

#include "ui_About.h"

class XAbout : public QDialog, public Ui::About
{
    Q_OBJECT
public:
    XAbout(bool registered, QWidget* pParent = 0);
    virtual ~XAbout();

private slots:
    void onActivate();
    void onLicTextChanged(const QString &text);

private:
    QPushButton *mActivateButton;
};

#endif // XABOUT_H
