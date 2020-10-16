#include "CopyDialog.h"
#include "ui_copydialog.h"

CCopyDialog::CCopyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCopyDialog)
{
    ui->setupUi(this);

    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(&mFileCopy, SIGNAL(tick()), this, SLOT(onTick()));
    connect(&mFileCopy, SIGNAL(done()), this, SLOT(onDone()));
    mFileCopy.start();
}

CCopyDialog::~CCopyDialog()
{
    delete ui;
}

void CCopyDialog::copy(const QString &src, QString dst)
{
    mFileCopy.source = src;
    mFileCopy.dest = dst;
}

void CCopyDialog::onCancel()
{
    mFileCopy.cancel();
    close();
}

void CCopyDialog::onTick()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

void CCopyDialog::onDone()
{
    close();
}
