#ifndef VEHICLESTATE_H
#define VEHICLESTATE_H

//! Structure contains vehicle state
struct VehicleState
{
    VehicleState()
        :ID(0), name(), IAS(0), coord(), hidden(false)
    {}

    quint32     ID;
    QString     name;
    double      IAS;
    CCoord      coord;
    bool        hidden;
};

#endif // VEHICLESTATE_H
