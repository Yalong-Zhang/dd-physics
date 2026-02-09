#include "timer.hpp"

void Timer::start() {
    start_time = std::chrono::high_resolution_clock::now();
}

void Timer::end() {
    end_time = std::chrono::high_resolution_clock::now();
}

double Timer::time() {
    return std::chrono::duration<double>(end_time - start_time).count();
}
