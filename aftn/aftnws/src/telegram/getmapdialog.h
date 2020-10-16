#ifndef GETMAPDIALOG_H
#define GETMAPDIALOG_H

#include "ui_getmapdialog.h"

struct Value
{
    QString name;
    QString description;

    Value()
    {}

    Value(const QString &n, const QString &d) :
        name(n), description(d)
    {}
};

typedef QMap < int, Value > ValueMap;

class GetMapDialog : public QDialog, private Ui::GetMapDialog
{
    Q_OBJECT
public:
    explicit GetMapDialog(QWidget *parent = 0);

    void setMap(const ValueMap &m);

    int selectedIndex() const;
    const QString selectedText() const;

    void setTitle(const QString &text);
    void setLabel(const QString &text);

    static int getIndex(QWidget *parent, const QString &title, const QString &text, const ValueMap &m
                        , bool *ok = 0);

protected slots:
    void onComboBoxChanged();

protected:
    ValueMap map;
};

#endif // GETMAPDIALOG_H
