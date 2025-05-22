#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include <memory>

#include "GL/gl.h"
#include "glm/glm.hpp"

#include "ray.h"
#include "hit_record.h"
#include "objects.h"
#include "hittable_list.h"
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
    std::shared_ptr<InertialFrame> inertial_frame;
};


// NOTE:
// vec3 intentionally replaced with vec4 to avoid alignment issues
// https://stackoverflow.com/questions/38172696/should-i-ever-use-a-vec3-inside-of-a-uniform-buffer-or-shader-storage-buffer-o

struct alignas(16) CameraUniform {
    alignas(4)  int sqrt_samples_per_pixel;
    alignas(4)  int max_recursion_depth;
    alignas(4)  float pixel_samples_scale;
    alignas(4)  float pixel_samples_delta;
    alignas(16) glm::vec4 lookfrom;
    alignas(16) glm::vec4 iframe;
    alignas(16) glm::vec4 viewport_lower_left;
    alignas(16) glm::vec4 viewport_dx;
    alignas(16) glm::vec4 viewport_dy;
};


class Camera {
public:
    Camera();
    Camera(const CameraConfig &config);
    GLuint renderAsTexture(const HittableList &world) const;
    void toUniform(CameraUniform &camera_uniform) const;

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
    glm::vec4 getRayColor(Ray &ray, const HittableList &world, const int &recursion_depth) const;
    glm::vec4 getPixelColor(float x, float y, const HittableList &world) const;
};

#endif
