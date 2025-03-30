#pragma once
#include <chrono>

class DirectxTimer {
public:
    DirectxTimer();
    float mark();
    float peek() const;
private:
    std::chrono::steady_clock::time_point last_;
};
