#ifndef FPLEDITWIDGET_H
#define FPLEDITWIDGET_H

#include "ui_fpleditwidget.h"

#include "AFTNDao.h"
#include "fpl.h"

#include <QtCore/QString>

class UppercaseValidator;

/// Widget display FPL in the detailed style and allows to edit
class FplEditWidget: public QWidget, public Ui::FplEditWidget
{
    Q_OBJECT
public:
    FplEditWidget(QWidget* pParent = 0);
    void clear();
    void setFpl(const QVariantMap &message);

signals:
    void apply(const FPL &);
    void maybeDushanbe(bool& bRes, const FPL& fpl);

protected:
    void checkFpl();
    void init();
    virtual void changeEvent(QEvent *event);

public slots:
    void onApply();

private slots:
    void onTextChanged();
    void onACIDEditChanged();
    void onAirplaneComboChanged();
    void onEquipmentEditChanged();
    void onAerodromeDepEditChanged();
    void onSpeedEditChanged();
    void onLevelEditChanged();
    void onRouteEditChanged();
    void onAerodromeDestEditChanged();
    void onEntryFIREETChanged();
    void onEETChanged();
    void onALTNEditChanged();
    void onALTN2EditChanged();

private:
    QString firName;
    QString validValue;
    QString invalidValue;
    UppercaseValidator *uppercaseValidator;
};

#endif // FPLEDITWIDGET_H
