#include "materials.h"

#include "randutil.h"
#include "vecutil.h"


Metal::Metal(const glm::vec4 &_albedo, const float &_fuzz): albedo(_albedo), fuzz(_fuzz) {}


bool Metal::scatter(HitRecord &rec) const {
    glm::vec3 reflected = glm::reflect(rec.ray.direction(), rec.normal);
    reflected = default_normalize(reflected + fuzz * rand_unit_length<glm::vec3>(), reflected);
    rec.scattered = Ray(rec.ray.origin(), reflected, rec.ray.referenceFramePtr());
    rec.attenuation = albedo;
    return glm::dot(reflected, rec.normal) > 0.0f;
}
