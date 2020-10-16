#ifndef _STEREO_COORD_SYS_H__
#define _STEREO_COORD_SYS_H__

#include <math.h>
#include <QtCore/QtGlobal>

#define RAD_TO_DEG	57.29577951308232
#define DEG_TO_RAD	.0174532925199432958

//! Conversion from geodesic coordinates to plane coordinates.
/*!
    \param lat-latitude, lng-longitude in degree
    \param lat0-center point latitude, lng0-center point longitude in degree
    \param zone Zone number
*/
inline void stereo_fwd(qreal lat, qreal lng, qreal lat0, qreal lng0,
                       qreal k0, qreal a, qreal e, qreal &x, qreal &y)
{
    qreal lambda, phi, lambda0, phi0;

    lambda = lng*DEG_TO_RAD;
    phi = lat*DEG_TO_RAD;
    lambda0 = lng0*DEG_TO_RAD;
    phi0 = lat0*DEG_TO_RAD;

    qreal s = 1.;
    if ( phi0 < 0. )  // phi0 is southern
    {
        lambda = -lambda;
        lambda0 = -lambda0;
        phi = -phi;
        phi0 = -phi0;
        s = -1.;
    }

    qreal sin_phi = sin(phi);
    qreal chi = 2.*atan(sqrt((1.+sin_phi)/(1.-sin_phi)*pow((1.-e*sin_phi)/(1.+e*sin_phi), e)))-0.5*M_PI;

    qreal sin_phi0 = sin(phi0);
    qreal chi0 = 2.*atan(sqrt((1.+sin_phi0)/(1.-sin_phi0)*pow((1.-e*sin_phi0)/(1.+e*sin_phi0), e)))-0.5*M_PI;

    qreal cos_chi = cos(chi);
    qreal sin_chi = sin(chi);
    qreal cos_chi0 = cos(chi0);
    qreal sin_chi0 = sin(chi0);
    qreal cos_lam_lam0 = cos(lambda-lambda0);
    qreal sin_lam_lam0 = sin(lambda-lambda0);

    qreal m0 = cos(phi0)/sqrt(1.-pow(e*sin_phi0, 2.0));
    if ( qAbs(phi0-0.5*M_PI) > 0.000001 )
    {
        qreal A = 2.0*a*k0*m0/(cos_chi0*(1.+sin_chi0*sin_chi+cos_chi0*cos_chi*cos_lam_lam0));
        x = s*A*cos_chi*sin_lam_lam0;
        y = s*A*(cos_chi0*sin_chi-sin_chi0*cos_chi*cos_lam_lam0);
    }
    else
    {
        qreal t = sqrt((1.-sin_phi)/(1.+sin_phi)*pow((1.+e*sin_phi)/(1.-e*sin_phi), e));
        qreal rho = 2.*a*k0*t/sqrt(pow((1.+e),(1.+e))*pow((1.-e),(1.-e)));
        x = s*rho*sin_lam_lam0;
        y = -s*rho*cos_lam_lam0;
    }
}

qreal sign(qreal val)
{
    return ( val > 0. ? 1. : (val < 0. ? -1. : 0.) );
}

//! Conversion from plane coordinates to geodesic coordinates.
/*!
    \param lat-latitude, lng-longitude in degree
    \param zone Zone number
*/
inline void stereo_inv(qreal x, qreal y, qreal lat0, qreal lng0,
                       qreal k0, qreal a, qreal e, qreal &lat, qreal &lng)
{
    qreal e2=e*e;

    // Degrees to radians
    qreal lambda0 = lng0*DEG_TO_RAD;
    qreal phi0 = lat0*DEG_TO_RAD;

    qreal s = 1.;
    if ( phi0 < 0. )   // phi0 is southern
    {
        lambda0 = -lambda0;
        phi0 = -phi0;
        x = -x;
        y = -y;
        s = -1.;
    }

    qreal sin_phi0 = sin(phi0);
    qreal chi0 = 2.*atan(sqrt((1.+sin_phi0)/(1.-sin_phi0)*pow((1.-e*sin_phi0)/(1.+e*sin_phi0), e)))-0.5*M_PI;
    qreal cos_chi0 = cos(chi0);
    qreal sin_chi0 = sin(chi0);

    qreal m0 = cos(phi0)/sqrt(1.-pow(e*sin_phi0, 2.0));
    qreal rho = sqrt(x*x+y*y);

    qreal chi, lambda;

    if ( qAbs(phi0-0.5*M_PI) > 0.000001 )
    {
        if ( rho > a*0.000000001 )
        {
            qreal ce = 2.0*atan(rho*cos_chi0/(2.0*a*k0*m0));
            qreal cos_ce = cos(ce);
            qreal sin_ce = sin(ce);
            chi = asin(cos_ce*sin_chi0+y/rho*sin_ce*cos_chi0);
            lambda = lambda0+atan(x*sin_ce/(rho*cos_chi0*cos_ce-y*sin_chi0*sin_ce));
        }
        else
        {
            chi = chi0;
            lambda = lambda0;
        }
    }
    else
    {
        qreal t = rho*sqrt(pow((1.+e),(1.+e))*pow((1.-e),(1.-e)))/(2.*a*k0);
        chi = 0.5*M_PI-2.0*atan(t);
        lambda = lambda0+atan2(x,-y);
        if ( !( (-M_PI <= lambda) && (lambda<=M_PI) ) )
            lambda = lambda - 2.0*M_PI*sign(lambda); // sign
    }

    qreal phi = chi
        +e2*((((13./360.*e2+1./12.)*e2+5./24.)*e2+1./2.)*sin(2.*chi)
        +e2*((pow((811./11520.*e2+29./240.),e2)+7./48.)*e2*sin(4.*chi)
        +e2*((81./1120.*e2+7./120.)*e2*sin(6.*chi)
        +e2*(4279./161280.)*sin(8.*chi))));

    lng = s*lambda*RAD_TO_DEG;
    lat = s*phi*RAD_TO_DEG;
}

#endif // _STEREO_COORD_SYS_H__
