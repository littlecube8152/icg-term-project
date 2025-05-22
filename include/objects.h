#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "glm/glm.hpp"

#include "inertial.h"
#include "materials.h"
#include "object.h"


class Sphere: public Object {
public:
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> material, InertialFrame frame);
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> material);
    bool hit(Ray &ray, HitRecord &record) const override;

private:
    glm::vec3 center;
    float radius;
};


class Cube: public Object {
public:
    Cube(const glm::vec3 &center, const float &side_length, std::shared_ptr<Material> material, InertialFrame frame);
    Cube(const glm::vec3 &center, const float &side_length, std::shared_ptr<Material> material);
    bool hit(Ray &ray, HitRecord &record) const override;

private:
    glm::vec3 corner;
    glm::vec3 x_axis, y_axis, z_axis;
};

#endif
