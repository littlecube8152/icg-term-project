#ifndef HITTABLE_H_
#define HITTABLE_H_

#include <memory>
#include <iostream>

#include "glm/glm.hpp"

#include "ray.h"
#include "interval.h"


// Always store the result in the camera's inertial frame
class HitRecord {
public:
    glm::vec4 p;
    glm::vec3 normal;  // assumed to be unit-length
    float alpha;
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
        normal = glm::normalize(normal);
    }
};


class Hittable {
public:
    virtual ~Hittable() = default;
    
    virtual bool hit(Ray&, HitRecord&) const = 0;
};

#endif
