#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "glm/glm.hpp"

#include "material.h"


class Lambertian: public Material {
public:
    Lambertian(const glm::vec4 &albedo);
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    glm::vec4 albedo;
};

#endif