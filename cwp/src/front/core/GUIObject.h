#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include <QGraphicsItem>
#include <QtCore/QMap>
#include <QVariant>


//! Base class for each CGUI* graphics objects.
class CGUIObject
    : public QGraphicsItem
{
public:
    //! Set new item's color.
    virtual void SetColor(const QColor& color)
    { m_color = color; }

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
    CGUIObject(QGraphicsItem * parent = 0)
        : QGraphicsItem(parent), m_color(Qt::black)
    {}

private:
    typedef QMap<QString, QVariant>     TPropertyMap;

    QColor          m_color;    //!< Color of an item
    TPropertyMap    m_props;    //!< Properties associated with a name
};

#endif // GUIOBJECT_H
