#ifndef _UI_MODEL_H__
#define _UI_MODEL_H__

#include "UiItems.h"

#include <QtCore/QMap>


namespace Uim {

//! Model of items for main menu, toolbar and user submenu groups.
/*!
    This model containts object representation of ".uim" xml config file.
*/
struct UiModel
{
    UiModel();

    //! Replace current model with the new one.
    UiModel& operator = (const UiModel &model);

    //! Returns true if model does not have a menu or toolbar.
    bool isEmpty() const;

public:
    /** Map of user defined menus */
    TGroupItem  mSubMenus;

    /** Right bar buttons of toolbar */
    TGroupItem  mRightBar;
    /** Toolbar - extended view */
    TGroupItem  mToolbar;
    /** Toolbar's main menu */
    TGroupItem  mToolbarMenu;

    /** Menu - clasic view. */
    TGroupItem  mRootMenu;
};

}   // namespace Uim

#endif // _UI_MODEL_H__
