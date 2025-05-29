#ifndef RENDERER_H_
#define RENDERER_H_

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"

#include "scene.h"
#include "arguments.h"
#include "scene.h"
#include "color.h"

#include <vector>
#include <cstdint>

class Renderer {

public:
    
    Renderer() = delete;
    Renderer(const Renderer &renderer) = delete;
    Renderer(const ArgumentParser &options, const Scene &scene, int texture_unit_index);
    ~Renderer();

    // collect and send scene data to GPU
    // call once before renderFrame
    void sendSceneData();

    // render the scene into a texture with compute shaders
    void dispatchRenderFrame(int frame_number);

    // draw the texture onto the screen
    void drawFrame(void);

    // poll and receive whether the computation result is ready
    bool pollFrame();

    // this does what you thinks
    std::vector<uint8_t> dumpPixelFromTexture();

private:

    const Scene &scene;

    // dimensions of the rendering area
    GLuint window_width;
    GLuint window_height;
    // compute shader program responsible for rendering the scene
    GLuint path_tracer;
    // texture holding the rendered image, and the texture unit to bind to
    GLuint texture_unit;
    GLuint texture;
    // VAO and vertex/fragment shaders responsible for drawing the texture onto the screen
    GLuint window_vao;
    GLuint shader;
    // synchronizes computation
    GLsync compute_fence;

    std::vector<GLuint> gpu_buffers;

};

#endif
