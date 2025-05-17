#ifndef EPSILON_H_
#define EPSILON_H_

#include <limits>

#include "GL/gl.h"


static constexpr GLuint kWindowWidth = 960;
static constexpr GLuint kWindowHeight = 540;

static constexpr float kInfinity = std::numeric_limits<float>::infinity();

static constexpr float kEpsilon = 1e-3f;

static constexpr float speedOfLight = 299'792'458.0f;

#endif
