#ifndef NAVIGATEACTIONSMANAGER_P_H
#define NAVIGATEACTIONSMANAGER_P_H

#include "navigateactionsmanager.h"

#include <QtCore/QSet>

class QWidget;
class NavigateActionsManagerPrivate
{
public:
  NavigateActionsManagerPrivate();
  virtual ~NavigateActionsManagerPrivate();

  QWidget *parent;

  bool isFocusChangedBlocked;

  QHash<QWidget*, QAction*> widgets;

  QSet<NavigateActionsManagerHelper*> helpers;
  NavigateActionsManagerHelper *currentHelper;

  QAction *firstAction;
  QAction *previousAction;
  QAction *upAction;
  QAction *topAction;
  QAction *downAction;
  QAction *bottomAction;
  QAction *nextAction;
  QAction *lastAction;
};

#endif // NAVIGATEACTIONSMANAGER_P_H
