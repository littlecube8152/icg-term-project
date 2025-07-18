#include "materials.h"

#include "randutil.h"
#include "vecutil.h"
#include "shaders/compute/materials/material_types.h"


Metal::Metal(const glm::vec4 &_albedo, const float &_fuzz): albedo(_albedo), fuzz(_fuzz) {}


bool Metal::scatter(HitRecord &rec) const {
    glm::vec3 reflected = glm::reflect(rec.ray.direction(), rec.normal);
    reflected = default_normalize(reflected + fuzz * rand_unit_length<glm::vec3>(), reflected);
    rec.scattered = Ray(rec.ray.origin(), reflected, rec.ray.referenceFramePtr());
    rec.attenuation = albedo;
    return glm::dot(reflected, rec.normal) > 0.0f;
}


void Metal::toUniform(MaterialUniform &material_uniform) const {
    material_uniform.material_type = MATERIAL_TYPE_METAL;
    material_uniform.metal = (MetalUniform) {
        .albedo = albedo,
        .fuzz = fuzz,
    };
}
