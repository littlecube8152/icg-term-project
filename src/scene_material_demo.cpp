#include "scene.h"

#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"


SceneMaterialDemo::SceneMaterialDemo(GLuint texture_width, GLuint texture_height) {
    camera = Camera((CameraConfig) {
        .image_width = texture_width,
        .image_height = texture_height,
        .vfov = 90,
        .lookfrom = glm::vec3(0, 0, 5),
        .lookat = glm::vec3(0, 0, 0),
        .lookup = glm::vec3(0, 1, 0),
        .sqrt_samples_per_pixel = 4,
        .max_recursion_depth = 10,
        .inertial_frame = std::make_shared<InertialFrame>(glm::vec3(-0.5, 0.0, 0.0))
    });

    auto material_ground = std::make_shared<Lambertian>(glm::vec4(0.8f, 0.8f, 0.0f, 1.0f));
    auto material_center = std::make_shared<Lambertian>(glm::vec4(0.1f, 0.2f, 0.5f, 1.0f));
    auto material_left   = std::make_shared<Dielectric>(1.50f);
    auto material_bubble = std::make_shared<Dielectric>(1.00f / 1.50f);
    auto material_right  = std::make_shared<Metal>(glm::vec4(0.8f, 0.6f, 0.2f, 1.0f), 1.0f);
    world.add(std::make_shared<Sphere>(glm::vec3( 0.0f, -1000.5f, -1.0f), 1000.0f, material_ground));
    world.add(std::make_shared<Sphere>(glm::vec3( 0.0f,    0.0f, -1.2f),   0.5f, material_center));
    world.add(std::make_shared<Sphere>(glm::vec3(-1.0f,    0.0f, -1.0f),   0.5f, material_left));
    world.add(std::make_shared<Sphere>(glm::vec3(-1.0f,    0.0f, -1.0f),   0.4f, material_bubble));
    world.add(std::make_shared<Sphere>(glm::vec3( 1.0f,    0.0f, -1.0f),   0.5f, material_right));
}