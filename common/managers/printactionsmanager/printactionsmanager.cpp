#include "printactionsmanager.h"
#include "private/printactionsmanager_p.h"

#include "printactionsmanagerhelper.h"

#include <QtDebug>

#include <QtCore/QMetaMethod>

#include <QtGui/QApplication>
#include <QtGui/QAction>

PrintActionsManager::~PrintActionsManager()
{
  delete d_ptr;
}


PrintActionsManager::PrintActionsManager(QWidget *parent)
  :QObject(parent)
  , d_ptr(new PrintActionsManagerPrivate)
  , isFocusChangedBlocked(false)
{
  Q_D(PrintActionsManager);

  d->parent = parent;

  QAction* a;

  a = d->pageSetupAction = new QAction(QIcon::fromTheme("document-page-setup"
     , QIcon(":/images/document/document-page-setup.png"))
                                   , tr("Page setup..."), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("pageSetupAction");
  a->setToolTip(tr("Setup page"));
  a->setStatusTip(tr("Setup page"));
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(pageSetup()));
  connect(this, SIGNAL(canPageSetupChanged(bool))
          , a, SLOT(setEnabled(bool)));


  a = d->printPreviewAction = new QAction(QIcon::fromTheme("document-print-preview"
    , QIcon(":/images/document/document-print-preview.png"))
                                  , tr("Preview..."), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("printPreviewAction");
  a->setToolTip(tr("Preview"));
  a->setStatusTip(tr("Preview"));
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(printPreview()));
  connect(this, SIGNAL(canPrintPreviewChanged(bool))
          , a, SLOT(setEnabled(bool)));

  a = d->printAction = new QAction(QIcon::fromTheme("document-print"
                                                    , QIcon(":/images/document/document-print.png"))
                                   , tr("Print..."), this);
  a->setPriority(QAction::LowPriority);
  a->setObjectName("printAction");
  a->setToolTip(tr("Print"));
  a->setStatusTip(tr("Print"));
  a->setShortcut(QKeySequence::Print);
  connect(a, SIGNAL(triggered())
          , this, SIGNAL(print()));
  connect(this, SIGNAL(canPrintChanged(bool))
          , a, SLOT(setEnabled(bool)));

  foreach(QAction * action, actions())
    action->setEnabled(false);

  addStandartHelpers();

  connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*))
          , this, SLOT(focusChanged(QWidget*,QWidget*)));
  focusChanged(0, QApplication::focusWidget());
}

void PrintActionsManager::addStandartHelpers()
{
  addHelper(new TextEditPrintActionsManagerHelper(this));
  addHelper(new TableViewPrintActionsManagerHelper(this));
  addHelper(new ItemViewPrintActionsManagerHelper(this));
  addHelper(new GraphicsViewPrintActionsManagerHelper(this));
}

QList<QAction *> PrintActionsManager::actions() const
{
  Q_D(const PrintActionsManager);
  QList<QAction*> result;
  result << d->pageSetupAction
      << d->printPreviewAction
      << d->printAction;

  return result;
}


void PrintActionsManager::addWidget(QWidget* widget, QAction* before)
{
  Q_D(PrintActionsManager);
  d->widgets.insert(widget, before);

  widget->insertActions(before, actions());
}

void PrintActionsManager::removeWidget(QWidget* widget)
{
  Q_D(PrintActionsManager);

  d->widgets.remove(widget);
}

void PrintActionsManager::focusChanged(QWidget *old, QWidget *now)
{
  Q_UNUSED(old)
  Q_D(PrintActionsManager);
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

void PrintActionsManager::blockFocusChanged(bool blocked)
{
  Q_D(PrintActionsManager);
  d->isFocusChangedBlocked = blocked;
}

void PrintActionsManager::addHelper(PrintActionsManagerHelper *helper)
{
  Q_D(PrintActionsManager);
  d->helpers.insert(helper);
}

void PrintActionsManager::removeHelper(PrintActionsManagerHelper *helper)
{
  Q_D(PrintActionsManager);
  if (helperFor(QApplication::focusWidget()) == helper) {
    disconnectFromHelper(helper);
  }
  d->helpers.remove(helper);
}

QSet<PrintActionsManagerHelper *> PrintActionsManager::helpers() const
{
  Q_D(const PrintActionsManager);
  return d->helpers;
}

PrintActionsManagerHelper *PrintActionsManager::helperFor(QWidget *widget) const
{
  Q_D(const PrintActionsManager);
  QSet<PrintActionsManagerHelper*>::ConstIterator it = d->helpers.constBegin();
  QSet<PrintActionsManagerHelper*>::ConstIterator end = d->helpers.constEnd();
  int maxApproach = 0;
  int approach;
  PrintActionsManagerHelper *findedHelper = 0;
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

void PrintActionsManager::connectToHelper(PrintActionsManagerHelper *helper)
{
  Q_D(PrintActionsManager);

  d->pageSetupAction->setEnabled(helper->canPageSetup());
  d->printPreviewAction->setEnabled(helper->canPrintPreview());
  d->printAction->setEnabled(helper->canPrint());

  QMetaMethod metaMethod;

  for (int i = helper->metaObject()->methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
    metaMethod = helper->metaObject()->method(i);
    if (metaMethod.methodType() == QMetaMethod::Slot
        && (metaObject()->indexOfSignal(metaMethod.signature()) != -1)) {
      connect(this, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data()
              , helper, QString("%1%2").arg(QSLOT_CODE).arg(metaMethod.signature()).toLatin1().data());
    }
  }

  for (int i = PrintActionsManagerHelper::staticMetaObject.methodOffset(); i < helper->metaObject()->methodCount(); ++i) {
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

void PrintActionsManager::disconnectFromHelper(PrintActionsManagerHelper *helper)
{
  //Q_D(PrintActionsManager);

  QList<QAction*> childActions = actions();

  for (int i = 0; i < childActions.count(); ++i)
    childActions.at(i)->setEnabled(false);

  disconnect(helper, 0
             , this, 0);
  disconnect(this, 0
             , helper, 0);
}


PrintActionsManagerPrivate::PrintActionsManagerPrivate()
  :isFocusChangedBlocked(false),currentHelper(0){}
PrintActionsManagerPrivate::~PrintActionsManagerPrivate(){}

void PrintActionsManager::retranslateUi()
{
  Q_D(PrintActionsManager);
  QAction* a;

  a = d->pageSetupAction;
  a->setText(tr("Page setup..."));
  a->setToolTip(tr("Setup page"));
  a->setStatusTip(tr("Setup page"));

  a = d->printPreviewAction;
  a->setText(tr("Preview..."));
  a->setToolTip(tr("Preview"));
  a->setStatusTip(tr("Preview"));

  a = d->printAction;
  a->setText(tr("Print..."));
  a->setToolTip(tr("Print"));
  a->setStatusTip(tr("Print"));
}
