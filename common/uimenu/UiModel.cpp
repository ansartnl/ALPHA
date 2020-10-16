#include "UiModel.h"


namespace Uim
{

UiModel::UiModel()
{
    mSubMenus = TGroupItem(new GroupItem());

    mRightBar = TGroupItem(new GroupItem());
    mToolbar = TGroupItem(new GroupItem());
    mToolbarMenu = TGroupItem(new GroupItem());

    mRootMenu = TGroupItem(new GroupItem());
}

UiModel& UiModel::operator = (const UiModel &model)
{
    *mSubMenus = *model.mSubMenus;
    *mRightBar = *model.mRightBar;
    *mToolbar = *model.mToolbar;
    *mToolbarMenu = *model.mToolbarMenu;
    *mRootMenu = *model.mRootMenu;

    return *this;
}

bool UiModel::isEmpty() const
{
    return ( mToolbar->count() < 1 && mRootMenu->count() < 1 );
}

}   // namespace Uim
