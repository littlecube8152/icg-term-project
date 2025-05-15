#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include "GL/gl.h"
#include "glm/glm.hpp"

#include "ray.h"
#include "hittable.h"
#include "inertial.h"

struct CameraConfig {
    GLuint image_width;
    GLuint image_height;
    float vfov;
    glm::vec3 lookfrom;
    glm::vec3 lookat;
    glm::vec3 lookup;
    int sqrt_samples_per_pixel;
    int max_recursion_depth;
    InertialFrame inertial_frame;
};


class Camera {
public:
    Camera();
    Camera(const CameraConfig &config);
    GLuint renderAsTexture(const Hittable &world);

private:
    CameraConfig config;
    float aspect_ratio;
    glm::vec3 viewport_lower_left;
    glm::vec3 viewport_dx;
    glm::vec3 viewport_dy;
    float pixel_samples_scale;
    float pixel_samples_delta;

    void initViewport();
    Ray getRayToPixel(float x, float y);
    glm::vec4 getRayColor(const Ray &ray, const Hittable &hittable, const int &recursion_depth);
    glm::vec4 getPixelColor(float x, float y, const Hittable &hittable);

    // gamma 2 correction
    float linear_to_gamma(float linear_component);
    glm::vec4 linear_to_gamma(glm::vec4 color);
};

#endif
