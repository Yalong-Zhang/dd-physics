#pragma once
#include <random>

bool try_random(double p) {
    return rand() / (double)RAND_MAX < p;
}