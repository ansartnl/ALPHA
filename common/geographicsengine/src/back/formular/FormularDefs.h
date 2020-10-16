#ifndef FORMULAR_DEFINITION_H
#define FORMULAR_DEFINITION_H

#include <QtCore/QMap>
#include <QtCore/QRectF>

namespace FormularSeparation
{

//! Formular parameters.
struct FormularInfo
{
    //! Default constructor
    FormularInfo()
    {}
    //! Constructor
    FormularInfo(const QPointF &pos, const QRectF &rect)
        : mPos(pos), mRect(rect)
    {}

    //! Returns true if the rectangle is a null rectangle, otherwise returns false.
    bool isNull() const
    { return mRect.isNull(); }

    /** Airplane position */
    QPointF mPos;
    /** Formular rectangle */
    QRectF  mRect;
};

typedef QMap<quint16, FormularInfo>     FormularMap;

}

#endif // FORMULAR_DEFINITION_H
