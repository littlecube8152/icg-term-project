#ifndef COLOR_H_
#define COLOR_H_

#include "glm/glm.hpp"

const glm::vec3 lightWavelengthShift(glm::vec3 sRGBcolor, float scale);
const glm::vec4 gammaCorrection(glm::vec4 color);

#endif // COLOR_H_
