#include "restrictionlineedit.h"

RestrictionLineEdit::RestrictionLineEdit(QWidget *parent):
    QLineEdit(parent), mMinimumContentsLength(0)
{
}

void RestrictionLineEdit::setMinimumContentsLength(int characters)
{
    mMinimumContentsLength = characters;
}

QSize RestrictionLineEdit::sizeHint() const
{
    QSize sz = QLineEdit::sizeHint();
    if (mMinimumContentsLength > 0)
        sz.setWidth(fontMetrics().width(QLatin1Char('x')) * mMinimumContentsLength);

    return sz;
}
