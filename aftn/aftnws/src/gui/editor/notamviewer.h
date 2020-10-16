#ifndef NOTAMVIEWER_H
#define NOTAMVIEWER_H

#include "ui_notamviewer.h"
#include "AFTNDao.h"
#include "notam.h"

#include "abstractdialog.h"

#include <QtCore/QString>

class QAbstractButton;
/// Disply notam in extendet form into dialog
class NotamViewer: public AbstractDialog, public Ui::NotamViewer
{
  Q_OBJECT
  public:

    NotamViewer(QWidget* pParent = 0);
    virtual ~NotamViewer();

    /** Copy information from NOTAM to dialog and over */
    void setNotam(const Notam& Notam);
  public:

    Q_SLOT
    void on_buttonBox_clicked(QAbstractButton* button);

  private:
    Notam mNotam;
    QPushButton* printButton;

  protected:
    virtual void changeEvent(QEvent* event);
};

#endif // NOTAMVIEWER_H
