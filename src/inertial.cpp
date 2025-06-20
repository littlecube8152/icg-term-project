#include "inertial.h"
#include "constants.h"

#include <cmath>
#include <utility>

// velocity is given in the unit of light speed
InertialFrame::InertialFrame(glm::vec3 velocity) { this->frame_velocity = velocity * speedOfLight; };
InertialFrame::InertialFrame() { this->frame_velocity = glm::vec3(0.0f); };

// Transforms velocity between two frames, given the measured velocity from the original frame S to the desired frame S'
glm::vec3 InertialFrame::transformVelocityFrom(glm::vec3 measured_relative_velocity, glm::vec3 velocity_to_transform)
{
    if (glm::length(measured_relative_velocity) < kEpsilon)
        return velocity_to_transform - measured_relative_velocity;

    // Velocity are all relative to the object inertial frame
    // Use change of basis such that the velocity measured from object inertial is on the x-axis
    glm::vec3 basis_x = glm::normalize(measured_relative_velocity);
    // basis_y and basis_z does not matter; they are indistinguishable

    float this_velocity_x = glm::dot(velocity_to_transform, basis_x);
    float from_velocity_x = glm::length(measured_relative_velocity);
    glm::vec3 this_velocity_yz = velocity_to_transform - this_velocity_x * basis_x;

    glm::vec3 beta = measured_relative_velocity / speedOfLight;
    float gamma = 1.0f / std::sqrt(1.0f - glm::dot(beta, beta));

    float one_minus_speed_prod = 1.0f - this_velocity_x * from_velocity_x / speedOfLight / speedOfLight;

    // Formula given by Lorentz transformation
    return basis_x * (this_velocity_x - from_velocity_x) / one_minus_speed_prod +
           this_velocity_yz / gamma / one_minus_speed_prod;
}

// Computes the velocity of the current frame, with respect to the frame `from_frame`.
// In other words, transform the frame velocity from the object frame to the frame `from_frame`.
glm::vec3 InertialFrame::relativeFrameVelocityFrom(const InertialFrame &from_frame) const
{
    return transformVelocityFrom(from_frame.frame_velocity, frame_velocity);
}

// Transforms velocity from the `from_frame` to the current frame.
glm::vec3 InertialFrame::transformVelocityFrom(const InertialFrame &from_frame, glm::vec3 velocity) const
{
    return transformVelocityFrom(relativeFrameVelocityFrom(from_frame), velocity);
}


glm::vec3 InertialFrame::getBeta(const InertialFrame &from_frame) const
{
    return relativeFrameVelocityFrom(from_frame) / speedOfLight;
}
float InertialFrame::getBetaScalar(const InertialFrame &from_frame) const
{
    return glm::length(getBeta(from_frame));
}
float InertialFrame::getBetaSquare(const InertialFrame &from_frame) const
{
    glm::vec3 beta = getBeta(from_frame);
    return glm::dot(beta, beta);
}
float InertialFrame::getGamma(const InertialFrame &from_frame) const
{
    return 1.0f / std::sqrt(1.0f - getBetaSquare(from_frame));
}


// Transforms time and space coordinate from the `from_frame` to the current frame.
std::pair<float, glm::vec3> InertialFrame::transformCoordinateFrom(const InertialFrame &from_frame, float time, glm::vec3 space) const
{
    glm::vec3 beta = getBeta(from_frame);
    float beta_square = glm::dot(beta, beta);
    float gamma = 1.0f / std::sqrt(1.0f - beta_square);
    // (gamma - 1) / beta^2, numerically more stable
    float gammam1betainv = gamma * gamma / (1.0f + gamma);

    // Constructing Lorentz matrix
    glm::mat4x4 lorentz(glm::vec4(gamma, -gamma * beta),
                        glm::vec4(-gamma, beta * gammam1betainv) * beta[0],
                        glm::vec4(-gamma, beta * gammam1betainv) * beta[1],
                        glm::vec4(-gamma, beta * gammam1betainv) * beta[2]);
    // Transpose because the constructor is column major
    lorentz = glm::transpose(lorentz);
    
    for (int i = 1; i <= 3; i++)
        lorentz[i][i] += 1.0f;

    glm::vec4 timespace = lorentz * glm::vec4(time * speedOfLight, space);

    return std::make_pair(timespace[0] / speedOfLight, glm::vec3(timespace[1], timespace[2], timespace[3]));
}

// Transforms normal vector  from the `from_frame` to the current frame.
glm::vec3 InertialFrame::transformNormalFrom(const InertialFrame &from_frame, glm::vec3 normal) const
{
    glm::vec3 beta = getBeta(from_frame);
    float beta_square = glm::dot(beta, beta);
    float gamma = 1.0f / std::sqrt(1.0f - beta_square);
    // (gamma - 1) / beta^2, numerically more stable
    float gammam1betainv = gamma * gamma / (1.0f + gamma);

    // Constructing Lorentz matrix without time coordinate
    glm::mat3x3 lorentz(beta * gammam1betainv * beta[0],
                        beta * gammam1betainv * beta[1],
                        beta * gammam1betainv * beta[2]);
    for (int i = 0; i < 3; i++)
        lorentz[i][i] += 1.0f;

    return glm::transpose(glm::inverse(lorentz)) * glm::vec3(normal);
}