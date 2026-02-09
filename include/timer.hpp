#pragma once
#include <chrono>

class Timer {
public:
    void start();
    void end();
    double time();

private:
    std::chrono::high_resolution_clock::time_point start_time, end_time;
};