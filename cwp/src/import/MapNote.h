#ifndef MAPNOTE_H
#define MAPNOTE_H

#include "Coordinate.h"
#include "ItemHandler.h"

class MapNote
    : public ItemHandler
{
public:
    MapNote()
        :mID(0)
    {}

    inline void setID(quint32 id)
    {mID = id;}

    inline quint32 id() const
    {return mID;}

    inline void setCoord(const CCoord& coord)
    {mCoord = coord;}

    inline CCoord& coord()
    {return mCoord;}

    inline void setText(const QString& text)
    {mText = text;}

    inline const QString& text() const
    {return mText;}

private:
    quint32 mID;
    CCoord  mCoord;
    QString mText;
};

#endif // MAPNOTE_H
