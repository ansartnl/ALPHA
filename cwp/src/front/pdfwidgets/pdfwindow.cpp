#include <QtGui>
#include <QMessageBox>

#include "pdfwindow.h"

PdfWindow::PdfWindow(QWidget *parent)
    : QMainWindow(parent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    setupUi(this);

    scale_factors_ << 0.25 << 0.5 << 0.75 << 1. << 1.25 << 1.5 << 2. << 3. << 4.;

    addDockWidget(Qt::BottomDockWidgetArea, selectionDockWidget);

    document_widget_ = new PdfDocumentWidget();
    scrollArea->setWidget(document_widget_);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(documentControlsAction, SIGNAL(toggled(bool)),
            controlsDockWidget, SLOT(setVisible(bool)));
    connect(selectedTextAction, SIGNAL(toggled(bool)),
            selectionDockWidget, SLOT(setVisible(bool)));
    connect(controlsDockWidget, SIGNAL(visibilityChanged(bool)),
            documentControlsAction, SLOT(setChecked(bool)));
    connect(selectionDockWidget, SIGNAL(visibilityChanged(bool)),
            selectedTextAction, SLOT(setChecked(bool)));

    connect(pageSpinBox, SIGNAL(valueChanged(int)),
            document_widget_, SLOT(setPage(int)));
    connect(document_widget_, SIGNAL(pageChanged(int)),
            pageSpinBox, SLOT(setValue(int)));
    connect(scaleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(scaleDocument(int)));
    connect(document_widget_, SIGNAL(textSelected(const QString &)),
            this, SLOT(showSelectedText(const QString &)));

    connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(searchDocument()));
    connect(findButton, SIGNAL(clicked()), this, SLOT(searchDocument()));
    connect(clearButton, SIGNAL(clicked()), document_widget_, SLOT(setPage()));
    connect(searchLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(checkSearchText(const QString &)));

    selectedTextAction->setChecked(false);
}

void PdfWindow::checkSearchText(const QString &text)
{
    if (text.isEmpty())
        document_widget_->setPage();
}

void PdfWindow::openFile(const QString &file)
{
    if (document_widget_->setDocument(file)) {
        last_path_ = file;
        searchLineEdit->setEnabled(true);
        searchComboBox->setEnabled(true);
        findButton->setEnabled(true);
        clearButton->setEnabled(true);
        scaleComboBox->setEnabled(true);
        pageSpinBox->setEnabled(true);
        pageSpinBox->setMinimum(1);
        pageSpinBox->setMaximum(document_widget_->document()->numPages());
        pageSpinBox->setValue(1);
    } else {
        QMessageBox::warning(this, tr("PDF Viewer - Failed to Open File"),
                             tr("The specified file could not be opened."));
    }
}

void PdfWindow::scaleDocument(int index)
{
    document_widget_->setScale(scale_factors_[index]);
}

void PdfWindow::searchDocument()
{
    QRectF location;
    if (searchComboBox->currentIndex() == 0)
        location = document_widget_->searchForwards(searchLineEdit->text());
    else
        location = document_widget_->searchBackwards(searchLineEdit->text());

    QPoint target = document_widget_->matrix().mapRect(location).center().toPoint();
    scrollArea->ensureVisible(target.x(), target.y());
}

void PdfWindow::showSelectedText(const QString &text)
{
    textViewer->setPlainText(text);
    selectedTextAction->setChecked(true);
}

void PdfWindow::showEvent(QShowEvent *)
{
    //documentControlsAction->setChecked(true);
}
