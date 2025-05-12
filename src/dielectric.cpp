#include "dielectric.h"


Dielectric::Dielectric(const float &_eta): eta(_eta) {}


bool Dielectric::scatter(const Ray &r_in, HitRecord &rec) const {
    rec.attenuation = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float real_eta = rec.front_face ? (1.0f / eta) : eta;
    glm::vec3 refracted = glm::refract(r_in.direction(), rec.normal, real_eta);
    rec.scattered = Ray(rec.p, refracted);
    return true;
}
