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
