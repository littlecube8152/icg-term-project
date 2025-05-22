#include "renderer.h"

#include <memory>

#include "shaders.h"
#include "scene.h"
#include "color.h"


static GLuint createTexture(GLuint texture_unit, GLuint width, GLuint height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(texture_unit, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    return texture;
}


static GLuint createWindowVao(GLuint shader) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat position_data[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
    };
    GLuint vbo_position;
    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position_data), position_data, GL_STATIC_DRAW);
    GLint a_position = glGetAttribLocation(shader, "a_position");
    glEnableVertexAttribArray(a_position);
    glVertexAttribPointer(a_position, 2, GL_FLOAT,  GL_FALSE, sizeof(GLfloat) * 2, 0);

    GLfloat texcoord_data[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    GLuint vbo_texcoord;
    glGenBuffers(1, &vbo_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord_data), texcoord_data, GL_STATIC_DRAW);
    GLint a_texcoord = glGetAttribLocation(shader, "a_texcoord");
    glEnableVertexAttribArray(a_texcoord);
    glVertexAttribPointer(a_texcoord, 2, GL_FLOAT,  GL_FALSE, sizeof(GLfloat) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return vao;
}


static GLuint sendUniform(GLuint program, void* data, GLuint size, GLuint bind_point, const char *block_name) {
    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, ubo);
    GLuint uniform_block_index = glGetUniformBlockIndex(program, block_name);
    glUniformBlockBinding(program, uniform_block_index, bind_point);
    return ubo;
}


Renderer::Renderer(GLuint _window_width, GLuint _window_height)
    : window_width(_window_width), window_height(_window_height) {
    path_tracer = loadPathTracerProgram();
    texture_unit = 0;
    texture = createTexture(texture_unit, window_width, window_height);
    shader = loadShaderProgram();
    window_vao = createWindowVao(shader);
}


void Renderer::renderFrame(const Scene &scene, int frame_number) {
    glUseProgram(path_tracer);

    std::unique_ptr<SceneUniform> scene_uniform = std::make_unique<SceneUniform>();
    scene.toUniform(*scene_uniform, frame_number);
    GLuint scene_ubo = sendUniform(path_tracer, scene_uniform.get(), sizeof(SceneUniform), 0, "SceneUniform");
    std::unique_ptr<ColorConstantsUniform> color_uniform = std::make_unique<ColorConstantsUniform>();
    toColorConstantsUniform(*color_uniform);
    GLuint color_ubo = sendUniform(path_tracer, color_uniform.get(), sizeof(ColorConstantsUniform), 1, "ColorConstantsUniform");

    glDispatchCompute(window_width, window_height, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glDeleteBuffers(1, &scene_ubo);
    glDeleteBuffers(1, &color_ubo);
}


void Renderer::drawFrame(void) {
    glUseProgram(shader);
    glBindVertexArray(window_vao);
    glUniform1i(glGetUniformLocation(shader, "u_texture"), texture_unit);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
