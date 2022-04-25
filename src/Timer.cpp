#include "Timer.h"

void Timer::Tick() noexcept
{
    auto ts = std::chrono::high_resolution_clock::now();

    dt = (std::chrono::duration_cast<std::chrono::nanoseconds>(ts - last).count() / 1000000000.0f);

    last = ts;
}

float Timer::GetDeltaTime() const noexcept
{
    return dt * TimeScale;
}

float Timer::GetUnscaleDeltaTime() const noexcept
{
    return dt;
}
