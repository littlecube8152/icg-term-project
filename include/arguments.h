#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include "constants.h"
#include "libavcodec/avcodec.h"

struct ArgumentParser
{
private:
    int option_depth = 32;
    int option_fps = 30;
    int option_length = 2;
    int option_samples = 4;
    int option_resolution = kWindowHeight;

public:
    void parse(int argc, char *argv[]);

    /// Raw options

    // Get maximum recursion depth of ray-tracer.
    int getDepthOption() const { return option_depth; }
    // Get FPS.
    int getFPSOption() const { return option_fps; }
    // Get samples per unit length (sqrt sample per pixel).
    int getSampleOption() const { return option_samples; }

    // Computed options

    // Get total frames to render.
    int getTotalFrames() const { return option_fps * option_length; }
    // Get time base.
    AVRational getTimeBase() const { return AVRational{1, option_fps}; }
};

#endif // ARGUMENTS_H_
