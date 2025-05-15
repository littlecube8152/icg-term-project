#ifndef RAY_H_
#define RAY_H_

#include "glm/glm.hpp"
#include "inertial.h"
#include "interval.h"

#include <memory>

// Represents a ray in the form (x0, y0, z0, t0) + alpha (xv, yv, zv, tv)
class Ray
{

private:
    // Origin of the ray.
    glm::vec4 ray_origin;
    // Velocity vector of the ray; xyz are measured in light speed.
    glm::vec4 ray_velocity;
    // Inertial frame of the ray.
    std::shared_ptr<InertialFrame> ray_reference_frame;

public:
    Interval interval;
    // Interval of the parameter alpha.

    Ray() {}

    // Velocity measured in light speed.
    Ray(const glm::vec3 &origin, const glm::vec3 &velocity, std::shared_ptr<InertialFrame> reference_frame)
        : ray_origin(origin, 0.0f), ray_velocity(velocity, 1.0f), ray_reference_frame(reference_frame), interval(Interval::positive) {}

    // Origin of the ray.
    const glm::vec4 &origin() const;
    // Velocity vector of the ray; measured in light speed.
    const glm::vec4 &velocity() const;
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
