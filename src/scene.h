#ifndef SCENE_H_
#define SCENE_H_

#include "GL/glew.h"

#include "camera.h"


class Scene {
public:
    Scene(void);
    GLuint renderAsTexture(GLuint texture_width, GLuint texture_height);

private:
    Camera camera;
};

#endif
