#include "scene.h"

#include <memory>
#include <chrono>
#include <iostream>


Scene::Scene(void) {}


GLuint Scene::renderAsTexture() {
    const auto start = std::chrono::high_resolution_clock::now();
    GLuint texture = camera.renderAsTexture(world);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cerr << "Frame time: " << elapsed << std::endl;
    return texture;
}
