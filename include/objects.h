#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "glm/glm.hpp"

#include "hit_record.h"
#include "inertial.h"
#include "materials.h"


struct alignas(16) SphereUniform {
    alignas(16) glm::vec4 center;
    alignas(4)  float radius;
    alignas(4)  int material_id;
};

struct alignas(16) CubeUniform {
    alignas(16) glm::vec4 center;
    alignas(4)  float side_length;
    alignas(4)  int material_id;
};

struct alignas(16) ObjectUniform {
    alignas(4)  int object_type;
    alignas(16) SphereUniform sphere;
    alignas(16) CubeUniform cube;
};


class Object {
public:
    Object(std::shared_ptr<Material> material, InertialFrame inertial_frame);
    std::shared_ptr<Material> getMaterial() const { return mat; }
    
    virtual bool hit(Ray&, HitRecord&) const = 0;
    virtual void toUniform(ObjectUniform &object_uniform) const = 0;

protected:
    std::shared_ptr<Material> mat;
    InertialFrame frame;
};
class Sphere: public Object {
public:
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> material, InertialFrame frame);
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> material);
    bool hit(Ray &ray, HitRecord &record) const override;
    void toUniform(ObjectUniform &object_uniform) const override;

private:
    glm::vec3 center;
    float radius;
};


class Cube: public Object {
public:
    Cube(const glm::vec3 &center, const float &side_length, std::shared_ptr<Material> material, InertialFrame frame);
    Cube(const glm::vec3 &center, const float &side_length, std::shared_ptr<Material> material);
    bool hit(Ray &ray, HitRecord &record) const override;
    void toUniform(ObjectUniform &object_uniform) const override;

private:
    glm::vec3 corner, x_axis, y_axis, z_axis;
};

#endif
