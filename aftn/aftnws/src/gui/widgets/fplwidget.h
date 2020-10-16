#ifndef FPLWIDGET_H
#define FPLWIDGET_H

#include "ui_fplwidget.h"
#include "fpl.h"

#include <QtCore/QString>

#include <QtGui/QDialog>

class QAbstractButton;

/**
* @brief Widget for display FPL in READONLY mode
*
* @class FplWidget
*/
class FplWidget
  : public QWidget
  , public Ui::FplWidget
{
    Q_OBJECT
  public:

    FplWidget(QWidget* pParent = 0);

    void setFPL(const FPL& Fpl);
    void clear();
protected:
    virtual void changeEvent(QEvent *event);
};


#endif // FPLWIDGET_H
