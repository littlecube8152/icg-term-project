#include "ray.h"
#include "constants.h"

#include <format>

Ray::Ray(const glm::vec4 &origin, const glm::vec3 &direction, std::shared_ptr<InertialFrame> reference_frame)
: ray_origin(origin), ray_direction(direction), ray_reference_frame(reference_frame), interval(Interval::positive) {}
Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction, std::shared_ptr<InertialFrame> reference_frame)
: Ray(glm::vec4(origin, 0.0f), direction, reference_frame) {}

const glm::vec4 &Ray::origin() const { return ray_origin; }
const glm::vec3 &Ray::direction() const { return ray_direction; }
// const Interval &Ray::interval() const { return ray_interval; }
const InertialFrame &Ray::referenceFrame() const { return *ray_reference_frame; }
std::shared_ptr<InertialFrame> Ray::referenceFramePtr() const { return ray_reference_frame; }

glm::vec4 Ray::at(float alpha) const
{
    return ray_origin + glm::vec4(alpha * ray_direction, alpha);
}

void Ray::transformFrame(InertialFrame to_frame)
{
    glm::vec3 ray_origin_xyz(ray_origin);
    glm::vec3 ray_velocity_xyz(ray_direction);
    auto [t0, new_origin_xyz] = to_frame.transformCoordinateFrom(*ray_reference_frame, ray_origin.w, ray_origin_xyz);
    auto new_velocity_xyz = to_frame.transformVelocityFrom(*ray_reference_frame, ray_velocity_xyz * speedOfLight);

    ray_origin = glm::vec4(new_origin_xyz, t0);
    ray_direction = glm::vec4(new_velocity_xyz / speedOfLight, 1);

    ray_reference_frame = std::make_shared<InertialFrame>(to_frame);
}