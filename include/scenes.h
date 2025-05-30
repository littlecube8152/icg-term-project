#ifndef SCENES_H_
#define SCENES_H_


#include "scene.h"
#include "arguments.h"

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

class SceneTerellRotationTest: public Scene {
public:
    SceneTerellRotationTest(const ArgumentParser &options, float time_scale);
};

#endif
