#ifndef GROUNDVEHICLE_H
#define GROUNDVEHICLE_H

#include "GroundMain.h"
#include "back/core/airplane/AirplaneData.h"
//#include "import/Point.h"

class CSimulator;
//! Base class for ground vehicle control
class GroundVehicle
        :  public QObject, public CAirplaneData
{
    Q_OBJECT
public:
    GroundVehicle();

    virtual ~GroundVehicle(){}

#ifndef GROUND_CWP
    //! Get pointer to simulator
    /*!
        \return pointer to simulator
    */
    CSimulator* GetSimulator();
    //! Set pointer to simulator
    /*!
        \param pSimulator pointer to simulator
    */
    void SetSimulator(CSimulator* pSimulator);

    //! Change IAS
    /*!
        \param dV IAS
    */
    void ChangeV(double dV);            /*!< m/sec */

    //! Fly on custom point
    /*!
        \param pPoint pointer to custom point
    */

    void ChangeNextPoint(const QSharedPointer<atd::CPoint>& pPoint);

    void makeMove();
    void makeStop();
    void makeReverse();
#else
    //void ChangeNextPoint(const QSharedPointer<CPoint>& pPoint);
    inline void setPathCoords(QList<QSharedPointer<CPoint> >& coords)
    {
        mPathCoords.clear();
        mPathCoords.append(coords);
    }

    inline const QList<QSharedPointer<CPoint> >& pathCoords() const
    {return mPathCoords;}
#endif

    inline const QString& name() const
    {return mName;}

    inline void setName(const QString& name)
    {mName = name;}

    inline bool isChanged() const
    {return mIsChanged;}
    inline void setIsChanged(bool isChanged)
    {mIsChanged = isChanged;}

    inline bool isHidden() const
    {return mIsHidden;}
    inline void setIsHidden(bool isHidden)
    {mIsHidden = isHidden;}

private:
    bool        mIsChanged;
    QString         mName;
    bool        mIsHidden;

#ifndef GROUND_CWP
    CSimulator*                mSimulator;
    double      mCacheIAS;    
#else
    QList<QSharedPointer<CPoint> > mPathCoords;
#endif

};

#endif // GROUNDVEHICLE_H
