#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "hittable_list.h"
#include "shaders/compute/macros.h"


struct alignas(16) SceneUniform {
    alignas(4)  int n_objects;
    alignas(4)  float scene_time;
    alignas(16) glm::vec4 world_iframe;
    alignas(16) CameraUniform camera_uniform;
    alignas(16) ObjectUniform objects[MAX_OBJECTS];
    alignas(16) MaterialUniform materials[MAX_MATERIALS];
};


class Scene {
public:
    Scene(void);
    GLuint renderAsTexture(int frame_number);
    void toUniform(SceneUniform &scene_uniform, int frame_number) const;

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
