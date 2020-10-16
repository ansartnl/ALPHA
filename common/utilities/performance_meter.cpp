#include "performance_meter.h"


namespace utilities {


namespace performance_meter {


using namespace std::chrono;


Measured::~Measured() {
    qDebug() << "*** " << name_ << " ***";
    qDebug() << "average time: " << int(average_.count()) << " milliseconds";
    qDebug() << "hit count: " << count_;
}

void Measured::tick() {
    average_ = average_ * (double(count_) / double(count_ + 1)) + measure() / double(count_ + 1);
    ++count_;
}

Measured::DurationType MeasurePoint::measure() {
    auto now = high_resolution_clock::now();
    auto new_duration = now - last_;
    last_ = std::move(now);
    return std::move(new_duration);
}

bool MeasurePoint::thresholdReached() {
    auto now = high_resolution_clock::now();
    int difference = int(DurationType(now - last_).count());
    if (threshold_ <= difference) {
        last_ = std::move(now);
        return true;
    } else {
        return false;
    }
}

Measured::DurationType MeasureBlock::measure() {
    return std::move(end_ - start_);
}

void MeasureBlock::start() {
    start_ = high_resolution_clock::now();
}

void MeasureBlock::end() {
    end_ = high_resolution_clock::now();
}


} // namespace performance_meter


} // namespace utilities
