
#include "comboboxdelegate.h"
#include "private/comboboxdelegate_p.h"

#include <QtCore/QDebug>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QApplication>
#include <QDesktopWidget>
#include <QRegExpValidator>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QKeyEvent>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    :QStyledItemDelegate(parent),
    d_ptr(new ComboBoxDelegatePrivate)
{
  if (objectName().isEmpty())
    setObjectName("comboBoxDelegate");
  Q_D(ComboBoxDelegate);
  d->rexpValidator = new QRegExpValidator(this);
}

ComboBoxDelegate::ComboBoxDelegate(ComboBoxDelegatePrivate &dd, QObject* parent)
    :QStyledItemDelegate(parent)
    , d_ptr(&dd)
{
  Q_D(ComboBoxDelegate);
  d->rexpValidator = new QRegExpValidator(this);
}

ComboBoxDelegate::~ComboBoxDelegate()
{
  delete d_ptr;
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                    const QStyleOptionViewItem &/* option */,
                    const QModelIndex & /*index*/ ) const
{
  Q_D(const ComboBoxDelegate);

  QComboBox *combo = new QComboBox(parent);
  combo->installEventFilter(const_cast<ComboBoxDelegate *>(this));
  combo->setFrame(false);
  combo->setEditable(d->editable && d->keyColumn == d->valueColumn);

  if (d->editable) {
    if (!d->inputMask.isEmpty())
      combo->lineEdit()->setInputMask(d->inputMask);
    combo->lineEdit()->setValidator(d->rexpValidator);
  }

  if (!d->enableCompleter)
    combo->setCompleter(0);

  if (d->model) {
    combo->setModel(d->model);
    combo->setModelColumn(d->valueColumn);
    combo->setRootModelIndex(d->rootIndex);
  }

  QTableView *view = new QTableView(combo);
  combo->setView(view);

  // настраиваем таблицу, чтобы в выпадающем списке она выглядела лучше
  view->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
  view->horizontalHeader()->setVisible(!d->hideHorizontalHeader);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  view->setAutoScroll(false);
  view->setColumnHidden(d->keyColumn, d->hideKeyColumn);
  view->setSelectionMode(QAbstractItemView::SingleSelection);
  view->setSelectionBehavior(QAbstractItemView::SelectRows);

  view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  view->verticalHeader()->setVisible(false);
  view->verticalHeader()->setDefaultSectionSize(20);
  int width = view->horizontalHeader()->length()
              + view->horizontalHeader()->offset() + 5; // про запас

  int height = view->verticalHeader()->length() +
               view->verticalHeader()->offset() + 10; // про запас

  // Вычисляем размер экрана, на котором родительский виджет
  QRect screen = popupGeometry(QApplication::desktop()->screenNumber(parent));

  // Чтобы комбо бокс не приуменьшал таблицу
  view->setMinimumSize(qMin(width, screen.width() - 10), qMin(height, screen.height() - 10));

  connect(combo, SIGNAL(currentIndexChanged(int)),
      this, SLOT(comboBoxCurrentIndexChanged(int)));

  return combo;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
                   const QModelIndex &index) const
{
  Q_D(const ComboBoxDelegate);

  QComboBox *combo = qobject_cast<QComboBox *>(editor);

  QAbstractItemModel* comboModel = combo->model();
  int i;
  int rowCount = comboModel->rowCount();

  for (i = 0; i < rowCount; ++i) {
    if (comboModel->data(comboModel->index(i, d->keyColumn), Qt::EditRole) == index.data(Qt::EditRole))
      break;
  }

  if (d->keyColumn == d->valueColumn) {
    if (i == rowCount) {
      if (!index.data(Qt::EditRole).isNull()) {
        combo->insertItem(0, index.data(Qt::EditRole).toString());
        combo->setCurrentIndex(0);
      }
    } else {
      combo->setCurrentIndex(i);
    }
  } else {
    combo->setCurrentIndex(i);
  }

  if (!combo->property("_pg_popupped").toBool() && d->autoPopup) {
    combo->showPopup();
    combo->setProperty("_pg_popupped", true);
  }
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
  Q_D(const ComboBoxDelegate);
  QComboBox *combo = qobject_cast<QComboBox *>(editor);
  QAbstractItemModel *comboModel = combo->model();

  if (!model || !index.isValid() || !combo)
    return;

  if (d->keyColumn == d->valueColumn && combo) {
    model->setData(index, combo->currentText());
  } else {
    if (combo->currentIndex() == -1) {
      if (!model->setData(index, combo->currentText()))
        setEditorData(editor, index);
    } else {
      if (!model->setData(index
        , comboModel->index(combo->currentIndex(), d->keyColumn, combo->rootModelIndex()).data(Qt::DisplayRole)))
        setEditorData(editor, index);
    }
  }
}

void ComboBoxDelegate::setModel(QAbstractItemModel* model)
{
  Q_D(ComboBoxDelegate);
  if (d->model)
    if (d->model->parent() == this ) {
      delete d->model;
      d->model = 0;
  }
  d->model = model;
}

QAbstractItemModel* ComboBoxDelegate::model() const
{
  Q_D(const ComboBoxDelegate);
  return d->model;
}

void ComboBoxDelegate::setRootModelIndex(const QModelIndex& index)
{
  Q_D(ComboBoxDelegate);
  d->rootIndex = index;
}
QModelIndex ComboBoxDelegate::rootModelIndex() const
{
  Q_D(const ComboBoxDelegate);
  return QModelIndex(d->rootIndex);
}

void ComboBoxDelegate::comboBoxCurrentIndexChanged(int current)
{
  Q_D(ComboBoxDelegate);
  if (sender()->isWidgetType()) {
    QComboBox *combo = qobject_cast<QComboBox *>(sender());
    if (combo) {
      // WORKAROUND
      combo->blockSignals(true);
      combo->setCurrentIndex(current);
      combo->blockSignals(false);
      if (d->endEditHint != NoHint) {
        disconnect(combo, SIGNAL(currentIndexChanged(int)),
                   this, SLOT(comboBoxCurrentIndexChanged(int)));
        emit commitData(combo);
        emit closeEditor(combo, d->endEditHint);
      }
    }
  }
}

int ComboBoxDelegate::keyColumn() const
{
  Q_D(const ComboBoxDelegate);
  return d->keyColumn;
}
void ComboBoxDelegate::setKeyColumn(int column)
{
  Q_D(ComboBoxDelegate);
  d->keyColumn = column;
}
int ComboBoxDelegate::valueColumn() const
{
  Q_D(const ComboBoxDelegate);
  return d->valueColumn;
}
void ComboBoxDelegate::setValueColumn(int column)
{
  Q_D(ComboBoxDelegate);
  d->valueColumn = column;
}

void ComboBoxDelegate::setEditable(bool editable)
{
  Q_D(ComboBoxDelegate);
  d->editable = editable;
}

bool ComboBoxDelegate::isEditable() const
{
  Q_D(const ComboBoxDelegate);
  return d->editable;
}

void ComboBoxDelegate::setAutoPopup(bool popup)
{
  Q_D(ComboBoxDelegate);
  d->autoPopup = popup;
}

bool ComboBoxDelegate::autoPopup() const
{
  Q_D(const ComboBoxDelegate);
  return d->autoPopup;
}

void ComboBoxDelegate::setEndEditHint(int hint)
{
  Q_D(ComboBoxDelegate);
  d->endEditHint = static_cast<EndEditHint>(hint);
}

int ComboBoxDelegate::endEditHint() const
{
  Q_D(const ComboBoxDelegate);
  return d->endEditHint;
}

QString ComboBoxDelegate::regExpValidatorString() const
{
  Q_D(const ComboBoxDelegate);
  return d->rexpValidator->regExp().pattern();
}

void ComboBoxDelegate::setRegExpValidatorString(const QString& regExpression)
{
  Q_D(ComboBoxDelegate);
  d->rexpValidator->setRegExp(QRegExp(regExpression));
}

bool ComboBoxDelegate::isCompleterEnabled() const
{
  Q_D(const ComboBoxDelegate);
  return d->enableCompleter;
}
void ComboBoxDelegate::enableCompleter(bool enable)
{
  Q_D(ComboBoxDelegate);
  d->enableCompleter = enable;
}

QString ComboBoxDelegate::inputMask() const
{
  Q_D(const ComboBoxDelegate);
  return d->inputMask;
}
void ComboBoxDelegate::setInputMask(const QString& mask)
{
  Q_D(ComboBoxDelegate);
  d->inputMask = mask;
}

bool ComboBoxDelegate::isHideHorizontalHeader() const
{
  Q_D(const ComboBoxDelegate);
  return d->hideHorizontalHeader;
}

void ComboBoxDelegate::setHideHorizontalHeader(bool hideHorizontalHeader)
{
  Q_D(ComboBoxDelegate);
  d->hideHorizontalHeader = hideHorizontalHeader;
}

//Windows and KDE allows menus to cover the taskbar, while GNOME and Mac don't
QRect ComboBoxDelegate::popupGeometry(int screen) const
{
#ifdef Q_WS_WIN
  return QApplication::desktop()->screenGeometry(screen);
#elif defined Q_WS_X11
  return QApplication::desktop()->availableGeometry(screen);
#else
  return QApplication::desktop()->availableGeometry(screen);
#endif
}

bool ComboBoxDelegate::isHideKeyColumn() const
{
  Q_D(const ComboBoxDelegate);
  return d->hideKeyColumn;
}

void ComboBoxDelegate::setHideKeyColumn(bool hideKeyColumn)
{
  Q_D(ComboBoxDelegate);
  d->hideKeyColumn = hideKeyColumn;
}

ComboBoxDelegatePrivate::ComboBoxDelegatePrivate()
  :editable(false)
  , autoPopup(false)
  , enableCompleter(false)
  , endEditHint(QAbstractItemDelegate::NoHint)
  , model(0)
  , keyColumn(0)
  , valueColumn(0)
  , hideKeyColumn(true)
  , hideHorizontalHeader(true)
{

}

ComboBoxDelegatePrivate::~ComboBoxDelegatePrivate()
{

}
