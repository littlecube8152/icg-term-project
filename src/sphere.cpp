#include "sphere.h"

#include "glm/gtx/projection.hpp"
#include "glm/gtx/norm.hpp"


Sphere::Sphere(const glm::vec3 &_center, const float &_radius, std::shared_ptr<Material> _mat)
    : center(_center), radius(_radius), mat(_mat) {}


bool Sphere::hit(const Ray &r, const Interval &ray_t, HitRecord &rec) const {
    glm::vec3 oc = center - r.origin();
    glm::vec3 proj = glm::proj(oc, r.direction());
    glm::vec3 dist = oc - proj;

    if (glm::length2(dist) > radius * radius)
        return false;

    float lenr = glm::length(r.direction());
    float t = glm::dot(proj, r.direction()) / lenr;
    float dt = sqrtf(radius * radius - glm::length2(dist)) / lenr;
    float root;
    if (ray_t.surrounds(t - dt)) {
        root = t - dt;
    } else if (ray_t.surrounds(t + dt)) {
        root = t + dt;
    } else {
        return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    rec.setFaceNormal(r, (rec.p - center) / radius);
    rec.has_scattered = mat.get() ? mat->scatter(r, rec) : false;

    return true;
}
