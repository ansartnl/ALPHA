#ifndef PRINTERHELPER_H
#define PRINTERHELPER_H

#include <QtCore/QRect>
#include <QtCore/QString>

class QPrinter;
class QTextDocument;

namespace PrinterHelper
{

/*!
 * \class IHeaderFooter
 * \brief Интерфейс, возвращающий параметры колонтитулов
 *
 * \c footerMBottom и \c footerMTop - это дополнения до нижней и верхней границ соответственно.
 */

class IHeaderFooter
{
public:
    // header parameters
    virtual QString header(int pageNumber, int totalPages) const = 0;
    virtual int headerHeight() const
    {
        return 20;
    }
    virtual int headerTop() const
    {
        return 10;
    }
    virtual int headerLeft() const
    {
        return 10;
    }
    virtual int headerRight() const
    {
        return headerLeft();
    }
    virtual int headerFlags() const
    {
        return Qt::AlignLeft;
    }

    int headerBottom() const;
    const QRect headerRect(QPrinter *p) const;
    // footer parameters
    virtual QString footer(int pageNumber, int totalPages) const = 0;
    virtual int footerHeight() const
    {
        return 30;
    }
    virtual int footerMBottom() const
    {
        return 10;
    }
    virtual int footerLeft() const
    {
        return 10;
    }
    virtual int footerRight() const
    {
        return footerLeft();
    }
    virtual int footerFlags() const
    {
        return Qt::AlignLeft;
    }

    int footerMTop() const;
    const QRect footerRect(QPrinter *p) const;
};

/*!
 * \class HeaderFooterPrinter
 * \brief Класс, позволяющий печатать с колонтитулами
 */

class HeaderFooterPrinter
{
public:
    HeaderFooterPrinter(IHeaderFooter *headerFooter);

    void print(QPrinter *printer, const QString &html);
    void printPlainText(QPrinter *printer, const QString &plainText);

protected:
    void p_print(QPrinter *printer, QTextDocument &doc);
    bool needPrint(QPrinter *printer, int pageNumber);

protected:
    IHeaderFooter *mHeaderFooter;
};

} // namespace PrinterHelper

#endif // PRINTERHELPER_H
