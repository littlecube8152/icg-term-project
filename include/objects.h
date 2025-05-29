#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <filesystem>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

#include "hit_record.h"
#include "inertial.h"
#include "materials.h"


class Object {
public:
    Object(std::shared_ptr<Material> material, InertialFrame inertial_frame);
    std::shared_ptr<Material> getMaterial() const { return mat; }
    
    virtual bool hit(Ray&, HitRecord&) const = 0;
    virtual void toUniform(SceneUniformCollector &collector) const = 0;

protected:
    std::shared_ptr<Material> mat;
    InertialFrame frame;
};

class Sphere: public Object {
public:
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> material, InertialFrame frame);
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> material);
    bool hit(Ray &ray, HitRecord &record) const override;
    void toUniform(SceneUniformCollector &collector) const override;

private:
    glm::vec3 center;
    float radius;
};

class Cube: public Object {
public:
    Cube(const glm::vec3 &center, const float &side_length, std::shared_ptr<Material> material, InertialFrame frame);
    Cube(const glm::vec3 &center, const float &side_length, std::shared_ptr<Material> material);
    bool hit(Ray &ray, HitRecord &record) const override;
    void toUniform(SceneUniformCollector &collector) const override;

private:
    glm::vec3 corner, x_axis, y_axis, z_axis;
};

class ObjMesh : public Object {
public:
    ObjMesh(const std::filesystem::path &file_path, glm::vec3 translation, float scale, std::shared_ptr<Material> material, InertialFrame inertial_frame);
    bool hit(Ray& ray, HitRecord& record) const override;
    void toUniform(SceneUniformCollector &collector) const override;

private:
    std::filesystem::path file_path;
    glm::vec3 translation;
    float scale;
};

#endif
