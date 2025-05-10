#include "camera.h"

#include <vector>

GLuint Camera::renderAsTexture(void) {
    texture_data.resize(image_width * image_height * 4);
    int data_index = 0;
    for (GLuint y = 0; y < image_height; y++) {
        for (GLuint x = 0; x < image_width; x++) {
            GLfloat r = (GLfloat)x / (GLfloat)image_width;
            GLfloat g = (GLfloat)y / (GLfloat)image_height;
            GLfloat b = (GLfloat)(image_width + image_height - x - y) / (GLfloat)(image_width + image_height);
            texture_data[data_index++] = (GLubyte)(r * 255);
            texture_data[data_index++] = (GLubyte)(g * 255);
            texture_data[data_index++] = (GLubyte)(b * 255);
            texture_data[data_index++] = (GLubyte)(255);
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data());

    return texture;
}
