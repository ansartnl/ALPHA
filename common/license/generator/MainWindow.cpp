#include "License.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->regLineEdit, SIGNAL(textChanged(QString)), SLOT(onRegTextChanged(QString)));
    connect(ui->generatePushButton, SIGNAL(clicked(bool)), SLOT(onGenerate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRegTextChanged(const QString &text)
{
    ui->generatePushButton->setEnabled(ui->regLineEdit->maxLength() == text.length());
}

void MainWindow::onGenerate()
{
    QByteArray regKey = ui->regLineEdit->text().remove('-').toLatin1();

    QByteArray licKey = License::generateLicKey(regKey, ui->keyLineEdit->text().toLatin1());

    ui->licLineEdit->setText(licKey);
}
