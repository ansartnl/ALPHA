#ifndef QTCOLORBUTTON_H
#define QTCOLORBUTTON_H

#include <QtGui/QPushButton>

class QtColorButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QtColorButton(QWidget *parent = 0);
    explicit QtColorButton(const QColor &initialColor, QWidget *parent = 0);
    explicit QtColorButton(const QColor &initialColor, QPalette::ColorRole role, QWidget *parent = 0);

    QColor color() const;

    QPalette::ColorRole colorRole() const;
    void setColorRole(QPalette::ColorRole r);

signals:
    void colorChanged(const QColor &newColor);

public slots:
    void setColor(const QColor &c);

protected slots:
    void onClicked();

protected:
    QPalette::ColorRole mColorRole;
};

#endif // QTCOLORBUTTON_H
