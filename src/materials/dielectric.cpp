#include "materials.h"

#include "glm/gtc/epsilon.hpp"

#include "constants.h"
#include "randutil.h"


Dielectric::Dielectric(const float &_eta): eta(_eta) {}


bool Dielectric::scatter(const Ray &r_in, HitRecord &rec) const {
    rec.attenuation = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float real_eta = rec.front_face ? (1.0f / eta) : eta;
    glm::vec3 refracted = glm::refract(r_in.direction(), rec.normal, real_eta);
    glm::vec3 reflected = glm::reflect(r_in.direction(), rec.normal);
    if (glm::epsilonEqual(glm::length(refracted), 0.0f, kEpsilon)) {
        // total internal reflection
        rec.scattered = Ray(rec.p, reflected, r_in.referenceFramePtr());
    } else {
        float cosine = glm::dot(-r_in.direction(), rec.normal);
        float r = reflectance(glm::min(cosine, 1.0f), real_eta);
        // reflect with probability (r), refract with probability (1 - r)
        float rand_sample = rand_unit_positive<float>();
        if (rand_sample < r) {
            rec.scattered = Ray(rec.p, reflected, r_in.referenceFramePtr());
        } else {
            rec.scattered = Ray(rec.p, refracted, r_in.referenceFramePtr());
        }
    }
    return true;
}


float Dielectric::reflectance(const float &cosine, const float &eta) {
    float r0 = (1 - eta) / (1 + eta);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::powf((1 - cosine), 5);
}
