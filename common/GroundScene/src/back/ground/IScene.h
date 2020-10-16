#ifndef ISCENE_H
#define ISCENE_H

#include "back/core/Airplane.h"
#include "common/AirplaneState.h"
#include "GroundMain.h"

//! Interface for handling scenes (firs or ground)
class IScene
{
public:

    typedef QHash<quint16, QSharedPointer<CAirplane> > TAirplanesMap;

    virtual ~IScene(){}
    virtual void DelAirplane(QSharedPointer<CAirplane> pAirplane,
                             bool bSend = true) = 0;
    virtual void UpdateAirplane(CAirplaneState& AirplaneState) = 0;
    virtual TAirplanesMap& GetAirplanes() = 0;
    virtual QSharedPointer<CAirplane> CreateAirplane() const = 0;

    virtual void SetFormularDefaultPos() = 0;
    virtual void setFormularsPos(qreal /*angle*/, int /*length*/) {}

#ifdef GROUND_CWP
    virtual const QSharedPointer<CPoint>& GetCentralPoint() const = 0;
#else
    virtual const QSharedPointer<atd::CPoint>& GetCentralPoint() const = 0;
#endif

    virtual void BearingData(CBearingData& BearingData) = 0;
    virtual void SetSTCA(AirplaneCollision& ac) = 0;
};

#endif // ISCENE_H
