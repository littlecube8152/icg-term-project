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

void Scene::toUniform(SceneUniformCollector &collector) const {
    // arrays in SSBO are 1-indexed
    collector.iframes.emplace_back();
    collector.spheres.emplace_back();
    collector.materials.emplace_back();
    collector.vertices.emplace_back();
    collector.triangles.emplace_back();

    collector.scene.world_iframe = glm::vec4(object_space_frame.frame_velocity, 0);
    camera.toUniform(collector.scene.camera_uniform);
    const auto &objects = world.getObjects();
    for (const auto &obj : objects) {
        obj->toUniform(collector);
    }
    collector.scene.n_spheres = static_cast<int>(collector.spheres.size() - 1);
    collector.scene.n_triangles = static_cast<int>(collector.triangles.size() - 1);
}

float Scene::getSceneTime(int frame_number) const {
    return camera.getDeltaT() * (float)frame_number;
}
