#ifndef ABSTRACTDIALOG_H
#define ABSTRACTDIALOG_H

#include <QPrinter>
#include <QDialog>

class AbstractDialog: public QDialog
{
  Q_OBJECT
  public:
    explicit AbstractDialog(QWidget *parent = 0);

  protected:
    virtual void saveState(const QString& objectName);
    virtual void restoreState(const QString& objectName);

    virtual void closeEvent(QCloseEvent *event);

    virtual void printWidget(QWidget* child, QPrinter::Orientation orientation =
        QPrinter::Portrait);

    Q_SLOT
    void render(QPrinter* printer);
};

#endif // ABSTRACTDIALOG_H
