#ifndef EDITACTIONSMANAGER_P_H
#define EDITACTIONSMANAGER_P_H

#include "editactionsmanager.h"

#include <QtCore/QSet>

class QWidget;
class EditActionsManagerPrivate
{
public:
  EditActionsManagerPrivate();
  virtual ~EditActionsManagerPrivate();

  QWidget *parent;

  bool isFocusChangedBlocked;

  QHash<QWidget*, QAction*> widgets;

  QSet<EditActionsManagerHelper*> helpers;
  EditActionsManagerHelper *currentHelper;

  QSet<QUndoView *> undoViews;

  // general undo/redo framework
  QAction *undoAction;
  QAction *redoAction;

  QAction *editSeparator;
  QAction *cutAction;
  QAction *copyAction;
  QAction *pasteAction;
  QAction *clearAction;

  QAction *selectSeparator;
  QAction *selectAllAction;

  QAction *columnSeparator;

  QAction *additionalActionsSeparator;
};

#endif // EDITACTIONSMANAGER_P_H
