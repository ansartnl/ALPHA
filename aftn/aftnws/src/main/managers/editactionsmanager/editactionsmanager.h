#ifndef EDITACTIONSMANAGER_H
#define EDITACTIONSMANAGER_H

#include <QtCore/QObject>

class QAction;
class QUndoView;
//class SearchWidget;

class EditActionsManagerPrivate;
class EditActionsManagerHelper;
/**
Class to manage the actions of undo-redo-cut-copy-paste framework
*
* @class EditActionsManager editactionsmanager.h
*/
class EditActionsManager : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(EditActionsManager)
protected:
  EditActionsManagerPrivate * const d_ptr;
public:
  EditActionsManager(QWidget* parent);
  virtual ~EditActionsManager();

  void addWidget(QWidget* widget, QAction* before = 0);
  void removeWidget(QWidget* widget);

  void addHelper(EditActionsManagerHelper* helper);
  void removeHelper(EditActionsManagerHelper* helper);
  QSet<EditActionsManagerHelper*> helpers() const;

  void addUndoView(QUndoView *undoView);
  void removeUndoView(QUndoView *undoView);
  QSet<QUndoView*> undoViews();

  void retranslateUi();

signals:
  void canUndoChanged(bool enable);
  void canRedoChanged(bool enable);

  void canCutChanged(bool enable);
  void canCopyChanged(bool enable);
  void canPasteChanged(bool enable);
  void canClearChanged(bool enable);

  void canSelectAllChanged(bool enable);

signals:
  void undo();
  void redo();

  void cut();
  void copy();
  void paste();
  void clear();

  void selectAll();

protected slots:
  virtual void focusChanged(QWidget* old, QWidget* now);

  void blockFocusChanged(bool blocked);

protected:
  EditActionsManagerHelper* helperFor(QWidget* widget)const;
  void connectToHelper(EditActionsManagerHelper* helper);
  void disconnectFromHelper(EditActionsManagerHelper* helper);
  QList<QAction*> actions() const;

private:
  void addStandardHelpers();
};

#endif // EDITACTIONSMANAGER_H
