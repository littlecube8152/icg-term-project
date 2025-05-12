#ifndef SCENE_H_
#define SCENE_H_

#include "GL/glew.h"

#include "camera.h"
#include "hittable_list.h"


class Scene {
public:
    Scene(void);
    GLuint renderAsTexture(GLuint texture_width, GLuint texture_height);

protected:
    Camera camera;
    HittableList world;
};


class SceneMaterialDemo: public Scene {
public:
    SceneMaterialDemo(void);
};


class SceneRandomBalls: public Scene {
public:
    SceneRandomBalls(void);
};

#endif
