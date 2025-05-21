#ifndef MATERIALS_H_
#define MATERIALS_H_

#include "glm/glm.hpp"

#include "hit_record.h"
#include "ray.h"


struct alignas(16) MetalUniform {
    alignas(16) glm::vec4 albedo;
    alignas(4)  float fuzz;
};

struct alignas(16) LambertianUniform {
    alignas(16) glm::vec4 albedo;
};

struct alignas(16) DielectricUniform {
    alignas(4) float eta;
};

struct alignas(16) MaterialUniform {
    alignas(4)  int material_type;
    alignas(16) MetalUniform metal;
    alignas(16) LambertianUniform lambertian;
    alignas(16) DielectricUniform dielectric;
};


class Material {
public:
    Material();
    const int& getId() const;

    virtual MaterialUniform toUniform() const = 0;
    virtual bool scatter(const Ray &r_in, HitRecord &rec) const = 0;

private:
    int material_id;
};

class Metal: public Material {
public:
    Metal(const glm::vec4 &albedo, const float &fuzz);
    MaterialUniform toUniform() const override;
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    glm::vec4 albedo;
    float fuzz;
};

class Dielectric: public Material {
public:
    Dielectric(const float &eta);
    MaterialUniform toUniform() const override;
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
    MaterialUniform toUniform() const override;
    bool scatter(const Ray &r_in, HitRecord &rec) const override;

private:
    glm::vec4 albedo;
}; 

#endif
