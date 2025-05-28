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
    collector.scene.world_iframe = glm::vec4(object_space_frame.frame_velocity, 0);
    camera.toUniform(collector.scene.camera_uniform);

    const auto &objects = world.getObjects();
    for (const auto &obj : objects) {
        obj->toUniform(collector);
    }

    collector.scene.n_spheres = static_cast<int>(collector.spheres.size());
    collector.scene.n_triangles = static_cast<int>(collector.triangles.size());
    #define copy_collected(container_name, max_items) {\
        if (collector.container_name.size() >= max_items) { \
            throw std::runtime_error(std::format("Failed to create uniform from scene: number of {} exceeds {}", #container_name, max_items)); \
        } \
        for (size_t i = 0; i < collector.container_name.size(); i++) { \
            collector.scene.u_##container_name[i + 1] = collector.container_name[i]; \
        } \
    }
    copy_collected(iframes, MAX_IFRAMES);
    copy_collected(spheres, MAX_OBJECTS);
    copy_collected(materials, MAX_MATERIALS);
    copy_collected(vertices, MAX_VERTICES);
    #undef copy_collected
}

float Scene::getSceneTime(int frame_number) const {
    return camera.getDeltaT() * (float)frame_number;
}
