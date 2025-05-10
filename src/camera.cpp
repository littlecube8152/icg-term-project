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
    sqrt_samples_per_pixel = 5;

    float tan_vfov2 = tanf(vfov / 2 / 180 * acosf(-1));
    viewport_lower_left = -tan_vfov2 * aspect_ratio * lookright + -tan_vfov2 * lookup + lookat;
    viewport_dx = tan_vfov2 * 2 * aspect_ratio / (float)image_width * lookright;
    viewport_dy = tan_vfov2 * 2 / (float)image_height * lookup;
}


Ray Camera::getRayToPixel(float x, float y) {
    glm::vec3 pixel_position = glm::vec3(viewport_lower_left + viewport_dx * x + viewport_dy * y);
    return Ray(
        lookfrom,
        glm::normalize(pixel_position - lookfrom)
    );
}


glm::vec4 Camera::getRayColor(const Ray &ray, const Hittable &hittable) {
    HitRecord rec;
    if (hittable.hit(ray, Interval::universe, rec)) {
        glm::vec3 n = (rec.normal + glm::vec3(1, 1, 1)) / 2.0f;
        return glm::vec4(n.r, n.g, n.b, 1.0f);
    } else {
        float dotval = glm::dot(glm::normalize(ray.direction()), glm::vec3(0, 1, 0));
        float ratio = (dotval + 1) / 2.0f;
        return (1.0f - ratio) * glm::vec4(1, 1, 1, 1) + ratio * glm::vec4(0.5, 0.7, 1.0, 1.0);
    }
}


glm::vec4 Camera::getPixelColor(float x, float y, const Hittable &hittable) {
    float delta = 1.0f / (float)sqrt_samples_per_pixel;
    glm::vec4 pixel_color = glm::vec4(0, 0, 0, 0);
    for (int dx = 0; dx < sqrt_samples_per_pixel; dx++) {
        for (int dy = 0; dy < sqrt_samples_per_pixel; dy++) {
            Ray sampled_ray = getRayToPixel(x + delta * (0.5f + (float)dx), y + delta * (0.5f + (float)dy));
            pixel_color += getRayColor(sampled_ray, hittable);
        }
    }
    return pixel_color / (float)(sqrt_samples_per_pixel * sqrt_samples_per_pixel);
}


GLuint Camera::renderAsTexture(GLuint texture_width, GLuint texture_height, const Hittable &world) {
    setImageDimension(texture_width, texture_height);
    initViewport();

    std::vector<GLubyte> texture_data(texture_width * texture_height * 4);
    int data_index = 0;
    for (GLuint y = 0; y < texture_height; y++) {
        for (GLuint x = 0; x < texture_width; x++) {
            glm::vec4 pixel_color = getPixelColor((float)x, (float)y, world);
            texture_data[data_index++] = (GLubyte)(pixel_color.r * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.g * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.b * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.a * 255);
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data());

    return texture;
}
