#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "hittable_list.h"


class Scene {
public:
    Scene(void);
    GLuint renderAsTexture(int frame_number);

protected:
    Camera camera;
    HittableList world;
};


class SceneMaterialDemo: public Scene {
public:
    SceneMaterialDemo(GLuint texture_width, GLuint texture_height, float time_scale, AVRational time_base);
};


class SceneRandomBalls: public Scene {
public:
    SceneRandomBalls(GLuint texture_width, GLuint texture_height, float time_scale, AVRational time_base);
};

class SceneRelativityTest: public Scene {
public:
    SceneRelativityTest(GLuint texture_width, GLuint texture_height, float time_scale, AVRational time_base);
};

class SceneRelativisticMovementTest: public Scene {
public:
    SceneRelativisticMovementTest(GLuint texture_width, GLuint texture_height, float time_scale, AVRational time_base);
};


#endif
