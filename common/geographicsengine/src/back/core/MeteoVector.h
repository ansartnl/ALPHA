#ifndef _METEO_VECTOR_H__
#define _METEO_VECTOR_H__

#include <QPointF>
#include <QPair>
#include <QVector>
#include "cs/Coordinate.h"
#include "MacroDefs.h"


//! Class defines array of meteo vectors
class MeteoVector
{
public:
    /** QPointf: x - latitude, y - longitude (degree) */
    typedef QPair<CCoord, CCoord>     TVector;
    typedef QVector<TVector>          TVectors;

    MeteoVector()
        : m_ID(0), m_Intencity(0)
    {}
    virtual ~MeteoVector() {}

    //! Add one more vector to the list
    void AddVector(const TVector& vec)
    { m_Vectors.push_back(vec); }

    //! Clear list of vectors
    void ClearVectors()
    { m_Vectors.clear(); }

    //! Retrieve modifiable list of vectors
    TVectors& GetVectors() { return m_Vectors; }

    //! Meteo vector identifier (locator ID)
    GEO_PROPERTY(quint16, ID)

    //! Intencity of vectors
    GEO_PROPERTY(qint16, Intencity)

    //! List of meteo vectors
    GEO_PROPERTY(TVectors, Vectors)
};

#endif // _METEO_VECTOR_H__
