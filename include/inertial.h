#ifndef INERTIAL_H_
#define INERTIAL_H_

#include "glm/glm.hpp"

// Implements special relativity inertial frames of reference.
// Every frame is constructed with respect to the object space frame as the standard frame.
struct InertialFrame
{
    glm::vec3 frame_velocity;

    InertialFrame(glm::vec3 velocity);
    InertialFrame();

    static glm::vec3 transform_velocity_from(glm::vec3 measured_relative_velocity, glm::vec3 target_velocity);

    glm::vec3 relative_frame_velocity_from(const InertialFrame &from_frame) const;
    std::pair<float, glm::vec3> transform_coordinate_from(const InertialFrame &from_frame, float time, glm::vec3 space) const;
    glm::vec3 transform_velocity_from(const InertialFrame &from_frame, glm::vec3 velocity) const;
    glm::vec3 transform_normal_from(const InertialFrame &from_frame, glm::vec3 normal) const;
};

static const InertialFrame object_space_frame;

#endif // INERTIAL_H_
