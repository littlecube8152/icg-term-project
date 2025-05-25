#include <iostream>
#include <string>
#include <filesystem>
#include <format>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <utility>

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GLFW/glfw3.h"

#include "constants.h"
#include "renderer.h"
#include "scene.h"
#include "shaders.h"
#include "exporter.h"
#include "arguments.h"

using namespace std::string_literals;

bool window_should_close = false;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    [](...) {}(window, scancode, mods); // modern way to supress unused variable warning
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        window_should_close = true;
}

enum TextureStatus
{
    TEXTURE_FREE,
    TEXTURE_INITIALIZING,
    TEXTURE_DISPATCHED,
    TEXTURE_DRAWING
};

std::condition_variable texture_free_cv;
std::mutex texture_mutex;
bool termination;
TextureStatus texture_status;
int frame_index = -1;


void worker_routine(GLFWwindow *worker_window, Renderer &renderer, ArgumentParser &arguments)
{
    glfwMakeContextCurrent(worker_window);

    std::cerr << "Generating scene" << std::endl;
    SceneRelativisticMovementTest scene(kWindowWidth, kWindowHeight, 1e-8f, arguments);

    // render the frame
    for (int i = 0; i < arguments.getTotalFrames(); i++)
    {
        std::unique_lock lock(texture_mutex);
        texture_free_cv.wait(lock, []()
                             { return texture_status == TEXTURE_FREE || termination == true; });
        if (termination == true)
        {
            lock.unlock();
            return;
        }
        texture_status = TEXTURE_INITIALIZING;
        lock.unlock();

        renderer.renderFrame(scene, i);

        lock.lock();
        texture_status = TEXTURE_DISPATCHED;
        frame_index = i;
    }
}

int main(int argc, char *argv[])
{
    // parse command-line arguments
    ArgumentParser arguments;
    arguments.parse(argc, argv);

    // suppress FFMpeg library logs
    av_log_set_level(AV_LOG_QUIET);

    // initialize glfw, the window, and OpenGL context
    GLFWwindow *window, *worker_window;

    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(kWindowWidth, kWindowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    worker_window = glfwCreateWindow(kWindowWidth, kWindowHeight, "Hello World", NULL, window);

    Renderer renderer(kWindowWidth, kWindowHeight, arguments.getDepthOption());

    texture_status = TEXTURE_FREE;
    termination = false;

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glfwMakeContextCurrent(NULL);
    std::thread worker_thread(worker_routine, worker_window, std::ref(renderer), std::ref(arguments));
    glfwMakeContextCurrent(window);

    // main loop
    std::filesystem::create_directory("outputs");
    MKVExporter video_exporter(kWindowWidth, kWindowHeight, arguments.getTimeBase());
    video_exporter.open(std::filesystem::path("outputs") / "video.mkv");

    while (!window_should_close && !glfwWindowShouldClose(window))
    {
        std::unique_lock lock(texture_mutex);

        if (texture_status == TEXTURE_DISPATCHED && renderer.pollFrame())
        {
            texture_status = TEXTURE_DRAWING;
            std::cerr << std::format("Drawing Frames {} / {}\n", frame_index + 1, arguments.getTotalFrames());

            glClear(GL_COLOR_BUFFER_BIT);
            renderer.drawFrame();

            auto pixels = dumpPixelFromGL(kWindowWidth, kWindowHeight);
            if (frame_index == 0)
            {
                std::filesystem::create_directory("outputs");
                saveToPNG((std::filesystem::path("outputs") / "screen.png").string(),
                          kWindowWidth, kWindowHeight,
                          dumpPixelFromGL(kWindowWidth, kWindowHeight));
            }
            video_exporter.addFrame(pixels);
            if (frame_index == arguments.getTotalFrames() - 1)
            {
                video_exporter.close();
                std::cerr << "Done!\n";
            }

            glfwSwapBuffers(window);
            texture_status = TEXTURE_FREE;
            texture_free_cv.notify_one();
        }
        lock.unlock();

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