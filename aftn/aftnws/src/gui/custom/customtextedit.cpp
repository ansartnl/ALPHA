#include "customtextedit.h"

CustomTextEdit::CustomTextEdit(QWidget *parent): QTextEdit(parent)
{
}

CustomTextEdit::~CustomTextEdit()
{
}

void CustomTextEdit::focusOutEvent(QFocusEvent *e)
{
    QTextEdit::focusOutEvent(e);

    emit editingFinished();
}
