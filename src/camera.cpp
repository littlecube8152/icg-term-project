#include "camera.h"

#include <vector>


void Camera::setImageDimension(int width, int height) {
    image_width = width;
    image_height = height;
}


void Camera::initViewport() {
    vfov = 90;
    lookfrom = glm::vec3(0, 0, 0);
    lookat = glm::vec3(0, 0, -1);
    lookup = glm::vec3(0, 1, 0);
    lookright = glm::cross(lookat, lookup);

    aspect_ratio = (float)image_width / (float)image_height;
    float tan_vfov2 = tanf(vfov / 2);
    viewport_lower_left = -tan_vfov2 * lookright + -tan_vfov2 / aspect_ratio * lookup + lookat;
    viewport_dx = tan_vfov2 * 2 / (float)image_width * lookright;
    viewport_dy = tan_vfov2 * 2 / aspect_ratio / (float)image_height * lookup;
}


Ray Camera::getRayToPixel(int x, int y) {
    return Ray(
        lookfrom,
        glm::vec3(viewport_lower_left + viewport_dx * ((float)x + 0.5f) + viewport_dy * ((float)y + 0.5f)) - lookfrom
    );
}


glm::vec4 Camera::getRayColor(const Ray &ray) {
    float dotval = glm::dot(glm::normalize(ray.direction()), glm::vec3(0, 1, 0));
    float ratio = (dotval + 1) / 2.0f;
    return (1.0f - ratio) * glm::vec4(1, 1, 1, 1) + ratio * glm::vec4(0.5, 0.7, 1.0, 1.0);
}
