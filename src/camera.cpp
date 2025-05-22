#include "camera.h"

#include <vector>
#include <numbers>
#include <iostream>

#include "glm/gtx/perpendicular.hpp"

#include "color.h"
#include "randutil.h"
#include "vecutil.h"


// CameraConfigUniform CameraConfig::toUniform() const {
//     return (CameraConfigUniform) {
//         .sqrt_samples_per_pixel = sqrt_samples_per_pixel,
//         .max_recursion_depth = max_recursion_depth,
//         .lookfrom = glm::vec4(lookfrom, 0),
//         .iframe = glm::vec4(inertial_frame->frame_velocity, 0),
//     };
// }


Camera::Camera() {}

Camera::Camera(const CameraConfig &_config)
{
    config = _config;
    initViewport();
    dt = config.time_scale * (float)config.time_base.den / (float)config.time_base.num;
}

void Camera::initViewport()
{
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

Ray Camera::getRayToPixel(float x, float y) const
{
    return getRayToPixel(x, y, 0.0f);
}

Ray Camera::getRayToPixel(float x, float y, float t) const
{
    glm::vec3 pixel_position = glm::vec3(viewport_lower_left + viewport_dx * x + viewport_dy * y);
    return Ray(
        glm::vec4(config.lookfrom, t),
        glm::normalize(pixel_position - config.lookfrom),
        config.inertial_frame);
}

glm::vec4 Camera::getRayColor(Ray &ray, const HittableList &world, const int &recursion_depth) const
{
    if (recursion_depth > config.max_recursion_depth)
        return glm::vec4(0.0, 0.0, 0.0, 1.0);
    HitRecord record;

    if (world.hit(ray, record))
    {
        // hit
        if (record.has_scattered)
        {
            glm::vec4 hit_space_color = record.attenuation * getRayColor(record.scattered, world, recursion_depth + 1);
            glm::vec4 shifted_color = hit_space_color;

            // perform Doppler shift from the hit position back to the ray
            {
                glm::vec3 rgb_color(hit_space_color);
                glm::vec3 velocity = ray.referenceFrame().transformVelocityFrom(record.ray.referenceFrame(), glm::vec3(0, 0, 0));
                glm::vec3 line_of_sight = ray.referenceFrame().transformCoordinateFrom(record.ray.referenceFrame(),
                                                                                       record.ray.origin().w / speedOfLight,
                                                                                       glm::vec3(record.ray.origin()))
                                              .second -
                                          (glm::vec3)ray.origin();
                float cosine = glm::dot(default_normalize(velocity, velocity), glm::normalize(line_of_sight));

                // float scale = 1.0f / config.inertial_frame->getGamma(object_space_frame)
                //                    / (1.0f + config.inertial_frame->getBetaScalar(object_space_frame) * cosine);

                float beta_cosine = ray.referenceFrame().getBetaScalar(record.ray.referenceFrame()) * cosine;
                float scale = std::sqrtf((1.0f - beta_cosine) / (1.0f + beta_cosine));

                shifted_color = glm::vec4(lightWavelengthShift(rgb_color, scale), shifted_color.a);
            }
            return shifted_color;
        }
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        // skybox
        float dotval = glm::dot(config.inertial_frame->transformVelocityFrom(ray.referenceFrame(), ray.direction()), glm::vec3(0, 1, 0));
        float ratio = (dotval + 1) / 2.0f;
        return (1.0f - ratio) * glm::vec4(1, 1, 1, 1) + ratio * glm::vec4(0.2, 0.2, 1.0, 1.0);
    }
}

glm::vec4 Camera::getPixelColor(float x, float y, float t, const HittableList &world) const
{
    glm::vec4 pixel_color = glm::vec4(0, 0, 0, 0);
    for (int dx = 0; dx < config.sqrt_samples_per_pixel; dx++)
    {
        for (int dy = 0; dy < config.sqrt_samples_per_pixel; dy++)
        {
            Ray sampled_ray = getRayToPixel(x + pixel_samples_delta * (0.5f + (float)dx),
                                            y + pixel_samples_delta * (0.5f + (float)dy),
                                            t);
            pixel_color += getRayColor(sampled_ray, world, 1);
        }
    }
    pixel_color *= pixel_samples_scale;
    pixel_color = gammaCorrection(pixel_color);

    return pixel_color;
}

glm::vec4 Camera::getPixelColor(float x, float y, const HittableList &world) const
{
    return getPixelColor(x, y, 0.0f, world);
}

GLuint Camera::renderAsTexture(const HittableList &world, int frame_number) const
{
    float t = dt * static_cast<float>(frame_number);
    std::vector<GLubyte> texture_data(config.image_width * config.image_height * 4);
    int data_index = 0;
    for (GLuint y = 0; y < config.image_height; y++)
    {
        for (GLuint x = 0; x < config.image_width; x++)
        {
            glm::vec4 pixel_color = getPixelColor((float)x, (float)y, t, world);
            texture_data[data_index++] = (GLubyte)(pixel_color.r * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.g * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.b * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.a * 255);
        }
        if ((y + 1) % 10 == 0)
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
        .pixel_samples_scale = pixel_samples_scale,
        .pixel_samples_delta = pixel_samples_delta,
        .lookfrom = glm::vec4(config.lookfrom, 0),
        .iframe = glm::vec4(config.inertial_frame->frame_velocity, 0),
        .viewport_lower_left = glm::vec4(viewport_lower_left, 0),
        .viewport_dx = glm::vec4(viewport_dx, 0),
        .viewport_dy = glm::vec4(viewport_dy, 0),
    };
}
