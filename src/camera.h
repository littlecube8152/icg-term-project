#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include "GL/gl.h"


class Camera {
public:
    GLuint image_width;
    GLuint image_height;
    std::vector<GLubyte> texture_data;

    GLuint renderAsTexture(void);
};

#endif
