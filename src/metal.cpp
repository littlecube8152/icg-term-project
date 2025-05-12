#include "metal.h"

#include "randutil.h"
#include "vecutil.h"


Metal::Metal(const glm::vec4 &_albedo, const float &_fuzz): albedo(_albedo), fuzz(_fuzz) {}


bool Metal::scatter(const Ray &r_in, HitRecord &rec) const {
    glm::vec3 reflected = glm::reflect(r_in.direction(), rec.normal);
    reflected = default_normalize(reflected + fuzz * rand_unit_length<glm::vec3>(), reflected);
    rec.scattered = Ray(rec.p, reflected);
    rec.attenuation = albedo;
    return glm::dot(reflected, rec.normal) > 0.0f;
}
