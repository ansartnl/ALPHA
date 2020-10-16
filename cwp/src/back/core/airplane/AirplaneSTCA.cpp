#include "AirplaneSTCA.h"

namespace stca
{
// CMapSTCA
AirplaneCollision::Type CMapSTCA::GetSTCAType() const
{
    AirplaneCollision::Type Res = AirplaneCollision::NoCollision;
    foreach(const QWeakPointer<Stca>& itMap, m_map)
    {
        QSharedPointer<Stca> pSTCA = itMap.toStrongRef();
        if(pSTCA)
        {
            if(pSTCA->type < Res)
                Res = pSTCA->type;
        }
    }
    return Res;
}

void CMapSTCA::appendSTCA(int idPlane, QSharedPointer<Stca> pSTCA)
{
    m_map.insert(idPlane, pSTCA.toWeakRef());
}

QSharedPointer<Stca> CMapSTCA::getSTCA(int idPlane) const
{
    QSharedPointer<Stca> pSTCA;
    QWeakPointer<Stca> pWeakSTCA = m_map.value(idPlane);
    if(pWeakSTCA)
    {
        pSTCA = pWeakSTCA.toStrongRef();
    }
    return pSTCA;
}

// CListSTCA
bool CListSTCA::addSTCA(const AirplaneCollision& ac)
{
    AirplaneCollision airplaneCol(ac);
    airplaneCol.id1 = qMin(ac.id1, ac.id2);
    airplaneCol.id2 = qMax(ac.id1, ac.id2);
    QSharedPointer<Stca> pSTCA;
    foreach(const QSharedPointer<Stca>& itList, m_list)
    {
        if((itList->id1 == airplaneCol.id1) && (itList->id2 == airplaneCol.id2))
        {
            pSTCA = itList;
            break;
        }
    }
    if(pSTCA)
    {
        *pSTCA = airplaneCol;
        if(airplaneCol.type != AirplaneCollision::NoCollision)
            pSTCA->mCounter = Stca::INITCOUNTER;
        else
            pSTCA->mCounter = 0;
        return false;
    }
    else
    {
        if(airplaneCol.type != AirplaneCollision::NoCollision)
        {
            pSTCA = QSharedPointer<Stca>(new Stca(airplaneCol));
            m_list.append(pSTCA);
            return true;
        }
        return false;
    }
}

CMapSTCA CListSTCA::mapSTCA(int idPlane) const
{
    CMapSTCA map;
    foreach(const QSharedPointer<Stca>& itList, m_list)
    {
        if(itList->id1 == idPlane)
        {
            map.appendSTCA(itList->id2, itList);
        }
        else if(itList->id2 == idPlane)
        {
            map.appendSTCA(itList->id1, itList);
        }
    }
    return map;
}

void CListSTCA::decreaseSTCA(int idPlane)
{
    TListSTCA::iterator itList = m_list.begin();
    while(itList != m_list.end())
    {
        if(*itList)
        {
            if(((*itList)->id1 == idPlane) || ((*itList)->id2 == idPlane))
            {
                if((*itList)->mCounter)
                {
                    --(*itList)->mCounter;
                }
                if((*itList)->mCounter)
                {
                    ++itList;
                }
                else
                {
                    itList = m_list.erase(itList);
                }
            }
            else
            {
                if((*itList)->mCounter)
                {
                    ++itList;
                }
                else
                {
                    itList = m_list.erase(itList);
                }
            }
        }
        else
        {
            itList = m_list.erase(itList);
        }
    }
}

void CListSTCA::deleteSTCA(int idPlane)
{
    TListSTCA::iterator itList = m_list.begin();
    while(itList != m_list.end())
    {
        if(*itList)
        {
            if(((*itList)->id1 == idPlane) || ((*itList)->id2 == idPlane))
            {
                itList = m_list.erase(itList);
            }
            else
            {
                ++itList;
            }
        }
        else
        {
            itList = m_list.erase(itList);
        }
    }
}

}
