#include "navigateactionsmanager.h"
#include "private/navigateactionsmanager_p.h"

#include "navigateactionsmanagerhelper.h"

#include <QtDebug>

#include <QtCore/QMetaMethod>

#include <QtGui/QApplication>
#include <QtGui/QAction>

NavigateActionsManager::~NavigateActionsManager()
{
  delete d_ptr;
}


NavigateActionsManager::NavigateActionsManager(QWidget* parent)
  : QObject(parent)
  , d_ptr(new NavigateActionsManagerPrivate)
{
  Q_D(NavigateActionsManager);

  d->parent = parent;

  QAction* a;

  a = d->firstAction = new QAction(QIcon::fromTheme("go-first"
                                   , QIcon(":/images/go/go-first.png"))
                                   , tr("First"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("firstAction");
  a->setToolTip(tr("Move current cursor to first record in model"));
  a->setStatusTip(tr("Move current cursor to first record in model"));
  a->setShortcut(QKeySequence::MoveToStartOfLine);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(first()));
  connect(this, SIGNAL(canFirstChanged(bool))
          , a, SLOT(setEnabled(bool)));

  a = d->previousAction = new QAction(QIcon::fromTheme("go-previous"
                                      , QIcon(":/images/go/go-previous.png"))
                                      , tr("Previous"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("previousAction");
  a->setToolTip(tr("Move current cursor to previous record in model"));
  a->setStatusTip(tr("Move current cursor to previous record in model"));
  a->setShortcut(QKeySequence::MoveToPreviousChar);
  a->setAutoRepeat(true);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(previous()));
  connect(this, SIGNAL(canPreviousChanged(bool))
          , a, SLOT(setEnabled(bool)));

  a = d->nextAction = new QAction(QIcon::fromTheme("go-next"
                                  , QIcon(":/images/go/go-next.png"))
                                  , tr("Next"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("nextAction");
  a->setToolTip(tr("Move current cursor to next record in model"));
  a->setStatusTip(tr("Move current cursor to next record in model"));
  a->setShortcut(QKeySequence::MoveToNextChar);
  a->setAutoRepeat(true);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(next()));
  connect(this, SIGNAL(canNextChanged(bool))
          , a, SLOT(setEnabled(bool)));

  a = d->lastAction = new QAction(QIcon::fromTheme("go-last"
                                  , QIcon(":/images/go/go-last.png"))
                                  , tr("Last"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("lastAction");
  a->setToolTip(tr("Move current cursor to last record in model"));
  a->setStatusTip(tr("Move current cursor to last record in model"));
  a->setShortcut(QKeySequence::MoveToEndOfLine);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(last()));
  connect(this, SIGNAL(canLastChanged(bool))
          , a, SLOT(setEnabled(bool)));


  a = d->upAction = new QAction(QIcon::fromTheme("go-up"
                                , QIcon(":/images/go/go-up.png"))
                                , tr("Up"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("upAction");
  a->setToolTip(tr("Move current cursor to up record in model"));
  a->setStatusTip(tr("Move current cursor to up record in model"));
  a->setShortcut(QKeySequence::MoveToPreviousWord);
  a->setAutoRepeat(true);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(up()));
  connect(this, SIGNAL(canUpChanged(bool))
          , a, SLOT(setEnabled(bool)));
  a = d->downAction = new QAction(QIcon::fromTheme("go-down"
                                  , QIcon(":/images/go/go-down.png"))
                                  , tr("Down"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("downAction");
  a->setToolTip(tr("Move current cursor to down record in model"));
  a->setStatusTip(tr("Move current cursor to down record in model"));
  a->setShortcut(QKeySequence::MoveToNextWord);
  a->setAutoRepeat(true);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(down()));
  connect(this, SIGNAL(canDownChanged(bool))
          , a, SLOT(setEnabled(bool)));
  a = d->topAction = new QAction(QIcon::fromTheme("go-top"
                                 , QIcon(":/images/go/go-top.png"))
                                 , tr("Top"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("topAction");
  a->setToolTip(tr("Move current cursor to top record in model"));
  a->setStatusTip(tr("Move current cursor to top record in model"));
  a->setShortcut(QKeySequence::MoveToStartOfDocument);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(top()));
  connect(this, SIGNAL(canTopChanged(bool))
          , a, SLOT(setEnabled(bool)));
  a = d->bottomAction = new QAction(QIcon::fromTheme("go-bottom"
                                    , QIcon(":/images/go/go-bottom.png"))
                                    , tr("Bottom"), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("bottomAction");
  a->setToolTip(tr("Move current cursor to bottom record in model"));
  a->setStatusTip(tr("Move current cursor to bottom record in model"));
  a->setShortcut(QKeySequence::MoveToEndOfDocument);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(bottom()));
  connect(this, SIGNAL(canBottomChanged(bool))
          , a, SLOT(setEnabled(bool)));

  foreach(QAction * action, actions())
  action->setEnabled(false);

  addStandartHelpers();

  connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*))
          , this, SLOT(focusChanged(QWidget*, QWidget*)));
  focusChanged(0, QApplication::focusWidget());
}

void NavigateActionsManager::addStandartHelpers()
{
  //addHelper(new ItemViewNavigateActionsManagerHelper(this));
  addHelper(new ListViewNavigateActionsManagerHelper(this));
  addHelper(new TableViewNavigateActionsManagerHelper(this));
  addHelper(new TreeViewNavigateActionsManagerHelper(this));
}

QList<QAction*> NavigateActionsManager::actions() const
{
  Q_D(const NavigateActionsManager);
  QList<QAction*> result;
  result << d->firstAction
         << d->previousAction
         << d->upAction
         << d->topAction
         << d->downAction
         << d->bottomAction
         << d->nextAction
         << d->lastAction;

  return result;
}


void NavigateActionsManager::addWidget(QWidget* widget, QAction* before)
{
  Q_D(NavigateActionsManager);
  d->widgets.insert(widget, before);

  widget->insertActions(before, actions());
}

void NavigateActionsManager::removeWidget(QWidget* widget)
{
  Q_D(NavigateActionsManager);

  d->widgets.remove(widget);
}

void NavigateActionsManager::focusChanged(QWidget* old, QWidget* now)
{
  Q_D(NavigateActionsManager);

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

void NavigateActionsManager::blockFocusChanged(bool blocked)
{
  Q_D(NavigateActionsManager);
  d->isFocusChangedBlocked = blocked;
}

void NavigateActionsManager::addHelper(NavigateActionsManagerHelper* helper)
{
  Q_D(NavigateActionsManager);
  d->helpers.insert(helper);
}

void NavigateActionsManager::removeHelper(NavigateActionsManagerHelper* helper)
{
  Q_D(NavigateActionsManager);

  if (d->currentHelper == helper) {
    disconnectFromHelper(helper);
    d->currentHelper = 0;
  }

  d->helpers.remove(helper);
}

QSet<NavigateActionsManagerHelper*> NavigateActionsManager::helpers() const
{
  Q_D(const NavigateActionsManager);
  return d->helpers;
}

NavigateActionsManagerHelper* NavigateActionsManager::helperFor(QWidget* widget) const
{
  Q_D(const NavigateActionsManager);
  QSet<NavigateActionsManagerHelper*>::ConstIterator it = d->helpers.constBegin();
  QSet<NavigateActionsManagerHelper*>::ConstIterator end = d->helpers.constEnd();

  NavigateActionsManagerHelper* findedHelper = 0;
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

void NavigateActionsManager::connectToHelper(NavigateActionsManagerHelper* helper)
{
  Q_D(NavigateActionsManager);

  d->firstAction->setEnabled(helper->canFirst());
  d->previousAction->setEnabled(helper->canPrevious());
  d->nextAction->setEnabled(helper->canNext());
  d->lastAction->setEnabled(helper->canLast());
  d->upAction->setEnabled(helper->canUp());
  d->downAction->setEnabled(helper->canDown());
  d->topAction->setEnabled(helper->canTop());
  d->bottomAction->setEnabled(helper->canBottom());

  QMetaMethod metaMethod;

  for (int i = helper->metaObject()->methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
    metaMethod = helper->metaObject()->method(i);

    if (metaMethod.methodType() == QMetaMethod::Slot
        && (metaObject()->indexOfSignal(metaMethod.signature()) != -1)) {
      connect(this, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data()
              , helper, QString("%1%2").arg(QSLOT_CODE).arg(metaMethod.signature()).toLatin1().data());
    }
  }

  for (int i = NavigateActionsManagerHelper::staticMetaObject.methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
    metaMethod = helper->metaObject()->method(i);

    if (metaMethod.methodType() == QMetaMethod::Signal
        && (metaObject()->indexOfSignal(metaMethod.signature()) != -1)) {
      connect(helper, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data()
              , this, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data());
    }
  }
}

void NavigateActionsManager::disconnectFromHelper(NavigateActionsManagerHelper* helper)
{
  //Q_D(NavigateActionsManager);

  QList<QAction*> childActions = actions();

  for (int i = 0; i < childActions.count(); ++i)
    childActions.at(i)->setEnabled(false);

  disconnect(helper, 0
             , this, 0);
  disconnect(this, 0
             , helper, 0);
}


NavigateActionsManagerPrivate::NavigateActionsManagerPrivate()
  : isFocusChangedBlocked(false), currentHelper(0) {}
NavigateActionsManagerPrivate::~NavigateActionsManagerPrivate() {}

void NavigateActionsManager::retranslateUi()
{
  Q_D(NavigateActionsManager);
  QAction* a;

  a = d->firstAction;
  a->setText(tr("First"));
  a->setToolTip(tr("Move current cursor to first record in model"));
  a->setStatusTip(tr("Move current cursor to first record in model"));

  a = d->previousAction;
  a->setText(tr("Previous"));
  a->setToolTip(tr("Move current cursor to previous record in model"));
  a->setStatusTip(tr("Move current cursor to previous record in model"));

  a = d->nextAction;
  a->setText( tr("Next"));
  a->setToolTip(tr("Move current cursor to next record in model"));
  a->setStatusTip(tr("Move current cursor to next record in model"));

  a = d->lastAction;
  a->setText(tr("Last"));
  a->setToolTip(tr("Move current cursor to last record in model"));
  a->setStatusTip(tr("Move current cursor to last record in model"));

  a = d->upAction;
  a->setText(tr("Up"));
  a->setToolTip(tr("Move current cursor to up record in model"));
  a->setStatusTip(tr("Move current cursor to up record in model"));

  a = d->downAction;
  a->setText(tr("Down"));
  a->setToolTip(tr("Move current cursor to down record in model"));
  a->setStatusTip(tr("Move current cursor to down record in model"));

  a = d->topAction;
  a->setText(tr("Top"));
  a->setToolTip(tr("Move current cursor to top record in model"));
  a->setStatusTip(tr("Move current cursor to top record in model"));

  a = d->bottomAction;
  a->setText(tr("Bottom"));
  a->setToolTip(tr("Move current cursor to bottom record in model"));
  a->setStatusTip(tr("Move current cursor to bottom record in model"));
}
