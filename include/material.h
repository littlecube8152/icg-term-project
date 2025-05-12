#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "glm/glm.hpp"

#include "hittable.h"
#include "ray.h"


class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &r_in, HitRecord &rec) const {
        [](...){}(r_in, rec);
        return false;
    }
};

#endif
