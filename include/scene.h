#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "hittable_list.h"


struct alignas(16) SceneUniform {
    alignas(16) CameraUniform camera_uniform;
};

class Scene {
public:
    Scene(void);
    GLuint renderAsTexture();
    SceneUniform toUniform() const;

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

class SceneRelativityTest: public Scene {
public:
    SceneRelativityTest(GLuint texture_width, GLuint texture_height);
};

#endif
