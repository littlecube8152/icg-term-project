#include "inertial.h"
#include "constants.h"

#include <cmath>
#include <utility>

struct InertialFrame
{
    static constexpr float light_of_speed = 299'792'458.0f;

    glm::vec3 velocity;

    // Computes the velocity of the current frame, with respect to the frame `from_frame`.
    glm::vec3 relative_velocity_from(const InertialFrame &from_frame)
    {
        if (glm::length(velocity) < kEpsilon)
            return velocity - from_frame.velocity;

        // Velocity are all relative to the object inertial frame
        // Use change of basis such that the velocity measured from object inertial is on the x-axis
        glm::vec3 basis_x = glm::normalize(from_frame.velocity);
        // basis_x and basis_y does not matter; they are indistinguishable

        float this_velocity_x = glm::dot(velocity, basis_x);
        float from_velocity_x = glm::length(from_frame.velocity);
        glm::vec3 this_velocity_yz = velocity - this_velocity_x * basis_x;

        glm::vec3 beta = from_frame.velocity / light_of_speed;
        float gamma = 1.0f / sqrtf(1.0f - glm::dot(beta, beta));

        float one_minus_speed_prod = 1.0f - this_velocity_x * from_velocity_x / light_of_speed / light_of_speed;

        // Formula given by Lorentz transformation
        return basis_x * (this_velocity_x - from_velocity_x) / one_minus_speed_prod +
               this_velocity_yz / gamma / one_minus_speed_prod;
    }

    // Transforms time and space coordinate from the `from_frame` to the current frame.
    std::pair<float, glm::vec3> transform_from(const InertialFrame &from_frame, float time, glm::vec3 space)
    {
        glm::vec3 beta = relative_velocity_from(from_frame) / light_of_speed;
        float gamma = 1.0f / sqrtf(1.0f - glm::dot(beta, beta));

        // Constructing Lorentz matrix
        glm::mat4x4 lorentz(glm::vec4(gamma, -gamma * beta),
                            glm::vec4(-gamma, beta * (gamma - 1.0f)) * beta[0],
                            glm::vec4(-gamma, beta * (gamma - 1.0f)) * beta[1],
                            glm::vec4(-gamma, beta * (gamma - 1.0f)) * beta[2]);
        for (int i = 1; i <= 3; i++)
            lorentz[i][i] += 1.0f;
        glm::vec4 timespace = lorentz * glm::vec4(time * light_of_speed, space);

        return std::make_pair(timespace[0] / light_of_speed, glm::vec3(timespace[1], timespace[2], timespace[3]));
    }
};
