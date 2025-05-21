#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "glm/glm.hpp"

#include "hit_record.h"
#include "materials.h"


class Object {
public:
    virtual bool hit(Ray&, HitRecord&) const = 0;
};


class Sphere: public Object {
public:
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> mat);
    bool hit(Ray &ray, HitRecord &record) const override;

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;
};


class Cube: public Object {
public:
    Cube(const glm::vec3 &corner, const float &side_length, std::shared_ptr<Material> mat);
    bool hit(Ray &ray, HitRecord &record) const override;

private:
    glm::vec3 corner, x_axis, y_axis, z_axis;
    std::shared_ptr<Material> mat;
};

#endif
