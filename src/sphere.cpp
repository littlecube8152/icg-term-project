#include "sphere.h"
#include "constants.h"

#include "glm/gtx/projection.hpp"
#include "glm/gtx/norm.hpp"

#include <utility>
#include <iostream>

Sphere::Sphere(const glm::vec3 &_center, const float &_radius, std::shared_ptr<Material> _mat)
    : center(_center), radius(_radius), mat(_mat) {}

bool Sphere::hit(const Ray &ray, const InertialFrame &frame, const Interval &valid_interval, HitRecord &record) const
{

    //// Object inertial frame
    // The closest point to the sphere on the ray
    Ray object_space_ray(
        object_space_frame.transform_coordinate_from(frame, 0.0f, ray.origin()).second /* extracts space coordinate */,
        object_space_frame.transform_velocity_from(frame, ray.velocity() * speedOfLight) / speedOfLight);


    glm::vec3 origin_to_center = center - object_space_ray.origin();
    glm::vec3 proj = glm::proj(origin_to_center, glm::normalize(object_space_ray.velocity()));
    glm::vec3 closest = origin_to_center - proj;

    if (glm::length2(closest) > radius * radius)
        return false;

    float ray_speed = glm::length(object_space_ray.velocity());

    float t = glm::dot(proj, glm::normalize(object_space_ray.velocity())) / ray_speed;
    float dt = sqrtf(radius * radius - glm::length2(closest)) / ray_speed;
    float hit;
    if (valid_interval.surrounds(t - dt))
    {
        hit = t - dt;
    }
    else if (valid_interval.surrounds(t + dt))
    {
        hit = t + dt;
    }
    else
    {
        return false;
    }

    //// Camera inertial frame
    std::tie(record.t, record.p) = frame.transform_coordinate_from(object_space_frame, hit / speedOfLight, object_space_ray.at(hit));
    record.t *= speedOfLight;

    record.t = speedOfLight * frame.transform_coordinate_from(object_space_frame, hit / speedOfLight, object_space_ray.at(hit)).first;
    record.p = ray.at(record.t);

    record.setFaceNormal(ray, frame.transform_normal_from(object_space_frame, (object_space_ray.at(hit) - center) / radius));
    record.has_scattered = mat.get() ? mat->scatter(ray, record) : false;

    return true;
}
