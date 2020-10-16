#include "GUIObject.h"


void CGUIObject::SetProperty(const QString& name, const QVariant& value)
{
    TPropertyMap::iterator it = m_props.find(name);
    if ( it == m_props.end() )
        m_props[name] = value;
    else
        it.value() = value;
}

QVariant CGUIObject::GetProperty(const QString& name) const
{
    TPropertyMap::const_iterator itc = m_props.find(name);
    if ( itc != m_props.end() )
        return itc.value();
    return QVariant();
}
