#include "PrinterHelper.h"

#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtGui/QTextDocument>

namespace PrinterHelper
{
int IHeaderFooter::headerBottom() const
{
    return headerTop() + headerHeight();
}
const QRect IHeaderFooter::headerRect(QPrinter *p) const
{
    return QRect(headerLeft()
                 , headerTop()
                 , p->pageRect().right() - headerLeft() - headerRight()
                 , headerHeight());
}

int IHeaderFooter::footerMTop() const
{
    return footerMBottom() + footerHeight();
}

const QRect IHeaderFooter::footerRect(QPrinter *p) const
{
    return QRect(footerLeft()
                 , p->pageRect().bottom() - footerMTop()
                 , p->pageRect().right() - footerLeft() - footerRight()
                 , footerHeight());
}

///////////////////////////

HeaderFooterPrinter::HeaderFooterPrinter(IHeaderFooter *headerFooter) :
    mHeaderFooter(headerFooter)
{
}

void HeaderFooterPrinter::print(QPrinter *printer, const QString &html)
{
    QTextDocument doc;
    doc.setHtml(html);
    p_print(printer, doc);
}

void HeaderFooterPrinter::printPlainText(QPrinter *printer, const QString &plainText)
{
    QTextDocument doc;
    doc.setPlainText(plainText);
    p_print(printer, doc);
}

void HeaderFooterPrinter::p_print(QPrinter *printer, QTextDocument &doc)
{
    doc.setPageSize(printer->pageRect().size());

    QRect innerRect = printer->pageRect();
    innerRect.setTop(innerRect.top() + mHeaderFooter->headerHeight());
    innerRect.setBottom(innerRect.bottom() - mHeaderFooter->footerHeight());

    QRect contentRect = QRect(QPoint(0,0), doc.size().toSize());
    QRect currentRect = QRect(QPoint(0,0), innerRect.size());

    QPainter painter(printer);

    int pageNumber = 0;
    int allHeight = contentRect.height();
    int pageHeight = currentRect.height() - mHeaderFooter->headerBottom() - mHeaderFooter->footerMTop();
    int totalPageCount = allHeight / pageHeight;
    if (allHeight % pageHeight)
        ++totalPageCount;

    painter.save();
    painter.translate(0, mHeaderFooter->headerTop() + mHeaderFooter->headerHeight());

    while (currentRect.intersects(contentRect))
    {
        pageNumber++;
        if (needPrint(printer, pageNumber))
        {
            doc.drawContents(&painter, currentRect);

            painter.restore();

            painter.drawText(mHeaderFooter->headerRect(printer), mHeaderFooter->headerFlags()
                             , mHeaderFooter->header(pageNumber, totalPageCount));
            painter.drawText(mHeaderFooter->footerRect(printer), mHeaderFooter->footerFlags()
                             , mHeaderFooter->footer(pageNumber, totalPageCount));

            painter.save();
        }

        currentRect.translate(0, currentRect.height());
        painter.translate(0, -currentRect.height() * pageNumber
                          + mHeaderFooter->headerTop() + mHeaderFooter->headerHeight());

        if (needPrint(printer, pageNumber))
        {
            if (needPrint(printer, pageNumber + 1))
            {
                if (currentRect.intersects(contentRect))
                    printer->newPage();
            }
        }
    }

    painter.restore();
    painter.end();
}

bool HeaderFooterPrinter::needPrint(QPrinter *printer, int pageNumber)
{
    if (!printer->fromPage() && !printer->toPage())
        return true;
    return ((printer->fromPage() <= pageNumber) && (pageNumber <= printer->toPage()));
}

} // namespace PrinterHelper
