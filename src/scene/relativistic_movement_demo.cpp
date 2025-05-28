#include "scene.h"

#include "objects.h"
#include "materials.h"

SceneRelativisticMovementDemo::SceneRelativisticMovementDemo(const ArgumentParser &options, float time_scale)
{
    auto camera_frame = glm::vec3(0.5, 0.0, 0.0);

    camera = Camera((CameraConfig){
        .image_width = static_cast<unsigned>(options.getWidth()),
        .image_height = static_cast<unsigned>(options.getHeight()),
        .vfov = 90,
        .lookfrom = glm::vec3(-1, 0, -3),
        .lookat = glm::vec3(0, 0, 0),
        .lookup = glm::vec3(0, 1, 0),
        .sqrt_samples_per_pixel = options.getSampleOption(),
        .max_recursion_depth = options.getDepthOption(),
        .inertial_frame = std::make_shared<InertialFrame>(camera_frame),
        .time_scale = time_scale,
        .time_base = options.getTimeBase()});

    auto material_ground = std::make_shared<Lambertian>(glm::vec4(0.8f, 0.8f, 0.0f, 1.0f));
    auto material_center = std::make_shared<Lambertian>(glm::vec4(0.1f, 0.2f, 0.5f, 1.0f));
    auto material_left = std::make_shared<Dielectric>(1.50f);
    auto material_bubble = std::make_shared<Dielectric>(1.00f / 1.50f);
    auto material_right = std::make_shared<Metal>(glm::vec4(0.8f, 0.6f, 0.2f, 1.0f), 1.0f);
    InertialFrame slower_frame(glm::vec3(0.2, 0.0, 0.0));
    InertialFrame faster_frame(glm::vec3(0.7, 0.0, 0.0));
    world.add(std::make_shared<Cube>(glm::vec3(-1.0f, 0.0f, 1.0f), 0.8f, material_right));
    world.add(std::make_shared<Cube>(glm::vec3(0.0f, -1000.5f, 1.0f), 2000.0f, material_ground));
    world.add(std::make_shared<Cube>(glm::vec3(0.0f, 0.0f, 1.0f), 0.8f, material_center));
    world.add(std::make_shared<Cube>(glm::vec3(1.0f, 0.0f, 1.0f), 0.8f, material_left));
    world.add(std::make_shared<Cube>(glm::vec3(1.0f, 0.0f, 1.0f), 0.6f, material_bubble));
    world.add(std::make_shared<Cube>(glm::vec3(2.0f, 0.0f, 1.0f), 0.8f, material_left, slower_frame));
    world.add(std::make_shared<Cube>(glm::vec3(2.0f, 0.0f, 1.0f), 0.6f, material_bubble, slower_frame));
    world.add(std::make_shared<Cube>(glm::vec3(2.0f, 0.0f, 1.0f), 0.8f, material_left, camera_frame));
    world.add(std::make_shared<Cube>(glm::vec3(2.0f, 0.0f, 1.0f), 0.6f, material_bubble, camera_frame));
    world.add(std::make_shared<Cube>(glm::vec3(2.0f, 0.0f, 1.0f), 0.8f, material_left, faster_frame));
    world.add(std::make_shared<Cube>(glm::vec3(2.0f, 0.0f, 1.0f), 0.6f, material_bubble, faster_frame));
    world.add(std::make_shared<Cube>(glm::vec3(3.0f, 0.0f, 1.0f), 0.8f, material_right, slower_frame));
    world.add(std::make_shared<Cube>(glm::vec3(3.0f, 0.0f, 1.0f), 0.8f, material_right, camera_frame));
    world.add(std::make_shared<Cube>(glm::vec3(3.0f, 0.0f, 1.0f), 0.8f, material_right, faster_frame));
    world.add(std::make_shared<Cube>(glm::vec3(3.0f, 0.0f, 1.0f), 0.8f, material_left, InertialFrame(glm::vec3(0.8, 0.0, 0.0))));
}