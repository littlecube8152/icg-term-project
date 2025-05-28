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

template<typename T>
static GLuint sendShaderStorage(GLuint program, std::vector<T> vec, GLuint bind_point, const char *buffer_name) {
    // the array to be sent must be 16-bytes aligned
    static_assert(sizeof(T) % 16 == 0);

    GLuint ssbo;
    glCreateBuffers(1, &ssbo);
    glNamedBufferStorage(ssbo, sizeof(T) * vec.size(), vec.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind_point, ssbo);
    GLuint shader_storage_index = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, buffer_name);
    glShaderStorageBlockBinding(program, shader_storage_index, bind_point);
    return ssbo;
}


Renderer::Renderer(const ArgumentParser &options, const Scene &_scene)
    : scene(_scene), window_width(options.getWidth()), window_height(options.getHeight()), gpu_buffers() {
    path_tracer = loadPathTracerProgram(options.getDepthOption());
    texture_unit = 0;
    texture = createTexture(texture_unit, window_width, window_height);
    shader = loadShaderProgram();
    window_vao = createWindowVao(shader);
}

Renderer::~Renderer() {
    for (auto &buf : gpu_buffers)
        glDeleteBuffers(1, &buf);
}

void Renderer::sendSceneData() {
    auto collector = std::make_unique<SceneUniformCollector>();
    auto color_uniform = std::make_unique<ColorConstantsUniform>();
    scene.toUniform(*collector);
    toColorConstantsUniform(*color_uniform);
    gpu_buffers.emplace_back(sendUniform(path_tracer, &collector->scene, sizeof(SceneUniform), 0, "SceneUniform"));
    gpu_buffers.emplace_back(sendUniform(path_tracer, color_uniform.get(), sizeof(ColorConstantsUniform), 1, "ColorConstantsUniform"));

    gpu_buffers.emplace_back(sendShaderStorage(path_tracer, collector->iframes, 1, "IframesStorage"));
    gpu_buffers.emplace_back(sendShaderStorage(path_tracer, collector->spheres, 2, "SpheresStorage"));
    gpu_buffers.emplace_back(sendShaderStorage(path_tracer, collector->materials, 3, "MaterialsStorage"));
    gpu_buffers.emplace_back(sendShaderStorage(path_tracer, collector->vertices, 4, "VerticesStorage"));
    gpu_buffers.emplace_back(sendShaderStorage(path_tracer, collector->triangles, 5, "TrianglesStorage"));
}

void Renderer::renderFrame(int frame_number) {
    glUseProgram(path_tracer);
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindImageTexture(texture_unit, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    GLuint scene_time_location = glGetUniformLocation(path_tracer, "scene_time");
    glUniform1f(scene_time_location, scene.getSceneTime(frame_number));

    glDispatchCompute(window_width, window_height, 1);
    compute_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}


bool Renderer::pollFrame() {
    GLenum result = glClientWaitSync(compute_fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
    return result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED;
}


void Renderer::drawFrame(void) {
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(window_vao);
    glUniform1i(glGetUniformLocation(shader, "u_texture"), texture_unit);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
}

std::vector<uint8_t> Renderer::dumpPixelFromTexture()
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);

    std::vector<uint8_t> pixels(window_width * window_height * 4);
    glGetnTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, window_width * window_height * 4, pixels.data());

    // Flip vertically (OpenGL is bottom-left origin; PNG is top-left)
    for (unsigned y = 0; y < window_height / 2; y++)
    {
        std::swap_ranges(pixels.begin() + y * window_width * 4, pixels.begin() + (y + 1) * window_width * 4,
                         pixels.begin() + (window_height - y - 1) * window_width * 4);
    }
    return pixels;
}
