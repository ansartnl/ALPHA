#include "printdatadialog.h"

#include "main.h"
#include "printactionsmanager.h"

#include <QtCore/QDateTime>

PrintDataDialog::PrintDataDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    printActionsManager = new PrintActionsManager(this);
    connect(this, SIGNAL(print()), printActionsManager, SIGNAL(print()));

    connect(printButton, SIGNAL(clicked()), SLOT(onPrintButton()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(onCancelButton()));
}

void PrintDataDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);

    switch (e->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        generate();
        break;
    default:
        break;
    }
}

void PrintDataDialog::generate()
{
    textEdit->clear();

    textEdit->append(tr("Document: %1").arg(documentName()));
    textEdit->append("\n\n");
    textEdit->append(tr("Printed by '%1'").arg(userName()));
    textEdit->append("\n\n");
    textEdit->append(tr("Printed at %1 UTC").arg(
                         QDateTime::currentDateTimeUtc().toString("yyyy.MM.dd hh:mm:ss")));
    textEdit->append("\n\n");
    textEdit->append(tr("Security level: %1").arg(securityLevel()));
    textEdit->append("\n\n");

    QStringList data = dataList();
    foreach (const QString &piece, data)
    {
        textEdit->append(piece);
        textEdit->append("\n\n");
    }
}

const QString PrintDataDialog::userName() const
{
    return currentUserFullName;
}

const QStringList PrintDataDialog::dataList() const
{
    return QStringList();
}

void PrintDataDialog::onPrintButton()
{
    textEdit->setFocus();
    emit print();
}

void PrintDataDialog::onCancelButton()
{
    reject();
}
