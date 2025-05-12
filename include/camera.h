#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include "GL/gl.h"
#include "glm/glm.hpp"

#include "ray.h"
#include "hittable.h"


class Camera {
public:
    GLuint renderAsTexture(GLuint texture_width, GLuint texture_height, const Hittable &world);

private:
    GLuint image_width;
    GLuint image_height;
    float vfov;
    glm::vec3 lookfrom;
    glm::vec3 lookat;
    glm::vec3 lookup;
    glm::vec3 lookright;
    float aspect_ratio;
    glm::vec3 viewport_lower_left;
    glm::vec3 viewport_dx;
    glm::vec3 viewport_dy;
    int sqrt_samples_per_pixel;
    int max_recursion_depth;

    void setImageDimension(int width, int height);
    void initViewport();
    Ray getRayToPixel(float x, float y);
    glm::vec4 getRayColor(const Ray &ray, const Hittable &hittable, const int &recursion_depth);
    glm::vec4 getPixelColor(float x, float y, const Hittable &hittable);
};

#endif
