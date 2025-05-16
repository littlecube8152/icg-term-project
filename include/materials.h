#ifndef MATERIALS_H_
#define MATERIALS_H_

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

class Metal: public Material {
public:
    Metal(const glm::vec4 &albedo, const float &fuzz);
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    glm::vec4 albedo;
    float fuzz;
};

class Dielectric: public Material {
public:
    Dielectric(const float &eta);
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    // Refractive index relative to air
    // i.e. [n_{material} / n_{air}] or [v_{air} / v_{material}]
    float eta;

    // Schlick's approximation for reflectance
    static float reflectance(const float &cosine, const float &eta);
};

   
class Lambertian: public Material {
public:
    Lambertian(const glm::vec4 &albedo);
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    glm::vec4 albedo;
}; 

#endif
