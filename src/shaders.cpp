#include "shaders.h"

#include <string>
#include <unordered_set>
#include <functional>
#include <format>
#include <fstream>
#include <stdexcept>
#include <memory>


static std::string readShaderSource(const std::string &main_shader_name) {
    std::string shader_source = "";
    std::unordered_set<std::string> files_loading;
    std::unordered_set<std::string> files_loaded;

    std::function<void(const std::string&)> dfs_shader_source = [&](const std::string &shader_name) -> void {
        files_loading.insert(shader_name);

        // open the source file to read from
        std::string shader_path = "shaders/" + shader_name;
        std::ifstream fin(shader_path);
        if (!fin.is_open()) {
            std::string err_msg = std::format("Failed to read shader source: cannot open source file {}", shader_path);
            throw std::runtime_error(err_msg);
        }

        int lineno = 0;
        std::string line;
        while (getline(fin, line)) {
            lineno++;
            if (line.substr(0, 8) == "#include") {
                // extract the filename to be included
                int l = 8, r = (int)line.size() - 1;
                while (l < (int)line.size() && line[l] != '"') l++;
                while (r >= 0 && line[r] != '"') r--;

                // empty filename or unmatched quotes
                if (l >= r) {
                    std::string err_msg = std::format("Failed to read shader source: invalid include format (file {}, line {})", shader_name, lineno);
                    throw std::runtime_error(err_msg);
                }

                std::string included = line.substr(l + 1, r - l - 1);
                if (files_loading.count(included)) {
                    // trying to include a file that caused inclusion of this file (circular inclusion)
                    std::string err_msg = std::format("Failed to read shader source: circular include detected (file {}, line {})", shader_name, lineno);
                    throw std::runtime_error(err_msg);
                } else if (files_loaded.count(included)) {
                    // avoid including the same file multiple times
                    shader_source += std::format("// include {} from {}, line {} (already included)\n", included, shader_name, lineno);
                } else {
                    // include the file recursively
                    shader_source += std::format("// include {} from {}, line {}\n", included, shader_name, lineno);
                    dfs_shader_source(included);
                    shader_source += std::format("// end of {}\n", included);
                }
            } else {
                shader_source += line + "\n";
            }
        }

        files_loading.erase(shader_name);
        files_loaded.insert(shader_name);
    };

    dfs_shader_source(main_shader_name);
    return shader_source;
}


static GLuint compileShader(GLuint type, const std::string shader_name) {
    const std::string shader_source = readShaderSource(shader_name);
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
    GLuint vert_shader = compileShader(GL_VERTEX_SHADER, "vertex.vert");
    GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER, "fragment.frag");
    GLuint program = createLinkedProgram({vert_shader, frag_shader});
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    return program;
}


GLuint loadPathTracerProgram(void) {
    GLuint compute_shader = compileShader(GL_COMPUTE_SHADER, "compute/main.comp");
    GLuint program = createLinkedProgram({compute_shader});
    glDeleteShader(compute_shader);
    return program;
}
