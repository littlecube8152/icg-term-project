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

    static glm::vec3 transformVelocityFrom(glm::vec3 measured_relative_velocity, glm::vec3 target_velocity);

    glm::vec3 getBeta(const InertialFrame &from_frame) const;
    float getBetaScalar(const InertialFrame &from_frame) const;
    float getBetaSquare(const InertialFrame &from_frame) const;
    float getGamma(const InertialFrame &from_frame) const;

    glm::vec3 relativeFrameVelocityFrom(const InertialFrame &from_frame) const;
    std::pair<float, glm::vec3> transformCoordinateFrom(const InertialFrame &from_frame, float time, glm::vec3 space) const;
    glm::vec3 transformVelocityFrom(const InertialFrame &from_frame, glm::vec3 velocity) const;
    glm::vec3 transformNormalFrom(const InertialFrame &from_frame, glm::vec3 normal) const;
};

static const InertialFrame object_space_frame;

#endif // INERTIAL_H_
