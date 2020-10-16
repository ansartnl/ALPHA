#ifndef _USER_SETTINGS_H__
#define _USER_SETTINGS_H__

#include <QtGui/QColor>
#include <QtGui/QFont>

namespace UserSettings {

//! Color data type
enum ColorType
{
    CtBackground = 1,
    CtGrid,
    CtMap,
    CtCorridor,
    CtFIR,
    CtSector,
    CtBorder,               //!< FIR border type
    CtLegend,
    CtMeteo,
    CtPeleng
};

//! Font data type
enum FontType
{
    FtGeodesy = 1
};


//! Grid settings
struct Grid
{
    //! Mesurement of distance (unit)
    enum DistanceUnit
    {
        Kilometers,
        NauticalMiles   //!< 1,852 metres
    };

    //! Returns true if this grid has the same settings as grid; otherwise returns false
    bool operator== (const Grid &grid) const
    {
        return ( mIsAzimuthRange == grid.mIsAzimuthRange && mIsCartesian == grid.mIsCartesian &&
              mIsGeodesic == grid.mIsGeodesic && mIsFullScreen == grid.mIsFullScreen &&
              mIsAutoLabelPos == grid.mIsAutoLabelPos &&
              mIsScale == grid.mIsScale && mDistUnit == grid.mDistUnit &&
              mRange == grid.mRange && mAzimuth == grid.mAzimuth );
    }

    //! Returns true if this grid is different from grid; otherwise returns false
    bool operator!= (const Grid &grid) const
    { return !(operator == (grid)); }

    /** Azimuth range is settled */
    bool            mIsAzimuthRange;
    /** Determines cartesian grid */
    bool            mIsCartesian;
    /** Determines geodesic grid */
    bool            mIsGeodesic;
    /** Determines fullscreen grid */
    bool            mIsFullScreen;
    /** Label's auto positioning */
    bool            mIsAutoLabelPos;

    /** Determines scalable grid */
    bool            mIsScale;

    /** The unit for measure distance */
    DistanceUnit    mDistUnit;

    /** Range in kilometers */
    int             mRange;
    /** Azimuth in degree */
    int             mAzimuth;
};


//! Declaration of interface to access user settings.
class IAccessor
{
public:
    //! Pure virtual function for get color
    virtual QColor color(ColorType typeColor) const = 0;
    //! Pure virtual function for get font
    virtual QFont font(FontType typeFont) const = 0;
    //! Pure virtual function for get grid settings
    virtual const Grid& grid() const = 0;
};


class Accessor
{
public:
    /** Retrieve reference to this singleton */
    static Accessor& instance(IAccessor *stg = 0);

    //! Gets the color for every element
    QColor color(ColorType typeColor) const;
    //! Gets the font for every element
    virtual QFont font(FontType typeFont) const;
    //! Gets the grid settings
    const Grid& grid() const;

private:
    Accessor(IAccessor *stg);

    IAccessor   *mStg;
};

}   // namespace UserProfile

#endif // _COLOR_USER_PROFILE_SETTINGS_H__
