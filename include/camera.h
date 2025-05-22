#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include <memory>

#include "GL/gl.h"
#include "glm/glm.hpp"
extern "C"
{
#include "libavcodec/avcodec.h"
}

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
    float time_scale;
    AVRational time_base;
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
    GLuint renderAsTexture(const HittableList &world, int frame_number) const;
    void toUniform(CameraUniform &camera_uniform) const;
    float getDeltaT() const { return dt; }

private:
    CameraConfig config;
    float aspect_ratio;
    glm::vec3 viewport_lower_left;
    glm::vec3 viewport_dx;
    glm::vec3 viewport_dy;
    float pixel_samples_scale;
    float pixel_samples_delta;
    float dt;

    void initViewport();
    Ray getRayToPixel(float x, float y) const;
    Ray getRayToPixel(float x, float y, float t) const;
    glm::vec4 getRayColor(Ray &ray, const HittableList &hittable, const int &recursion_depth) const;
    glm::vec4 getPixelColor(float x, float y, const HittableList &hittable) const;
    glm::vec4 getPixelColor(float x, float y, float t, const HittableList &hittable) const;
};

#endif
