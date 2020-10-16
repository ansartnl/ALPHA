#include "EditHeaderDialog.h"

#include "QtCheckHeaderView.h"

#include <QtCore/QSettings>

const QString sysWindowGeometry = "WindowState";
const QString sysHeaderState = "HeaderState";

EditHeaderModel::EditHeaderModel(const QStringList &languages, QObject *parent) :
    QAbstractTableModel(parent)
{
    mLanguages = languages;
}

void EditHeaderModel::setItems(const EditHeaderItems &items)
{
    mItems.clear();
    foreach (const EditHeaderItem &i, items)
        mItems.insert(i.number, i);
}

EditHeaderItems EditHeaderModel::items() const
{
    return mItems.values();
}

int EditHeaderModel::columnCount(const QModelIndex &) const
{
    return 2 + mLanguages.count();
}

int EditHeaderModel::rowCount(const QModelIndex &) const
{
    return mItems.count();
}

QModelIndex EditHeaderModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

QModelIndex EditHeaderModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QVariant EditHeaderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Vertical) || (role != Qt::DisplayRole))
        return QAbstractTableModel::headerData(section, orientation, role);

    if (isNumber(section))
        return tr("Number");
    else if (isText(section))
        return mLanguages[section - 1];
    else if (isVisibility(section))
        return tr("Show");
    return QVariant();
}

QVariant EditHeaderModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (isNumber(index))
            return QString::number(item(index).number);
        else if (isText(index))
            return text(item(index).number, index.column() - 1);
    } else if (role == Qt::EditRole)
    {
        if (isText(index))
            return text(item(index).number, index.column() - 1);
    } else if (role == Qt::CheckStateRole)
    {
        if (isVisibility(index))
            return item(index).visible ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant();
}

Qt::ItemFlags EditHeaderModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags ret = QAbstractTableModel::flags(index);
    if (isText(index))
        ret |= Qt::ItemIsEditable;
    if (isVisibility(index))
        ret |= Qt::ItemIsUserCheckable;
    return ret;
}

bool EditHeaderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole)
    {
        if (isVisibility(index))
        {
            item(index).visible = (value.toInt() == Qt::Checked);
            emit dataChanged(index, index);
            return true;
        }
    } else if (role == Qt::EditRole)
    {
        if (isText(index))
        {
            if (item(index).texts.count () < (index.column() - 1))
                item(index).texts.reserve(index.column() - 1);
            item(index).texts[index.column() - 1] = value.toString();
            emit dataChanged(index, index);
            return true;
        }
    } else
    {
        return QAbstractTableModel::setData(index, value, role);
    }
    return false;
}

void EditHeaderModel::swap(const QModelIndex &index1, const QModelIndex &index2)
{
    beginResetModel();

    EditHeaderItem item1 = item(index1);
    EditHeaderItem item2 = item(index2);

    ::swap(item1, item2);

    mItems[item1.number] = item1;
    mItems[item2.number] = item2;

    endResetModel();
}

EditHeaderItem & EditHeaderModel::item(const QModelIndex &index)
{
    return mItems[mItems.keys()[index.row()]];
}

EditHeaderItem EditHeaderModel::item(const QModelIndex &index) const
{
    return item(index.row());
}

EditHeaderItem EditHeaderModel::item(int row) const
{
    return mItems[mItems.keys()[row]];
}

QString EditHeaderModel::text(int number, int language) const
{
    QStringList texts = mItems[number].texts;
    if (texts.count() < language)
        return QString();
    return texts[language];
}

bool EditHeaderModel::isNumber(const QModelIndex &index) const
{
    return isNumber(index.column());
}

bool EditHeaderModel::isText(const QModelIndex &index) const
{
    return isText(index.column());
}

bool EditHeaderModel::isVisibility(const QModelIndex &index) const
{
    return isVisibility(index.column());
}

bool EditHeaderModel::isNumber(int column) const
{
    return !column;
}

bool EditHeaderModel::isText(int column) const
{
    return (column > 0 && column <= mLanguages.count());
}

bool EditHeaderModel::isVisibility(int column) const
{
    return (column == (mLanguages.count() + 1));
}


EditHeaderDialog::EditHeaderDialog(QWidget *parent) :
    QDialog(parent), model(0)
{
    setupUi(this);

    mName = "EditHeaderDialog";

    connect(upButton, SIGNAL(clicked()), SLOT(onUpButton()));
    connect(downButton, SIGNAL(clicked()), SLOT(onDownButton()));
    connect(this, SIGNAL(accepted()), SLOT(onAccepted()));

    onSelected(QModelIndex());
}

EditHeaderDialog::~EditHeaderDialog()
{
    saveSettings();
}

void EditHeaderDialog::setEditHeaderInterface(QSharedPointer<EditHeaderInterface> &edit_interface)
{
    editInterface = edit_interface;

    model = new EditHeaderModel(editInterface->languages(), this);
    model->setItems(editInterface->items());

    tableView->setModel(model);

    QtCheckHeaderView *header = new QtCheckHeaderView(Qt::Horizontal, tableView);
    header->setResizeMode(QHeaderView::ResizeToContents);
    tableView->setHorizontalHeader(header);
    header->addCheckable(editInterface->languages().count() + 1);

    connect(tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex))
            , SLOT(onSelected(QModelIndex)));       

    loadSettings();
}

void EditHeaderDialog::setDialogName(const QString &name)
{
    mName = name;
    loadSettings();
}

QModelIndex EditHeaderDialog::getModelSelection()
{
    QModelIndex current = tableView->selectionModel()->currentIndex();
    if(current.isValid())
        return model->index(current.row(), current.column());
    return current;
}

void EditHeaderDialog::saveSettings()
{
    QSettings settings;
    settings.beginGroup(mName);
    settings.setValue(sysWindowGeometry, saveGeometry());
    settings.setValue(sysHeaderState, tableView->horizontalHeader()->saveState());
    settings.endGroup();
}

void EditHeaderDialog::loadSettings()
{
    QSettings settings;
    settings.beginGroup(mName);
    tableView->horizontalHeader()->restoreState(settings.value(sysHeaderState).toByteArray());
    restoreGeometry(settings.value(sysWindowGeometry).toByteArray());
    settings.endGroup();
}

void EditHeaderDialog::onSelected(const QModelIndex &newIndex)
{
    upButton->setEnabled(newIndex.isValid() && newIndex.row());
    downButton->setEnabled(newIndex.isValid() && (newIndex.row() < (model->rowCount() - 1)));
}

void EditHeaderDialog::onUpButton()
{
    QModelIndex current = tableView->selectionModel()->currentIndex();
    QModelIndex prev = model->index(current.row() - 1, current.column());
    model->swap(prev, current);
    tableView->selectionModel()->setCurrentIndex(prev, QItemSelectionModel::Select
                                                 | QItemSelectionModel::Rows);
}

void EditHeaderDialog::onDownButton()
{
    QModelIndex current = tableView->selectionModel()->currentIndex();
    QModelIndex next = model->index(current.row() + 1, current.column());
    model->swap(current, next);
    tableView->selectionModel()->setCurrentIndex(next, QItemSelectionModel::Select
                                                 | QItemSelectionModel::Rows);
}

void EditHeaderDialog::onAccepted()
{
    editInterface->setItems(model->items());
}
