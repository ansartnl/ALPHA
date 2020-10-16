#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QtCore/qglobal.h>

class CCoord;

namespace gui
{
    enum Z_order
    {
        Zscene = 0,
        ZRadioRestriction = 1,
        Zsector = 2,
        Zbound = 3,
        Zroute = 9,
        Zholding = 10,
        Zmeteo = 11,
        Zrestriction = 12,
        Zuserrestriction = 13,
        Zapron = 14,
        Zpath = 15,
        Zcrossroads = 16,
        Zrunway = 17,
        Zbuilding = 18,
        Ztrace = 19,
        Zpoint = 20,
        Zairport = 30,
        Zssa = 30,
        Zstart = 31,
        ZSign = 32,
        Zparking = 33,
        ZwaypointMarker,
        Ztower,
        Zsectorborder,
        Zrcgrid,
        Zgauge,
        Zairplane,
        Ztracerfpl,
        Ztracerhdg,
        Zairplanepoint = 50,
        Zairplanelabel = 60,
        Zlabelitem = 70,
        Zactiveairplane = 100,
        Zdigitizer = 100,
        Zbearing = 100,
        Ztracerpoint = 110,
        Zmtcdpoint = 120,
        Zstca = 10001,
        Zcircles = 10002
    };
}

//! Interface to graphics objects
class IGUIItemHandler
{
public:
    virtual ~IGUIItemHandler(){};
    //! Set graphics item new position
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord) = 0;
};

#endif // GUIINTERFACE_H
