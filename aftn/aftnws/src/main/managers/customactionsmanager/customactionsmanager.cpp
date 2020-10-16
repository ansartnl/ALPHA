#include "customactionsmanager.h"
#include "private/customactionsmanager_p.h"

#include "customactionsmanagerhelper.h"

#include <QtDebug>

#include <QtCore/QMetaMethod>

#include <QtGui/QApplication>
#include <QtGui/QAction>

CustomActionsManager::~CustomActionsManager()
{
  delete d_ptr;
}


CustomActionsManager::CustomActionsManager(QWidget* parent)
  : QObject(parent)
  , d_ptr(new CustomActionsManagerPrivate)
{
  Q_D(CustomActionsManager);
  setObjectName("customActionsManager");

  d->parent = parent;


  //  QAction *a;
  //  a = d->additionalActionsSeparator = new QAction(this);
  //  a->setSeparator(true);

  addStandardHelpers();

  foreach(QAction * action, actions())
    action->setEnabled(false);

  connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*))
          , this, SLOT(focusChanged(QWidget*, QWidget*)));
  focusChanged(0, QApplication::focusWidget());
}

void CustomActionsManager::addStandardHelpers()
{
  addHelper(new ItemViewCustomActionsManagerHelper(this));
}

QList<QAction*> CustomActionsManager::actions() const
{
  //Q_D(const CustomActionsManager);
  QList<QAction*> result;

  return result;
}


void CustomActionsManager::addWidget(QWidget* widget, QAction* before)
{
  Q_D(CustomActionsManager);
  d->widgets.insert(widget, before);

  widget->insertActions(before, actions());
}

void CustomActionsManager::removeWidget(QWidget* widget)
{
  Q_D(CustomActionsManager);

  foreach(QAction * action, actions())
  widget->removeAction(action);

  d->widgets.remove(widget);
}


void CustomActionsManager::focusChanged(QWidget* old, QWidget* now)
{
  Q_D(CustomActionsManager);
  if (!d->isFocusChangedBlocked) {
    if (d->currentHelper) {
      disconnectFromHelper(d->currentHelper);
      d->currentHelper->deactivate();
      d->currentHelper = 0;
    }

    if (!d->parent->isAncestorOf(now)) return;

    if ((d->currentHelper = helperFor(now))) {
      d->currentHelper->activateFor(now);
      connectToHelper(d->currentHelper);
    }
  }
}

void CustomActionsManager::blockFocusChanged(bool blocked)
{
  Q_D(CustomActionsManager);
  d->isFocusChangedBlocked = blocked;
}

void CustomActionsManager::addHelper(CustomActionsManagerHelper* helper)
{
  Q_D(CustomActionsManager);
  d->helpers.insert(helper);
}

void CustomActionsManager::removeHelper(CustomActionsManagerHelper* helper)
{
  Q_D(CustomActionsManager);
  if (d->currentHelper == helper) {
    disconnectFromHelper(helper);
    d->currentHelper = 0;
  }
  d->helpers.remove(helper);
}

QSet<CustomActionsManagerHelper*> CustomActionsManager::helpers() const
{
  Q_D(const CustomActionsManager);
  return d->helpers;
}

CustomActionsManagerHelper* CustomActionsManager::helperFor(QWidget* widget) const
{
  Q_D(const CustomActionsManager);
  QSet<CustomActionsManagerHelper*>::ConstIterator it = d->helpers.constBegin();
  QSet<CustomActionsManagerHelper*>::ConstIterator end = d->helpers.constEnd();
  CustomActionsManagerHelper *findedHelper = 0;
  int maxApproach = 0;
  int approach;
  while (it != end) {
    approach = (*it)->approach(widget);
    if (approach > maxApproach) {
      findedHelper = (*it);
      maxApproach = approach;
    }
    ++it;
  }
  return findedHelper;
}

void CustomActionsManager::connectToHelper(CustomActionsManagerHelper* helper)
{
  Q_D(CustomActionsManager);

  QList<QAction*> additionalActions = helper->additionalActions();
  if (!additionalActions.isEmpty()) {
    QHash<QWidget*, QAction*>::ConstIterator it = d->widgets.constBegin();
    QHash<QWidget*, QAction*>::ConstIterator end = d->widgets.constEnd();
    for (; it != end; ++it) {
      it.key()->insertActions(it.value(), additionalActions);
    }
  }

  QMetaMethod metaMethod;

  for (int i = helper->metaObject()->methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
    metaMethod = helper->metaObject()->method(i);
    if (metaMethod.methodType() == QMetaMethod::Slot
        && (metaObject()->indexOfSignal(metaMethod.signature()) != -1)) {
      connect(this, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data()
              , helper, QString("%1%2").arg(QSLOT_CODE).arg(metaMethod.signature()).toLatin1().data());
    }
  }

  for (int i = CustomActionsManagerHelper::staticMetaObject.methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
    metaMethod = helper->metaObject()->method(i);
    if (metaMethod.methodType() == QMetaMethod::Signal
        && (metaObject()->indexOfSignal(metaMethod.signature()) != -1)) {
      connect(helper, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data()
              , this, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data());
    }
  }

  connect(helper, SIGNAL(blockFocusChanged(bool))
          , this, SLOT(blockFocusChanged(bool)));
}

void CustomActionsManager::disconnectFromHelper(CustomActionsManagerHelper* helper)
{
  Q_D(CustomActionsManager);

  QList<QAction*> additionalActions = helper->additionalActions();
  if (!additionalActions.isEmpty()) {
    int additionalActionsCount = additionalActions.count();
    QHash<QWidget*, QAction*>::ConstIterator it = d->widgets.constBegin();
    QHash<QWidget*, QAction*>::ConstIterator end = d->widgets.constEnd();
    for (; it != end; ++it) {
      //it.key()->removeAction(d->additionalActionsSeparator);
      for (int i = 0; i < additionalActionsCount; ++i)
        it.key()->removeAction(additionalActions.at(i));
    }
  }

  foreach(QAction * action, actions())
    action->setEnabled(false);

  disconnect(helper, 0
             , this, 0);

  disconnect(this, 0
             , helper, 0);
}

void CustomActionsManager::retranslateUi()
{
  Q_D(CustomActionsManager);
  QSet<CustomActionsManagerHelper*>::ConstIterator it = d->helpers.constBegin();
  QSet<CustomActionsManagerHelper*>::ConstIterator end = d->helpers.constEnd();
  while (it != end) {
    (*it)->retranslateUi();
    ++it;
  }
}

CustomActionsManagerPrivate::CustomActionsManagerPrivate()
  : parent(0),isFocusChangedBlocked(false),currentHelper(0) {}
CustomActionsManagerPrivate::~CustomActionsManagerPrivate() {}
