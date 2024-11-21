#include "Clock.hpp"

namespace Core
{
    Clock::Clock()
    {
    }

    Clock::~Clock()
    {
    }

    void Clock::Start()
    {
        startTime = high_resolution_clock::now();
    }

    double Clock::Delta()
    {
        high_resolution_clock::time_point now = high_resolution_clock::now();
        return duration_cast<milliseconds>(now - startTime).count();
    }
}