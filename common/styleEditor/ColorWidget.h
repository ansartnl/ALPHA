#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include "ui_ColorWidget.h"

class ColorWidget : public QWidget, private Ui::ColorWidget
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = 0);

    QPalette currentPalette() const;
    void setCurrentPalette(const QPalette &p);

public slots:
    void updateColors();

signals:
    void paletteChanged(const QPalette &p);

protected slots:
    void onWindowActiveChanged(const QColor &color);
    void onWindowInactiveChanged(const QColor &color);
    void onWindowDisabledChanged(const QColor &color);

    void onWindowTextActiveChanged(const QColor &color);
    void onWindowTextInactiveChanged(const QColor &color);
    void onWindowTextDisabledChanged(const QColor &color);

    void onBaseActiveChanged(const QColor &color);
    void onBaseInactiveChanged(const QColor &color);
    void onBaseDisabledChanged(const QColor &color);

    void onAlternateBaseActiveChanged(const QColor &color);
    void onAlternateBaseInactiveChanged(const QColor &color);
    void onAlternateBaseDisabledChanged(const QColor &color);

    void onTextActiveChanged(const QColor &color);
    void onTextInactiveChanged(const QColor &color);
    void onTextDisabledChanged(const QColor &color);

    void onToolTipBaseActiveChanged(const QColor &color);
    void onToolTipBaseInactiveChanged(const QColor &color);
    void onToolTipBaseDisabledChanged(const QColor &color);

    void onToolTipTextActiveChanged(const QColor &color);
    void onToolTipTextInactiveChanged(const QColor &color);
    void onToolTipTextDisabledChanged(const QColor &color);

    void onButtonActiveChanged(const QColor &color);
    void onButtonInactiveChanged(const QColor &color);
    void onButtonDisabledChanged(const QColor &color);

    void onButtonTextActiveChanged(const QColor &color);
    void onButtonTextInactiveChanged(const QColor &color);
    void onButtonTextDisabledChanged(const QColor &color);

    void onBrightTextActiveChanged(const QColor &color);
    void onBrightTextInactiveChanged(const QColor &color);
    void onBrightTextDisabledChanged(const QColor &color);

    void onLightActiveChanged(const QColor &color);
    void onLightInactiveChanged(const QColor &color);
    void onLightDisabledChanged(const QColor &color);

    void onMidLightActiveChanged(const QColor &color);
    void onMidLightInactiveChanged(const QColor &color);
    void onMidLightDisabledChanged(const QColor &color);

    void onMidActiveChanged(const QColor &color);
    void onMidInactiveChanged(const QColor &color);
    void onMidDisabledChanged(const QColor &color);

    void onDarkActiveChanged(const QColor &color);
    void onDarkInactiveChanged(const QColor &color);
    void onDarkDisabledChanged(const QColor &color);

    void onShadowActiveChanged(const QColor &color);
    void onShadowInactiveChanged(const QColor &color);
    void onShadowDisabledChanged(const QColor &color);

    void onHighlightActiveChanged(const QColor &color);
    void onHighlightInactiveChanged(const QColor &color);
    void onHighlightDisabledChanged(const QColor &color);

    void onHighlightedTextActiveChanged(const QColor &color);
    void onHighlightedTextInactiveChanged(const QColor &color);
    void onHighlightedTextDisabledChanged(const QColor &color);

    void onLinkActiveChanged(const QColor &color);
    void onLinkInactiveChanged(const QColor &color);
    void onLinkDisabledChanged(const QColor &color);

    void onLinkVisitedActiveChanged(const QColor &color);
    void onLinkVisitedInactiveChanged(const QColor &color);
    void onLinkVisitedDisabledChanged(const QColor &color);

    void onNoRoleActiveChanged(const QColor &color);
    void onNoRoleInactiveChanged(const QColor &color);
    void onNoRoleDisabledChanged(const QColor &color);

protected:
    QPalette mPalette;
};

#endif // COLORWIDGET_H
