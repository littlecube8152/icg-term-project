#ifndef RAY_H_
#define RAY_H_

#include "glm/glm.hpp"
#include "inertial.h"
#include "interval.h"

#include <memory>

// Represents a ray in the form (x0, y0, z0, t0) + alpha (xv, yv, zv, 1)
class Ray
{

private:
    // Origin of the ray.
    glm::vec4 ray_origin;
    // Direction vector of the ray. 
    glm::vec3 ray_direction;
    // Inertial frame of the ray.
    std::shared_ptr<InertialFrame> ray_reference_frame;

public:
    Interval interval;
    // Interval of the parameter alpha.

    Ray() {}

    // Velocity measured in light speed (so it should be a unit vector).
    Ray(const glm::vec3 &origin, const glm::vec3 &direction, std::shared_ptr<InertialFrame> reference_frame);
    Ray(const glm::vec4 &origin, const glm::vec3 &direction, std::shared_ptr<InertialFrame> reference_frame);

    // Origin of the ray.
    const glm::vec4 &origin() const;
    // Direction vector of the ray. Always an unit. That is, it is velocity measured in light speed.
    const glm::vec3 &direction() const;
    // Inertial frame of the ray.
    const InertialFrame &referenceFrame() const;
    // Get shared_ptr of the Inertial frame of the ray.
    std::shared_ptr<InertialFrame> referenceFramePtr() const;

    // Get the point on the ray at time point (or parameter) alpha
    glm::vec4 at(float alpha) const;

    // Transform the inertial frame
    void transformFrame(InertialFrame to_frame);
};

#endif
