#include "scene.h"

#include <memory>
#include <chrono>
#include <iostream>


Scene::Scene(): camera(), world() {}


GLuint Scene::renderAsTexture(GLuint texture_width, GLuint texture_height) {
    const auto start = std::chrono::high_resolution_clock::now();
    GLuint texture = camera.renderAsTexture(texture_width, texture_height, world);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cerr << "Frame time: " << elapsed << std::endl;
    return texture;
}
