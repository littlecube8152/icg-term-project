#include "shaders.h"

// placeholders to supress warnings
#if __has_include("shaders/compute.h")
#include "shaders/compute.h"
#else 
unsigned char compute_shader_source[] = {}; 
unsigned int compute_shader_source_len = 0;
#endif
#if __has_include("shaders/fragment.h")
#include "shaders/fragment.h"
#else 
unsigned char fragment_shader_source[] = {};
unsigned int fragment_shader_source_len = 0;
#endif
#if __has_include("shaders/vertex.h")
#include "shaders/vertex.h"
#else 
unsigned char vertex_shader_source[] = {};
unsigned int vertex_shader_source_len = 0;
#endif

#include <string>
#include <unordered_set>
#include <functional>
#include <format>
#include <fstream>
#include <stdexcept>
#include <memory>

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
    std::string vert_source(reinterpret_cast<char*>(vertex_shader_source), vertex_shader_source_len);
    std::string frag_source(reinterpret_cast<char*>(fragment_shader_source), fragment_shader_source_len);
    GLuint vert_shader = compileShader(GL_VERTEX_SHADER, vert_source);
    GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER, frag_source);
    GLuint program = createLinkedProgram({vert_shader, frag_shader});
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    return program;
}


GLuint loadPathTracerProgram(int max_recursion_depth) {
    std::string raw_source(reinterpret_cast<char*>(compute_shader_source), compute_shader_source_len);

    std::string flags = std::format("#define MAX_RECURSION_DEPTH {}\n", max_recursion_depth);
    raw_source.insert(raw_source.find_first_of('\n') + 1, flags);
    GLuint compute_shader = compileShader(GL_COMPUTE_SHADER, raw_source);
    GLuint program = createLinkedProgram({compute_shader});
    glDeleteShader(compute_shader);
    return program;
}
