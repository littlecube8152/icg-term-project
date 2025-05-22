#include "shaders.h"

// placeholders to supress warnings
#if __has_include("shaders/compute.h")
#include "shaders/compute.h"
#else 
unsigned char compute_shader_source[] = {}; 
#endif
#if __has_include("shaders/fragment.h")
#include "shaders/fragment.h"
#else 
unsigned char fragment_shader_source[] = {};
#endif
#if __has_include("shaders/vertex.h")
#include "shaders/vertex.h"
#else 
unsigned char vertex_shader_source[] = {};
#endif

#include <string>
#include <unordered_set>
#include <functional>
#include <format>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <regex>
#include <ranges>
#include <string_view>

static GLuint compileShader(GLuint type, const std::string shader_source) {
    const char* shader_source_ptr = shader_source.c_str();
    const GLint shader_source_length = (int)shader_source.size();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_source_ptr, &shader_source_length);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::unique_ptr<char[]> msg = std::make_unique<char[]>(length);
        glGetShaderInfoLog(shader, length, &length, msg.get());
        glDeleteShader(shader);
        throw std::runtime_error(msg.get());
    }

    return shader;
}


static GLuint createLinkedProgram(const std::initializer_list<GLuint> &shaders) {
    GLuint program = glCreateProgram();
    for (auto &shader:shaders)
        glAttachShader(program, shader);
    glLinkProgram(program);

    GLint link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (!link_status)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::unique_ptr<char[]> msg = std::make_unique<char[]>(length);
        glGetProgramInfoLog(program, length, &length, msg.get());
        glDeleteProgram(program);
        throw std::runtime_error(msg.get());
    }

    glValidateProgram(program);
    return program;
}


GLuint loadShaderProgram(void) {
    GLuint vert_shader = compileShader(GL_VERTEX_SHADER, reinterpret_cast<char*>(vertex_shader_source));
    GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER, reinterpret_cast<char*>(fragment_shader_source));
    GLuint program = createLinkedProgram({vert_shader, frag_shader});
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    return program;
}


GLuint loadPathTracerProgram(void) {
    GLuint compute_shader = compileShader(GL_COMPUTE_SHADER, reinterpret_cast<char*>(compute_shader_source));
    GLuint program = createLinkedProgram({compute_shader});
    glDeleteShader(compute_shader);
    return program;
}
