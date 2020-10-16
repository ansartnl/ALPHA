#ifndef CUSTOMACTIONSMANAGER_P_H
#define CUSTOMACTIONSMANAGER_P_H

#include "customactionsmanager.h"

#include <QtCore/QSet>

class QUndoGroup;
class QWidget;
class CustomActionsManagerPrivate
{
public:
  CustomActionsManagerPrivate();
  virtual ~CustomActionsManagerPrivate();

  QWidget *parent;

  bool isFocusChangedBlocked;

  QHash<QWidget*, QAction*> widgets;

  QSet<CustomActionsManagerHelper*> helpers;
  CustomActionsManagerHelper *currentHelper;

  QAction *additionalActionsSeparator;
};

#endif // CUSTOMACTIONSMANAGER_P_H
