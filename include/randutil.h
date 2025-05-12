#ifndef RANDUTIL_H_
#define RANDUTIL_H_

#include <type_traits>
#include <random>

#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"

#include "constants.h"


// 1 to 4 dimensional vectors with float value type
template<typename T>
concept FloatVector =
    std::same_as<T, float> ||
    std::same_as<T, glm::vec1> ||
    std::same_as<T, glm::vec2> ||
    std::same_as<T, glm::vec3> ||
    std::same_as<T, glm::vec4>;


// Generate vectors with each component in range [-1, 1]
// inline is necessary, otherwise it will not compile because the functions will be strong symbols
template<FloatVector T>
inline T rand_unit_box() {
    return T(rand_unit_box<glm::vec<T::length() - 1, typename T::value_type>>(), rand_unit_box<float>());
}

template<>
inline float rand_unit_box<float>() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<float> dist(-1.0, 1.0);
    return dist(rng);
}

template<>
inline glm::vec1 rand_unit_box<glm::vec1>() {
    return glm::vec1(rand_unit_box<float>());
}


// Generate a random vector with length at most 1 (inside the unit sphere)
template<FloatVector T>
inline T rand_unit_sphere() {
    T result;
    do {
        result = rand_unit_box<T>();
    } while(glm::length(result) > 1.0f);
    return result;
}

template<>
inline float rand_unit_sphere<float>() {
    return rand_unit_box<float>();
}

template<>
inline glm::vec1 rand_unit_sphere<glm::vec1>() {
    return glm::vec1(rand_unit_sphere<float>());
}


// Generate a random vector with length 1 (on the unit sphere)
template<FloatVector T>
inline T rand_unit_length() {
    T result;
    typename T::value_type length;
    do {
        result = rand_unit_sphere<T>();
        length = glm::length(result);
    } while(glm::epsilonEqual(length, 0.0f, kEpsilon));
    return result / length;
}

template<>
inline float rand_unit_length<float>() {
    return rand_unit_box<float>() > 0.0f ? 1.0f : -1.0f;
}

template<>
inline glm::vec1 rand_unit_length<glm::vec1>() {
    return glm::vec1(rand_unit_length<float>());
}


// Generate a unit vector on a hemisphere (on the same side as the normal vector)
template<FloatVector T>
inline T rand_unit_hemisphere(const T &normal) {
    T result = rand_unit_length<T>();
    return glm::dot(normal, result) > 0.0f ? result : -result;
}

template<>
inline float rand_unit_hemisphere<float>(const float &normal) {
    return normal > 0.0f ? 1.0f : -1.0f;
}

template<>
inline glm::vec1 rand_unit_hemisphere<glm::vec1>(const glm::vec1 &normal) {
    return glm::vec1(rand_unit_hemisphere<float>(normal.x));
}


// Generate vectors with each component in range [0, 1]
template<FloatVector T>
inline T rand_unit_positive() {
    return T(rand_unit_positive<glm::vec<T::length() - 1, typename T::value_type>>(), rand_unit_positive<float>());
}

template<>
inline float rand_unit_positive<float>() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<float> dist(0.0, 1.0);
    return dist(rng);
}

template<>
inline glm::vec1 rand_unit_positive<glm::vec1>() {
    return glm::vec1(rand_unit_positive<float>());
}

#endif
