#include "scene.h"

#include "objects.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

SceneRelativityTest::SceneRelativityTest(GLuint texture_width, GLuint texture_height)
{
    camera = Camera((CameraConfig){
        .image_width = texture_width,
        .image_height = texture_height,
        .vfov = 90,
        .lookfrom = glm::vec3(0.2, 0, 1),
        .lookat = glm::vec3(-0.2, 0, 0),
        .lookup = glm::vec3(0, 1, 0),
        .sqrt_samples_per_pixel = 4,
        .max_recursion_depth = 20,
        .inertial_frame = std::make_shared<InertialFrame>(glm::vec3(0.9, 0.0, 0.0))});

    auto material_ground = std::make_shared<Lambertian>(glm::vec4(0.8f, 0.8f, 0.0f, 1.0f));
    auto material_center = std::make_shared<Lambertian>(glm::vec4(0.1f, 0.2f, 0.5f, 1.0f));
    auto material_left = std::make_shared<Dielectric>(1.50f);
    auto material_bubble = std::make_shared<Dielectric>(1.00f / 1.50f);
    auto material_right = std::make_shared<Metal>(glm::vec4(0.8f, 0.6f, 0.2f, 1.0f), 1.0f);
    world.add(std::make_shared<Cube>(glm::vec3(0.0f, -1000.5f, -1.0f), 2000.0f, material_ground));
    world.add(std::make_shared<Cube>(glm::vec3(0.0f, 0.0f, -1.0f), 0.8f, material_center));
    world.add(std::make_shared<Cube>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.8f, material_left));
    world.add(std::make_shared<Cube>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.6f, material_bubble));
    world.add(std::make_shared<Cube>(glm::vec3(1.0f, 0.0f, -1.0f), 0.8f, material_right));
}