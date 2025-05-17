#include "shaders.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <exception>
#include <initializer_list>

#include "shaders/fragment.frag.h"
#include "shaders/vertex.vert.h"
#include "shaders/compute/main.comp.h"


static GLuint compileShader(GLuint type, const char* source, const int &source_length) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, &source_length);
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
    GLuint vert_shader = compileShader(GL_VERTEX_SHADER, (const char*)shaders_vertex_vert, shaders_vertex_vert_len);
    GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER, (const char*)shaders_fragment_frag, shaders_fragment_frag_len);
    GLuint program = createLinkedProgram({vert_shader, frag_shader});
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    return program;
}


GLuint loadPathTracerProgram(void) {
    GLuint compute_shader = compileShader(GL_COMPUTE_SHADER, (const char*)shaders_compute_main_comp, shaders_compute_main_comp_len);
    GLuint program = createLinkedProgram({compute_shader});
    glDeleteShader(compute_shader);
    return program;
}
