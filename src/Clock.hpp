#pragma once

#include <chrono>

using namespace std::chrono;

namespace Core
{
    class Clock
    {
    public:
        void Start();
        double Delta();

        Clock();
        ~Clock();

    private:
        high_resolution_clock::time_point startTime;
    };
}