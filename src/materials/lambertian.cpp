#include "materials.h"

#include "glm/gtc/epsilon.hpp"

#include "randutil.h"
#include "vecutil.h"
#include "constants.h"


Lambertian::Lambertian(const glm::vec4 &_albedo): albedo(_albedo) {}


bool Lambertian::scatter(const Ray &r_in, HitRecord &rec) const {
    [](...){}(r_in);
    auto scatter_dir = rec.normal + rand_unit_length<glm::vec3>();
    scatter_dir = default_normalize(scatter_dir, rec.normal);
    rec.scattered = Ray(rec.p, scatter_dir, r_in.referenceFramePtr());
    rec.attenuation = albedo;
    return true;
}
