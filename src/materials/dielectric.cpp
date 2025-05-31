#include "materials.h"

#include <cmath>

#include "glm/gtc/epsilon.hpp"

#include "constants.h"
#include "randutil.h"
#include "shaders/compute/materials/material_types.h"


Dielectric::Dielectric(const float &_eta): eta(_eta) {}


bool Dielectric::scatter(HitRecord &rec) const {
    rec.attenuation = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float real_eta = rec.front_face ? (1.0f / eta) : eta;
    glm::vec3 refracted = glm::refract(rec.ray.direction(), rec.normal, real_eta);
    glm::vec3 reflected = glm::reflect(rec.ray.direction(), rec.normal);
    if (glm::epsilonEqual(glm::length(refracted), 0.0f, kEpsilon)) {
        // total internal reflection
        rec.scattered = Ray(rec.ray.origin(), reflected, rec.ray.referenceFramePtr());
    } else {
        float cosine = glm::dot(-rec.ray.direction(), rec.normal);
        float r = reflectance(glm::min(cosine, 1.0f), real_eta);
        // reflect with probability (r), refract with probability (1 - r)
        float rand_sample = rand_unit_positive<float>();
        if (rand_sample < r) {
            rec.scattered = Ray(rec.ray.origin(), reflected, rec.ray.referenceFramePtr());
        } else {
            rec.scattered = Ray(rec.ray.origin(), refracted, rec.ray.referenceFramePtr());
        }
    }
    return true;
}


float Dielectric::reflectance(const float &cosine, const float &eta) {
    float r0 = (1 - eta) / (1 + eta);
    r0 = r0 * r0;
    return r0 + (1 - r0) * (float)std::pow((1.0f - cosine), 5.0f);
}


void Dielectric::toUniform(MaterialUniform &material_uniform) const {
    material_uniform.material_type = MATERIAL_TYPE_DIELECTRIC;
    material_uniform.dielectric = (DielectricUniform) {
        .eta = eta,
    };
}
