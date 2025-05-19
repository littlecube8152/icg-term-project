#include <iostream>
#include <string>
#include <chrono>

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GLFW/glfw3.h"

#include "constants.h"
#include "renderer.h"
#include "scene.h"


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
    SceneRandomBalls scene(kWindowWidth, kWindowHeight);

    std::cerr << "Rendering" << std::endl;
    int frame_count = 0;
    // render the frame and record time usage
    std::chrono::_V2::system_clock::time_point start, end;
    start = std::chrono::high_resolution_clock::now();
    Renderer renderer(kWindowWidth, kWindowHeight);
    renderer.renderFrame(scene);

    // main loop
    while (!window_should_close && !glfwWindowShouldClose(window)) {
        frame_count++;
        if (frame_count == 2) {
            end = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> elapsed = end - start;
            std::cerr << "Frame time: " << elapsed << std::endl;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        renderer.drawFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}