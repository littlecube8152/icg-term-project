#ifndef HITTABLE_H_
#define HITTABLE_H_

#include <memory>

#include "glm/glm.hpp"

#include "ray.h"
#include "interval.h"


class HitRecord {
public:
    glm::vec3 p;
    glm::vec3 normal;  // assumed to be unit-length
    float t;
    bool front_face;

    // Is the ray bounced off the meterial?
    bool has_scattered = false;
    glm::vec4 attenuation;
    Ray scattered;

    // Set the normal of the hit face.
    // Direction will be auto corrected. outward_normal assumed to be unit-length
    void setFaceNormal(const Ray &r, const glm::vec3 &outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};


class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray &r, const Interval &ray_t, HitRecord &rec) const = 0;
};

#endif
