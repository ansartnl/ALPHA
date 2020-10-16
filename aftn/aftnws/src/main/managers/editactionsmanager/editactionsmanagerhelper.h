#ifndef EDITACTIONSMANAGERHELPER_H
#define EDITACTIONSMANAGERHELPER_H

#include <QtCore/QObject>

#include <QtGui/QAction>

class QUndoStack;
class QLineEdit;

/**
* @ Brief Helper for managing undo-redo-cut-copy-paster for a specific widget
*
* @class EditActionsManagerHelper editactionsmanagerhelper.h
*/
class EditActionsManagerHelper : public QObject
{
  Q_OBJECT
public:
  explicit EditActionsManagerHelper(QObject* parent = 0);
  virtual ~EditActionsManagerHelper();

  virtual unsigned char approach(QWidget *widget) const = 0;
  virtual void activateFor(QWidget* widget) = 0;
  virtual void deactivate() = 0;


  virtual bool canUndo() const {return false;};
  virtual bool canRedo() const {return false;};
  virtual QUndoStack* undoStack() const {return 0;};

  virtual bool canCut() const {return false;};
  virtual bool canCopy() const {return false;};
  virtual bool canPaste() const {return false;};
  virtual bool canClear() const {return false;};
  virtual bool canSelectAll() const {return false;};

  virtual QList<QAction*> additionalActions() const {return QList<QAction*>();};

  virtual void setActions(const QList<QAction*> actions) {mActions = actions;};
  QList<QAction*> actions() const {return mActions;};

public slots:
  virtual void undo(){};
  virtual void redo(){};
  virtual void cut(){};
  virtual void copy(){};
  virtual void paste(){};
  virtual void clear(){};
  virtual void selectAll(){};

  virtual void refresh(){};

signals:
  void canUndoChanged(bool enable);
  void canRedoChanged(bool enable);

  void canCutChanged(bool enable);
  void canCopyChanged(bool enable);
  void canPasteChanged(bool enable);
  void canClearChanged(bool enable);

  void canSelectAllChanged(bool enable);

  void blockFocusChanged(bool block);
private:
  QList<QAction *> mActions;
};

/**
* @ Brief Helper for managing undo-redo-cut-copy-paster for line edit
*
* @class LineEditEditActionsManagerHelper editactionsmanagerhelper.h
*/
class LineEditEditActionsManagerHelper : public EditActionsManagerHelper
{
  Q_OBJECT
public:
  explicit LineEditEditActionsManagerHelper(QObject* parent = 0);

  virtual unsigned char approach(QWidget* widget) const;

  virtual void activateFor(QWidget* widget);
  virtual void deactivate();

  virtual bool canUndo() const ;
  virtual bool canRedo() const ;
  virtual bool canCut() const ;
  virtual bool canCopy() const ;
  virtual bool canPaste() const ;
  virtual bool canClear() const ;
  virtual bool canSelectAll() const ;

public slots:
  virtual void undo();
  virtual void redo();
  virtual void cut();
  virtual void copy();
  virtual void paste();
  virtual void clear();
  virtual void selectAll();

private slots:
  void clipboardDataChanged();

  void lineEditTextChanged();
  void lineEditSelectionChanged();
private:
  QLineEdit *lineEdit;
};

class QTextEdit;
class QPlainTextEdit;
/**
* @ Brief Helper for managing undo-redo-cut-copy-paster for text edit
*
* @class TextEditEditActionsManagerHelper editactionsmanagerhelper.h
*/
class TextEditEditActionsManagerHelper : public EditActionsManagerHelper
{
  Q_OBJECT
public:
  explicit TextEditEditActionsManagerHelper(QObject* parent = 0);

  virtual unsigned char approach(QWidget* widget) const;

  virtual void activateFor(QWidget* widget);
  virtual void deactivate();

  virtual bool canUndo() const ;
  virtual bool canRedo() const ;

  //virtual QUndoStack *undoStack() const;

  virtual bool canCut() const ;
  virtual bool canCopy() const ;
  virtual bool canPaste() const ;
  virtual bool canClear() const ;
  virtual bool canSelectAll() const ;

public slots:
  virtual void undo();
  virtual void redo();
  virtual void cut();
  virtual void copy();
  virtual void paste();
  virtual void clear();
  virtual void selectAll();

private slots:
  void clipboardDataChanged();
private:
  QTextEdit *textEdit;
  QPlainTextEdit *plainTextEdit;
};


#define COPYPASTEROWSFORMAT "application/x-pg-modelrows"
#include <QtGui/QItemSelection>

class QAbstractItemView;
/**
* @ Brief Helper for managing undo-redo-cut-copy-paster for item view
*
* @class ItemViewEditActionsManagerHelper editactionsmanagerhelper.h
*/
class ItemViewEditActionsManagerHelper : public EditActionsManagerHelper
{
  Q_OBJECT
public:
  explicit ItemViewEditActionsManagerHelper(QObject* parent = 0);

  virtual unsigned char approach(QWidget* widget) const;

  virtual void activateFor(QWidget* widget);
  virtual void deactivate();

  virtual bool canUndo() const ;
  virtual bool canRedo() const ;
  virtual QUndoStack *undoStack() const;

  virtual bool canCut() const ;
  virtual bool canCopy() const ;
  virtual bool canPaste() const ;
  virtual bool canClear() const ;
  virtual bool canSelectAll() const ;

  //QList<QAction*> additionalActions() const;

public slots:
  virtual void undo();
  virtual void redo();
  virtual void cut();
  virtual void copy();
  virtual void paste();
  virtual void clear();
  virtual void selectAll();

protected:
  bool eventFilter(QObject *object, QEvent *event);

private slots:
  void clipboardDataChanged();

  void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
private:
  QAbstractItemView *itemView;
};

#endif //#ifdef EDITACTIONSMANAGERHELPER_H
