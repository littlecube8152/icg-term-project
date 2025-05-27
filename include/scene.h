#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "hittable_list.h"
#include "shaders/compute/macros.h"
#include "arguments.h"


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
    SceneMaterialDemo(const ArgumentParser &options, float time_scale);
};


class SceneRandomBalls: public Scene {
public:
    SceneRandomBalls(const ArgumentParser &options, float time_scale);
};

class SceneRelativityTest: public Scene {
public:
    SceneRelativityTest(const ArgumentParser &options, float time_scale);
};

class SceneRelativisticMovementTest: public Scene {
public:
    SceneRelativisticMovementTest(const ArgumentParser &options, float time_scale);
};

class SceneRelativisticDopplerTest: public Scene {
public:
    SceneRelativisticDopplerTest(const ArgumentParser &options, float time_scale);
};

#endif
