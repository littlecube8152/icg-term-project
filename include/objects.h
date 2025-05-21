#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "glm/glm.hpp"

#include "hit_record.h"
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
    Object(std::shared_ptr<Material> _mat): mat(_mat) {}
    std::shared_ptr<Material> getMaterial() const { return mat; }
    
    virtual bool hit(Ray&, HitRecord&) const = 0;
    virtual ObjectUniform toUniform() const = 0;

protected:
    std::shared_ptr<Material> mat;
};


class Sphere: public Object {
public:
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> mat);
    bool hit(Ray &ray, HitRecord &record) const override;
    ObjectUniform toUniform() const override;

private:
    glm::vec3 center;
    float radius;
};


class Cube: public Object {
public:
    Cube(const glm::vec3 &corner, const float &side_length, std::shared_ptr<Material> mat);
    bool hit(Ray &ray, HitRecord &record) const override;
    ObjectUniform toUniform() const override;

private:
    glm::vec3 corner, x_axis, y_axis, z_axis;
};

#endif
