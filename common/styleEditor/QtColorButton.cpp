#include "QtColorButton.h"

#include <QtGui/QColorDialog>

QtColorButton::QtColorButton(QWidget *parent) :
    QPushButton(parent)
{
    setColorRole(QPalette::Button);

    connect(this, SIGNAL(clicked()), SLOT(onClicked()));
}

QtColorButton::QtColorButton(const QColor &initialColor, QWidget *parent) :
    QPushButton(parent)
{
    setColorRole(QPalette::Button);

    setColor(initialColor);

    connect(this, SIGNAL(clicked()), SLOT(onClicked()));
}

QtColorButton::QtColorButton(const QColor &initialColor, QPalette::ColorRole role, QWidget *parent) :
    QPushButton(parent)
{
    setColorRole(role);

    setColor(initialColor);

    connect(this, SIGNAL(clicked()), SLOT(onClicked()));
}

QColor QtColorButton::color() const
{
    return palette().color(colorRole());
}

QPalette::ColorRole QtColorButton::colorRole() const
{
    return mColorRole;
}

void QtColorButton::setColorRole(QPalette::ColorRole r)
{
    mColorRole = r;
}

void QtColorButton::setColor(const QColor &c)
{
    QPalette p = palette();
    p.setColor(colorRole(), c);
    setPalette(p);
}

void QtColorButton::onClicked()
{
    QColor newColor = QColorDialog::getColor(color(), this);
    if (!newColor.isValid())
        return;
    setColor(newColor);
    emit colorChanged(newColor);
}
