#ifndef PDFWINDOW_H
#define PDFWINDOW_H

#include <QMainWindow>
#include "pdfdocumentwidget.h"
#include "ui_pdfwindow.h"

class QShowEvent;

class PdfWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT

public:
    explicit PdfWindow(QWidget *parent = 0);

public slots:
	void openFile(const QString &file);

private slots:
    void checkSearchText(const QString &text);
    void scaleDocument(int index);
    void searchDocument();
    void showSelectedText(const QString &text);

protected:
    virtual void showEvent(QShowEvent *event);

private:
	Q_DISABLE_COPY(PdfWindow)

    PdfDocumentWidget *document_widget_;
    QString last_path_;
    QVector<qreal> scale_factors_;
};

#endif
