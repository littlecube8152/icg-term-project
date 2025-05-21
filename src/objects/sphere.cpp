#include "objects.h"
#include "constants.h"
#include "shaders/compute/objects/object_types.h"

#include "glm/gtx/projection.hpp"
#include "glm/gtx/norm.hpp"

#include <utility>
#include <iostream>

Sphere::Sphere(const glm::vec3 &_center, const float &_radius, std::shared_ptr<Material> _mat)
    : Object(_mat), center(_center), radius(_radius) {}

bool Sphere::hit(Ray &ray, HitRecord &record) const
{

    // The closest point to the sphere on the ray
    // Cast to vec3 extracts the first three component: xyz
    glm::vec3 origin_to_center = center - (glm::vec3)ray.origin();
    glm::vec3 proj = glm::proj(origin_to_center, ray.direction());
    glm::vec3 closest = origin_to_center - proj;

    if (glm::length2(closest) > radius * radius)
        return false;

    float ray_speed = 1.0f; // glm::length(ray.direction()); but it is unit length

    float alpha = glm::dot(proj, ray.direction()) / ray_speed;
    float d_alpha = sqrtf(radius * radius - glm::length2(closest)) / ray_speed;

    float hit;
    if (ray.interval.surrounds(alpha - d_alpha))
        hit = alpha - d_alpha;
    else if (ray.interval.surrounds(alpha + d_alpha))
        hit = alpha + d_alpha;
    else
        return false;

    glm::vec4 object_hit_space_time = ray.at(hit);
    record.alpha = hit;
    record.p = object_hit_space_time;

    record.setFaceNormal(ray, ((glm::vec3)ray.at(hit) - center) / radius);
    record.has_scattered = mat.get() ? mat->scatter(ray, record) : false;

    return true;
}

ObjectUniform Sphere::toUniform() const {
    ObjectUniform uniform;
    uniform.object_type = OBJECT_TYPE_SPHERE;
    uniform.sphere = (SphereUniform) {
        .center = glm::vec4(center, 0),
        .radius = radius,
        .material_id = mat.get() ? mat->getId() : -1,
    };
    return uniform;
}
