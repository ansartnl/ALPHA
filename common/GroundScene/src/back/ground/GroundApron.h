#ifndef GROUNDAPRON_H
#define GROUNDAPRON_H

#include <QList>
#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

class GroundApron
        : ItemHandler
{
public:
    GroundApron(){}

    //! Vertex
    inline const QList<CCoord>& vertex() const
    {return mVertex;}

    inline void centerVertex(int i, const CCoord& center)
    {
        if(i < mVertex.count())
        {
            mVertex[i].Center(center);
        }
    }

    inline void addVertex(CCoord& vertex)
    {mVertex.append(vertex);}

private:
    QList<CCoord> mVertex;
};

#endif // GROUNDAPRON_H
