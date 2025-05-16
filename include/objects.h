#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "glm/glm.hpp"

#include "hittable.h"
#include "material.h"


class Sphere: public Hittable {
public:
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> mat);
    bool hit(Ray &ray, HitRecord &record) const override;

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;
};


class Cube: public Hittable {
public:
    Cube(const glm::vec3 &corner, const float &side_length, std::shared_ptr<Material> mat);
    bool hit(Ray &ray, HitRecord &record) const override;

private:
    glm::vec3 corner, x_axis, y_axis, z_axis;
    std::shared_ptr<Material> mat;
};

#endif
