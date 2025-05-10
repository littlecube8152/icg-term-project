#include "scene.h"

#include <memory>

#include "sphere.h"


Scene::Scene(): camera(), world() {
    world.add(std::make_shared<Sphere>(glm::vec3(0, 0, -5), 2.0f));
}


GLuint Scene::renderAsTexture(GLuint texture_width, GLuint texture_height) {
    camera.setImageDimension(texture_width, texture_height);
    camera.initViewport();

    std::vector<GLubyte> texture_data(texture_width * texture_height * 4);
    int data_index = 0;
    for (GLuint y = 0; y < texture_height; y++) {
        for (GLuint x = 0; x < texture_width; x++) {
            Ray pixel_ray = camera.getRayToPixel(x, y);
            glm::vec4 pixel_color = camera.getRayColor(pixel_ray, world);
            texture_data[data_index++] = (GLubyte)(pixel_color.r * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.g * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.b * 255);
            texture_data[data_index++] = (GLubyte)(pixel_color.a * 255);
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data());

    return texture;
}
