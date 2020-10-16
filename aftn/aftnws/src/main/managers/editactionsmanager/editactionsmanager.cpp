#include "editactionsmanager.h"
#include "private/editactionsmanager_p.h"

#include "editactionsmanagerhelper.h"

#include <QtDebug>


#include <QtCore/QMetaMethod>

#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QUndoView>
#include <QtGui/QUndoGroup>

EditActionsManager::~EditActionsManager()
{
  delete d_ptr;
}


EditActionsManager::EditActionsManager(QWidget* parent)
  : QObject(parent)
  , d_ptr(new EditActionsManagerPrivate)
{
  Q_D(EditActionsManager);

  setObjectName("editActionsManager");

  d->parent = parent;

  QAction* a;

  a = d->editSeparator = new QAction(this);
  a->setObjectName("editSeparator");
  a->setSeparator(true);
  a = d->selectSeparator = new QAction(this);
  a->setObjectName("selectSeparator");
  a->setSeparator(true);

  a = d->undoAction = new QAction(QIcon::fromTheme("edit-undo"
                                  , QIcon(":/images/edit/edit-undo.png")),
                                  tr("&Undo"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Undo);
  a->setObjectName("undoAction");
  a->setToolTip(tr("Undo last action"));
  a->setStatusTip(tr("Undo last action"));
  // connect to signal, that changed enable status
  connect(this, SIGNAL(canUndoChanged(bool))
          , a, SLOT(setEnabled(bool)));

  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(undo()));

  a = d->redoAction = new QAction(QIcon::fromTheme("edit-redo"
                                  , QIcon(":/images/edit/edit-redo.png")),
                                  tr("&Redo"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Redo);
  a->setToolTip(tr("Redo last action"));
  a->setStatusTip(tr("Redo last action"));
  a->setObjectName("redoAction");

  // connect to signal, that changed enable status
  connect(this, SIGNAL(canRedoChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(redo()));

  a = d->cutAction = new QAction(QIcon::fromTheme("edit-cut"
                                 , QIcon(":/images/edit/edit-cut.png")),
                                 tr("Cu&t"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Cut);
  a->setObjectName("cutAction");
  a->setToolTip(tr("Cut selected cells into clipboard"));
  a->setStatusTip(tr("Cut selection into clipboard"));

  connect(this, SIGNAL(canCutChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(cut()));

  a = d->copyAction = new QAction(QIcon::fromTheme("edit-copy"
                                  , QIcon(":/images/edit/edit-copy.png")),
                                  tr("&Copy"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Copy);
  a->setObjectName("copyAction");
  a->setToolTip(tr("Copy selection into clipboard"));
  a->setStatusTip(tr("Copy selection into clipboard"));
  connect(this, SIGNAL(canCopyChanged(bool))
          , a, SLOT(setEnabled(bool)));

  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(copy()));

  a = d->pasteAction = new QAction(QIcon::fromTheme("edit-paste"
                                   , QIcon(":/images/edit/edit-paste.png")),
                                   tr("&Paste"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::Paste);
  a->setObjectName("pasteAction");
  a->setToolTip(tr("Paste text from clipboard"));
  a->setStatusTip(tr("Paste text from clipboard"));
  connect(this, SIGNAL(canPasteChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(paste()));


  a = d->clearAction = new QAction(QIcon::fromTheme("edit-clear"
                                   , QIcon(":/images/edit/edit-clear.png")),
                                   tr("&Clear"), this);
  a->setPriority(QAction::LowPriority);
  //a->setShortcut(Qt::Key_Control + Qt::Key_D);
  a->setToolTip(tr("Remove selection"));
  a->setStatusTip(tr("Remove selection"));
  a->setObjectName("clearAction");

  connect(this, SIGNAL(canClearChanged(bool))
          , a, SLOT(setEnabled(bool)));

  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(clear()));

  a = d->selectAllAction = new QAction(QIcon::fromTheme("edit-select-all"
                                       , QIcon(":/images/edit/edit-select-all.png")),
                                       tr("&Select All"), this);
  a->setPriority(QAction::LowPriority);
  a->setShortcut(QKeySequence::SelectAll);
  a->setObjectName("selectAllAction");
  a->setToolTip("Select all");
  a->setStatusTip("Select all");
  connect(this, SIGNAL(canSelectAllChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(selectAll()));

  foreach(QAction * action, actions())
    action->setEnabled(false);

  addStandardHelpers();

  connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*))
          , this, SLOT(focusChanged(QWidget*, QWidget*)));

  //focusChanged(0, QApplication::focusWidget());
}

void EditActionsManager::addStandardHelpers()
{
  addHelper(new LineEditEditActionsManagerHelper(this));
  addHelper(new TextEditEditActionsManagerHelper(this));
//  addHelper(new ItemViewEditActionsManagerHelper(this));
}

QList<QAction*> EditActionsManager::actions() const
{
  Q_D(const EditActionsManager);
  QList<QAction*> result;
  result << d->undoAction
         << d->redoAction
         << d->editSeparator
         << d->cutAction
         << d->copyAction
         << d->pasteAction
         << d->clearAction
         << d->selectSeparator
         << d->selectAllAction;

  return result;
}


void EditActionsManager::addWidget(QWidget* widget, QAction* before)
{
  Q_D(EditActionsManager);
  d->widgets.insert(widget, before);

  widget->insertActions(before, actions());
}

void EditActionsManager::removeWidget(QWidget* widget)
{
  Q_D(EditActionsManager);

  foreach(QAction * action, actions())
  widget->removeAction(action);

  d->widgets.remove(widget);
}


void EditActionsManager::focusChanged(QWidget* old, QWidget* now)
{
  Q_D(EditActionsManager);
  if (!d->isFocusChangedBlocked) {
    if (d->currentHelper) {
      disconnectFromHelper(d->currentHelper);
      d->currentHelper->deactivate();
      d->currentHelper = 0;
    }

    if (!d->parent->isAncestorOf(now)) return;

    if ((d->currentHelper = helperFor(now))) {
      d->currentHelper->setActions(actions());
      d->currentHelper->activateFor(now);
      connectToHelper(d->currentHelper);
    }
  }
}

void EditActionsManager::blockFocusChanged(bool blocked)
{
  Q_D(EditActionsManager);
  d->isFocusChangedBlocked = blocked;
}

void EditActionsManager::addHelper(EditActionsManagerHelper* helper)
{
  Q_D(EditActionsManager);
  d->helpers.insert(helper);
}

void EditActionsManager::removeHelper(EditActionsManagerHelper* helper)
{
  Q_D(EditActionsManager);
  if (d->currentHelper == helper) {
    disconnectFromHelper(helper);
    d->currentHelper = 0;
  }
  d->helpers.remove(helper);
}

QSet<EditActionsManagerHelper*> EditActionsManager::helpers() const
{
  Q_D(const EditActionsManager);
  return d->helpers;
}

void EditActionsManager::addUndoView(QUndoView* undoView)
{
  Q_D(EditActionsManager);
  if (!undoView || d->undoViews.contains(undoView))
    return;
  d->undoViews.insert(undoView);
}

void EditActionsManager::removeUndoView(QUndoView* undoView)
{
  Q_D(EditActionsManager);
  d->undoViews.remove(undoView);
}

QSet<QUndoView*> EditActionsManager::undoViews()
{
  Q_D(const EditActionsManager);
  return d->undoViews;
}

EditActionsManagerHelper* EditActionsManager::helperFor(QWidget* widget) const
{
  Q_D(const EditActionsManager);
  QSet<EditActionsManagerHelper*>::ConstIterator it = d->helpers.constBegin();
  QSet<EditActionsManagerHelper*>::ConstIterator end = d->helpers.constEnd();
  int maxApproach = 0;
  int approach;
  EditActionsManagerHelper *findedHelper = 0;
  while (it != end) {
    approach = (*it)->approach(widget);
    if (approach > maxApproach) {
      maxApproach = approach;
      findedHelper = (*it);
    }
    ++it;
  }

  return findedHelper;
}

void EditActionsManager::connectToHelper(EditActionsManagerHelper* helper)
{
  Q_D(EditActionsManager);

  QList<QAction*> additionalActions = helper->additionalActions();
  if (!additionalActions.isEmpty()) {
    QHash<QWidget*, QAction*>::ConstIterator it = d->widgets.constBegin();
    QHash<QWidget*, QAction*>::ConstIterator end = d->widgets.constEnd();
    for (; it != end; ++it) {
      it.key()->insertAction(it.value(), d->additionalActionsSeparator);
      it.key()->insertActions(it.value(), additionalActions);
    }
  }

  QUndoStack* undoStack = helper->undoStack();
  if (undoStack) {
    QSet<QUndoView *>::ConstIterator undoViewsIt = d->undoViews.begin();
    QSet<QUndoView *>::ConstIterator undoViewsEnd = d->undoViews.end();
    while (undoViewsIt != undoViewsEnd) {
      (*undoViewsIt)->setStack(undoStack);
      ++undoViewsIt;
    }
  }

  d->undoAction->setEnabled(helper->canUndo());
  d->redoAction->setEnabled(helper->canRedo());
  d->cutAction->setEnabled(helper->canCut());
  d->copyAction->setEnabled(helper->canCopy());
  d->pasteAction->setEnabled(helper->canPaste());
  d->clearAction->setEnabled(helper->canClear());
  d->selectAllAction->setEnabled(helper->canSelectAll());

  QMetaMethod metaMethod;

  for (int i = helper->metaObject()->methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
    metaMethod = helper->metaObject()->method(i);
    if (metaMethod.methodType() == QMetaMethod::Slot
        && (metaObject()->indexOfSignal(metaMethod.signature()) != -1)) {
      connect(this, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data()
              , helper, QString("%1%2").arg(QSLOT_CODE).arg(metaMethod.signature()).toLatin1().data());
    }
  }

  for (int i = EditActionsManagerHelper::staticMetaObject.methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
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

void EditActionsManager::disconnectFromHelper(EditActionsManagerHelper* helper)
{
  Q_D(EditActionsManager);

  QList<QAction*> additionalActions = helper->additionalActions();
  if (!additionalActions.isEmpty()) {
    int additionalActionsCount = additionalActions.count();
    QHash<QWidget*, QAction*>::ConstIterator it = d->widgets.constBegin();
    QHash<QWidget*, QAction*>::ConstIterator end = d->widgets.constEnd();
    for (; it != end; ++it) {
      it.key()->removeAction(d->additionalActionsSeparator);
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

EditActionsManagerPrivate::EditActionsManagerPrivate()
  : currentHelper(0) {}
EditActionsManagerPrivate::~EditActionsManagerPrivate() {}

void EditActionsManager::retranslateUi()
{
  Q_D(EditActionsManager);
  QAction *a = d->undoAction;
  a->setText(tr("&Undo"));
  a->setToolTip(tr("Undo last action"));
  a->setStatusTip(tr("Undo last action"));

  a = d->redoAction;
  a->setText(tr("&Redo"));
  a->setToolTip(tr("Redo last action"));
  a->setStatusTip(tr("Redo last action"));

  a = d->cutAction;
  a->setText(tr("Cu&t"));
  a->setToolTip(tr("Cut selected cells into clipboard"));
  a->setStatusTip(tr("Cut selection into clipboard"));

  a = d->copyAction;
  a->setText(tr("&Copy"));
  a->setToolTip(tr("Copy selection into clipboard"));
  a->setStatusTip(tr("Copy selection into clipboard"));

  a = d->pasteAction;
  a->setText(tr("&Paste"));
  a->setToolTip(tr("Paste text from clipboard"));
  a->setStatusTip(tr("Paste text from clipboard"));

  a = d->clearAction;
  a->setText(tr("&Clear"));
  a->setToolTip(tr("Remove selection"));
  a->setStatusTip(tr("Remove selection"));

  a = d->selectAllAction;
  a->setText(tr("&Select All"));
  a->setToolTip("Select all");
  a->setStatusTip("Select all");
}
