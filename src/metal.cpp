#include "metal.h"


Metal::Metal(const glm::vec4 &_albedo): albedo(_albedo) {}


bool Metal::scatter(const Ray &r_in, HitRecord &rec) const {
    glm::vec3 reflected = glm::reflect(r_in.direction(), rec.normal);
    rec.scattered = Ray(rec.p, reflected);
    rec.attenuation = albedo;
    return true;
}
