#ifndef NAVIGATEACTIONSMANAGER_H
#define NAVIGATEACTIONSMANAGER_H

#include <QtCore/QObject>

class QAction;
class NavigateActionsManagerPrivate;
class NavigateActionsManagerHelper;
/**
Class to manage the actions of navigation (up / down, etc.)
* @class NavigateActionsManager navigateactionsmanager.h
*/
class NavigateActionsManager : public QObject//: public AbstractActionsManager
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(NavigateActionsManager)
protected:
  NavigateActionsManagerPrivate * const d_ptr;
public:
  NavigateActionsManager(QWidget* parent);
  virtual ~NavigateActionsManager();

  void addWidget(QWidget* widget, QAction* before = 0);
  void removeWidget(QWidget* widget);

  void addHelper(NavigateActionsManagerHelper* helper);
  void removeHelper(NavigateActionsManagerHelper* helper);
  QSet<NavigateActionsManagerHelper*> helpers() const;

  void retranslateUi();

signals:
  void canFirstChanged(bool enable);
  void canPreviousChanged(bool enable);

  void canTopChanged(bool enable);
  void canUpChanged(bool enable);

  void canBottomChanged(bool enable);
  void canDownChanged(bool enable);

  void canNextChanged(bool enable);
  void canLastChanged(bool enable);

signals:
  void first();
  void previous();
  void top();
  void up();
  void down();
  void bottom();
  void next();
  void last();

protected slots:
  virtual void focusChanged(QWidget* old, QWidget* now);

  void blockFocusChanged(bool blocked);

protected:
  NavigateActionsManagerHelper* helperFor(QWidget* widget)const;
  void connectToHelper(NavigateActionsManagerHelper* helper);
  void disconnectFromHelper(NavigateActionsManagerHelper* helper);
  QList<QAction*> actions() const;

private:
  void addStandartHelpers();
};

#endif // NAVIGATEACTIONSMANAGER_H
