#include "materials.h"

#include "glm/gtc/epsilon.hpp"

#include "randutil.h"
#include "vecutil.h"
#include "constants.h"
#include "shaders/compute/materials/material_types.h"


Lambertian::Lambertian(const glm::vec4 &_albedo): albedo(_albedo) {}


bool Lambertian::scatter(HitRecord &rec) const {
    auto scatter_dir = rec.normal + rand_unit_length<glm::vec3>();
    scatter_dir = default_normalize(scatter_dir, rec.normal);
    rec.scattered = Ray(rec.ray.origin(), scatter_dir, rec.ray.referenceFramePtr());
    rec.attenuation = albedo;
    return true;
}


void Lambertian::toUniform(MaterialUniform &material_uniform) const {
    material_uniform.material_type = MATERIAL_TYPE_LAMBERTIAN;
    material_uniform.lambertian = (LambertianUniform) {
        .albedo = albedo,
    };
}
