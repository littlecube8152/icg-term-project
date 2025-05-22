#include "scene.h"

#include <memory>
#include <chrono>
#include <iostream>


Scene::Scene(void) : world(std::make_shared<InertialFrame>(object_space_frame)) {}


GLuint Scene::renderAsTexture() {
    const auto start = std::chrono::high_resolution_clock::now();
    GLuint texture = camera.renderAsTexture(world);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cerr << "Frame time: " << elapsed << std::endl;
    return texture;
}

void Scene::toUniform(SceneUniform &scene_uniform) const {
    auto objects = world.getObjects();
    int n_objects = (int)objects.size();
    assert(n_objects <= MAX_OBJECTS);

    scene_uniform.n_objects = n_objects;
    scene_uniform.world_iframe = glm::vec4(object_space_frame.frame_velocity, 0);
    camera.toUniform(scene_uniform.camera_uniform);

    for (int obj_id = 0; obj_id < scene_uniform.n_objects; obj_id++) {
        const auto &obj = objects[obj_id];
        obj->toUniform(scene_uniform.objects[obj_id]);
        const auto &mat = obj->getMaterial();
        if (mat.get()) {
            int mat_id = mat->getId();
            assert(0 <= mat_id && mat_id < MAX_MATERIALS);
            mat->toUniform(scene_uniform.materials[mat_id]);
        }
    }
}
