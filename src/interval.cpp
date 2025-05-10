#include "interval.h"


const static float kInfinity = std::numeric_limits<float>::infinity();


Interval::Interval() : min(+kInfinity), max(-kInfinity) {} // Default interval is empty


Interval::Interval(float _min, float _max) : min(_min), max(_max) {}


float Interval::size() const {
    return max - min;
}


bool Interval::contains(float x) const {
    return min <= x && x <= max;
}


bool Interval::surrounds(float x) const {
    return min < x && x < max;
}


float Interval::clamp(float x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


const Interval Interval::empty    = Interval(+kInfinity, -kInfinity);
const Interval Interval::universe = Interval(-kInfinity, +kInfinity);
