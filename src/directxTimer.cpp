#include "directxTimer.h"
#include <chrono>

using namespace std::chrono;

DirectxTimer::DirectxTimer()
    : last_ {steady_clock::now()}
{}

float DirectxTimer::mark() {
    const auto old {last_};
    last_ = steady_clock::now();
    const duration<float> frameTime {last_ - old};
    return frameTime.count();
}

float DirectxTimer::peek() const {
    return duration<float>(steady_clock::now() - last_).count();
}
