#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <map>
#include <functional>

extern "C"
{
#include "libavcodec/avcodec.h"
}

struct ArgumentParser;

#include "constants.h"
#include "scene.h"

struct ArgumentParser
{
private:
    int option_depth = 32;
    int option_fps = 30;
    int option_length = 2;
    int option_samples = 4;
    int option_resolution = kWindowHeight;
    int option_crf = 18;
    int option_worker_count = 1;
    float option_time_ratio = 2e-9f;
    bool option_no_window = false;
    std::string option_scene = "";

    std::map<std::string, std::function<Scene(void)>> scene_constructors;
    void registerScene(std::function<Scene(void)> constructor, std::string name, bool set_default = false);

public:
    // Exists because initializer causes cyclic dependency
    void initSceneSelection();
    void parse(int argc, char *argv[]);

    /// Raw options

    // Get maximum recursion depth of ray-tracer.
    int getDepthOption() const { return option_depth; }
    // Get FPS.
    int getFPSOption() const { return option_fps; }
    // Get samples per unit length (sqrt sample per pixel).
    int getSampleOption() const { return option_samples; }
    // Get CRF (Constant Rate Factor)
    int getCRF() const { return option_crf; }
    // Get whether the program should operate without window.
    int getWindowless() const { return option_no_window; }
    // Get worker thread numbers.
    int getWorkerCount() const { return option_worker_count; }

    // Computed options

    // Get total frames to render.
    int getTotalFrames() const { return option_fps * option_length; }
    // Get time base.
    AVRational getTimeBase() const { return AVRational{1, option_fps}; }
    // Get height.
    int getHeight() const { return option_resolution; }
    // Get width.
    int getWidth() const { return option_resolution / 9 * 16; }
    // Get Scene.
    Scene getScene() { return scene_constructors[option_scene](); }
};

#endif // ARGUMENTS_H_
