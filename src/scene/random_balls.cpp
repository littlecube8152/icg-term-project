#include "scene.h"

#include "objects.h"
#include "materials.h"
#include "randutil.h"


SceneRandomBalls::SceneRandomBalls(GLuint texture_width, GLuint texture_height, float time_scale, const ArgumentParser &options) {
    camera = Camera(CameraConfig{
        .image_width = texture_width,
        .image_height = texture_height,
        .vfov = 20,
        .lookfrom = glm::vec3(13, 2, 3),
        .lookat = glm::vec3(0, 0, 0),
        .lookup = glm::vec3(0, 1, 0),
        .sqrt_samples_per_pixel = options.getSampleOption(),
        .max_recursion_depth = options.getDepthOption(),
        .inertial_frame = std::make_shared<InertialFrame>(glm::vec3(0.0, 0.0, 0.0)),
        .time_scale = time_scale,
        .time_base = options.getTimeBase()
    });

    auto material_ground = std::make_shared<Lambertian>(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, material_ground));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            glm::vec3 center(a + 0.4 * rand_unit_box<float>(), 0.2f, b + 0.4 * rand_unit_box<float>());
            if (glm::length(center - glm::vec3(4, 0.2, 0)) > 0.9) {
                auto mat_sample = std::abs(rand_unit_box<float>());
                std::shared_ptr<Material> material_sphere;
                if (mat_sample < 0.6) {
                    auto albedo = glm::vec4(rand_unit_positive<glm::vec3>(), 1.0f) * glm::vec4(rand_unit_positive<glm::vec3>(), 1.0f);
                    material_sphere = std::make_shared<Lambertian>(albedo);
                } else if (mat_sample < 0.95) {
                    auto albedo = glm::vec4(rand_unit_positive<glm::vec3>() / 2.0f + 0.5f, 1.0f);
                    auto fuzz = rand_unit_positive<float>() / 2.0f;
                    material_sphere = std::make_shared<Metal>(albedo, fuzz);
                } else {
                    material_sphere = std::make_shared<Dielectric>(1.50f);
                }
                world.add(std::make_shared<Sphere>(center, 0.2, material_sphere));
            }
        }
    }
    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f), 1.0f, material1));
    auto material2 = std::make_shared<Lambertian>(glm::vec4(0.4f, 0.2f, 0.1f, 1.0f));
    world.add(std::make_shared<Sphere>(glm::vec4(-4.0f, 1.0f, 0.0f, 1.0f), 1.0f, material2));
    auto material3 = std::make_shared<Metal>(glm::vec4(0.7f, 0.6f, 0.5f, 1.0f), 0.0f);
    world.add(std::make_shared<Sphere>(glm::vec4( 4.0f, 1.0f, 0.0f, 1.0f), 1.0f, material3));
}