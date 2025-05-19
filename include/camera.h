#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include <memory>

#include "GL/gl.h"
#include "glm/glm.hpp"

#include "ray.h"
#include "hittable.h"
#include "inertial.h"


struct alignas(16) CameraConfigUniform {
    alignas(4)  glm::uint image_width;
    alignas(4)  glm::uint image_height;
    alignas(4)  float vfov;
    alignas(16) glm::vec3 lookfrom;
    alignas(16) glm::vec3 lookat;
    alignas(16) glm::vec3 lookup;
    alignas(4)  int sqrt_samples_per_pixel;
    alignas(4)  int max_recursion_depth;
};

class CameraConfig {
public:
    GLuint image_width;
    GLuint image_height;
    float vfov;
    glm::vec3 lookfrom;
    glm::vec3 lookat;
    glm::vec3 lookup;
    int sqrt_samples_per_pixel;
    int max_recursion_depth;
    std::shared_ptr<InertialFrame> inertial_frame;

    CameraConfigUniform toUniform() const;
};


struct alignas(16) CameraUniform {
    alignas(4)  float aspect_ratio;
    alignas(16) glm::vec3 viewport_lower_left;
    alignas(16) glm::vec3 viewport_dx;
    alignas(16) glm::vec3 viewport_dy;
    alignas(4)  float pixel_samples_scale;
    alignas(4)  float pixel_samples_delta;
};

class Camera {
public:
    Camera();
    Camera(const CameraConfig &config);
    GLuint renderAsTexture(const Hittable &world) const;
    CameraUniform toUniform() const;
    const CameraConfig& getConfig() const;

private:
    CameraConfig config;
    float aspect_ratio;
    glm::vec3 viewport_lower_left;
    glm::vec3 viewport_dx;
    glm::vec3 viewport_dy;
    float pixel_samples_scale;
    float pixel_samples_delta;

    void initViewport();
    Ray getRayToPixel(float x, float y) const;
    glm::vec4 getRayColor(Ray &ray, const Hittable &hittable, const int &recursion_depth) const;
    glm::vec4 getPixelColor(float x, float y, const Hittable &hittable) const;

    // gamma 2 correction
    float linear_to_gamma(float linear_component) const;
    glm::vec4 linear_to_gamma(glm::vec4 color) const;
};

#endif
