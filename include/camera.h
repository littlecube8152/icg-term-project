#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include "GL/gl.h"
#include "glm/glm.hpp"

#include "ray.h"
#include "hittable.h"


class Camera {
public:
    void setImageDimension(int width, int height);
    void initViewport();
    Ray getRayToPixel(int x, int y);
    glm::vec4 getRayColor(const Ray &ray, const Hittable &hittable);

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
};

#endif
