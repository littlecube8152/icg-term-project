#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "hittable_list.h"


class Scene {
public:
    Scene(void);
    GLuint renderAsTexture();

protected:
    Camera camera;
    HittableList world;
};


class SceneMaterialDemo: public Scene {
public:
    SceneMaterialDemo(GLuint texture_width, GLuint texture_height);
};


class SceneRandomBalls: public Scene {
public:
    SceneRandomBalls(GLuint texture_width, GLuint texture_height);
};

#endif
