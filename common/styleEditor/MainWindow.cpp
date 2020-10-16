#include "MainWindow.h"

#include "ExtendedStyle.h"

#include <QtGui/QFileDialog>
#include <QtGui/QStyleFactory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    defaultStyle = qApp->style()->objectName();

    treeWidget->topLevelItem(1)->setExpanded(true);
    treeWidget->topLevelItem(1)->child(0)->setExpanded(true);

    styleCombo->addItems(QStyleFactory::keys());
    styleCombo->setCurrentIndex(styleCombo->findText(qApp->style()->objectName()
                                                     , Qt::MatchFixedString));

    styleLabel = new QLabel(this);
    statusBar()->addWidget(styleLabel);

    updateStyle();

    connect(styleCombo, SIGNAL(currentIndexChanged(QString)), SLOT(onStyleChanged(QString)));
    connect(styleCombo, SIGNAL(currentIndexChanged(QString))
            , colorWidget, SLOT(updateColors()));

    connect(colorWidget, SIGNAL(paletteChanged(QPalette)), SLOT(onPaletteChange(QPalette)));

    connect(enabledCheckBox, SIGNAL(toggled(bool)), tabWidget, SLOT(setEnabled(bool)));

    connect(actionSave, SIGNAL(triggered()), SLOT(onSave()));
    connect(actionLoad, SIGNAL(triggered()), SLOT(onLoad()));
    connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::updateStyle()
{
    styleLabel->setText(qApp->style()->objectName());
}

void MainWindow::onStyleChanged(const QString &newStyle)
{
    qApp->setStyle(QStyleFactory::create(newStyle));
    updateStyle();
}

void MainWindow::onPaletteChange(const QPalette &p)
{
    qApp->setPalette(p);
}

void MainWindow::onSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save to..."), "/"
                                                    , tr("Extended style files (*.es)"));
    if (fileName.isEmpty())
        return;

    ExtendedStyle es;
    es.palette = palette();
    es.style = styleCombo->currentText();

    es.saveToFile(fileName);
}

void MainWindow::onLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load from..."), "/"
                                                    , tr("Extended style files (*.es)"));
    if (fileName.isEmpty())
        return;

    ExtendedStyle es;

    es.loadFromFile(fileName);

    onPaletteChange(es.palette);
    onStyleChanged(es.style);
    styleCombo->setCurrentIndex(styleCombo->findText(es.style, Qt::MatchFixedString));

    colorWidget->setCurrentPalette(es.palette);
}
