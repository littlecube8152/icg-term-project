#include "shaders.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <exception>

#include "shaders/fragment.frag.h"
#include "shaders/vertex.vert.h"


GLuint compileShader(GLuint type, const char* source, const int &source_length) {
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


GLuint loadShaderProgram(void) {
    GLuint program = glCreateProgram();
    GLuint vert_shader = compileShader(GL_VERTEX_SHADER, (const char*)shaders_vertex_vert, shaders_vertex_vert_len);
    GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER, (const char*)shaders_fragment_frag, shaders_fragment_frag_len);

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
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

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}
