#ifndef METAL_H_
#define METAL_H_

#include "glm/glm.hpp"

#include "material.h"


class Metal: public Material {
public:
    Metal(const glm::vec4 &albedo, const float &fuzz);
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    glm::vec4 albedo;
    float fuzz;
};

#endif