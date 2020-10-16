#include "editactionsmanagerhelper.h"

#include <QtDebug>

#include <QtCore/QMimeData>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QAction>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

EditActionsManagerHelper::EditActionsManagerHelper(QObject* parent)
  : QObject(parent) {}

EditActionsManagerHelper::~EditActionsManagerHelper() {}

// --------------------------------------------------
// QLineEdit
LineEditEditActionsManagerHelper::LineEditEditActionsManagerHelper(QObject* parent)
  : EditActionsManagerHelper(parent)
  , lineEdit(0) {}

unsigned char LineEditEditActionsManagerHelper::approach(QWidget* widget) const
{
  QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widget);
  if (!lineEdit) {
    QComboBox* comboBox;
    QAbstractSpinBox* spinBox;
    if ((comboBox = qobject_cast<QComboBox*>(widget)))
      lineEdit = comboBox->lineEdit();
    else if ((spinBox = qobject_cast<QAbstractSpinBox*>(widget))) {
      lineEdit = spinBox->findChild<QLineEdit*>();
    }
  }
  return (lineEdit) ? 10 : 0;
}

void LineEditEditActionsManagerHelper::deactivate()
{
  disconnect(lineEdit, 0, this, 0);
  disconnect(QApplication::clipboard(), 0, this, 0);
  lineEdit = 0;
}

void LineEditEditActionsManagerHelper::activateFor(QWidget* widget)
{
  lineEdit = qobject_cast<QLineEdit*>(widget);
  if (!lineEdit) {
    QComboBox* comboBox;
    QAbstractSpinBox* spinBox;
    if ((comboBox = qobject_cast<QComboBox*>(widget)))
      lineEdit = comboBox->lineEdit();
    else if ((spinBox = qobject_cast<QAbstractSpinBox*>(widget))) {
      lineEdit = spinBox->findChild<QLineEdit*>();
    }
  }

  connect(lineEdit, SIGNAL(selectionChanged())
          , this, SLOT(lineEditSelectionChanged()));

  connect(lineEdit, SIGNAL(textChanged(QString))
          , this, SLOT(lineEditTextChanged()));

  connect(QApplication::clipboard(), SIGNAL(dataChanged())
          , this, SLOT(clipboardDataChanged()));
}

void LineEditEditActionsManagerHelper::clipboardDataChanged()
{
  emit canPasteChanged(!QApplication::clipboard()->text().isEmpty());
}

void LineEditEditActionsManagerHelper::lineEditTextChanged()
{
  emit canUndoChanged(canUndo());
  emit canRedoChanged(canRedo());

  // because after undo text selected but selection changed signal not emitted
  emit canCutChanged(canCut());
  emit canCopyChanged(canCopy());

  emit canClearChanged(canClear());

  emit canSelectAllChanged(canSelectAll());

}

void LineEditEditActionsManagerHelper::lineEditSelectionChanged()
{
  emit canCutChanged(canCut());
  emit canCopyChanged(canCopy());

  emit canSelectAllChanged(canSelectAll());
}

bool LineEditEditActionsManagerHelper::canUndo() const
{
  return lineEdit->isUndoAvailable();
  return false;
}

bool LineEditEditActionsManagerHelper::canRedo() const
{
  return lineEdit->isRedoAvailable();
  return false;
}

bool LineEditEditActionsManagerHelper::canCut() const
{
  return lineEdit->hasSelectedText();
  return false;
}

bool LineEditEditActionsManagerHelper::canCopy() const
{
  return lineEdit->hasSelectedText();
  return false;
}

bool LineEditEditActionsManagerHelper::canPaste() const
{
  return QApplication::clipboard()->mimeData()->hasText();
  return false;
}

bool LineEditEditActionsManagerHelper::canClear() const
{
  return !lineEdit->text().isEmpty();
  return false;
}

bool LineEditEditActionsManagerHelper::canSelectAll() const
{
  return lineEdit->selectedText() != lineEdit->text();
  return false;
}

void LineEditEditActionsManagerHelper::undo()
{
  lineEdit->undo();
}

void LineEditEditActionsManagerHelper::redo()
{
  lineEdit->redo();
}

void LineEditEditActionsManagerHelper::cut()
{
  lineEdit->cut();
}

void LineEditEditActionsManagerHelper::copy()
{
  lineEdit->copy();
}

void LineEditEditActionsManagerHelper::paste()
{
  lineEdit->paste();
}

void LineEditEditActionsManagerHelper::clear()
{
  lineEdit->clear();
}

void LineEditEditActionsManagerHelper::selectAll()
{
  lineEdit->selectAll();
}

#include <QtGui/QTextEdit>
#include <QtGui/QPlainTextEdit>

// -------------------------------------------------
// Text Edit
TextEditEditActionsManagerHelper::TextEditEditActionsManagerHelper(QObject* parent)
  : EditActionsManagerHelper(parent)
  , textEdit(0)
  , plainTextEdit(0)
{
}

unsigned char TextEditEditActionsManagerHelper::approach(QWidget* widget) const
{
  return qobject_cast<QTextEdit*>(widget) || qobject_cast<QPlainTextEdit*>(widget) ? 10 : 0;
}

void TextEditEditActionsManagerHelper::activateFor(QWidget* widget)
{
  if ((textEdit = qobject_cast<QTextEdit*>(widget))) {
    connect(textEdit, SIGNAL(undoAvailable(bool))
            , this, SIGNAL(canUndoChanged(bool)));
    connect(textEdit, SIGNAL(redoAvailable(bool))
            , this, SIGNAL(canRedoChanged(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool))
            , this, SIGNAL(canCopyChanged(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool))
            , this, SIGNAL(canCutChanged(bool)));

    connect(QApplication::clipboard(), SIGNAL(dataChanged())
            , this, SLOT(clipboardDataChanged()));
  } else if ((plainTextEdit = qobject_cast<QPlainTextEdit*>(widget))) {
    connect(plainTextEdit, SIGNAL(undoAvailable(bool))
            , this, SIGNAL(canUndoChanged(bool)));
    connect(plainTextEdit, SIGNAL(redoAvailable(bool))
            , this, SIGNAL(canRedoChanged(bool)));
    connect(plainTextEdit, SIGNAL(copyAvailable(bool))
            , this, SIGNAL(canCopyChanged(bool)));
    connect(plainTextEdit, SIGNAL(copyAvailable(bool))
            , this, SIGNAL(canCutChanged(bool)));

    connect(QApplication::clipboard(), SIGNAL(dataChanged())
            , this, SLOT(clipboardDataChanged()));
  }
}

void TextEditEditActionsManagerHelper::deactivate()
{
  if (textEdit ) {
    disconnect(textEdit, 0, this, 0);
    disconnect(QApplication::clipboard(), SIGNAL(dataChanged())
               , this, SLOT(clipboardDataChanged()));
    textEdit = 0;
  } else if (plainTextEdit ) {
    disconnect(plainTextEdit, 0, this, 0);
    disconnect(QApplication::clipboard(), SIGNAL(dataChanged())
               , this, SLOT(clipboardDataChanged()));
    plainTextEdit = 0;
  }
}

bool TextEditEditActionsManagerHelper::canUndo() const
{
  if (textEdit) return textEdit->document()->availableUndoSteps();
  if (plainTextEdit) return plainTextEdit->document()->availableUndoSteps();
  return false;
}

bool TextEditEditActionsManagerHelper::canRedo() const
{
  if (textEdit) return textEdit->document()->availableRedoSteps();
  if (plainTextEdit) return plainTextEdit->document()->availableRedoSteps();
  return false;
}

bool TextEditEditActionsManagerHelper::canCut() const
{
  if (textEdit) return !textEdit->isReadOnly() && textEdit->textCursor().hasSelection();
  if (plainTextEdit) return !plainTextEdit->isReadOnly() && plainTextEdit->textCursor().hasSelection();
  return false;
}

bool TextEditEditActionsManagerHelper::canCopy() const
{
  if (textEdit) return !textEdit->isReadOnly() && textEdit->textCursor().hasSelection();
  if (plainTextEdit) return !plainTextEdit->isReadOnly() && plainTextEdit->textCursor().hasSelection();
  return false;
}

bool TextEditEditActionsManagerHelper::canPaste() const
{
  if (textEdit) return !textEdit->isReadOnly() && textEdit->canPaste();
  if (plainTextEdit) return !plainTextEdit->isReadOnly() && plainTextEdit->canPaste();
  return false;
}

bool TextEditEditActionsManagerHelper::canClear() const
{
  if (textEdit) return !textEdit->isReadOnly();
  if (plainTextEdit) return !plainTextEdit->isReadOnly();
  return false;
}

bool TextEditEditActionsManagerHelper::canSelectAll() const
{
  if (textEdit) return true;
  if (plainTextEdit) return true;
  return false;
}

void TextEditEditActionsManagerHelper::undo()
{
  if (textEdit) textEdit->undo();
  if (plainTextEdit) plainTextEdit->undo();
}

void TextEditEditActionsManagerHelper::redo()
{
  if (textEdit) textEdit->redo();
  if (plainTextEdit) plainTextEdit->redo();
}

void TextEditEditActionsManagerHelper::cut()
{
  if (textEdit) textEdit->cut();
  if (plainTextEdit) plainTextEdit->cut();
}

void TextEditEditActionsManagerHelper::copy()
{
  if (textEdit) textEdit->copy();
  if (plainTextEdit) plainTextEdit->copy();
}

void TextEditEditActionsManagerHelper::paste()
{
  if (textEdit) textEdit->paste();
  if (plainTextEdit) plainTextEdit->paste();
}

void TextEditEditActionsManagerHelper::clear()
{
  if (textEdit) textEdit->clear();
  if (plainTextEdit) plainTextEdit->clear();
}

void TextEditEditActionsManagerHelper::selectAll()
{
  if (textEdit) textEdit->selectAll();
  if (plainTextEdit) plainTextEdit->selectAll();
}

void TextEditEditActionsManagerHelper::clipboardDataChanged()
{
  emit canPasteChanged(canPaste());
}

#include <QtGui/QAbstractItemView>
#include <QtGui/QTableView>
#include <QtGui/QMenu>
#include "ociquerymodel.h"
#include "modelinterfaces.h"
// ---------------------------------------------
// ItemView
ItemViewEditActionsManagerHelper::ItemViewEditActionsManagerHelper(QObject* parent)
  : EditActionsManagerHelper(parent)
  , itemView(0) {}

unsigned char ItemViewEditActionsManagerHelper::approach(QWidget* widget) const
{
  return qobject_cast<QAbstractItemView*>(widget) ? 10 : 0;
}

void ItemViewEditActionsManagerHelper::activateFor(QWidget* widget)
{
  if ((itemView = qobject_cast<QAbstractItemView*>(widget))) {
    connect(qApp->clipboard(), SIGNAL(dataChanged())
            , this, SLOT(clipboardDataChanged()));
    if (itemView->selectionModel()) {
      connect(itemView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
              this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
      selectionChanged(itemView->selectionModel()->selection(), itemView->selectionModel()->selection());
      clipboardDataChanged();
    }

    itemView->addActions(actions());

    itemView->installEventFilter(const_cast<ItemViewEditActionsManagerHelper*>(this));

    IUndoRedoModel* model = qobject_cast<IUndoRedoModel*>(itemView->model());
    if (model) {
      connect(model->undoStack(), SIGNAL(canUndoChanged(bool)), this, SIGNAL(canUndoChanged(bool)));
      connect(model->undoStack(), SIGNAL(canRedoChanged(bool)), this, SIGNAL(canRedoChanged(bool)));
    }
  }
}

void ItemViewEditActionsManagerHelper::deactivate()
{
  disconnect(qApp->clipboard(), SIGNAL(dataChanged())
             , this, SLOT(clipboardDataChanged()));
  if (itemView->selectionModel())
    disconnect(itemView->selectionModel(), 0
               , this, 0);

  for (int i = 0; i < actions().count(); ++i)
    itemView->removeAction(actions().at(i));

  itemView->removeEventFilter(const_cast<ItemViewEditActionsManagerHelper*>(this));

  IUndoRedoModel* model = qobject_cast<IUndoRedoModel*>(itemView->model());
  if (model) {
    disconnect(model->undoStack(), 0, this, 0);
  }
  itemView = 0;
}

bool ItemViewEditActionsManagerHelper::canUndo() const
{
  IUndoRedoModel* model = qobject_cast<IUndoRedoModel*>(itemView->model());
  if (model)
    return model->undoStack()->canUndo();
  return false;
}

bool ItemViewEditActionsManagerHelper::canRedo() const
{
  IUndoRedoModel* model = qobject_cast<IUndoRedoModel*>(itemView->model());
  if (model)
    return model->undoStack()->canRedo();
  return false;
}

QUndoStack* ItemViewEditActionsManagerHelper::undoStack() const
{
  IUndoRedoModel* model = qobject_cast<IUndoRedoModel*>(itemView->model());
  if (model)
    return model->undoStack();
  return 0;
}

bool ItemViewEditActionsManagerHelper::canCut() const
{
  if (itemView->selectionModel())
    return itemView->selectionModel()->selectedIndexes().count();
  return false;
}

bool ItemViewEditActionsManagerHelper::canCopy() const
{
  if (itemView->selectionModel())
    return itemView->selectionModel()->selectedIndexes().count();
  return false;
}

bool ItemViewEditActionsManagerHelper::canPaste() const
{
  if (itemView->model()) {
    if (itemView->currentIndex().flags() & Qt::ItemIsEditable) {
      QStringList types = itemView->model()->mimeTypes();
      int typesCount = types.count();
      const QMimeData* mimeData = QApplication::clipboard()->mimeData();
      for (int i = 0; i < typesCount; ++i) {
        if (mimeData->hasFormat(types.at(i)))
          return true;
      }
    }
  }

  return false;
}

bool ItemViewEditActionsManagerHelper::canClear() const
{
  if (itemView->selectionModel())
    return itemView->selectionModel()->selectedIndexes().count();
  return false;
}

bool ItemViewEditActionsManagerHelper::canSelectAll() const
{
  return itemView->selectionModel();
}


void ItemViewEditActionsManagerHelper::undo()
{
  IUndoRedoModel* model = qobject_cast<IUndoRedoModel*>(itemView->model());
  if (model)
    model->undoStack()->undo();
}

void ItemViewEditActionsManagerHelper::redo()
{
  IUndoRedoModel* model = qobject_cast<IUndoRedoModel*>(itemView->model());
  if (model)
    model->undoStack()->redo();
}

void ItemViewEditActionsManagerHelper::cut()
{
  copy();
  clear();
}

void ItemViewEditActionsManagerHelper::copy()
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    QModelIndexList indexes = itemView->selectionModel()->selectedIndexes();
    QModelIndexList::Iterator it = indexes.begin();
    QModelIndexList::Iterator end = indexes.end();
    while (it != end) {
      if (itemView->visualRect((*it)).isEmpty())
        it = indexes.erase(it);
      else
        ++it;
    }
    QApplication::clipboard()->setMimeData(model->mimeData(indexes));
  }
}

void ItemViewEditActionsManagerHelper::paste()
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    model->dropMimeData(QApplication::clipboard()->mimeData()
                        , Qt::CopyAction, -1, -1, itemView->currentIndex());
  }
}


void ItemViewEditActionsManagerHelper::clear()
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    QModelIndexList indexes = itemView->selectionModel()->selectedIndexes();
    QModelIndexList::Iterator it = indexes.begin();
    QModelIndexList::Iterator end = indexes.end();
    while (it != end) {
      if (itemView->visualRect((*it)).isEmpty())
        it = indexes.erase(it);
      else
        ++it;
    }
    foreach(QModelIndex index, indexes) if (index.flags() & Qt::ItemIsEditable)
      model->setData(index, QVariant(index.data(Qt::EditRole).type()), Qt::EditRole);
  }
}


void ItemViewEditActionsManagerHelper::selectAll()
{
  itemView->selectAll();
}

bool ItemViewEditActionsManagerHelper::eventFilter(QObject* object, QEvent* event)
{
  if (itemView != object) return false;

  if (event->type() == QEvent::ContextMenu) {
    QContextMenuEvent* menuEvent = static_cast<QContextMenuEvent*>(event);
    //if (itemView->childAt(menuEvent->pos()) == itemView->viewport()) {
      QMenu::exec(itemView->actions(), menuEvent->globalPos(), 0, itemView);
      return true;
    //}
  }
  return QObject::eventFilter(object, event);
}

void ItemViewEditActionsManagerHelper::selectionChanged(const QItemSelection& selected
    , const QItemSelection &/*deselected*/)
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    emit canCopyChanged(canCut());
    emit canCutChanged(canCopy());
    emit canClearChanged(canClear());
  }
}

void ItemViewEditActionsManagerHelper::clipboardDataChanged()
{
  emit canPasteChanged(canPaste());
}
