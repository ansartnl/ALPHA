#ifndef _TIME_SCALE_ENGINE_H_
#define _TIME_SCALE_ENGINE_H_ 1

#include "timedate.h"
#include <qwt_scale_engine.h>

class TimeScaleEngine: public QwtLinearScaleEngine
{
public:
    TimeScaleEngine();
    virtual ~TimeScaleEngine();

    void setMaxWeeks( int );
    int maxWeeks() const;

    virtual void autoScale( int maxNumSteps,
        double &x1, double &x2, double &stepSize ) const;

    virtual QwtScaleDiv divideScale( double x1, double x2,
        int maxMajSteps, int maxMinSteps,
        double stepSize = 0.0 ) const;

    virtual QwtTransform *transformation() const;

    virtual TimeDate::IntervalType intervalType( 
        double x1, double x2, int maxSteps ) const;

private:
    QwtScaleDiv divideToSeconds( double x1, double x2,
        int maxMajSteps, int maxMinSteps, double stepSize ) const;

    QwtScaleDiv divideToMinutes( double x1, double x2,
        int maxMajSteps, int maxMinSteps, double stepSize ) const;

    QwtScaleDiv divideToHours( double x1, double x2,
        int maxMajSteps, int maxMinSteps, double stepSize ) const;

    QwtScaleDiv divideToDays( double x1, double x2,
        int maxMajSteps, int maxMinSteps, double stepSize ) const;

    QwtScaleDiv divideToWeeks( double x1, double x2,
        int maxMajSteps, int maxMinSteps, double stepSize ) const;

    QwtScaleDiv divideToMonths( double x1, double x2,
        int maxMajSteps, int maxMinSteps, double stepSize ) const;

    QwtScaleDiv divideToYears( double x1, double x2,
        int maxMajSteps, int maxMinSteps, double stepSize ) const;

private:
    int d_maxWeeks;
};

#endif
