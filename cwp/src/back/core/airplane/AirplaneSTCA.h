#ifndef AIRPLANESTCA_H
#define AIRPLANESTCA_H

#include "AirplaneCollision.h"
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>

//! Namespace contains the airplane's STCA
namespace stca
{
    struct Stca : public AirplaneCollision
    {
        static const int INITCOUNTER = 7;
        int mCounter;
        Stca()
            : AirplaneCollision(), mCounter(INITCOUNTER) {}
        Stca(const AirplaneCollision& ac)
            : AirplaneCollision(ac), mCounter(INITCOUNTER) {}
    };

    class CMapSTCA
    {
    public:
        CMapSTCA() {}
        AirplaneCollision::Type GetSTCAType() const;
        void appendSTCA(int idPlane, QSharedPointer<Stca> pSTCA);
        QSharedPointer<Stca> getSTCA(int idPlane) const;
    private:
        typedef QMap<int, QWeakPointer<Stca> > TMapSTCA;
        TMapSTCA m_map;
    };

    class CListSTCA
    {
    public:
        CListSTCA() {}
        bool addSTCA(const AirplaneCollision& ac);
        CMapSTCA mapSTCA(int idPlane) const;
        void decreaseSTCA(int idPlane);
        void deleteSTCA(int idPlane);
    private:
        typedef QList<QSharedPointer<Stca> > TListSTCA;
        TListSTCA m_list;
    };
}

#endif // AIRPLANESTCA_H
