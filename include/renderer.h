#ifndef RENDERER_H_
#define RENDERER_H_

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"


class Renderer {

public:
    
    Renderer() = delete;
    Renderer(const Renderer &renderer) = delete;
    Renderer(GLuint window_width, GLuint window_height);

    // render the scene into a texture with compute shaders
    // TODO: render the actual scene
    void renderFrame(void);

    // draw the texture onto the screen
    void drawFrame(void);

private:

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

};

#endif
