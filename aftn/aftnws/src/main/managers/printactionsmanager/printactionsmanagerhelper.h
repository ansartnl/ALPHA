#ifndef PRINTACTIONSMANAGERHELPER_H
#define PRINTACTIONSMANAGERHELPER_H

#include <QtGui/QAction>
#include <QtGui/QStyleOptionViewItemV4>


class QPrinter;
/**
* @brief Assistant to manage the activities page setup-preview-print for any widget
*
* @class PrintActionsManagerHelper printactionsmanagerhelper.h
*/
class PrintActionsManagerHelper : public QObject
{
    Q_OBJECT
public:
    explicit PrintActionsManagerHelper(QObject* parent = 0);
    virtual ~PrintActionsManagerHelper();

    virtual unsigned char approach(QWidget *widget) const {return 5;};
    virtual void activateFor(QWidget* widget) {this->widget = widget; };
    virtual void deactivate() {this->widget = 0;};


    virtual bool canPageSetup() const {return true;};
    virtual bool canPrintPreview() const {return true;};
    virtual bool canPrint() const {return true;};

public slots:
    virtual void pageSetup();
    virtual void printPreview();
    virtual void print();

signals:
    void canPageSetupChanged(bool enable);
    void canPrintPreviewChanged(bool enable);
    void canPrintChanged(bool enable);

    void blockFocusChanged(bool blocked);

protected:

    bool hasSelection() { return false;}
    QPrinter *printer;
    QWidget *widget;

protected slots:
    virtual void render(QPrinter *printer);
};

class QTextEdit;
class QPlainTextEdit;
/**
* @brief Assistant to manage the activities page setup-preview-print for text edit
*
* @class TextEditPrintActionsManagerHelper printactionsmanagerhelper.h
*/
class TextEditPrintActionsManagerHelper : public PrintActionsManagerHelper
{
    Q_OBJECT
public:
    explicit TextEditPrintActionsManagerHelper(QObject* parent = 0);

    virtual unsigned char approach(QWidget *widget) const;
    virtual void activateFor(QWidget* widget);
    virtual void deactivate();

    virtual bool canPageSetup() const;
    virtual bool canPrintPreview() const;
    virtual bool canPrint() const;

public slots:
    virtual void pageSetup() {PrintActionsManagerHelper::pageSetup();};
    virtual void printPreview(){PrintActionsManagerHelper::printPreview();};
    virtual void print(){PrintActionsManagerHelper::print();};
protected:
    virtual bool hasSelection();
protected slots:
    virtual void render(QPrinter* printer);
private:
    QTextEdit *textEdit;
    QPlainTextEdit *plainTextEdit;
};


class QAbstractItemView;
/**
* @brief Assistant to manage the activities page setup-preview-print for any item view
*
* @class ItemViewPrintActionsManagerHelper printactionsmanagerhelper.h
*/
class ItemViewPrintActionsManagerHelper : public PrintActionsManagerHelper
{
    Q_OBJECT
public:
    explicit ItemViewPrintActionsManagerHelper(QObject* parent = 0);

    virtual unsigned char approach(QWidget *widget) const;
    virtual void activateFor(QWidget* widget);
    virtual void deactivate();
public slots:
    virtual void pageSetup() {PrintActionsManagerHelper::pageSetup();};
    virtual void printPreview(){PrintActionsManagerHelper::printPreview();};
    virtual void print(){PrintActionsManagerHelper::print();};
protected:
    virtual bool hasSelection();
protected slots:
    virtual void render(QPrinter* printer);
private:
    QAbstractItemView *itemView;
};

class QTableView;
/**
* @brief Assistant to manage the activities page setup-preview-print for any table view
*
* @class TableViewPrintActionsManagerHelper printactionsmanagerhelper.h
*/
class TableViewPrintActionsManagerHelper : public PrintActionsManagerHelper
{
    Q_OBJECT
public:
    explicit TableViewPrintActionsManagerHelper(QObject* parent = 0);

    virtual unsigned char approach(QWidget *widget) const;
    virtual void activateFor(QWidget* widget);
    virtual void deactivate();
public slots:
    virtual void pageSetup() {PrintActionsManagerHelper::pageSetup();};
    virtual void printPreview(){PrintActionsManagerHelper::printPreview();};
    virtual void print(){PrintActionsManagerHelper::print();};
protected:
    virtual bool hasSelection();
protected slots:
    virtual void render(QPrinter* printer);
private:
    QTableView *tableView;
};

class QGraphicsView;
/**
* @brief Assistant to manage the activities page setup-preview-print for any graphics view
*
* @class GraphicsViewPrintActionsManagerHelper printactionsmanagerhelper.h
*/
class GraphicsViewPrintActionsManagerHelper : public PrintActionsManagerHelper
{
    Q_OBJECT
public:
    explicit GraphicsViewPrintActionsManagerHelper(QObject* parent = 0);

    virtual unsigned char approach(QWidget *widget) const;
    virtual void activateFor(QWidget* widget);
    virtual void deactivate();
public slots:
    virtual void pageSetup() {PrintActionsManagerHelper::pageSetup();};
    virtual void printPreview(){PrintActionsManagerHelper::printPreview();};
    virtual void print(){PrintActionsManagerHelper::print();};
protected slots:
    virtual void render(QPrinter* printer);
private:
    QGraphicsView *graphicsView;
};

#endif //PRINTACTIONSMANAGERHELPER_H
