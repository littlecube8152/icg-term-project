#include "objects.h"
#include "constants.h"
#include "scene.h"

#include "glm/gtx/projection.hpp"
#include "glm/gtx/norm.hpp"

Sphere::Sphere(const glm::vec3 &_center, const float &_radius, std::shared_ptr<Material> _material, InertialFrame _frame)
    : Object(_material, _frame), center(_center), radius(_radius)
{
    center = frame.transformCoordinateFrom(object_space_frame, 0.0f, center).second;
}

Sphere::Sphere(const glm::vec3 &_center, const float &_radius, std::shared_ptr<Material> _material)
    : Sphere(_center, _radius, _material, object_space_frame) {}

bool Sphere::hit(Ray &ray, HitRecord &record) const
{
    Ray observed_ray = ray;
    observed_ray.transformFrame(frame);

    // The closest point to the sphere on the observed ray
    // Cast to vec3 extracts the first three component: xyz
    glm::vec3 origin_to_center = center - (glm::vec3)observed_ray.origin();
    glm::vec3 proj = glm::proj(origin_to_center, observed_ray.direction());
    glm::vec3 closest = origin_to_center - proj;

    if (glm::length2(closest) > radius * radius)
        return false;

    float alpha = glm::dot(proj, observed_ray.direction());
    float d_alpha = sqrtf(radius * radius - glm::length2(closest));

    float hit;
    if (observed_ray.interval.surrounds(alpha - d_alpha))
        hit = alpha - d_alpha;
    else if (observed_ray.interval.surrounds(alpha + d_alpha))
        hit = alpha + d_alpha;
    else
        return false;

    glm::vec4 observed_hit_space_time = observed_ray.at(hit);

    record.alpha = observed_hit_space_time.w;
    record.ray = Ray(observed_hit_space_time, observed_ray.direction(), observed_ray.referenceFramePtr());
    record.setFaceNormal(((glm::vec3)observed_ray.at(hit) - center) / radius);
    record.has_scattered = mat.get() ? mat->scatter(record) : false;

    return true;
}

void Sphere::toUniform(SceneUniformCollector &collector) const {
    int material_id = 0;
    if (mat.get()) {
        collector.materials.emplace_back();
        mat->toUniform(collector.materials.back());
        material_id = static_cast<int>(collector.materials.size());
    }
    collector.spheres.emplace_back((SphereUniform) {
        .center = glm::vec4(center, 0),
        .radius = radius,
        .material_id = material_id,
        .iframe = glm::vec4(frame.frame_velocity, 1.0f),
    });
}
