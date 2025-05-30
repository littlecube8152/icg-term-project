#include "scenes.h"

#include "objects.h"
#include "materials.h"

SceneTerellRotationTest::SceneTerellRotationTest(const ArgumentParser &options, float time_scale)
{
    camera = Camera((CameraConfig){
        .image_width = static_cast<unsigned>(options.getWidth()),
        .image_height = static_cast<unsigned>(options.getHeight()),
        .vfov = 90,
        .lookfrom = glm::vec3(0, 0, -2),
        .lookat = glm::vec3(0, 0, 0),
        .lookup = glm::vec3(0, 1, 0),
        .sqrt_samples_per_pixel = options.getSampleOption(),
        .max_recursion_depth = options.getDepthOption(),
        .inertial_frame = std::make_shared<InertialFrame>(glm::vec3(0.0, 0.0, 0.0)),
        .time_scale = time_scale,
        .time_base = options.getTimeBase()});

    auto material_cube = std::make_shared<Metal>(glm::vec4(0.8f, 0.6f, 0.2f, 1.0f), 1.0f);

    float fraction = 0.9f;
    world.add(std::make_shared<Cube>(glm::vec3(3.0f, 0.0f, 0.0f), 0.8f, material_cube, glm::vec3(fraction, 0, 0)));
    world.add(std::make_shared<Cube>(glm::vec3(-3.0f, 0.0f, 0.0f), 0.8f, material_cube, glm::vec3(-fraction, 0, 0)));
    world.add(std::make_shared<Cube>(glm::vec3(0.0f, -3.0f, 0.0f), 0.8f, material_cube, glm::vec3(0, -fraction, 0)));
    world.add(std::make_shared<Cube>(glm::vec3(0.0f, 3.0f, 0.0f), 0.8f, material_cube, glm::vec3(0, fraction, 0)));
}