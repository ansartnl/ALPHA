#ifndef PRINTACTIONSMANAGER_P_H
#define PRINTACTIONSMANAGER_P_H

#include "printactionsmanager.h"

#include <QtCore/QSet>

class QWidget;
class PrintActionsManagerPrivate
{
public:
  PrintActionsManagerPrivate();
  virtual ~PrintActionsManagerPrivate();

  QWidget *parent;

  bool isFocusChangedBlocked;

  QHash<QWidget*, QAction*> widgets;

  QSet<PrintActionsManagerHelper*> helpers;

  PrintActionsManagerHelper *currentHelper;

  QAction *pageSetupAction;
  QAction *printPreviewAction;
  QAction *printAction;
};

#endif // PRINTACTIONSMANAGER_P_H
