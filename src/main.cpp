#include <iostream>
#include <string>
#include <filesystem>
#include <condition_variable>
#include <thread>
#include <mutex>

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

enum {
    TEXTURE_FREE,
    TEXTURE_INITIALIZING,
    TEXTURE_DISPATCHED,
    TEXTURE_DRAWING
};

std::condition_variable texture_free_cv;
std::mutex texture_mutex;
bool termination;
int texture_status;


int max_recursion_depth = 32;
void worker_routine(GLFWwindow *worker_window, Renderer &renderer)
{
    glfwMakeContextCurrent(worker_window);

    std::cerr << "Generating scene" << std::endl;
    SceneRelativisticMovementTest scene(kWindowWidth, kWindowHeight, 1e-7f, AVRational{1, 30}, max_recursion_depth);

    // render the frame
    for (int i = 0; i < 50; i++)
    {
        std::unique_lock lock(texture_mutex);
        texture_free_cv.wait(lock, [](){ return texture_status == TEXTURE_FREE || termination == true; });
        if (termination == true)
        {    
            lock.unlock();
            return;
        }
        texture_status = TEXTURE_INITIALIZING;
        lock.unlock();

        std::cerr << "Rendering" << std::endl;
        renderer.renderFrame(scene, i);
        
        lock.lock();
        texture_status = TEXTURE_DISPATCHED;
    }
}


int main(void)
{
    // initialize glfw, the window, and OpenGL context
    GLFWwindow *window, *worker_window;    
    
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(kWindowWidth, kWindowHeight, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    worker_window = glfwCreateWindow(kWindowWidth, kWindowHeight, "Hello World", NULL, window);

    Renderer renderer(kWindowWidth, kWindowHeight, max_recursion_depth);

    texture_status = TEXTURE_FREE;
    termination = false;

    bool saved = false;

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    
    glfwMakeContextCurrent(NULL);
    std::thread worker_thread(worker_routine, worker_window, std::ref(renderer));
    glfwMakeContextCurrent(window);

    // main loop
    while (!window_should_close && !glfwWindowShouldClose(window)) 
    {
        std::unique_lock lock(texture_mutex);

        if (texture_status == TEXTURE_DISPATCHED && renderer.pollFrame())
        {
            texture_status = TEXTURE_DRAWING;
            glClear(GL_COLOR_BUFFER_BIT);
            std::cerr << "Drawing" << std::endl;
            renderer.drawFrame();
            if (!saved)
            {
                std::filesystem::create_directory("outputs");
                saveToPNG((std::filesystem::path("outputs") / "screen.png").string(), kWindowWidth, kWindowHeight);
                saved = true;
            }
            glfwSwapBuffers(window);
            texture_status = TEXTURE_FREE;
        }
        lock.unlock();
        texture_free_cv.notify_one();

        glfwPollEvents();
    }

    std::unique_lock lock(texture_mutex);
    termination = true;
    lock.unlock();
    texture_free_cv.notify_one();
    worker_thread.join();

    glfwTerminate();
    return 0;
}