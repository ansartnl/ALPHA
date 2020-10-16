#include "getmapdialog.h"

GetMapDialog::GetMapDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    connect(comboBox, SIGNAL(currentIndexChanged(int)), SLOT(onComboBoxChanged()));
}

void GetMapDialog::setMap(const ValueMap &m)
{
    map = m;
    foreach (int key, m.keys())
        comboBox->addItem(m.value(key).name, key);
}

int GetMapDialog::selectedIndex() const
{
    return comboBox->itemData(comboBox->currentIndex()).toInt();
}

const QString GetMapDialog::selectedText() const
{
    return comboBox->currentText();
}

void GetMapDialog::setTitle(const QString &text)
{
    setWindowTitle(text);
}

void GetMapDialog::setLabel(const QString &text)
{
    label->setText(text);
}

int GetMapDialog::getIndex(QWidget *parent, const QString &title, const QString &text, const ValueMap &m
                           , bool *ok)
{
    GetMapDialog dialog(parent);
    dialog.setTitle(title);
    dialog.setLabel(text);
    dialog.setMap(m);

    bool result = (dialog.exec() == QDialog::Accepted);
    if (ok)
        *ok = result;

    if (result)
        return dialog.selectedIndex();
    return -1;
}

void GetMapDialog::onComboBoxChanged()
{
    description->setText(map.value(selectedIndex()).description);
}
