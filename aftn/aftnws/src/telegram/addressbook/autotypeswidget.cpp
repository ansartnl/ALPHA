#include "autotypeswidget.h"

AutoTypesWidget::AutoTypesWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(freeTextCheckBox, SIGNAL(toggled(bool)), SLOT(onCheckBox()));
    connect(fplCheckBox, SIGNAL(toggled(bool)), SLOT(onCheckBox()));
    connect(arrCheckBox, SIGNAL(toggled(bool)), SLOT(onCheckBox()));
    connect(chgCheckBox, SIGNAL(toggled(bool)), SLOT(onCheckBox()));
    connect(cnlCheckBox, SIGNAL(toggled(bool)), SLOT(onCheckBox()));
    connect(depCheckBox, SIGNAL(toggled(bool)), SLOT(onCheckBox()));
    connect(dlaCheckBox, SIGNAL(toggled(bool)), SLOT(onCheckBox()));
}

void AutoTypesWidget::setTypes(const AutoTypesWidget::Types &t)
{
    blockSignals(true);
    freeTextCheckBox->setChecked(t.testFlag(FREE_TEXT));
    fplCheckBox->setChecked(t.testFlag(FPL));
    arrCheckBox->setChecked(t.testFlag(ARR));
    chgCheckBox->setChecked(t.testFlag(CHG));
    cnlCheckBox->setChecked(t.testFlag(CNL));
    depCheckBox->setChecked(t.testFlag(DEP));
    dlaCheckBox->setChecked(t.testFlag(DLA));
    blockSignals(false);
}

const AutoTypesWidget::Types AutoTypesWidget::types() const
{
    Types ret;

    if (freeTextCheckBox->isChecked())
        ret |= FREE_TEXT;
    if (fplCheckBox->isChecked())
        ret |= FPL;
    if (arrCheckBox->isChecked())
        ret |= ARR;
    if (chgCheckBox->isChecked())
        ret |= CHG;
    if (cnlCheckBox->isChecked())
        ret |= CNL;
    if (depCheckBox->isChecked())
        ret |= DEP;
    if (dlaCheckBox->isChecked())
        ret |= DLA;

    return ret;
}


void AutoTypesWidget::onCheckBox()
{
    emit typesChanged(types());
}
