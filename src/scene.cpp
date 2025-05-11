#include "scene.h"

#include <memory>

#include "sphere.h"


Scene::Scene(): camera(), world() {
    world.add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));
}


GLuint Scene::renderAsTexture(GLuint texture_width, GLuint texture_height) {
    return camera.renderAsTexture(texture_width, texture_height, world);
}
