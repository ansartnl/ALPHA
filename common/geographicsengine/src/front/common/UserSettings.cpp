#include "UserSettings.h"


namespace UserSettings {

Accessor::Accessor(IAccessor *stg)
    : mStg(stg)
{
}

Accessor& Accessor::instance(IAccessor *stg)
{
    static Accessor gStg(stg);
    return gStg;
}

QColor Accessor::color(ColorType typeColor) const
{
    return ( mStg ? mStg->color(typeColor) : QColor() );
}

QFont Accessor::font(FontType typeFont) const
{
    return ( mStg ? mStg->font(typeFont) : QFont() );
}

const Grid& Accessor::grid() const
{
    static Grid gGrid;
    return ( mStg ? mStg->grid() : gGrid );
}

}   // namespace UserProfile
