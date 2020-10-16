#ifndef MAINWINDOWS_P_H
#define MAINWINDOWS_P_H

#include "mainwindow.h"

class QtWindowMenu;
class QtToolBarManager;
class QUndoView;
class QLabel;

class FileActionsManager;
class EditActionsManager;
class CustomActionsManager;
class FindActionsManager;
class NavigateActionsManager;
class PrintActionsManager;


class StandardTreeModel;

class QTcpSocket;

class MainWindowPrivate
{
public:
  MainWindowPrivate();
  ~MainWindowPrivate();

  RedToolButton* manualMessageButton;
  RedToolButton* aftnServiceButton;
  int redrawTitleTimerId;

  bool firstShow;

  QMenu *fileMenu;
  QAction *printActionsSeparator;
  QAction *exitActionSeparator;
  QAction *exitAction;

  QMenu *editMenu;
  QMenu *customMenu;
  QMenu *navigateMenu;

  QtWindowMenu *windowMenu;

  QMenu *aboutMenu;

  QMenu *scriptEngineMenu;
  QAction *evaluateFile;
  QAction *evaulateFileWithDebugger;
  QAction *restartEngine;
  QAction *showDebugger;

  QToolBar *fileToolBar;
  QToolBar *printToolBar;
  QToolBar *editToolBar;
  QToolBar *customToolBar;
  QToolBar *navigateToolBar;

  QtToolBarManager* toolBarManager;

  QAction *searchWidgetAction;
  QAction *configureToolBarsAction;

  QUndoView *undoView;

  FileActionsManager *fileActionsManager;

  EditActionsManager *editActionsManager;

  CustomActionsManager *customActionsManager;

  FindActionsManager *findActionsManager;

  NavigateActionsManager *navigateActionsManager;

  PrintActionsManager *printActionsManager;

  StandardTreeModel *dbObjectsModels;

  QLabel *m_pUserStatusLabel;

  bool m_bShutdown;
  bool m_bFullScreen;

};

#endif
