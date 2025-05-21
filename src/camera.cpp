#include "camera.h"

#include <vector>
#include <numbers>
#include <iostream>

#include "glm/gtx/perpendicular.hpp"

#include "randutil.h"


// CameraConfigUniform CameraConfig::toUniform() const {
//     return (CameraConfigUniform) {
//         .sqrt_samples_per_pixel = sqrt_samples_per_pixel,
//         .max_recursion_depth = max_recursion_depth,
//         .lookfrom = glm::vec4(lookfrom, 0),
//         .iframe = glm::vec4(inertial_frame->frame_velocity, 0),
//     };
// }


Camera::Camera() {}


Camera::Camera(const CameraConfig &_config) {
    config = _config;
    initViewport();
}


void Camera::initViewport() {
    aspect_ratio = (float)config.image_width / (float)config.image_height;
    pixel_samples_delta = 1.0f / (float)config.sqrt_samples_per_pixel;

    // unit vectors in camera's coordinate system
    glm::vec3 uz = glm::normalize(config.lookfrom - config.lookat);
    glm::vec3 uy = glm::normalize(glm::perp(config.lookup, uz));
    glm::vec3 ux = glm::cross(uy, uz);
    
    pixel_samples_scale = 1.0f / (float)(config.sqrt_samples_per_pixel * config.sqrt_samples_per_pixel);
    float tan_vfov2 = tanf(config.vfov / 2 / 180 * std::numbers::pi_v<float>);
    viewport_lower_left = config.lookfrom + -tan_vfov2 * aspect_ratio * ux + -tan_vfov2 * uy - uz;
    viewport_dx = tan_vfov2 * 2 * aspect_ratio / (float)config.image_width * ux;
    viewport_dy = tan_vfov2 * 2 / (float)config.image_height * uy;
}


Ray Camera::getRayToPixel(float x, float y) const {
    glm::vec3 pixel_position = glm::vec3(viewport_lower_left + viewport_dx * x + viewport_dy * y);
    return Ray(
        config.lookfrom,
        glm::normalize(pixel_position - config.lookfrom),
        config.inertial_frame
    );
}


glm::vec4 Camera::getRayColor(Ray &ray, const HittableList &world, const int &recursion_depth) const {
    if (recursion_depth > config.max_recursion_depth)
        return glm::vec4(0.0, 0.0, 0.0, 1.0);
    HitRecord rec;
    if (world.hit(ray, rec)) {
        if (rec.has_scattered)
            return rec.attenuation * getRayColor(rec.scattered, world, recursion_depth + 1);
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        float dotval = glm::dot(ray.direction(), glm::vec3(0, 1, 0));
        float ratio = (dotval + 1) / 2.0f;
        return (1.0f - ratio) * glm::vec4(1, 1, 1, 1) + ratio * glm::vec4(0.5, 0.7, 1.0, 1.0);
    }
}


glm::vec4 Camera::getPixelColor(float x, float y, const HittableList &world) const {
    glm::vec4 pixel_color = glm::vec4(0, 0, 0, 0);
    for (int dx = 0; dx < config.sqrt_samples_per_pixel; dx++) {
        for (int dy = 0; dy < config.sqrt_samples_per_pixel; dy++) {
            Ray sampled_ray = getRayToPixel(x + pixel_samples_delta * (0.5f + (float)dx), y + pixel_samples_delta * (0.5f + (float)dy));
            pixel_color += getRayColor(sampled_ray, world, 1);
        }
    }
    pixel_color *= pixel_samples_scale;
    pixel_color = linear_to_gamma(pixel_color);
    return pixel_color;
}


float Camera::linear_to_gamma(float linear_component) const {
    if (linear_component > 0.0f)
        return std::sqrt(linear_component);
    return 0.0f;
}


glm::vec4 Camera::linear_to_gamma(glm::vec4 color) const {
    color.r = linear_to_gamma(color.r);
    color.g = linear_to_gamma(color.g);
    color.b = linear_to_gamma(color.b);
    return color;
}


GLuint Camera::renderAsTexture(const HittableList &world) const {
    std::vector<GLubyte> texture_data(config.image_width * config.image_height * 4);
    int data_index = 0;
    for (GLuint y = 0; y < config.image_height; y++) {
        for (GLuint x = 0; x < config.image_width; x++) {
            glm::vec4 pixel_color = getPixelColor((float)x, (float)y, world);
            texture_data[data_index++] = (GLubyte)(pixel_color.r * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.g * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.b * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.a * 255);
        }
        std::cerr << "Render progress " << (y + 1) << " / " << config.image_height << std::endl;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, config.image_width, config.image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data());

    return texture;
}


void Camera::toUniform(CameraUniform &camera_uniform) const {
    camera_uniform = (CameraUniform) {
        .sqrt_samples_per_pixel = config.sqrt_samples_per_pixel,
        .max_recursion_depth = config.max_recursion_depth,
        .pixel_samples_scale = pixel_samples_scale,
        .pixel_samples_delta = pixel_samples_delta,
        .lookfrom = glm::vec4(config.lookfrom, 0),
        .iframe = glm::vec4(config.inertial_frame->frame_velocity, 0),
        .viewport_lower_left = glm::vec4(viewport_lower_left, 0),
        .viewport_dx = glm::vec4(viewport_dx, 0),
        .viewport_dy = glm::vec4(viewport_dy, 0),
    };
}
