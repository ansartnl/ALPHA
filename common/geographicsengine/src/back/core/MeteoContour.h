#ifndef _METEO_CONTOUR_H__
#define _METEO_CONTOUR_H__

#include <QPointF>
#include <QVector>
#include "cs/Coordinate.h"
#include "MacroDefs.h"


//! Class defines meteo zone
class MeteoContour
{
public:
    /** Array of points: x - latitude, y - longitude (degree) */
    typedef QVector<CCoord>     TPoints;

    //! Constructor
    MeteoContour()
        : m_ID(0), m_Intencity(0)
    {}
    //! Destructor
    virtual ~MeteoContour() {}

    //! Add one more vector to the list
    void AddPoint(const CCoord& point)
    { m_Contour.push_back(point); }

    //! Clear list of meteo vectors
    void ClearContour()
    { m_Contour.clear(); }

    //! Retrieve modifiable list of contour points
    TPoints& GetContour()
    { return m_Contour; }

    //! Meteo vector identifier (locator ID)
    GEO_PROPERTY(quint16, ID)

    //! Intencity of vectors
    GEO_PROPERTY(qint16, Intencity)

    //! List of meteo vectors
    GEO_PROPERTY(TPoints, Contour)
};


#endif // _METEO_CONTOUR_H__
