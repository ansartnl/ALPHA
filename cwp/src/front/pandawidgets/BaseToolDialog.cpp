#include "BaseToolDialog.h"


BaseToolDialog::BaseToolDialog(bool modal, bool close_button, QWidget *parent) :
    QWidget(parent, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint
            | Qt::CustomizeWindowHint | Qt::WindowTitleHint)
{
    if (close_button)
        setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);

    if (modal)
        setWindowModality(Qt::WindowModal);

    if (parent)
        move(parent->frameGeometry().center());
}

void BaseToolDialog::setVisible(bool visible)
{
    QWidget::setVisible(visible);

    if (visible) {
        activateWindow();
        raise();
    }
}

void BaseToolDialog::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);

    emit hidden();
}
