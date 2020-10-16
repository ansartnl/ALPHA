#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>

/// Custom text edit for notifying UI about end of typing
class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CustomTextEdit(QWidget *parent = 0);
    virtual ~CustomTextEdit();

protected:
    virtual void focusOutEvent(QFocusEvent *e);

public:
Q_SIGNALS:
    void editingFinished();
};

#endif // CUSTOMTEXTEDIT_H
