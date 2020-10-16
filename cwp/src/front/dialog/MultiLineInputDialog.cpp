#include "MultiLineInputDialog.h"
#include "ui_MultiLineInputDialog.h"

MultiLineInputDialog::MultiLineInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiLineInputDialog)
{
    ui->setupUi(this);
}

MultiLineInputDialog::~MultiLineInputDialog()
{
    delete ui;
}

QString MultiLineInputDialog::getText() const
{
    return mData;
}

void MultiLineInputDialog::setText(const QString &str) const
{
    ui->textEdit->setPlainText(str);
}

void MultiLineInputDialog::accept()
{
    mData = ui->textEdit->toPlainText();
    QDialog::accept();
}
