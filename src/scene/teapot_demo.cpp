#include "scene.h"

#include "objects.h"
#include "materials.h"

SceneTeapotDemo::SceneTeapotDemo(const ArgumentParser &options, float time_scale)
{
    camera = Camera((CameraConfig){
        .image_width = static_cast<unsigned>(options.getWidth()),
        .image_height = static_cast<unsigned>(options.getHeight()),
        .vfov = 90,
        .lookfrom = glm::vec3(0, 1, -1),
        .lookat = glm::vec3(0, 0, 0),
        .lookup = glm::vec3(0, 1, 0),
        .sqrt_samples_per_pixel = options.getSampleOption(),
        .max_recursion_depth = options.getDepthOption(),
        .inertial_frame = std::make_shared<InertialFrame>(glm::vec3(0.0, 0.0, 0.0)),
        .time_scale = time_scale,
        .time_base = options.getTimeBase()});

    InertialFrame iframe(glm::vec3(0, 0, 0));
    auto material = std::make_shared<Lambertian>(glm::vec4(0.6f, 0.4f, 0.0f, 1.0f));
    world.add(std::make_shared<ObjMesh>(std::filesystem::path("model") / "Teapot.obj", glm::vec3(0, 0, 0), 0.8, iframe));
}