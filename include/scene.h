#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "hittable_list.h"
#include "shaders/compute/macros.h"


struct alignas(16) SceneUniform {
    alignas(4)  int n_objects;
    alignas(16) glm::vec4 world_iframe;
    alignas(16) CameraUniform camera_uniform;
    alignas(16) ObjectUniform objects[MAX_OBJECTS];
    alignas(16) MaterialUniform materials[MAX_MATERIALS];
};


class Scene {
public:
    Scene(void);
    GLuint renderAsTexture();
    void toUniform(SceneUniform &scene_uniform) const;

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
