#ifndef INTCHECKBOX_H
#define INTCHECKBOX_H

#include <QtGui/QCheckBox>

// workaround class for mazafaka ORACLE numeric (as boolean) type
class IntCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(int ichecked READ isiChecked WRITE setiChecked)
  public:
    IntCheckBox(QWidget* parent = 0):QCheckBox(parent) {};
    virtual ~IntCheckBox() {};

    int isiChecked() const {return isChecked();}
    void setiChecked(int iChecked) {setChecked(iChecked);}
};
#endif // INTCHECKBOX_H
