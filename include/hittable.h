#ifndef HITTABLE_H_
#define HITTABLE_H_

#include <memory>

#include "glm/glm.hpp"

#include "ray.h"
#include "interval.h"
#include "inertial.h"


// Always store the result in the camera's inertial frame
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
        front_face = glm::dot(r.velocity(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};


class Hittable {
public:
    virtual ~Hittable() = default;
    // Ray is not transformed into the object frame
    virtual bool hit(const Ray&, const InertialFrame&, const Interval&, HitRecord&) const = 0;
};

#endif
