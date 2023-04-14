#ifndef UTILS_H
#define UTILS_H

#include <random>

#include "common/constants.hh"
// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

template <typename T>
T random_number(T min, T max) {
    // Initializing random distribution generator each call is expensive.
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<T> distribution(min, max);
    return distribution(gen);
}

template <typename T>
T clamp(T x, T min, T max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif