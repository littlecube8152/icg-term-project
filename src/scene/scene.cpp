#include "scene.h"

#include <memory>
#include <chrono>
#include <iostream>


Scene::Scene(void) : world(std::make_shared<InertialFrame>(object_space_frame)) {}


GLuint Scene::renderAsTexture(int frame_number) {
    const auto start = std::chrono::high_resolution_clock::now();
    GLuint texture = camera.renderAsTexture(world, frame_number);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cerr << "Frame time: " << elapsed << std::endl;
    return texture;
}

void Scene::toUniform(SceneUniform &scene_uniform, int frame_number) const {
    auto objects = world.getObjects();
    int n_spheres = 0;
    int n_cubes = 0;

    scene_uniform.scene_time = camera.getDeltaT() * (float)frame_number;
    scene_uniform.world_iframe = glm::vec4(object_space_frame.frame_velocity, 0);
    camera.toUniform(scene_uniform.camera_uniform);

    for (const auto &obj : objects) {
        Sphere *sphere = dynamic_cast<Sphere*>(obj.get());
        Cube *cube = dynamic_cast<Cube*>(obj.get());
        if (sphere) {
            sphere->toUniform(scene_uniform.spheres[n_spheres]);
            n_spheres++;
        } else if (cube) {
            cube->toUniform(scene_uniform.cubes[n_cubes]);
            n_cubes++;
        } else {
            throw std::runtime_error("Type conversion failed (unknown object type)");
        }
        const auto &mat = obj->getMaterial();
        if (mat.get()) {
            int mat_id = mat->getId();
            assert(0 <= mat_id && mat_id < MAX_MATERIALS);
            mat->toUniform(scene_uniform.materials[mat_id]);
        }
    }

    scene_uniform.n_spheres = n_spheres;
    scene_uniform.n_cubes = n_cubes;
}
