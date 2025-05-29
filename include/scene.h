#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "hittable_list.h"
#include "arguments.h"
#include "shader_data.h"


class Scene {
public:
    Scene(void);
    GLuint renderAsTexture(int frame_number);
    void toUniform(SceneUniformCollector &collector) const;
    float getSceneTime(int frame_number) const;

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

class SceneTeapotDemo: public Scene {
public:
    SceneTeapotDemo(const ArgumentParser &options, float time_scale);
};

class SceneRelativisticMovementDemo: public Scene {
public:
    SceneRelativisticMovementDemo(const ArgumentParser &options, float time_scale);
};

#endif
