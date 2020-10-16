#include "ColorWidget.h"

ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    updateColors();

    mPalette = palette();

    connect(windowActive, SIGNAL(colorChanged(QColor)), SLOT(onWindowActiveChanged(QColor)));
    connect(windowInactive, SIGNAL(colorChanged(QColor)), SLOT(onWindowInactiveChanged(QColor)));
    connect(windowDisabled, SIGNAL(colorChanged(QColor)), SLOT(onWindowDisabledChanged(QColor)));

    connect(windowTextActive, SIGNAL(colorChanged(QColor)), SLOT(onWindowTextActiveChanged(QColor)));
    connect(windowTextInactive, SIGNAL(colorChanged(QColor)), SLOT(onWindowTextInactiveChanged(QColor)));
    connect(windowTextDisabled, SIGNAL(colorChanged(QColor)), SLOT(onWindowTextDisabledChanged(QColor)));

    connect(baseActive, SIGNAL(colorChanged(QColor)), SLOT(onBaseActiveChanged(QColor)));
    connect(baseInactive, SIGNAL(colorChanged(QColor)), SLOT(onBaseInactiveChanged(QColor)));
    connect(baseDisabled, SIGNAL(colorChanged(QColor)), SLOT(onBaseDisabledChanged(QColor)));

    connect(alternateBaseActive, SIGNAL(colorChanged(QColor)), SLOT(onAlternateBaseActiveChanged(QColor)));
    connect(alternateBaseInactive, SIGNAL(colorChanged(QColor)), SLOT(onAlternateBaseInactiveChanged(QColor)));
    connect(alternateBaseDisabled, SIGNAL(colorChanged(QColor)), SLOT(onAlternateBaseDisabledChanged(QColor)));

    connect(textActive, SIGNAL(colorChanged(QColor)), SLOT(onTextActiveChanged(QColor)));
    connect(textInactive, SIGNAL(colorChanged(QColor)), SLOT(onTextInactiveChanged(QColor)));
    connect(textDisabled, SIGNAL(colorChanged(QColor)), SLOT(onTextDisabledChanged(QColor)));

    connect(toolTipBaseActive, SIGNAL(colorChanged(QColor)), SLOT(onToolTipBaseActiveChanged(QColor)));
    connect(toolTipBaseInactive, SIGNAL(colorChanged(QColor)), SLOT(onToolTipBaseInactiveChanged(QColor)));
    connect(toolTipBaseDisabled, SIGNAL(colorChanged(QColor)), SLOT(onToolTipBaseDisabledChanged(QColor)));

    connect(toolTipTextActive, SIGNAL(colorChanged(QColor)), SLOT(onToolTipTextActiveChanged(QColor)));
    connect(toolTipTextInactive, SIGNAL(colorChanged(QColor)), SLOT(onToolTipTextInactiveChanged(QColor)));
    connect(toolTipTextDisabled, SIGNAL(colorChanged(QColor)), SLOT(onToolTipTextDisabledChanged(QColor)));

    connect(buttonActive, SIGNAL(colorChanged(QColor)), SLOT(onButtonActiveChanged(QColor)));
    connect(buttonInactive, SIGNAL(colorChanged(QColor)), SLOT(onButtonInactiveChanged(QColor)));
    connect(buttonDisabled, SIGNAL(colorChanged(QColor)), SLOT(onButtonDisabledChanged(QColor)));

    connect(buttonTextActive, SIGNAL(colorChanged(QColor)), SLOT(onButtonTextActiveChanged(QColor)));
    connect(buttonTextInactive, SIGNAL(colorChanged(QColor)), SLOT(onButtonTextInactiveChanged(QColor)));
    connect(buttonTextDisabled, SIGNAL(colorChanged(QColor)), SLOT(onButtonTextDisabledChanged(QColor)));

    connect(brightTextActive, SIGNAL(colorChanged(QColor)), SLOT(onBrightTextActiveChanged(QColor)));
    connect(brightTextInactive, SIGNAL(colorChanged(QColor)), SLOT(onBrightTextInactiveChanged(QColor)));
    connect(brightTextDisabled, SIGNAL(colorChanged(QColor)), SLOT(onBrightTextDisabledChanged(QColor)));

    connect(lightActive, SIGNAL(colorChanged(QColor)), SLOT(onLightActiveChanged(QColor)));
    connect(lightInactive, SIGNAL(colorChanged(QColor)), SLOT(onLightInactiveChanged(QColor)));
    connect(lightDisabled, SIGNAL(colorChanged(QColor)), SLOT(onLightDisabledChanged(QColor)));

    connect(midLightActive, SIGNAL(colorChanged(QColor)), SLOT(onMidLightActiveChanged(QColor)));
    connect(midLightInactive, SIGNAL(colorChanged(QColor)), SLOT(onMidLightInactiveChanged(QColor)));
    connect(midLightDisabled, SIGNAL(colorChanged(QColor)), SLOT(onMidLightDisabledChanged(QColor)));

    connect(midActive, SIGNAL(colorChanged(QColor)), SLOT(onMidActiveChanged(QColor)));
    connect(midInactive, SIGNAL(colorChanged(QColor)), SLOT(onMidInactiveChanged(QColor)));
    connect(midDisabled, SIGNAL(colorChanged(QColor)), SLOT(onMidDisabledChanged(QColor)));

    connect(darkActive, SIGNAL(colorChanged(QColor)), SLOT(onDarkActiveChanged(QColor)));
    connect(darkInactive, SIGNAL(colorChanged(QColor)), SLOT(onDarkInactiveChanged(QColor)));
    connect(darkDisabled, SIGNAL(colorChanged(QColor)), SLOT(onDarkDisabledChanged(QColor)));

    connect(shadowActive, SIGNAL(colorChanged(QColor)), SLOT(onShadowActiveChanged(QColor)));
    connect(shadowInactive, SIGNAL(colorChanged(QColor)), SLOT(onShadowInactiveChanged(QColor)));
    connect(shadowDisabled, SIGNAL(colorChanged(QColor)), SLOT(onShadowDisabledChanged(QColor)));

    connect(highlightActive, SIGNAL(colorChanged(QColor)), SLOT(onHighlightActiveChanged(QColor)));
    connect(highlightInactive, SIGNAL(colorChanged(QColor)), SLOT(onHighlightInactiveChanged(QColor)));
    connect(highlightDisabled, SIGNAL(colorChanged(QColor)), SLOT(onHighlightDisabledChanged(QColor)));

    connect(highlightedTextActive, SIGNAL(colorChanged(QColor)), SLOT(onHighlightedTextActiveChanged(QColor)));
    connect(highlightedTextInactive, SIGNAL(colorChanged(QColor)), SLOT(onHighlightedTextInactiveChanged(QColor)));
    connect(highlightedTextDisabled, SIGNAL(colorChanged(QColor)), SLOT(onHighlightedTextDisabledChanged(QColor)));

    connect(linkActive, SIGNAL(colorChanged(QColor)), SLOT(onLinkActiveChanged(QColor)));
    connect(linkInactive, SIGNAL(colorChanged(QColor)), SLOT(onLinkInactiveChanged(QColor)));
    connect(linkDisabled, SIGNAL(colorChanged(QColor)), SLOT(onLinkDisabledChanged(QColor)));

    connect(linkVisitedActive, SIGNAL(colorChanged(QColor)), SLOT(onLinkVisitedActiveChanged(QColor)));
    connect(linkVisitedInactive, SIGNAL(colorChanged(QColor)), SLOT(onLinkVisitedInactiveChanged(QColor)));
    connect(linkVisitedDisabled, SIGNAL(colorChanged(QColor)), SLOT(onLinkVisitedDisabledChanged(QColor)));

    connect(noRoleActive, SIGNAL(colorChanged(QColor)), SLOT(onNoRoleActiveChanged(QColor)));
    connect(noRoleInactive, SIGNAL(colorChanged(QColor)), SLOT(onNoRoleInactiveChanged(QColor)));
    connect(noRoleDisabled, SIGNAL(colorChanged(QColor)), SLOT(onNoRoleDisabledChanged(QColor)));
}

QPalette ColorWidget::currentPalette() const
{
    return mPalette;
}

void ColorWidget::setCurrentPalette(const QPalette &p)
{
    mPalette = p;
    setPalette(p);
    updateColors();
}

void ColorWidget::updateColors()
{
    windowActive->setColor(palette().color(QPalette::Active, QPalette::Window));
    windowInactive->setColor(palette().color(QPalette::Inactive, QPalette::Window));
    windowDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Window));

    windowTextActive->setColor(palette().color(QPalette::Active, QPalette::WindowText));
    windowTextInactive->setColor(palette().color(QPalette::Inactive, QPalette::WindowText));
    windowTextDisabled->setColor(palette().color(QPalette::Disabled, QPalette::WindowText));

    baseActive->setColor(palette().color(QPalette::Active, QPalette::Base));
    baseInactive->setColor(palette().color(QPalette::Inactive, QPalette::Base));
    baseDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Base));

    alternateBaseActive->setColor(palette().color(QPalette::Active, QPalette::AlternateBase));
    alternateBaseInactive->setColor(palette().color(QPalette::Inactive, QPalette::AlternateBase));
    alternateBaseDisabled->setColor(palette().color(QPalette::Disabled, QPalette::AlternateBase));

    textActive->setColor(palette().color(QPalette::Active, QPalette::Text));
    textInactive->setColor(palette().color(QPalette::Inactive, QPalette::Text));
    textDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Text));

    toolTipBaseActive->setColor(palette().color(QPalette::Active, QPalette::ToolTipBase));
    toolTipBaseInactive->setColor(palette().color(QPalette::Inactive, QPalette::ToolTipBase));
    toolTipBaseDisabled->setColor(palette().color(QPalette::Disabled, QPalette::ToolTipBase));

    toolTipTextActive->setColor(palette().color(QPalette::Active, QPalette::ToolTipText));
    toolTipTextInactive->setColor(palette().color(QPalette::Inactive, QPalette::ToolTipText));
    toolTipTextDisabled->setColor(palette().color(QPalette::Disabled, QPalette::ToolTipText));

    buttonActive->setColor(palette().color(QPalette::Active, QPalette::Button));
    buttonInactive->setColor(palette().color(QPalette::Inactive, QPalette::Button));
    buttonDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Button));

    buttonTextActive->setColor(palette().color(QPalette::Active, QPalette::ButtonText));
    buttonTextInactive->setColor(palette().color(QPalette::Inactive, QPalette::ButtonText));
    buttonTextDisabled->setColor(palette().color(QPalette::Disabled, QPalette::ButtonText));

    brightTextActive->setColor(palette().color(QPalette::Active, QPalette::BrightText));
    brightTextInactive->setColor(palette().color(QPalette::Inactive, QPalette::BrightText));
    brightTextDisabled->setColor(palette().color(QPalette::Disabled, QPalette::BrightText));

    lightActive->setColor(palette().color(QPalette::Active, QPalette::Light));
    lightInactive->setColor(palette().color(QPalette::Inactive, QPalette::Light));
    lightDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Light));

    midLightActive->setColor(palette().color(QPalette::Active, QPalette::Midlight));
    midLightInactive->setColor(palette().color(QPalette::Inactive, QPalette::Midlight));
    midLightDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Midlight));

    midActive->setColor(palette().color(QPalette::Active, QPalette::Mid));
    midInactive->setColor(palette().color(QPalette::Inactive, QPalette::Mid));
    midDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Mid));

    darkActive->setColor(palette().color(QPalette::Active, QPalette::Dark));
    darkInactive->setColor(palette().color(QPalette::Inactive, QPalette::Dark));
    darkDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Dark));

    shadowActive->setColor(palette().color(QPalette::Active, QPalette::Shadow));
    shadowInactive->setColor(palette().color(QPalette::Inactive, QPalette::Shadow));
    shadowDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Shadow));

    highlightActive->setColor(palette().color(QPalette::Active, QPalette::Highlight));
    highlightInactive->setColor(palette().color(QPalette::Inactive, QPalette::Highlight));
    highlightDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Highlight));

    highlightedTextActive->setColor(palette().color(QPalette::Active, QPalette::HighlightedText));
    highlightedTextInactive->setColor(palette().color(QPalette::Inactive, QPalette::HighlightedText));
    highlightedTextDisabled->setColor(palette().color(QPalette::Disabled, QPalette::HighlightedText));

    linkActive->setColor(palette().color(QPalette::Active, QPalette::Link));
    linkInactive->setColor(palette().color(QPalette::Inactive, QPalette::Link));
    linkDisabled->setColor(palette().color(QPalette::Disabled, QPalette::Link));

    linkVisitedActive->setColor(palette().color(QPalette::Active, QPalette::LinkVisited));
    linkVisitedInactive->setColor(palette().color(QPalette::Inactive, QPalette::LinkVisited));
    linkVisitedDisabled->setColor(palette().color(QPalette::Disabled, QPalette::LinkVisited));

    noRoleActive->setColor(palette().color(QPalette::Active, QPalette::NoRole));
    noRoleInactive->setColor(palette().color(QPalette::Inactive, QPalette::NoRole));
    noRoleDisabled->setColor(palette().color(QPalette::Disabled, QPalette::NoRole));

    emit paletteChanged(palette());
}

void ColorWidget::onWindowActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Window, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onWindowInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Window, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onWindowDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Window, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onWindowTextActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::WindowText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onWindowTextInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::WindowText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onWindowTextDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::WindowText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onBaseActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Base, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onBaseInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Base, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onBaseDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Base, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onAlternateBaseActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::AlternateBase, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onAlternateBaseInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::AlternateBase, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onAlternateBaseDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::AlternateBase, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onTextActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Text, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onTextInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Text, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onTextDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Text, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onToolTipBaseActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::ToolTipBase, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onToolTipBaseInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::ToolTipBase, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onToolTipBaseDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::ToolTipBase, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onToolTipTextActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::ToolTipText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onToolTipTextInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::ToolTipText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onToolTipTextDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::ToolTipText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onButtonActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Button, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onButtonInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Button, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onButtonDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Button, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onButtonTextActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::ButtonText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onButtonTextInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::ButtonText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onButtonTextDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::ButtonText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onBrightTextActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::BrightText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onBrightTextInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::BrightText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onBrightTextDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::BrightText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLightActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Light, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLightInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Light, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLightDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Light, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onMidLightActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Midlight, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onMidLightInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Midlight, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onMidLightDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Midlight, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onMidActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Mid, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onMidInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Mid, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onMidDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Mid, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onDarkActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Dark, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onDarkInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Dark, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onDarkDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Dark, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onShadowActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Shadow, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onShadowInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Shadow, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onShadowDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Shadow, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onHighlightActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Highlight, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onHighlightInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Highlight, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onHighlightDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Highlight, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onHighlightedTextActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::HighlightedText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onHighlightedTextInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::HighlightedText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onHighlightedTextDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLinkActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::Link, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLinkInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::Link, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLinkDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::Link, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLinkVisitedActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::LinkVisited, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLinkVisitedInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::LinkVisited, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onLinkVisitedDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::LinkVisited, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onNoRoleActiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Active, QPalette::NoRole, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onNoRoleInactiveChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Inactive, QPalette::NoRole, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}

void ColorWidget::onNoRoleDisabledChanged(const QColor &color)
{
    mPalette.setColor(QPalette::Disabled, QPalette::NoRole, color);
    setPalette(mPalette);

    emit paletteChanged(palette());
}
