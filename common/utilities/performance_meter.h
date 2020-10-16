#pragma once

#include <chrono>
#include <functional>

#include <QString>
#include <QDebug>

#include "utilities_global.h"


namespace utilities {


namespace performance_meter {


class UTILITIES_EXPORT Measured {
public:
    typedef std::chrono::duration<double, std::milli> DurationType;

    Measured(const QString& name) : name_(name), count_(0), average_(0) {}
    ~Measured();

    void tick();

protected:
    virtual DurationType measure() = 0;

private:
    Q_DISABLE_COPY(Measured)

    QString name_;
    qint64 count_;
    DurationType average_;
};


class UTILITIES_EXPORT MeasurePoint : public Measured {
public:
    MeasurePoint(const QString& name, int threshold)
        : Measured(name)
        , last_(std::chrono::high_resolution_clock::now())
        , threshold_(threshold)
    {}

    bool thresholdReached();

protected:
    virtual DurationType measure() override;

private:
    Q_DISABLE_COPY(MeasurePoint)

    std::chrono::time_point<std::chrono::high_resolution_clock> last_;
    int threshold_;
};


class UTILITIES_EXPORT MeasureBlock : public Measured {
public:
    typedef std::function<void()> BlockType;

    MeasureBlock(const QString& name)
        : Measured(name)
        , start_(std::chrono::high_resolution_clock::now())
        , end_(std::chrono::high_resolution_clock::now())
    {}

    void start();
    void end();

protected:
    virtual DurationType measure() override;

private:
    Q_DISABLE_COPY(MeasureBlock)

    std::chrono::time_point<std::chrono::high_resolution_clock> start_, end_;
};


} // namespace performance_meter


} // namespace utilities


#define MEASURE_POINT(pointName, threshold) \
    static utilities::performance_meter::MeasurePoint pointName(#pointName, threshold); \
    if (!pointName.thresholdReached()) \
        pointName.tick();

#define START_MEASURE_BLOCK(blockName) \
    static utilities::performance_meter::MeasureBlock blockName(#blockName); \
    blockName.start();
#define END_MEASURE_BLOCK(blockName) \
    blockName.end(); \
    blockName.tick();
