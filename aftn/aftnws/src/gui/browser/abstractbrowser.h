#ifndef ABSTRACTBROWSER_H
#define ABSTRACTBROWSER_H

#include <QWidget>

/**
* @brief Base class for mdi child windows.
\warning DON'T CREATE SOME WIDGETS IN THIS CLASS. IT'S ONLY FOR NON-GUI MANIPULATIONS
*
*/

class PrintDataDialog;

class AbstractBrowser : public QWidget
{
  Q_OBJECT

public:
  explicit AbstractBrowser(QWidget* parent = 0);

  PrintDataDialog *printDialog();
  virtual const QStringList dataList() const;

protected:
  virtual void restoreState(const QString& objectName);
  virtual void saveState(const QString& objectName);

  virtual void closeEvent(QCloseEvent *event);
};

#endif // ABSTRACTBROWSER_H
