#include "printactionsmanagerhelper.h"

#include <QtDebug>

#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrintEngine>
#include <QtGui/QPrinter>
#include <QtGui/QPrinterInfo>
#include <QtGui/QPrintPreviewDialog>
#include <QtGui/QPageSetupDialog>

// ----------------------------------------------------------------------
// QWidget
// ----------------------------------------------------------------------
PrintActionsManagerHelper::PrintActionsManagerHelper(QObject* parent)
  : QObject(parent)
  , printer(new QPrinter) {}

PrintActionsManagerHelper::~PrintActionsManagerHelper()
{
  delete printer;
}


void PrintActionsManagerHelper::pageSetup()
{
  emit blockFocusChanged(true);
  QPageSetupDialog pageSetupDialog(printer, qApp->activeWindow());
  pageSetupDialog.exec();
  emit blockFocusChanged(false);
}

void PrintActionsManagerHelper::printPreview()
{
  emit blockFocusChanged(true);
  QPrintPreviewDialog printPreviewDialog(printer, qApp->activeWindow());
  connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter*))
          , this, SLOT(render(QPrinter*)));
  printPreviewDialog.exec();
  emit blockFocusChanged(false);
}

void PrintActionsManagerHelper::print()
{
  emit blockFocusChanged(true);
  QPrintDialog printDialog(printer, qApp->activeWindow());

  if (hasSelection()) printDialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);

  printDialog.setWindowTitle(tr("Print Widget"));

  if (printDialog.exec() == QPrintDialog::Accepted) {
    render(printer);
  }

  emit blockFocusChanged(false);
}

void PrintActionsManagerHelper::render(QPrinter* printer)
{
  if (widget) {
    // Simplify widget
    QPainter painter(printer);

    // Check that there is a valid device to print to.
    if (!painter.isActive())
      return;

    QList<QWidget*> widgetChildren = widget->findChildren<QWidget*>();

    QWidget *parent = 0;
    QRect childRect;
    foreach (QWidget* currentChild, widgetChildren) {

      parent = currentChild;
      childRect = currentChild->frameGeometry();
      while (parent != widget) {
        childRect.translate(parent->pos());
        parent = parent->parentWidget();
      }
      painter.drawRect(currentChild->geometry());
      painter.drawText(currentChild->geometry()
                       , Qt::TextWordWrap
                       | Qt::AlignVCenter
                       | Qt::AlignHCenter
                       , currentChild->property("text").toString());
    }

    //widget->render(printer);
  }
}

// ----------------------------------------------------------------------
// QTextEdit, QPlainTextEdit
// ----------------------------------------------------------------------
#include <QtGui/QTextEdit>
#include <QtGui/QPlainTextEdit>

TextEditPrintActionsManagerHelper::TextEditPrintActionsManagerHelper(QObject* parent)
  : PrintActionsManagerHelper(parent), textEdit(0), plainTextEdit(0) {}

unsigned char  TextEditPrintActionsManagerHelper::approach(QWidget* widget) const
{
  return qobject_cast<QPlainTextEdit*>(widget) || qobject_cast<QTextEdit*>(widget) ? 10 : 0;
}

void TextEditPrintActionsManagerHelper::activateFor(QWidget* widget)
{
  plainTextEdit = qobject_cast<QPlainTextEdit*>(widget);
  textEdit = qobject_cast<QTextEdit*>(widget);
}

void TextEditPrintActionsManagerHelper::deactivate()
{
  plainTextEdit = 0;
  textEdit = 0;
}

bool TextEditPrintActionsManagerHelper::canPageSetup() const
{
  return true;
}

bool TextEditPrintActionsManagerHelper::canPrintPreview() const
{
  return true;
}

bool TextEditPrintActionsManagerHelper::canPrint() const
{
  return true;
}

bool TextEditPrintActionsManagerHelper::hasSelection()
{
  if (textEdit) return textEdit->textCursor().hasSelection();

  if (plainTextEdit) return plainTextEdit->textCursor().hasSelection();

  return false;
}

void TextEditPrintActionsManagerHelper::render(QPrinter* printer)
{
  if (textEdit) {
    textEdit->print(printer);
  } else if (plainTextEdit) {
    plainTextEdit->print(printer);
  }
}


// ----------------------------------------------------------------------
// ItemView
// ----------------------------------------------------------------------
#include <QtGui/QAbstractItemView>
ItemViewPrintActionsManagerHelper::ItemViewPrintActionsManagerHelper(QObject* parent)
  : PrintActionsManagerHelper(parent), itemView(0) {}

unsigned char ItemViewPrintActionsManagerHelper::approach(QWidget* widget) const
{
  return qobject_cast<QAbstractItemView*>(widget) ? 10 : 0;
}

void ItemViewPrintActionsManagerHelper::activateFor(QWidget* widget)
{
  itemView = qobject_cast<QAbstractItemView*>(widget);
}

void ItemViewPrintActionsManagerHelper::deactivate()
{
  itemView = 0;
}

bool ItemViewPrintActionsManagerHelper::hasSelection()
{
  if (itemView->selectionModel())
    return itemView->selectionModel()->selectedIndexes().count();

  return false;
}

//! IMPLEMENT ME
void ItemViewPrintActionsManagerHelper::render(QPrinter* printer)
{
  Q_UNUSED(printer)
  if (itemView) {
    QAbstractItemModel* model = itemView->model();
    if (model) {
      // TODO: implement itemView printing
    }
  }
}



// ----------------------------------------------------------------------
// ItemView
// ----------------------------------------------------------------------
#include <QtGui/QTableView>
TableViewPrintActionsManagerHelper::TableViewPrintActionsManagerHelper(QObject* parent)
  : PrintActionsManagerHelper(parent), tableView(0) {}

unsigned char TableViewPrintActionsManagerHelper::approach(QWidget* widget) const
{
  return qobject_cast<QTableView*>(widget) ? 15 : 0;
}

void TableViewPrintActionsManagerHelper::activateFor(QWidget* widget)
{
  tableView = qobject_cast<QTableView*>(widget);
}

void TableViewPrintActionsManagerHelper::deactivate()
{
  tableView = 0;
}

bool TableViewPrintActionsManagerHelper::hasSelection()
{
  if (tableView->selectionModel())
    return tableView->selectionModel()->selectedIndexes().count();

  return false;
}

#include <QtGui/QHeaderView>
//! IMPLEMENT ME
void TableViewPrintActionsManagerHelper::render(QPrinter* printer)
{
  if (tableView) {
    QAbstractItemModel* model = tableView->model();
    QHeaderView* hHeader = tableView->horizontalHeader();
    QHeaderView* vHeader = tableView->verticalHeader();

    if (model) {
      QPainter painter(printer);

      // Check that there is a valid device to print to.
      if (!painter.isActive())
        return;

      int pageLeft = 0;
      int pageTop = 0;
      int pageHeight = painter.device()->height();
      int pageWidth = painter.device()->width();
      int x = pageLeft;
      int y = pageTop;
      QList<QVector<QLine> > linePages;
      QVector<QLine> verticalLines;
      QList<QList<int> > sectionPages;
      QList<int> sections;
      QList<QList<int> > widthPages;
      QList<int> widths;
      QList<int> pageRights;
      QList<QStringList> pageLabels;
      QStringList labels;
      int logicalIndex;

      // Перечисляем столбцы
      for (int i = 0; i < hHeader->count(); ++i) {
        logicalIndex = hHeader->logicalIndex(i);

        // Если столбец не скрыт
        if (!hHeader->isSectionHidden(logicalIndex)) {
          // Если уже не помещаемся на страницу пишем новую страницу
          if (pageWidth <= x + hHeader->sectionSize(logicalIndex)) {
            verticalLines.append(QLine(x, pageTop, x, pageHeight));
            pageRights.append(x);
            x = pageLeft;
            sectionPages.append(sections);
            sections.clear();
            linePages.append(verticalLines);
            verticalLines.clear();
            widthPages.append(widths);
            widths.clear();
            pageLabels.append(labels);
            labels.clear();
          }

          // Добавляем, так как колонка видимая
          sections.append(logicalIndex);
          widths.append(x + hHeader->sectionSize(logicalIndex) < pageWidth ? hHeader->sectionSize(logicalIndex) : pageWidth - x);
          labels.append(model->headerData(logicalIndex, Qt::Horizontal).toString());
          verticalLines.append(QLine(x, pageTop, x, pageHeight));
          // Смещаемся правее на ширину колонки
          x = x + hHeader->sectionSize(logicalIndex) < pageWidth ? x + hHeader->sectionSize(logicalIndex) : pageWidth - x;
        }
      }

      // Add last page columns
      if (!sections.isEmpty()) {
        verticalLines.append(QLine(x, pageTop, x, pageHeight));
        pageRights.append(x);
        x = pageLeft;
        sectionPages.append(sections);
        sections.clear();
        linePages.append(verticalLines);
        verticalLines.clear();
        widthPages.append(widths);
        widths.clear();
        pageLabels.append(labels);
        labels.clear();
      }

      int columnPageCount = sectionPages.count();
      int columnCount = sections.count();
      int rowCount = model->rowCount(tableView->rootIndex());
      int rowHeight;
      int columnWidth;
      QModelIndex currentIndex;
      int row = 0;
      int lastPageRow = 0;
      int columnPageIndex = 0;
      int rightColumn;

      while (true) {
        // Получаем колонки текущей страницы
        sections = sectionPages.at(columnPageIndex);
        widths = widthPages.at(columnPageIndex);
        verticalLines = linePages.at(columnPageIndex);
        // Количество колонок текущей страницы
        columnCount = sections.count();
        rightColumn = pageRights.at(columnPageIndex);
        labels = pageLabels.at(columnPageIndex);
        // Перемещаемся в левый верхний угол страницы
        x = pageLeft;
        y = pageTop;
        rowHeight = hHeader->height();

        // Рисуем колонки
        for (int i = 0; i < columnCount; ++i) {
          // Текущее значение
          columnWidth = widths.at(i);
          painter.drawText(x, y, columnWidth, rowHeight
                           , currentIndex.data(Qt::TextAlignmentRole).toInt()
                           | Qt::TextWordWrap
                           | Qt::AlignVCenter
                           | Qt::AlignHCenter
                           , labels.at(i)
                          );
          painter.drawLine(pageLeft, y, rightColumn, y);
          // Перемащаемся правее
          x += widths.at(i);
        }

        y += rowHeight;
        x = pageLeft;

        // Пока можем рисовать на текущей странице
        while (true) {
          // Высота текущей строки
          rowHeight = vHeader->sectionSize(row);

          if (y + rowHeight > pageHeight) {
            //--row;
            break;
          }

          // Рисуем колонки
          for (int i = 0; i < columnCount; ++i) {
            // Текущее значение
            currentIndex = model->index(row, sections.at(i), tableView->rootIndex());
            columnWidth = widths.at(i);

            if (!currentIndex.data().isNull())
              painter.drawText(x, y, columnWidth, rowHeight
                               , currentIndex.data(Qt::TextAlignmentRole).toInt()
                               | Qt::TextWordWrap
                               | Qt::AlignVCenter
                               | Qt::AlignHCenter
                               , currentIndex.data().toString()
                              );

            painter.drawLine(pageLeft, y, rightColumn, y);
            // Перемащаемся правее
            x += widths.at(i);
          }

          x = pageLeft;
          // Перемещаемся ниже
          y += rowHeight;
          // Берем следующую строку
          ++row;

          // Если строки кончились
          if (row >= rowCount)
            break;
        }

        // Рисуем завершающую горизонтальную линию
        painter.drawLine(pageLeft, y, rightColumn, y);
        // Рисуем вертикальные линии
        painter.drawLines(verticalLines);
        // Вытираем лишние концы
        // TODO: hard-coded background color for page
        painter.fillRect(pageLeft, y + 1, pageWidth, pageHeight - (y + 1), QBrush(Qt::white));

        // Если предыдущие колонки не поместились
        if (columnPageIndex < columnPageCount - 1) {
          ++columnPageIndex;
          row = lastPageRow;
        // Предыдущие колонки поместились
        } else {
          columnPageIndex = 0;
          lastPageRow = row;

          // Строки закончились
          if (row >= rowCount)
            break;
        }

        // Создаем новую страницу
        printer->newPage();
      }
    }
  }
}

#include <QtGui/QGraphicsView>

GraphicsViewPrintActionsManagerHelper::GraphicsViewPrintActionsManagerHelper(QObject* parent)
  : PrintActionsManagerHelper(parent) {}

unsigned char GraphicsViewPrintActionsManagerHelper::approach(QWidget* widget) const
{
  return qobject_cast<QGraphicsView*>(widget) ? 10 : 0;
}

void GraphicsViewPrintActionsManagerHelper::activateFor(QWidget* widget)
{
  graphicsView = qobject_cast<QGraphicsView*>(widget);
}

void GraphicsViewPrintActionsManagerHelper::deactivate()
{
  graphicsView = 0;
}

void GraphicsViewPrintActionsManagerHelper::render(QPrinter* printer)
{
  if (graphicsView) {
    QRectF pageRect = printer->pageRect();
    QRectF sceneRect = graphicsView->sceneRect();
    QRect sourceRect;
    QPainter painter;
    painter.begin(printer);

    for (int i = 0; i < sceneRect.height(); i += pageRect.height()) {
      for (int g = 0; g < sceneRect.width(); g += pageRect.width()) {
        sourceRect = QRect(i, g, pageRect.width(), pageRect.height());
        graphicsView->scene()->render(&painter, pageRect, sourceRect, Qt::KeepAspectRatio);

        if (g + pageRect.width() < sceneRect.width())
          printer->newPage();
      }

      if (i + pageRect.height() < sceneRect.height())
        printer->newPage();
    }

    painter.end();
  }
}
