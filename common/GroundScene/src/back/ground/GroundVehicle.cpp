#include "GroundVehicle.h"

#ifndef GROUND_CWP
#include "Simulator.h"
#include "proto/obelix/CmdVehicleControl.h"
#endif

GroundVehicle::GroundVehicle()
    : CAirplaneData(),
      mIsChanged(false)
      ,mIsHidden(false)
#ifndef GROUND_CWP
      , mSimulator(0)
      , mCacheIAS(8)
#endif
{
}

#ifndef GROUND_CWP
CSimulator *GroundVehicle::GetSimulator()
{
    return mSimulator;
}

void GroundVehicle::ChangeV(double dV)
{
    mCacheIAS = dV;
    QSharedPointer<obelix::CCmdVehicleControl> pCmd(new obelix::CCmdVehicleControl(GetID()));
    pCmd->SetIAS(dV);
    pCmd->SetSlowDown(true);
    mSimulator->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
}

void GroundVehicle::ChangeNextPoint(const QSharedPointer<atd::CPoint> &pPoint)
{
    QSharedPointer<obelix::CCmdVehicleControl> pCmd(new obelix::CCmdVehicleControl(GetID()));
    pCmd->SetPoint(pPoint);
    pCmd->SetIAS(0);
    mSimulator->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
}


void GroundVehicle::makeMove()
{
    QSharedPointer<obelix::CCmdVehicleControl> pCmd(new obelix::CCmdVehicleControl(GetID()));
    pCmd->SetIAS(mCacheIAS);
    mSimulator->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
}

void GroundVehicle::makeStop()
{
    QSharedPointer<obelix::CCmdVehicleControl> pCmd(new obelix::CCmdVehicleControl(GetID()));
    pCmd->SetIAS(0);
    pCmd->SetSlowDown(true);
    mSimulator->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
}

void GroundVehicle::makeReverse()
{
    QSharedPointer<obelix::CCmdVehicleControl> pCmd(new obelix::CCmdVehicleControl(GetID()));
    pCmd->SetReverse(true);
    pCmd->SetIAS(mCacheIAS);
    mSimulator->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
}

void GroundVehicle::SetSimulator(CSimulator *pSimulator)
{
    mSimulator = pSimulator;
}
#endif
