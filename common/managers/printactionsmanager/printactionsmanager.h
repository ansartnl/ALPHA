#ifndef PRINTACTIONSMANAGER_H
#define PRINTACTIONSMANAGER_H

#include <QtCore/QObject>

class QAction;
class PrintActionsManagerPrivate;
class PrintActionsManagerHelper;
/**
* @brief Class to manage setup page-preview-print action
*
* @class PrintActionsManager printactionsmanager.h
*/
class PrintActionsManager : public QObject//: public AbstractActionsManager
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(PrintActionsManager)
protected:
  PrintActionsManagerPrivate * const d_ptr;
public:
  PrintActionsManager(QWidget* parent);
  virtual ~PrintActionsManager();

  void addWidget(QWidget* widget, QAction* before = 0);
  void removeWidget(QWidget* widget);

  void addHelper(PrintActionsManagerHelper* helper);
  void removeHelper(PrintActionsManagerHelper* helper);
  QSet<PrintActionsManagerHelper*> helpers() const;

  void retranslateUi();

signals:
  void canPageSetupChanged(bool enable);
  void canPrintPreviewChanged(bool enable);

  void canPrintChanged(bool enable);

signals:
  void pageSetup();
  void printPreview();
  void print();

protected slots:
  virtual void focusChanged(QWidget* old, QWidget* now);
  void blockFocusChanged(bool blocked);

protected:
  PrintActionsManagerHelper* helperFor(QWidget* widget)const;
  void connectToHelper(PrintActionsManagerHelper* helper);
  void disconnectFromHelper(PrintActionsManagerHelper* helper);
  QList<QAction*> actions() const;

private:
  void addStandartHelpers();

  bool isFocusChangedBlocked;
};

#endif // PRINTACTIONSMANAGER_H
