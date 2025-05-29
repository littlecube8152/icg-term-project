#ifndef VECUTIL_H_
#define VECUTIL_H_

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"

#include "constants.h"


// normalize vec, but fallback to default_vec when vec is near-zero
// assumes default_vec is already normalized
template<typename genType>
inline genType default_normalize(const genType &vec, const genType &default_vec) {
    auto length = glm::length(vec);
    if (glm::epsilonEqual(length, (typename genType::value_type)(0), kEpsilon)) {
        return default_vec;
    } else {
        return vec / length;
    }
}

template<>
inline float default_normalize<float>(const float &value, const float &default_value) {
    if (glm::epsilonEqual(value, 0.0f, kEpsilon)) {
        return default_value;
    } else {
        return value < 0.0f ? -1.0f : 1.0f;
    }
}

#endif
