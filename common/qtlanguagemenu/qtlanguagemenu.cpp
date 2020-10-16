#include "qtlanguagemenu.h"
#include "qttranslator.h"
#include <QtCore/QEvent>

QtLanguageMenu::QtLanguageMenu(QWidget *parent) : QMenu(parent)
{
    connect(QtTranslator::instance(), SIGNAL(actionListChanged()), this, SLOT(reloadTranslations()));
    addActions(QtTranslator::instance()->languages());
    retranslateUi();
}

QtLanguageMenu::~QtLanguageMenu()
{}

void QtLanguageMenu::reloadTranslations()
{
    clear();
    addActions(QtTranslator::instance()->languages());
}

void QtLanguageMenu::retranslateUi()
{
    setTitle(tr("Language"));
}

void QtLanguageMenu::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
}
