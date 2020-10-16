#include "abstractdialog.h"

#include "main.h"

#include "printutils.h"

#include <QtDebug>

#include <QSplitter>
#include <QTableView>
#include <QHeaderView>
#include <QCloseEvent>
#include <QPrintPreviewDialog>

AbstractDialog::AbstractDialog(QWidget* parent) :
  QDialog(parent)
{
}

void AbstractDialog::saveState(const QString& objectName)
{
  globalSettings()->beginGroup(objectName);

  globalSettings()->setValue("geometry", saveGeometry().toHex());

  globalSettings()->setValue("position", pos());

  QList<QSplitter*> splitters = findChildren<QSplitter*> ();
  foreach(QSplitter *splitter, splitters)
    {
      globalSettings()->setValue(splitter->objectName(),
        splitter->saveState().toHex());
    }

  QList<QTableView*> tables = findChildren<QTableView*> ();
  foreach(QTableView *table, tables)
    {
      if (table->objectName().isEmpty())
        continue;
      globalSettings()->beginGroup(table->objectName());
      QHeaderView* header = table->horizontalHeader();
      globalSettings()->setValue("horizontal_header", header->saveState().toHex());
      globalSettings()->endGroup();
    }
  globalSettings()->endGroup();
}

void AbstractDialog::restoreState(const QString& objectName)
{
  globalSettings()->beginGroup(objectName);

  if (globalSettings()->contains("geometry"))
    restoreGeometry(QByteArray::fromHex(
      globalSettings()->value("geometry").toByteArray()));
  if (globalSettings()->contains("position"))
    move(globalSettings()->value("position").toPoint());

  QList<QSplitter*> splitters = findChildren<QSplitter*> ();
  foreach(QSplitter *splitter, splitters)
    {
      splitter->restoreState(QByteArray::fromHex(globalSettings()->value(
        splitter->objectName()).toByteArray()));
    }

  QList<QTableView*> tables = findChildren<QTableView*> ();
  foreach(QTableView *table, tables)
    {
      if (table->objectName().isEmpty())
        continue;
      globalSettings()->beginGroup(table->objectName());
      QHeaderView* header = table->horizontalHeader();
      header->restoreState(QByteArray::fromHex(globalSettings()->value(
        "horizontal_header").toByteArray()));
      globalSettings()->endGroup();
    }
  globalSettings()->endGroup();
}

void AbstractDialog::closeEvent(QCloseEvent *event)
{
  QDialog::closeEvent(event);
}

void AbstractDialog::printWidget(QWidget* child, QPrinter::Orientation orientation)
{
  QPrinter printer;
  printer.setOrientation(QPrinter::Landscape);
  QPrintPreviewDialog printPreviewDialog(&printer, this);
  connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter*)), this,
    SLOT(render(QPrinter*)));
  printPreviewDialog.setProperty("widget", qVariantFromValue(child));
  printPreviewDialog.resize(500, 600);
  printPreviewDialog.exec();
}

void AbstractDialog::render(QPrinter* printer)
{
  QWidget *widget = qVariantValue<QWidget*>(sender()->property("widget"));
  ::printWidget(widget, printer);
}
