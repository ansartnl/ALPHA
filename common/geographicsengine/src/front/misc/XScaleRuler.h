#ifndef _X_SCALE_RULER_H__
#define _X_SCALE_RULER_H__

#include <QPainter>


//! Draw scale ruler in screen coordinates
class CXScaleRuler
{
public:
    //! Measure of distance (units).
    enum Units
    {
        Kilometers,
        NauticalMiles
    };

    //! Constructor.
    CXScaleRuler()
        : m_units(Kilometers), m_intervalDist(50.0), m_intervalNumber(6),
        m_isVisible(false)
    {}

    //! Screen painter.
    void paint(QPainter& painter, const QRect& rect);

    //! Set distance of one interval.
    void SetIntervalDistance(qreal dist)
    { m_intervalDist = dist; }

    //! Set length of an interval ruler in pixels.
    void SetIntervalLength(int lengthPixel)
    { m_intervalLength = lengthPixel; }

    //! Set number of ruler intervals.
    void SetIntervalNumber(int count)
    { m_intervalNumber = count; }

    //! Set units for measure the distance.
    void SetUnits(Units units)
    { m_units = units; }

    //! Set new color.
    void SetColor(const QColor& color)
    { m_color = color; }

    //! Set ruler visibility.
    void SetVisible(bool isVisible)
    { m_isVisible = isVisible; }

private:
    //! Make string from double
    QString formatLabel(qreal value) const;

private:
    /** Measure of distance */
    Units   m_units;
    /** Length of one interval (distance) */
    qreal   m_intervalDist;
    /** Number of intervals */
    int     m_intervalNumber;
    /** Length of an interval in pixels */
    int     m_intervalLength;
    /** Ruler color */
    QColor  m_color;
    /** True if ruler is visible */
    bool    m_isVisible;
};

#endif // _X_SCALE_RULER_H__
