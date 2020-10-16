#ifndef PLANE_H
#define PLANE_H

#include <QTime>
#include "Coordinate.h"
#include "AirplaneData.h"
#include "Point.h"
#include "Holding.h"
#include "SSA.h"
#include "front/core/airplane/AClist/SFPL.h"

class CScene;
namespace core
{
    class CWind;
}

//! Class describes the exercise plane
class CPlane
    : public CAirplaneData
{
public:
    //! Constructor
    /*!
        \param pSFPL pointer to exercise plane
        \param iBeforehandFly beforehand delay for execution
        \param bLevelOnPoint take in calculation height on points
    */
    CPlane(const QSharedPointer<CSFPL>& pSFPL, const CCoord& centralCoord,
           int iBeforehandFly, bool bLevelOnPoint);

    CPlane(const QSharedPointer<CSFPL>& pSFPL, const CCoord& centralCoord,
           int iBeforehandFly, bool bLevelOnPoint,
           const CCoord& coord, double flightLevel);

    //! Destructor
    virtual ~CPlane();

    //! Time of previous calculated position
    TVA_PROPERTIE(QTime, LastTime)
    //! Current azimuth
    TVA_PROPERTIE(double, AzimuthPlane)
    //! Azimuth taking into account wind
    TVA_PROPERTIE(double, WindAzimuth)
    //! Current IAS
    TVA_PROPERTIE(double, IAS)
    //! Current IAS + wind speeds
    TVA_PROPERTIE(double, WindTAS)
    //! Aim IAS
    TVA_PROPERTIE(double, AimIAS)
    //! Flag defines, whether plane parameters were changed
    TVA_PROPERTIE(bool, IsChanged)
    //! Flag defines, whether reminder parameters were changed
    TVA_PROPERTIE(bool, IsRemChanged)
    //! Reminder identifier
    TVA_PROPERTIE(quint32, ReminderID)
    //! Stop plane after STCA
    TVA_PROPERTIE(bool, PlanePause)
    //! Take into account the height at the points
    TVA_PROPERTIE(bool, LevelOnPoint)

    //! Calculate next point from fpl chain
    void NextPoint();

public:
    //! Set aimed flight level
    /*!
        \param dFL flight level
    */
    void SetAimFL(double dFL);
    //! Get aimed flight level
    /*!
        \return flight level
    */
    double GetAimFL() const;
    //! Set current bank
    /*!
        \param dBank bank
    */
    //! Set level on point to value from config
    void SetLevelOnPoint();
    void SetBank(double dBank);
    //! Get current bank
    /*!
        \return bank
    */
    double GetBank() const;
    //! Set aimed vertical speed
    /*!
        \param dVAimVelocity vertical speed
    */
    void SetVAimVelocity(double dVAimVelocity);
    //! Get aimed vertical speed
    /*!
        \return vertical speed
    */
    double GetVAimVelocity() const;
    //! Set aimed azimuth
    /*!
        \param dAzim azimuth
    */
    void SetAimAzimuth(double dAzim);
    //! Get aimed azimuth
    /*!
        \return aimed azimuth
    */
    double GetAimAzimuth() const;
    //! Define whether the plane has azimuth aim
    /*!
        \return true if the plane flies on azimuth
    */
    bool IsAimAzimuth() const;
    //! Set whether the plane has azimuth aim
    /*!
        \return bAzimuth whether the plane flies on azimuth
    */
    void MarkAimAzimuth(bool bAzimuth);
    //! Calculate next position of the plane
    /*!
        \param iPlaySpeed execution factor of exercise
    */
    bool CalculatePosition(int iPlaySpeed);
    //! Skip plane calculation
    void SkipTime();

    void SetWasAzimChangedInPause(bool flag);
    bool GetWasAzimChangedInPause();

    void SetWasFLChangedInPause(bool flag);
    bool GetWasFLChangedInPause();

    const QSharedPointer<CSFPL> &GetSFPL() const { return m_SFPL; }
    void SetSFPL(const QSharedPointer<CSFPL> &V) { m_SFPL = V; }

private:
    CPlane(const QSharedPointer<CSFPL>& pSFPL, const CCoord& centralCoord, bool bLevelOnPoint);

    void CalculateWind(const QSharedPointer<core::CWind>& pWind);
    bool _CheckPoint();
    QSharedPointer<CPointFrag> _GetNextPoint();
    double CalcAimVelocity(const QSharedPointer<CPoint>& pPoint, double dAimFL, bool bQuick);

    //////////////// for simulation
public:
    static int GetStepSeconds();

    int GetCurFPLPoint() const;
    void SetCurFPLPoint(int i);
private:
    int m_iCurFPLPoint;

    TVA_PROPERTIE(CCoord, CentralCoord)
    bool IsValidCurFPLPlan() const;
    bool IsValidCurFPLPoint() const;
    QSharedPointer<CPointFrag> GetSimRoutePoint(int i) const;
    void CopySFPL(const QSharedPointer<CSFPL>& pSFPL);
    CFPLFrag::TProcFragList::const_iterator m_itProc;
    CProcFrag::TPointFragList::const_iterator m_itPoint;
    ////////////////////////////////

    double              m_dAimFL;
    double              m_dBank;
    double              m_dVAimVelocity;
    double              m_AimAzimuth;
    bool                m_bAimAzimuth;
    bool                m_bAutoVz;
    bool                m_bAutoIAS;
    int                 m_iLevelN;
    bool                m_bLevelOnPoint;
    bool                m_bWasFLChangedInPause;
    bool                m_bWasAzimChangedInPause;
    bool                m_bIsSTAR;
    bool                m_bIsSID;
    QSharedPointer<CSFPL> m_SFPL;
};

#endif // PLANE_H
