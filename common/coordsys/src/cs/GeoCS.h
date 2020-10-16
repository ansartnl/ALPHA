#ifndef _GEO_COORD_SYS_H__
#define _GEO_COORD_SYS_H__

#include <math.h>

const double AREF = 6378137;				//WGS-84
const double FREF = 1/298.257223563;
const double BREF = AREF*(1-FREF);
const double esq = (2-FREF)*FREF;
const double oneesq = 1-esq;
const double onef =1-FREF;
const double tol = 1e-10;
const double MM_PI = 3.1415926535897932384626433832795;

struct coordinates
{
    double x_el_phi, y_az_lambda, z_h;
};

//pz90.x_el_phi - x
//pz90.y_az_lambda - y
//pz90.z_h - z
//geo.x_el_phi - latitude
//geo.y_az_lambda - longitude
//geo.z_h - H

inline coordinates pz2geo(coordinates pz90)
{
    coordinates geo;

    double R = sqrt(pz90.x_el_phi*pz90.x_el_phi+pz90.y_az_lambda*pz90.y_az_lambda);
    if(R>tol)
        geo.y_az_lambda = atan2(pz90.y_az_lambda,pz90.x_el_phi);
    else
        geo.y_az_lambda = 0;

    if(geo.y_az_lambda < 0)
    {
        geo.y_az_lambda = geo.y_az_lambda + 2*MM_PI;
    }

    double t_new, RA, ZA, r, rA;
    double del_t = 1;
    double t=R/(pz90.z_h*sqrt(oneesq)); // First guess

    while(del_t>tol)
    {
        t_new=t-((pz90.z_h*sqrt(oneesq))*t+(AREF*esq)*t/sqrt(1+t*t)-R)/((pz90.z_h*sqrt(oneesq))+(AREF*esq)/pow((1+t*t),1.5));
        del_t = qAbs(t_new-t);
        t=t_new;
    }
    geo.x_el_phi=atan(1/(t*onef));

    RA=AREF*t/sqrt(1+t*t);
    ZA=AREF*onef/sqrt(1+t*t);

    r = sqrt(R*R+pz90.z_h*pz90.z_h);
    rA = sqrt(RA*RA+ZA*ZA);

    if((r-rA)<0)
    {
        geo.z_h = -sqrt((R-RA)*(R-RA)+(pz90.z_h-ZA)*(pz90.z_h-ZA));
    }
    else if((r-rA)>0)
    {
        geo.z_h = sqrt((R-RA)*(R-RA)+(pz90.z_h - ZA)*(pz90.z_h - ZA));
    }
    else
    {
        geo.z_h = 0;
    }
    return geo;

}

inline coordinates geo2pz(coordinates geo)
{
    coordinates pz90;

    pz90.z_h = ((AREF/sqrt(1-esq*sin(geo.x_el_phi)*sin(geo.x_el_phi)))*oneesq + geo.z_h)*sin(geo.x_el_phi);
    pz90.x_el_phi = (((AREF/sqrt(1-esq*sin(geo.x_el_phi)*sin(geo.x_el_phi))) + geo.z_h)*cos(geo.x_el_phi))*cos(geo.y_az_lambda);
    pz90.y_az_lambda = (((AREF/sqrt(1-esq*sin(geo.x_el_phi)*sin(geo.x_el_phi))) + geo.z_h)*cos(geo.x_el_phi))*sin(geo.y_az_lambda);

    return pz90;
}

inline double InverseProblem(coordinates geo1, coordinates geo2)
{
    double b1 = geo1.y_az_lambda*MM_PI/180;
    double l1 = geo1.x_el_phi*MM_PI/180;
    double b2 = geo2.y_az_lambda*MM_PI/180;
    double l2 = geo2.x_el_phi*MM_PI/180;
    const double a = AREF + geo1.z_h;
    const double f = FREF;

    double ex2,b,t2,eta2,V2,V4,N1,db;
    double ss,br,dl,deltaA,sc;
    double R;

    ex2 = (2-f)*f/((1-f)*(1-f));
    b = (b1+b2)/2;
    t2 = pow(sin(b)/cos(b),2);
    eta2 = ex2*pow(cos(b),2);
    V2 = 1+ex2*pow(cos(b),2);
    V4 = V2*V2;
    N1 = a*sqrt((1+ex2)/V2);
    db = b2-b1;
    b2 = db*db;
    br = db/V2;
    dl = l2-l1;

    ss = N1*dl*cos(b)*(1-pow(dl*sin(b),2)/24	  +(1+eta2-9*eta2*t2)*b2/(24*V4));
    sc = N1*br*cos(dl/2)*(1+(1-2*eta2)*pow(dl*cos(b),2)/24		 +eta2*(1-t2)*br*br/(24*V4));
    deltaA = dl*sin(b)*(1+(1+eta2)*pow(dl*cos(b),2)/12		+(3+8*eta2)*b2/(24*V4));
    R = (int)sqrt(ss*ss+sc*sc);

    return R;
}


#endif // _GEO_COORD_SYS_H__
