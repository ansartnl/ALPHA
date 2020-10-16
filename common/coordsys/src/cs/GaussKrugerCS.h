#ifndef _GAUSS_KRUGER_COORD_SYS_H__
#define _GAUSS_KRUGER_COORD_SYS_H__

#include "coordsys_global.h"
#include <math.h>

const double MM_PI = 3.1415926535897932384626433832795;

//! Conversion from geodesic coordinates to plane coordinates.
/*!
    \param B-latitude, L-longitude in degree
    \param zone Zone number
*/
inline void geo2xy(qreal B, qreal L, qreal &X, qreal &Y, qint32 zone = -1)
{
    B *= MM_PI/180.0;   // conversion to radians

    qreal n = zone >= 0 ? zone : qint32((6.0+L)/6.0);
    qreal l = (L - (3.0+6.0*(n-1.0))) / 57.29577951;
    qreal l2 = l*l;

    qreal sinB = sin(B);
    qreal sin2B = sinB*sinB;
    qreal sin4B = sin2B*sin2B;
    qreal sin6B = sin4B*sin2B;

    X = 6367558.4968*B - sin(2.0*B)*(16002.89 + 66.9607*sin2B + 0.3515*sin4B -
        l2*(1594561.25 + 5336.535*sin2B + 26.790*sin4B + 0.149*sin6B +
        l2*(672483.4 - 811219.9*sin2B + 5420.0*sin4B - 10.6*sin6B +
        l2*(278194.0 - 830174.0*sin2B + 572434.0*sin4B - 16010.0*sin6B +
        l2*(109500.0 - 574700.0*sin2B + 863700.0*sin4B - 398600.0*sin6B)))));

    Y = (5.0 + 10.0*n)*100000.0 + l*cos(B)*(6378245.0 + 21346.1415*sin2B + 107.1590*sin4B + 0.5977*sin6B +
        l2*(1070204.16 - 2136826.66*sin2B + 17.98*sin4B - 11.99*sin6B +
        l2*(270806.0 - 1523417.0*sin2B + 1327645.0*sin4B - 21701.0*sin6B +
        l2*(79690.0 - 866190.0*sin2B + 1730360.0*sin4B - 945460.0*sin6B))));
}

//! Conversion from plane coordinates to geodesic coordinates.
/*!
    \param B-latitude, L-longitude in radians
    \param zone Zone number
*/
inline void xy2geo(qreal X, qreal Y, qreal &B, qreal &L, qint32 zone = -1)
{
    qreal n = zone >= 0 ? zone : qint32(Y / 1000000.0);
    qreal betta = X / 6367558.4968;

    qreal sinBetta, sin2Betta, sin4Betta;
    sinBetta = sin(betta);
    sin2Betta = sinBetta*sinBetta;
    sin4Betta = sin2Betta*sin2Betta;

    qreal Bo, z0, z02, dB, l;

    Bo = betta + sin(2.0*betta) * (0.00252588685 - 0.0000149186*sin2Betta + 
        0.00000011904*sin4Betta);
    z0 = (Y - (10.0*n +5.0)*100000.0) / (6378245.0*cos(Bo));
    z02 = z0*z0;

    qreal sinBo, sin2Bo, sin4Bo, sin6Bo;
    sinBo = sin(Bo);
    sin2Bo = sinBo*sinBo;
    sin4Bo = sin2Bo*sin2Bo;
    sin6Bo = sin4Bo*sin2Bo;

    dB = -z02*sin(2.0*Bo)*(0.251684631 - 0.003369263*sin2Bo + 0.000011276*sin4Bo -
        z02*(0.10500614 - 0.04559916*sin2Bo + 0.00228901*sin4Bo - 0.00002987*sin6Bo - 
        z02*(0.042858 - 0.025318*sin2Bo + 0.014346*sin4Bo - 0.001264*sin6Bo - 
        z02*(0.01672 - 0.0063*sin2Bo + 0.01188*sin4Bo - 0.00328*sin6Bo))));

    l = z0*(1.0 - 0.0033467108*sin2Bo - 0.0000056002*sin4Bo - 0.0000000187*sin6Bo - 
        z02*(0.16778975 + 0.16273586*sin2Bo - 0.00052490*sin4Bo - 0.00000846*sin6Bo - 
        z02*(0.0420025 + 0.1487407*sin2Bo + 0.005942*sin4Bo - 0.000015*sin6Bo - 
        z02*(0.01225 + 0.09477*sin2Bo + 0.03282*sin4Bo - 0.00034*sin6Bo - 
        z02*(0.0038 + 0.0524*sin2Bo + 0.0482*sin4Bo + 0.0032*sin6Bo)))));

    B = Bo + dB;
    L = 6.0*(n-0.5)/57.29577951 + l;
}

#endif // _GAUSS_KRUGER_COORD_SYS_H__
