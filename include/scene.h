#ifndef SCENE_H_
#define SCENE_H_

#include "GL/gl.h"

#include "camera.h"
#include "hittable_list.h"
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

#endif
