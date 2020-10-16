#include "GUIObject.h"


//! Geodesy graphics engine
namespace geoGE {

///////////////////////////////////////////////////////////////////////////////////////
// GUIObject implementation

void GUIObject::SetProperty(const QString& name, const QVariant& value)
{
    TPropertyMap::iterator it = m_props.find(name);
    if ( it == m_props.end() )
        m_props[name] = value;
    else
        it.value() = value;
}

QVariant GUIObject::GetProperty(const QString& name) const
{
    TPropertyMap::const_iterator itc = m_props.find(name);
    if ( itc != m_props.end() )
        return itc.value();
    return QVariant();
}


///////////////////////////////////////////////////////////////////////////////////////
// GUIObject implementation

//! Constructor
GUIUntransformableObject::GUIUntransformableObject(QGraphicsItem * parent)
    : GUIObject(parent), m_xratio(1.0), m_yratio(1.0)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

QPointF GUIUntransformableObject::transformToRatio(const QPointF& pointScene) const
{
    return QPointF(pointScene.x()*m_xratio,
                   pointScene.y()*m_yratio);
}

bool GUIUntransformableObject::isTransformEnabled() const
{
    return ( !qFuzzyCompare(m_xratio, 1.0) || !qFuzzyCompare(m_yratio, 1.0) );
}

}   // namespace geoGE
