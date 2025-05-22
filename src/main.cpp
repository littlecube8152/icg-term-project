#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std::string_literals;

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GLFW/glfw3.h"

#include "constants.h"
#include "renderer.h"
#include "scene.h"
#include "shaders.h"
#include "writer.h"

bool window_should_close = false;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    [](...){}(window, scancode, mods); // modern way to supress unused variable warning
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        window_should_close = true;
}

int main(void)
{
    // initialize glfw, the window, and OpenGL context
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(kWindowWidth, kWindowHeight, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    std::cerr << "Generating scene" << std::endl;
    SceneRelativisticMovementTest scene(kWindowWidth, kWindowHeight, 1e-7f, AVRational{1, 30});

    // render the frame
    std::cerr << "Rendering" << std::endl;
    Renderer renderer(kWindowWidth, kWindowHeight);
    renderer.renderFrame(scene, 0);

    bool saved = false;
    // main loop
    while (!window_should_close && !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.drawFrame();

        if (!saved)
        {
            std::filesystem::create_directory("outputs");
            saveToPNG("outputs/screen.png", kWindowWidth, kWindowHeight);
            saved = true;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}