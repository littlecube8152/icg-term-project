#ifndef SPHERE_H_
#define SPHERE_H_

#include "glm/glm.hpp"

#include "hittable.h"
#include "material.h"


class Sphere: public Hittable {
public:
    Sphere(const glm::vec3 &center, const float &radius, std::shared_ptr<Material> mat);
    bool hit(const Ray &ray, const InertialFrame &frame, const Interval &valid_interval, HitRecord &record) const override;

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;
};

#endif
