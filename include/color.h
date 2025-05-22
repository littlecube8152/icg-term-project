#ifndef COLOR_H_
#define COLOR_H_

#include "glm/glm.hpp"

constexpr int wavelength_start = 360;
constexpr int wavelength_end = 700;
constexpr int wavelength_size = wavelength_end - wavelength_start;
constexpr int hue_range = 360;
constexpr int hue_split = 300;

// each item in float array has alignment 16 as specified by std140 layout
struct alignas(16) FloatArrayUniform {
    alignas(4)  float value;
};

struct alignas(16) ColorConstantsUniform {
    alignas(16) FloatArrayUniform table_hue_to_wavelength[hue_range];
    alignas(16) FloatArrayUniform table_wavelength_to_hue[wavelength_size];
};

const glm::vec3 lightWavelengthShift(glm::vec3 sRGBcolor, float scale);
const glm::vec4 gammaCorrection(glm::vec4 color);
void toColorConstantsUniform(ColorConstantsUniform &color_constants_uniform);

#endif // COLOR_H_
