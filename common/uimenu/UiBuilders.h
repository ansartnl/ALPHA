#ifndef _UI_BUILDERS_H__
#define _UI_BUILDERS_H__

#include "UiModel.h"
#include "AbstractUiModel.h"

#include <QtGui/QMainWindow>


namespace Uim {

/*! Property holds state of translation the QAction or QMenu.
    QVariant value of this property contains bool value:
    - true when text for QAction or QMenu was defined by the user
    - false when used predefined QAction or QMenu
 */
static const char userTextProperty[] = "UserText";

//! Ui menu or toolbar builders.
namespace UiBuilder
{
    //! Build menu bar.
    void buildMenu(QMainWindow *mainWindow, const UiModel &uiModel,
                   const AbstractUiModel &cmdModel);

    //! Build toolbar - tabcontrol with toolbuttons.
    void buildToolbar(QMainWindow *mainWindow, const UiModel &uiModel,
                      const AbstractUiModel &cmdModel);

    //! Translation of Ui.
    void retranslateUi(QMainWindow *mainWindow);
}

}   // namespace Uim

#endif // _UI_BUILDERS_H__
