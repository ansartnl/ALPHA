#ifndef _GUI_OBJECT_H__
#define _GUI_OBJECT_H__

#include <QGraphicsItem>
#include <QMap>
#include <QVariant>

#define M_INCH_MM       25.4


//! Geodesy graphics engine
namespace geoGE {

//! Base class for each CGUI* graphics objects.
class GUIObject
    : public QGraphicsItem
{
public:
    //! Set new item's color.
    virtual void SetColor(const QColor& color)
    { m_color = color; }

    //! Get item's color
    const QColor& GetColor() const
    { return m_color; }

    //! Add new or replace old property.
    void SetProperty(const QString& name, const QVariant& value);

    //! Retrieve property associated with a name.
    /*!
        \return Return property value or null variant otherwise
    */
    QVariant GetProperty(const QString& name) const;

protected:
    //! Constructor
    GUIObject(QGraphicsItem * parent = 0)
        : QGraphicsItem(parent), m_color(Qt::black)
    {}

private:
    typedef QMap<QString, QVariant>     TPropertyMap;

    QColor          m_color;    //!< Color of an item
    TPropertyMap    m_props;    //!< Properties associated with a name
};



//! CGUI* graphics object that does not support transformation.
class GUIUntransformableObject
    : public GUIObject
{
public:
    //! Set new aspect ratio
    /*! When drawing with different sizes of source and target rectangles
        this function should be called before painting them. This
        can happen for example when printing untransformable items that
        should paint with scene transformation.
    */
    void SetAspectRatio(qreal xratio, qreal yratio)
    {
        m_xratio = xratio;
        m_yratio = yratio;
    }

protected:
    //! Constructor
    GUIUntransformableObject(QGraphicsItem * parent = 0);

    //! Transform scene point to the point with respect to ratio coefficients.
    QPointF transformToRatio(const QPointF& pointScene) const;

    //! Returns true if any ratio is not 1.0.
    bool isTransformEnabled() const;

    /** Aspect ratio between source and target rectangles (X axis) */
    qreal   m_xratio;
    /** Aspect ratio between source and target rectangles (Y axis) */
    qreal   m_yratio;
};

}   // namespace geoGE

#endif // _GUI_OBJECT_H__
