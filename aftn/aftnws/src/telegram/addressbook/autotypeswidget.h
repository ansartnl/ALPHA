#ifndef AUTOTYPESWIDGET_H
#define AUTOTYPESWIDGET_H

#include "ui_autotypeswidget.h"

#include <QFlags>

class AutoTypesWidget : public QWidget, private Ui::AutoTypesWidget
{
    Q_OBJECT
public:
    explicit AutoTypesWidget(QWidget *parent = 0);

    enum Type {
        NONE = 0x00000000
        , FREE_TEXT = 0x00000001
        , FPL = 0x00000002
        , ARR = 0x00000004
        , CHG = 0x00000008
        , CNL = 0x00000010
        , DEP = 0x00000020
        , DLA = 0x00000040

        , ALL = FREE_TEXT | FPL | ARR | CHG | CNL | DEP | DLA
    };

    typedef QFlags < Type > Types;

    void setTypes(const Types &t);
    const Types types() const;

signals:
    void typesChanged(const AutoTypesWidget::Types &t);

protected slots:
    void onCheckBox();
};

#endif // AUTOTYPESWIDGET_H
