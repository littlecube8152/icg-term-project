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
    return (SceneUniform) {
        .camera_uniform = camera.toUniform(),
    };
}
