#include <iostream>
#include <string>
#include <filesystem>

using namespace std::string_literals;

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GLFW/glfw3.h"

#include "scene.h"
#include "shaders.h"
#include "writer.h"

bool window_should_close = false;


GLuint createWindowVao(GLuint shader) {
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
    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

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
    glVertexAttribPointer(a_texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return vao;
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    [](...){}(window, scancode, mods); // modern way to supress unused variable warning
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        window_should_close = true;
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLuint window_width = 960;
    GLuint window_height = 540;
    window = glfwCreateWindow(window_width, window_height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLuint shader = loadShaderProgram();
    glUseProgram(shader);

    GLuint vao = createWindowVao(shader);
    glBindVertexArray(vao);

    // SceneRandomBalls scene(window_width, window_height);
    // SceneMaterialDemo scene(window_width, window_height);
    // SceneRelativityTest scene(window_width, window_height);
    SceneRelativisticMovementTest scene(window_width, window_height, 1e-7f, AVRational{1, 30});

    GLuint texture = scene.renderAsTexture(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint u_texture = glGetUniformLocation(shader, "u_texture");
    glUniform1i(u_texture, 0);

    glfwSetKeyCallback(window, keyCallback);

    bool saved = false;
    /* Loop until the user closes the window */
    while (!window_should_close && !glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (!saved)
        {
            std::filesystem::create_directory("outputs");
            saveToPNG("outputs/screen.png", window_width, window_height);
            saved = true;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}