#include "materials.h"

#include "glm/gtc/epsilon.hpp"

#include "randutil.h"
#include "vecutil.h"
#include "constants.h"


Lambertian::Lambertian(const glm::vec4 &_albedo): albedo(_albedo) {}


bool Lambertian::scatter(HitRecord &rec) const {
    auto scatter_dir = rec.normal + rand_unit_length<glm::vec3>();
    scatter_dir = default_normalize(scatter_dir, rec.normal);
    rec.scattered = Ray(rec.ray.origin(), scatter_dir, rec.ray.referenceFramePtr());
    rec.attenuation = albedo;
    return true;
}
