#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

#include "glm/glm.hpp"

#include "material.h"


class Dielectric: public Material {
public:
    Dielectric(const float &eta);
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    // Refractive index relative to air
    // i.e. [n_{material} / n_{air}] or [v_{air} / v_{material}]
    float eta;
};

#endif