#include "interval.h"

#include <algorithm>
#include <cassert>

#include "constants.h"


Interval::Interval() : Interval(Interval::empty) { }; // Default interval is empty


Interval::Interval(float _min, float _max) : min(_min), max(_max) {}

// Returns the length of the interval.
float Interval::size() const {
    return max - min;
}

// Test if the interval contains `x`.
bool Interval::contains(float x) const {
    return min <= x && x <= max;
}

// Test if the interval strictly contains `x`.
bool Interval::surrounds(float x) const {
    return min < x && x < max;
}

// Clamp the given value `x` into the interval.
float Interval::clamp(float x) const {
    assert(min <= max);
    return std::clamp(x, min, max);
}


const Interval Interval::empty    = Interval(+kInfinity, -kInfinity);
const Interval Interval::universe = Interval(-kInfinity, +kInfinity);
const Interval Interval::positive = Interval(+kEpsilon , +kInfinity);
