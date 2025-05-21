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

SceneUniform Scene::toUniform() const {
    auto objects = world.getObjects();
    int n_objects = (int)objects.size();
    assert(n_objects <= MAX_OBJECTS);

    SceneUniform uniform;
    uniform.n_objects = n_objects;
    uniform.world_iframe = glm::vec4(object_space_frame.frame_velocity, 0);
    uniform.camera_uniform = camera.toUniform();

    for (int obj_id = 0; obj_id < uniform.n_objects; obj_id++) {
        const auto &obj = objects[obj_id];
        uniform.objects[obj_id] = obj->toUniform();
        const auto &mat = obj->getMaterial();
        if (mat.get()) {
            int mat_id = mat->getId();
            assert(0 <= mat_id && mat_id < MAX_MATERIALS);
            uniform.materials[mat_id] = mat->toUniform();
        }
    }
    
    return uniform;
}
