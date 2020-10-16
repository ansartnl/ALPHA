#ifndef TRACERHANDLER_H
#define TRACERHANDLER_H

#include <QPointF>
#include <QSharedPointer>

class CTracerPoint;
class CCoord;
class CPoint;

//! Interface to CTracerFPL
class ITracerHandler
{
public:
    //! Destructor
    virtual ~ITracerHandler(){}
    //! Delete point from route
    /*! \param pPoint a pointer to CTracerPoint */
    virtual void DeleteTracerPoint(CTracerPoint* pPoint) = 0;
    //! Check new position of point
    /*!
      \param sName a name of point
      \param coord a coordinate of point
    */
    virtual bool IsCanAddPoint(const QString& sName, const CCoord& coord, QSharedPointer<CPoint>& pPoint) = 0;
    //! Can delete the point
    /*!
      \param pPoint a pointer to CTracerPoint
    */
    virtual bool IsCanDeletePoint(CTracerPoint* pPoint) = 0;
    //! Update points
    virtual void UpdateTracer(CTracerPoint* pPoint = 0) = 0;
    //! Recalculate time of route
    virtual void RecalculateTracer(quint32 seconds = 0) = 0;
    //! Calculate coordinate at point
    virtual CCoord GetCoordAtPoint(const QPointF &pos) const = 0;

    //! ZValue
    static quint32    m_zValue;
};

#endif // TRACERHANDLER_H
