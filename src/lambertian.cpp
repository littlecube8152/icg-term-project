#include "lambertian.h"

#include "glm/gtc/epsilon.hpp"

#include "randutil.h"
#include "constants.h"


Lambertian::Lambertian(const glm::vec4 &_albedo): albedo(_albedo) {}


bool Lambertian::scatter(const Ray &r_in, HitRecord &rec) const {
    [](...){}(r_in);
    auto scatter_dir = rec.normal + rand_unit_length<glm::vec3>();
    auto length = glm::length(scatter_dir);
    if (glm::epsilonEqual(length, 0.0f, kEpsilon)) {
        scatter_dir = rec.normal;
    } else {
        scatter_dir /= length;
    }
    rec.scattered = Ray(rec.p, scatter_dir);
    rec.attenuation = albedo;
    return true;
}
