#include "navigateactionsmanagerhelper.h"

#include <QtCore/QDebug>

NavigateActionsManagerHelper::NavigateActionsManagerHelper(QObject* parent)
    : QObject(parent)
{}

NavigateActionsManagerHelper::~NavigateActionsManagerHelper() {}

// -------------------------------------------------------------------------
// QAbstractItemView
// -------------------------------------------------------------------------
#include <QtGui/QAbstractItemView>
ItemViewNavigateActionsManagerHelper::ItemViewNavigateActionsManagerHelper(QObject* parent)
    : NavigateActionsManagerHelper(parent) {}

ItemViewNavigateActionsManagerHelper::~ItemViewNavigateActionsManagerHelper() {}

unsigned char ItemViewNavigateActionsManagerHelper::approach(QWidget* widget) const
{
    QAbstractItemView *view = qobject_cast<QAbstractItemView*>(widget);
    if (view)
        return view->model() ? 10 : 0;

    return false;
}

void ItemViewNavigateActionsManagerHelper::activateFor(QWidget* widget)
{
    itemView = qobject_cast<QAbstractItemView*>(widget);
    if (itemView->selectionModel())
        connect(itemView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex))
                , this, SLOT(currentChanged(QModelIndex, QModelIndex)));
    currentIndex = itemView->currentIndex();
}

void ItemViewNavigateActionsManagerHelper::deactivate()
{
    if (itemView->selectionModel())
        disconnect(itemView->selectionModel(), 0
                   , this, 0);
    currentIndex = QModelIndex();
    itemView = 0;
}


bool ItemViewNavigateActionsManagerHelper::canFirst() const
{
    return currentIndex.isValid();
}
bool ItemViewNavigateActionsManagerHelper::canPrevious() const
{
    return currentIndex.isValid();
}
bool ItemViewNavigateActionsManagerHelper::canTop() const
{
    return currentIndex.isValid();
}
bool ItemViewNavigateActionsManagerHelper::canUp() const
{
    return currentIndex.isValid();
}
bool ItemViewNavigateActionsManagerHelper::canDown() const
{
    return currentIndex.isValid();
}
bool ItemViewNavigateActionsManagerHelper::canBottom() const
{
    return currentIndex.isValid();
}
bool ItemViewNavigateActionsManagerHelper::canNext() const
{
    return currentIndex.isValid();
}
bool ItemViewNavigateActionsManagerHelper::canLast() const
{
    return currentIndex.isValid();
}

void ItemViewNavigateActionsManagerHelper::first()
{
    if (itemView) {
        if (currentIndex.isValid()) {
            int row = currentIndex.row();
            int columnCount = itemView->model()->columnCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;

            for (int i = 0; i < columnCount; ++i) {
                newIndex = itemView->model()->index(row, i, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid())
                    break;
            }
            if (newIndex.isValid() && !indexRect.isNull()) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::previous()
{
    if (itemView) {
        if (currentIndex.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();
            int columnCount = itemView->model()->columnCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;
            bool success = false;
            for (int i = column - 1; i >= 0; --i) {
                newIndex = itemView->model()->index(row, i, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid()) {
                    success = true;
                    break;
                }
            }
            if (success) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            } else {
                for (int i = columnCount - 1; i >= column; --i) {
                    newIndex = itemView->model()->index(row, i, currentIndex.parent());
                    indexRect = itemView->visualRect(newIndex);
                    if (!indexRect.isNull() && newIndex.isValid()) {
                        success = true;
                        break;
                    }
                }
                if (success) {
                    itemView->setCurrentIndex(newIndex);
                    itemView->scrollTo(newIndex);
                }
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::top()
{
    if (itemView) {
        if (currentIndex.isValid()) {
            int column = currentIndex.column();
            int rowCount = itemView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;

            for (int i = 0; i < rowCount; ++i) {
                newIndex = itemView->model()->index(i, column, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid())
                    break;
            }
            if (newIndex.isValid() && !indexRect.isNull()) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::up()
{
    if (itemView) {
        QModelIndex current = itemView->currentIndex();
        if (current.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();
            int rowCount = itemView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;
            bool success = false;
            for (int i = row - 1; i >= 0; --i) {
                newIndex = itemView->model()->index(i, column, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid()) {
                    success = true;
                    break;
                }
            }
            if (success) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            } else {
                for (int i = rowCount - 1; i >= row; --i) {
                    newIndex = itemView->model()->index(i, column, currentIndex.parent());
                    indexRect = itemView->visualRect(newIndex);
                    if (!indexRect.isNull() && newIndex.isValid()) {
                        success = true;
                        break;
                    }
                }
                if (success) {
                    itemView->setCurrentIndex(newIndex);
                    itemView->scrollTo(newIndex);
                }
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::down()
{
    if (itemView) {
        if (currentIndex.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();
            int rowCount = itemView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;
            bool success = false;
            for (int i = row + 1; i < rowCount; ++i) {
                newIndex = itemView->model()->index(i, column, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid()) {
                    success = true;
                    break;
                }
            }
            if (success) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            } else {
                for (int i = 0; i <= row; ++i) {
                    newIndex = itemView->model()->index(i, column, currentIndex.parent());
                    indexRect = itemView->visualRect(newIndex);
                    if (!indexRect.isNull() && newIndex.isValid()) {
                        success = true;
                        break;
                    }
                }
                if (success) {
                    itemView->setCurrentIndex(newIndex);
                    itemView->scrollTo(newIndex);
                }
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::bottom()
{
    if (itemView) {
        if (currentIndex.isValid()) {
            int column = currentIndex.column();
            int rowCount = itemView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;

            for (int i = rowCount - 1; i >= 0; --i) {
                newIndex = itemView->model()->index(i, column, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid())
                    break;
            }
            if (newIndex.isValid() && !indexRect.isNull()) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::next()
{
    if (itemView) {
        if (currentIndex.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();
            int columnCount = itemView->model()->columnCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;
            bool success = false;
            for (int i = column + 1; i < columnCount; ++i) {
                newIndex = itemView->model()->index(row, i, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid()) {
                    success = true;
                    break;
                }
            }
            if (success) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            } else {
                for (int i = 0; i <= column; ++i) {
                    newIndex = itemView->model()->index(row, i, currentIndex.parent());
                    indexRect = itemView->visualRect(newIndex);
                    if (!indexRect.isNull() && newIndex.isValid()) {
                        success = true;
                        break;
                    }
                }
                if (success) {
                    itemView->setCurrentIndex(newIndex);
                    itemView->scrollTo(newIndex);
                }
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::last()
{
    if (itemView) {
        QModelIndex current = itemView->currentIndex();
        if (current.isValid()) {
            int row = current.row();
            int columnCount = itemView->model()->columnCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;

            for (int i = columnCount - 1; i >= 0; --i) {
                newIndex = itemView->model()->index(row, i, currentIndex.parent());
                indexRect = itemView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid())
                    break;
            }
            if (newIndex.isValid() && !indexRect.isNull()) {
                itemView->setCurrentIndex(newIndex);
                itemView->scrollTo(newIndex);
            }
        }
    }
}

void ItemViewNavigateActionsManagerHelper::currentChanged(const QModelIndex& now, const QModelIndex& old)
{
    currentIndex = now;
    bool isValid = currentIndex.isValid();
    emit canLastChanged(isValid);
    emit canPreviousChanged(isValid);
    emit canFirstChanged(isValid);
    emit canNextChanged(isValid);
    emit canTopChanged(isValid);
    emit canUpChanged(isValid);
    emit canBottomChanged(isValid);
    emit canDownChanged(isValid);
}

// -----------------------------------------------------------------------------------
// QTableView
// -----------------------------------------------------------------------------------
#include <QtGui/QTableView>
#include <QtGui/QHeaderView>
TableViewNavigateActionsManagerHelper::TableViewNavigateActionsManagerHelper(QObject* parent)
    : NavigateActionsManagerHelper(parent) {}

TableViewNavigateActionsManagerHelper::~TableViewNavigateActionsManagerHelper() {}

unsigned char TableViewNavigateActionsManagerHelper::approach(QWidget* widget) const
{
    QTableView *view = qobject_cast<QTableView*>(widget);
    if (view)
        return view->model() ? 10 : 0;

    return false;
}
void TableViewNavigateActionsManagerHelper::activateFor(QWidget* widget)
{
    tableView = qobject_cast<QTableView*>(widget);
    if (tableView) {
        if (tableView->selectionModel())
            connect(tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex))
                    , this, SLOT(currentChanged(QModelIndex, QModelIndex)));
        currentIndex = tableView->currentIndex();
    }
}

void TableViewNavigateActionsManagerHelper::deactivate()
{
    if (tableView) {
        if (tableView->selectionModel())
            disconnect(tableView->selectionModel(), 0
                       , this, 0);
        currentIndex = QModelIndex();
        tableView = 0;
    }
}


bool TableViewNavigateActionsManagerHelper::canFirst() const
{
    return currentIndex.isValid();
}

bool TableViewNavigateActionsManagerHelper::canPrevious() const
{
    return currentIndex.isValid();
}

bool TableViewNavigateActionsManagerHelper::canTop() const
{
    return currentIndex.isValid();
}

bool TableViewNavigateActionsManagerHelper::canUp() const
{
    return currentIndex.isValid();
}

bool TableViewNavigateActionsManagerHelper::canDown() const
{
    return currentIndex.isValid();
}

bool TableViewNavigateActionsManagerHelper::canBottom() const
{
    return currentIndex.isValid();
}

bool TableViewNavigateActionsManagerHelper::canNext() const
{
    return currentIndex.isValid();
}

bool TableViewNavigateActionsManagerHelper::canLast() const
{
    return currentIndex.isValid();
}

void TableViewNavigateActionsManagerHelper::first()
{
    if (tableView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = tableView->horizontalHeader();
            int row = currentIndex.row();
            int columnCount = tableView->model()->columnCount(currentIndex.parent());
            int column;
            QModelIndex newIndex;

            for (int i = 0; i < columnCount; ++i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::previous()
{
    if (tableView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = tableView->horizontalHeader();
            int row = currentIndex.row();
            int column = currentIndex.column();
            int columnCount = tableView->model()->columnCount(currentIndex.parent());
            int visualColumn = header->visualIndex(column);

            QModelIndex newIndex ;
            for (int i = visualColumn - 1; i >= 0; --i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            } else {
                for (int i = columnCount - 1; i > visualColumn; --i) {
                    column = header->logicalIndex(i);
                    if (!header->isSectionHidden(column)) {
                        newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    tableView->setCurrentIndex(newIndex);
                    tableView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::top()
{
    if (tableView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = tableView->verticalHeader();
            int row = currentIndex.row();
            int rowCount = tableView->model()->rowCount(currentIndex.parent());
            int column = currentIndex.column();
            QModelIndex newIndex;

            for (int i = 0; i < rowCount; ++i) {
                row = header->logicalIndex(i);
                if (!header->isSectionHidden(row)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::up()
{
    if (tableView) {
        QModelIndex current = tableView->currentIndex();
        if (current.isValid()) {
            QHeaderView* header = tableView->verticalHeader();
            int row = currentIndex.row();
            int column = currentIndex.column();
            int rowCount = tableView->model()->rowCount(currentIndex.parent());
            int visualColumn = header->visualIndex(row);

            QModelIndex newIndex ;
            for (int i = visualColumn - 1; i >= 0; --i) {
                row = header->logicalIndex(i);
                if (!header->isSectionHidden(row)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            } else {
                for (int i = rowCount - 1; i > visualColumn; --i) {
                    row = header->logicalIndex(i);
                    if (!header->isSectionHidden(row)) {
                        newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    tableView->setCurrentIndex(newIndex);
                    tableView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::down()
{
    if (tableView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = tableView->verticalHeader();
            int row = currentIndex.row();
            int column = currentIndex.column();
            int rowCount = tableView->model()->rowCount(currentIndex.parent());
            int visualColumn = header->visualIndex(row);

            QModelIndex newIndex ;
            for (int i = visualColumn + 1; i < rowCount; ++i) {
                row = header->logicalIndex(i);
                if (!header->isSectionHidden(row)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            } else {
                for (int i = 0; i < visualColumn; ++i) {
                    row = header->logicalIndex(i);
                    if (!header->isSectionHidden(row)) {
                        newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    tableView->setCurrentIndex(newIndex);
                    tableView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::bottom()
{
    if (tableView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = tableView->verticalHeader();
            int row = currentIndex.row();
            int rowCount = tableView->model()->rowCount(currentIndex.parent());
            int column = currentIndex.column();
            QModelIndex newIndex;

            for (int i = rowCount - 1; i >= 0; --i) {
                row = header->logicalIndex(i);
                if (!header->isSectionHidden(row)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::next()
{
    if (tableView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = tableView->horizontalHeader();
            int row = currentIndex.row();
            int column = currentIndex.column();
            int columnCount = tableView->model()->columnCount(currentIndex.parent());
            int visualColumn = header->visualIndex(column);

            QModelIndex newIndex ;
            for (int i = visualColumn + 1; i < columnCount; ++i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            } else {
                for (int i = 0; i < visualColumn; ++i) {
                    column = header->logicalIndex(i);
                    if (!header->isSectionHidden(column)) {
                        newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    tableView->setCurrentIndex(newIndex);
                    tableView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::last()
{
    if (tableView) {
        QModelIndex current = tableView->currentIndex();
        if (current.isValid()) {
            QHeaderView* header = tableView->horizontalHeader();
            int row = currentIndex.row();
            int columnCount = tableView->model()->columnCount(currentIndex.parent());
            int column;
            QModelIndex newIndex;

            for (int i = columnCount - 1; i >= 0; --i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = tableView->model()->index(row, column, tableView->rootIndex());
                    break;
                }
            }
            if (newIndex.isValid()) {
                tableView->setCurrentIndex(newIndex);
                tableView->scrollTo(newIndex);
            }
        }
    }
}

void TableViewNavigateActionsManagerHelper::currentChanged(const QModelIndex& now, const QModelIndex& old)
{
    currentIndex = now;
    bool isValid = currentIndex.isValid();
    emit canLastChanged(isValid);
    emit canPreviousChanged(isValid);
    emit canFirstChanged(isValid);
    emit canNextChanged(isValid);
    emit canTopChanged(isValid);
    emit canUpChanged(isValid);
    emit canBottomChanged(isValid);
    emit canDownChanged(isValid);
}

// -----------------------------------------------------------------------------------
// QTreeView
// -----------------------------------------------------------------------------------
#include <QtGui/QTreeView>
TreeViewNavigateActionsManagerHelper::TreeViewNavigateActionsManagerHelper(QObject* parent)
    : NavigateActionsManagerHelper(parent) {}

TreeViewNavigateActionsManagerHelper::~TreeViewNavigateActionsManagerHelper() {}

unsigned char TreeViewNavigateActionsManagerHelper::approach(QWidget* widget) const
{
    QTreeView *view = qobject_cast<QTreeView*>(widget);
    if (view)
        return view->model() ? 10 : 0;

    return false;
}
void TreeViewNavigateActionsManagerHelper::activateFor(QWidget* widget)
{
    treeView = qobject_cast<QTreeView*>(widget);
    if (treeView->selectionModel())
        connect(treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex))
                , this, SLOT(currentChanged(QModelIndex, QModelIndex)));
    currentIndex = treeView->currentIndex();
}

void TreeViewNavigateActionsManagerHelper::deactivate()
{
    if (treeView->selectionModel())
        disconnect(treeView->selectionModel(), 0
                   , this, 0);
    currentIndex = QModelIndex();
    treeView = 0;
}


bool TreeViewNavigateActionsManagerHelper::canFirst() const
{
    return currentIndex.isValid();
}
bool TreeViewNavigateActionsManagerHelper::canPrevious() const
{
    return currentIndex.isValid();
}
bool TreeViewNavigateActionsManagerHelper::canTop() const
{
    return currentIndex.isValid();
}
bool TreeViewNavigateActionsManagerHelper::canUp() const
{
    return currentIndex.isValid();
}
bool TreeViewNavigateActionsManagerHelper::canDown() const
{
    return currentIndex.isValid();
}
bool TreeViewNavigateActionsManagerHelper::canBottom() const
{
    return currentIndex.isValid();
}
bool TreeViewNavigateActionsManagerHelper::canNext() const
{
    return currentIndex.isValid();
}
bool TreeViewNavigateActionsManagerHelper::canLast() const
{
    return currentIndex.isValid();
}

void TreeViewNavigateActionsManagerHelper::first()
{
    if (treeView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = treeView->header();
            int row = currentIndex.row();
            int columnCount = treeView->model()->columnCount(currentIndex.parent());
            int column;
            QModelIndex newIndex;

            for (int i = 0; i < columnCount; ++i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = treeView->model()->index(row, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::previous()
{
    if (treeView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = treeView->header();
            int row = currentIndex.row();
            int column = currentIndex.column();
            int columnCount = treeView->model()->columnCount(currentIndex.parent());
            int visualColumn = header->visualIndex(column);

            QModelIndex newIndex ;
            for (int i = visualColumn - 1; i >= 0; --i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = treeView->model()->index(row, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            } else {
                for (int i = columnCount - 1; i > visualColumn; --i) {
                    column = header->logicalIndex(i);
                    if (!header->isSectionHidden(column)) {
                        newIndex = treeView->model()->index(row, column, currentIndex.parent());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    treeView->setCurrentIndex(newIndex);
                    treeView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::top()
{
    if (treeView) {
        if (currentIndex.isValid()) {
            int rowCount = treeView->model()->rowCount(currentIndex.parent());
            int column = currentIndex.column();
            QModelIndex newIndex;

            for (int i = 0; i < rowCount; ++i) {
                if (!treeView->isRowHidden(i, currentIndex.parent())) {
                    newIndex = treeView->model()->index(i, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::up()
{
    if (treeView) {
        QModelIndex current = treeView->currentIndex();
        if (current.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();
            int rowCount = treeView->model()->rowCount(currentIndex.parent());

            QModelIndex newIndex ;
            for (int i = row - 1; i >= 0; --i) {
                if (!treeView->isRowHidden(i, currentIndex.parent())) {
                    newIndex = treeView->model()->index(i, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            } else {
                for (int i = rowCount - 1; i > row; --i) {
                    if (!treeView->isRowHidden(i, currentIndex.parent())) {
                        newIndex = treeView->model()->index(i, column, currentIndex.parent());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    treeView->setCurrentIndex(newIndex);
                    treeView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::down()
{
    if (treeView) {
        if (currentIndex.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();
            int rowCount = treeView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;

            for (int i = row + 1; i < rowCount; ++i) {
                if (!treeView->isRowHidden(i, currentIndex.parent())) {
                    newIndex = treeView->model()->index(i, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            } else {
                for (int i = 0; i < row; ++i) {
                    if (!treeView->isRowHidden(i, currentIndex.parent())) {
                        newIndex = treeView->model()->index(i, column, currentIndex.parent());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    treeView->setCurrentIndex(newIndex);
                    treeView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::bottom()
{
    if (treeView) {
        if (currentIndex.isValid()) {
            int rowCount = treeView->model()->rowCount(currentIndex.parent());
            int column = currentIndex.column();
            QModelIndex newIndex;

            for (int i = rowCount - 1; i >= 0; --i) {
                if (!treeView->isRowHidden(i, currentIndex.parent())) {
                    newIndex = treeView->model()->index(i, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::next()
{
    if (treeView) {
        if (currentIndex.isValid()) {
            QHeaderView* header = treeView->header();
            int row = currentIndex.row();
            int column = currentIndex.column();
            int columnCount = treeView->model()->columnCount(currentIndex.parent());
            int visualColumn = header->visualIndex(column);

            QModelIndex newIndex ;
            for (int i = visualColumn + 1; i < columnCount; ++i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = treeView->model()->index(row, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            } else {
                for (int i = 0; i < visualColumn; ++i) {
                    column = header->logicalIndex(i);
                    if (!header->isSectionHidden(column)) {
                        newIndex = treeView->model()->index(row, column, currentIndex.parent());
                        break;
                    }
                }
                if (newIndex.isValid()) {
                    treeView->setCurrentIndex(newIndex);
                    treeView->scrollTo(newIndex);
                }
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::last()
{
    if (treeView) {
        QModelIndex current = treeView->currentIndex();
        if (current.isValid()) {
            QHeaderView* header = treeView->header();
            int row = currentIndex.row();
            int columnCount = treeView->model()->columnCount(currentIndex.parent());
            int column;
            QModelIndex newIndex;

            for (int i = columnCount - 1; i >= 0; --i) {
                column = header->logicalIndex(i);
                if (!header->isSectionHidden(column)) {
                    newIndex = treeView->model()->index(row, column, currentIndex.parent());
                    break;
                }
            }
            if (newIndex.isValid()) {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            }
        }
    }
}

void TreeViewNavigateActionsManagerHelper::currentChanged(const QModelIndex& now, const QModelIndex& old)
{
    currentIndex = now;
    bool isValid = currentIndex.isValid();
    emit canLastChanged(isValid);
    emit canPreviousChanged(isValid);
    emit canFirstChanged(isValid);
    emit canNextChanged(isValid);
    emit canTopChanged(isValid);
    emit canUpChanged(isValid);
    emit canBottomChanged(isValid);
    emit canDownChanged(isValid);
}


// -----------------------------------------------------------------------------------
// QListView
// -----------------------------------------------------------------------------------

#include <QtGui/QListView>
ListViewNavigateActionsManagerHelper::ListViewNavigateActionsManagerHelper(QObject* parent)
    : NavigateActionsManagerHelper(parent) {}

ListViewNavigateActionsManagerHelper::~ListViewNavigateActionsManagerHelper() {}

unsigned char ListViewNavigateActionsManagerHelper::approach(QWidget* widget) const
{
    QListView *view = qobject_cast<QListView*>(widget);
    if (view)
        return view->model() ? 10 : 0;

    return false;
}
void ListViewNavigateActionsManagerHelper::activateFor(QWidget* widget)
{
    listView = qobject_cast<QListView*>(widget);
    if (listView->selectionModel())
        connect(listView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex))
                , this, SLOT(currentChanged(QModelIndex, QModelIndex)));
    currentIndex = listView->currentIndex();
}

void ListViewNavigateActionsManagerHelper::deactivate()
{
    if (listView->selectionModel())
        disconnect(listView->selectionModel(), 0
                   , this, 0);
    currentIndex = QModelIndex();
    listView = 0;
}


bool ListViewNavigateActionsManagerHelper::canTop() const
{
    if (currentIndex.isValid())
        return currentIndex.row() != 0;
    return false;
}
bool ListViewNavigateActionsManagerHelper::canUp() const
{
    return currentIndex.isValid();
}
bool ListViewNavigateActionsManagerHelper::canDown() const
{
    return currentIndex.isValid();
}
bool ListViewNavigateActionsManagerHelper::canBottom() const
{
    if (currentIndex.isValid())
        return currentIndex.row() != currentIndex.model()->rowCount(currentIndex.parent()) - 1;
    return false;
}

void ListViewNavigateActionsManagerHelper::top()
{
    if (listView) {
        if (currentIndex.isValid()) {
            int column = currentIndex.column();
            int rowCount = listView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            QRect indexRect;

            for (int i = 0; i < rowCount; ++i) {
                newIndex = listView->model()->index(i, column, currentIndex.parent());
                indexRect = listView->visualRect(newIndex);
                if (!indexRect.isNull() && newIndex.isValid())
                    break;
            }
            if (newIndex.isValid() && !indexRect.isNull()) {
                listView->setCurrentIndex(newIndex);
                listView->scrollTo(newIndex);
            }
        }
    }
}

void ListViewNavigateActionsManagerHelper::up()
{
    if (listView) {
        QModelIndex current = listView->currentIndex();
        if (current.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();
            int rowCount = listView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            bool isRowHidden;
            bool success = false;
            for (int i = row - 1; i >= 0; --i) {
                newIndex = listView->model()->index(i, column, currentIndex.parent());
                isRowHidden = listView->isRowHidden(i);
                if (!isRowHidden && newIndex.isValid()) {
                    success = true;
                    break;
                }
            }
            if (success) {
                listView->setCurrentIndex(newIndex);
                listView->scrollTo(newIndex);
            } else {
                for (int i = rowCount - 1; i >= row; --i) {
                    newIndex = listView->model()->index(i, column, currentIndex.parent());
                    isRowHidden = listView->isRowHidden(i);
                    if (!isRowHidden && newIndex.isValid()) {
                        success = true;
                        break;
                    }
                }
                if (success) {
                    listView->setCurrentIndex(newIndex);
                    listView->scrollTo(newIndex);
                }
            }
        }
    }
}

void ListViewNavigateActionsManagerHelper::down()
{
    if (listView) {
        if (currentIndex.isValid()) {
            int row = currentIndex.row();
            int column = currentIndex.column();

            int rowCount = listView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            bool isRowHidden;
            bool success = false;
            for (int i = row + 1; i < rowCount; ++i) {
                newIndex = listView->model()->index(i, column, currentIndex.parent());
                isRowHidden = listView->isRowHidden(i);
                if (!isRowHidden && newIndex.isValid()) {
                    success = true;
                    break;
                }
            }
            if (success) {
                listView->setCurrentIndex(newIndex);
                listView->scrollTo(newIndex);
            } else {
                for (int i = 0; i <= row; ++i) {
                    newIndex = listView->model()->index(i, column, currentIndex.parent());
                    isRowHidden = listView->isRowHidden(i);
                    if (!isRowHidden && newIndex.isValid()) {
                        success = true;
                        break;
                    }
                }
                if (success) {
                    listView->setCurrentIndex(newIndex);
                    listView->scrollTo(newIndex);
                }
            }
        }
    }
}

void ListViewNavigateActionsManagerHelper::bottom()
{
    if (listView) {
        if (currentIndex.isValid()) {
            int column = currentIndex.column();
            int rowCount = listView->model()->rowCount(currentIndex.parent());
            QModelIndex newIndex;
            bool isRowHidden;

            for (int i = rowCount - 1; i >= 0; --i) {
                newIndex = listView->model()->index(i, column, currentIndex.parent());
                isRowHidden = listView->isRowHidden(i);
                if (!isRowHidden && newIndex.isValid())
                    break;
            }
            if (newIndex.isValid() && !isRowHidden) {
                listView->setCurrentIndex(newIndex);
                listView->scrollTo(newIndex);
            }
        }
    }
}

void ListViewNavigateActionsManagerHelper::currentChanged(const QModelIndex& now, const QModelIndex& old)
{
    currentIndex = now;
    bool isValid = currentIndex.isValid();
    bool isFirstRow = currentIndex.row() == 0;
    bool isLastRow = false;
    if (isValid) {
        isLastRow = currentIndex.row() == currentIndex.model()->rowCount(currentIndex.parent()) - 1;
    }
    emit canTopChanged(!isFirstRow);
    emit canUpChanged(isValid);
    emit canBottomChanged(!isLastRow);
    emit canDownChanged(isValid);
}
