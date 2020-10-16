#ifndef RESTRICTIONLINEEDIT_H
#define RESTRICTIONLINEEDIT_H

#include <QLineEdit>

class RestrictionLineEdit : public QLineEdit
{
public:
    RestrictionLineEdit(QWidget *parent = 0);
    void setMinimumContentsLength(int characters);

    QSize sizeHint() const;

private:
    int mMinimumContentsLength;
};

#endif // RESTRICTIONLINEEDIT_H
