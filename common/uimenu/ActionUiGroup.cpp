#include "ActionUiGroup.h"

#include <QtGui/QApplication>

namespace Uim {

// Customize
const QString g_actCustomize = QT_TRANSLATE_NOOP("Uim::ActionUiGroup", "Customize...");

void retranslateUi()
{
    QAction *action = 0;

    action = CustomizeCmd::action();
    action->setText( QApplication::translate("Uim::ActionUiGroup", g_actCustomize.toLatin1().constData()) );
}


#define BEGIN_ACTION_GROUP(parent) \
        static QAction *gAction = 0; \
        if ( gAction ) \
            return gAction; \
        Q_ASSERT(parent); \
        gAction = new QAction(parent);

#define END_ACTION_GROUP \
        return gAction;


QAction* CustomizeCmd::action(QObject *parent)
{
    BEGIN_ACTION_GROUP(parent)

    gAction->setText( QApplication::translate("Uim::ActionUiGroup", g_actCustomize.toLatin1().constData()) );

    END_ACTION_GROUP
}

}   // namespace Actions
