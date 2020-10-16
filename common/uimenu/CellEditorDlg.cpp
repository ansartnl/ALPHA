#include "CellEditorDlg.h"
#include "ui_CellEditorDlg.h"

namespace Uim
{

CellEditorDlg::CellEditorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CellEditorDlg)
{
    ui->setupUi(this);
}

CellEditorDlg::~CellEditorDlg()
{
    delete ui;
}

void CellEditorDlg::setCellLabelText(const QString &text)
{
    ui->labelCell->setText(text);
}

void CellEditorDlg::setCellValue(int value)
{
    ui->spinBoxCell->setValue(value);
}

int CellEditorDlg::cellValue() const
{
    return ui->spinBoxCell->value();
}

void CellEditorDlg::setSpanValue(int value)
{
    ui->spinBoxSpan->setValue(value);
}

int CellEditorDlg::spanValue() const
{
    return ui->spinBoxSpan->value();
}

int CellEditorDlg::getCell(QWidget *parent, const QString &title, const QString &labelText,
                           int valueCell, int &valueSpan, bool *ok)
{
    CellEditorDlg dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setCellLabelText(labelText);
    dlg.setCellValue(valueCell);
    dlg.setSpanValue(valueSpan);
    if ( dlg.exec() == QDialog::Accepted )
    {
        if ( ok )
            *ok = true;

        valueSpan = dlg.spanValue();
        return dlg.cellValue();
    }

    if ( ok )
        *ok = false;
    return -1;
}

void CellEditorDlg::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }

    QDialog::changeEvent(event);
}

}   // namespace Uim
