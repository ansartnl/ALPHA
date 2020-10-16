#include "abstractbrowser.h"

#include "main.h"

#include <QtDebug>

#include <QSplitter>
#include <QTableView>
#include <QHeaderView>
#include <QCloseEvent>

AbstractBrowser::AbstractBrowser(QWidget* parent) :
  QWidget(parent)
{
}

PrintDataDialog *AbstractBrowser::printDialog()
{
    return 0;
}

const QStringList AbstractBrowser::dataList() const
{
  return QStringList();
}

void AbstractBrowser::saveState(const QString& objectName)
{
  globalSettings()->beginGroup(objectName);

  QList<QSplitter*> splitters = findChildren<QSplitter*>();
  foreach(QSplitter *splitter, splitters) {
    if (splitter->objectName().isEmpty())
      continue;
    globalSettings()->setValue(splitter->objectName(), splitter->saveState().toHex());
  }

  QList<QTableView*> tables = findChildren<QTableView*>();
  foreach(QTableView *table, tables) {
    if (table->objectName().isEmpty())
      continue;
    globalSettings()->beginGroup(table->objectName());
    QHeaderView* header = table->horizontalHeader();
    globalSettings()->setValue("horizontal_header", header->saveState().toHex());
    globalSettings()->endGroup();
  }
  globalSettings()->endGroup();
}

void AbstractBrowser::restoreState(const QString& objectName)
{
  globalSettings()->beginGroup(objectName);

  QList<QSplitter*> splitters = findChildren<QSplitter*>();
  foreach(QSplitter *splitter, splitters) {
    if (splitter->objectName().isEmpty())
      continue;
    splitter->restoreState(QByteArray::fromHex(globalSettings()->value(splitter->objectName()).toByteArray()));
  }

  QList<QTableView*> tables = findChildren<QTableView*>();
  foreach(QTableView *table, tables) {
    if (table->objectName().isEmpty())
      continue;
    globalSettings()->beginGroup(table->objectName());
    QHeaderView* header = table->horizontalHeader();
    header->restoreState(QByteArray::fromHex(globalSettings()->value("horizontal_header").toByteArray()));
    globalSettings()->endGroup();
  }
  globalSettings()->endGroup();
}

void AbstractBrowser::closeEvent(QCloseEvent *event)
{
  QWidget::closeEvent(event);
}
