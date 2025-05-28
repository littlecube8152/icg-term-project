#include <iostream>
#include <string>
#include <filesystem>
#include <format>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <utility>
#include <numeric>

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GLFW/glfw3.h"

#include "constants.h"
#include "renderer.h"
#include "scene.h"
#include "shaders.h"
#include "exporter.h"
#include "arguments.h"
#include "window.h"

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

std::vector<std::condition_variable> texture_free_cv;
std::vector<TextureStatus> texture_status;
std::vector<int> texture_drawing;
std::mutex texture_mutex; // protects frame_index, must be a single one.
bool termination = false;
int next_draw_frame = 0;

void workerRoutine(Window worker_window, Renderer &renderer, const Scene &scene, int worker_index, int max_frame)
{
    glfwMakeContextCurrent(worker_window);

    // render the frame
    while (true)
    {
        std::unique_lock lock(texture_mutex);
        auto pred = [worker_index, max_frame]()
        { return texture_status[worker_index] == TEXTURE_FREE ||
                 termination == true ||
                 next_draw_frame >= max_frame; };
        texture_free_cv[worker_index].wait(lock, pred);

        if (termination == true || next_draw_frame >= max_frame)
            break;

        texture_status[worker_index] = TEXTURE_INITIALIZING;
        texture_drawing[worker_index] = next_draw_frame;
        ++next_draw_frame;
        lock.unlock();

        renderer.dispatchRenderFrame(scene, texture_drawing[worker_index]);

        lock.lock();
        texture_status[worker_index] = TEXTURE_DISPATCHED;
    }
}

int main(int argc, char *argv[])
{
    // parse command-line arguments
    ArgumentParser arguments;
    arguments.parse(argc, argv);

    // suppress FFMpeg library logs
    av_log_set_level(AV_LOG_QUIET);

    // initialize GLFW (with OpenGL context)

    if (!glfwInit())
    {
        const char *description;
        int code = glfwGetError(&description);
        throw std::runtime_error(std::format("failed to init GLFW: error code {} ({})", code, std::string(description)));
    }

    Window window(kWindowWidth, kWindowHeight, !arguments.getWindowless());
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    std::cerr << "Generating scene" << std::endl;
    SceneRelativisticMovementDemo scene(arguments, 2e-9f);

    const int thread_numbers = arguments.getWorkerCount();
    std::vector<std::shared_ptr<Renderer>> renderers;
    std::vector<std::thread> worker_threads;
    texture_status.resize(thread_numbers, TEXTURE_DRAWING);                 // prevent start
    texture_free_cv = std::vector<std::condition_variable>(thread_numbers); // it can only be default constructed
    texture_drawing.resize(thread_numbers, -1);
    for (int i = 0; i < thread_numbers; i++)
    {
        renderers.emplace_back(std::make_shared<Renderer>(arguments, i));
        Window worker_window(arguments.getWidth(), arguments.getHeight(), false, window);
        glfwMakeContextCurrent(NULL);
        worker_threads.emplace_back(workerRoutine, worker_window, std::ref(*renderers[i]), std::cref(scene), i, arguments.getTotalFrames());
        glfwMakeContextCurrent(window);
    }

    for (int i = 0; i < thread_numbers; i++)
    {
        std::unique_lock lock(texture_mutex);
        texture_status[i] = TEXTURE_FREE;
        lock.unlock();
        texture_free_cv[i].notify_one();
    }
    // main loop
    std::filesystem::create_directory("outputs");
    MKVExporter video_exporter(arguments);
    video_exporter.open(std::filesystem::path("outputs") / "video.mkv");

    int next_display_frame = 0;
    while (!window_should_close && !glfwWindowShouldClose(window) &&
           !(next_display_frame == arguments.getTotalFrames() && arguments.getWindowless()))
    {
        std::unique_lock lock(texture_mutex);

        for (int i = 0; i < thread_numbers; i++)
            if (texture_status[i] == TEXTURE_DISPATCHED &&
                texture_drawing[i] == next_display_frame &&
                renderers[i]->pollFrame())
            {
                texture_status[i] = TEXTURE_DRAWING;

                glClear(GL_COLOR_BUFFER_BIT);
                renderers[i]->drawFrame();
                auto pixels = renderers[i]->dumpPixelFromTexture();
                if (next_display_frame == 0)
                {
                    std::filesystem::create_directory("outputs");
                    saveToPNG((std::filesystem::path("outputs") / "screen.png").string(),
                              arguments.getWidth(), arguments.getHeight(), pixels);
                }
                video_exporter.addFrame(pixels);
                next_display_frame++;
                std::cerr << std::format("Drawing Frames {} / {}\n", next_display_frame, arguments.getTotalFrames());
                if (next_display_frame == arguments.getTotalFrames())
                {
                    video_exporter.close();
                    std::cerr << "Done!\n";
                }

                glfwSwapBuffers(window);
                texture_status[i] = TEXTURE_FREE;
                texture_free_cv[i].notify_one();
            }
        lock.unlock();

        glfwPollEvents();
    }

    std::unique_lock lock(texture_mutex);
    termination = true;
    lock.unlock();
    for (int i = 0; i < thread_numbers; i++)
    {
        texture_free_cv[i].notify_one();
        worker_threads[i].join();
    }

    glfwTerminate();
    return 0;
}