#ifndef RAY_H_
#define RAY_H_

#include "glm/glm.hpp"
#include "inertial.h"

class Ray {
    public:
        Ray() {}
    
        // Velocity measured in light speed.
        Ray(const glm::vec3& origin, const glm::vec3& direction) : _origin(origin), _velocity(direction) {}
    
        // Origin of the ray.
        const glm::vec3& origin() const  { return _origin; }
        // Velocity vector of the ray; measured in light speed.
        const glm::vec3& velocity() const { return _velocity; }
    
        glm::vec3 at(float t) const {
            return _origin + t * _velocity;
        }
        
    private:
        // Origin of the ray.
        glm::vec3 _origin;
        // Velocity vector of the ray; measured in light speed.
        glm::vec3 _velocity;
  };

#endif
