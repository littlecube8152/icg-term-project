#include "dielectric.h"

#include "glm/gtc/epsilon.hpp"

#include "constants.h"


Dielectric::Dielectric(const float &_eta): eta(_eta) {}


bool Dielectric::scatter(const Ray &r_in, HitRecord &rec) const {
    rec.attenuation = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float real_eta = rec.front_face ? (1.0f / eta) : eta;
    glm::vec3 refracted = glm::refract(r_in.direction(), rec.normal, real_eta);
    if (glm::epsilonEqual(glm::length(refracted), 0.0f, kEpsilon)) {
        // total internal reflection
        rec.scattered = Ray(rec.p, glm::reflect(r_in.direction(), rec.normal));
    } else {
        rec.scattered = Ray(rec.p, refracted);
    }
    return true;
}
