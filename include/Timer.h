#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
    using TimePoint = std::chrono::high_resolution_clock::time_point;

private:
    float dt = 1.0 / 60.0;
    TimePoint last = std::chrono::high_resolution_clock::now();

public:
    void Tick() noexcept;

    [[nodiscard]] float GetDeltaTime() const noexcept;
    [[nodiscard]] float GetUnscaleDeltaTime() const noexcept;

    float TimeScale = 1.0;
};

#endif
