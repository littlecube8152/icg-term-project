#ifndef MATERIALS_H_
#define MATERIALS_H_

#include "glm/glm.hpp"

#include "hit_record.h"
#include "ray.h"
#include "shader_data.h"


class Material {
public:
    Material();
    const int& getId() const;

    virtual void toUniform(MaterialUniform &material_uniform) const = 0;
    virtual bool scatter(HitRecord &rec) const = 0;

private:
    int material_id;
};

class Metal: public Material {
public:
    Metal(const glm::vec4 &albedo, const float &fuzz);
    void toUniform(MaterialUniform &material_uniform) const override;
    bool scatter(HitRecord &rec) const override;

private:
    glm::vec4 albedo;
    float fuzz;
};

class Dielectric: public Material {
public:
    Dielectric(const float &eta);
    void toUniform(MaterialUniform &material_uniform) const override;
    bool scatter(HitRecord &rec) const override;

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
    void toUniform(MaterialUniform &material_uniform) const override;
    bool scatter(HitRecord &rec) const override;

private:
    glm::vec4 albedo;
}; 

#endif
