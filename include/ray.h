#ifndef RAY_H_
#define RAY_H_

#include "glm/glm.hpp"

class Ray {
    public:
        Ray() {}
    
        // direction assumed to be unit-length
        Ray(const glm::vec3& origin, const glm::vec3& direction) : orig(origin), dir(direction) {}
    
        // Origin of the ray.
        const glm::vec3& origin() const  { return orig; }
        // Direction vector of the ray. assumed to be unit-length
        const glm::vec3& direction() const { return dir; }
    
        glm::vec3 at(float t) const {
            return orig + t * dir;
        }
  
    private:
        // Origin of the ray.
        glm::vec3 orig;
        // Direction vector of the ray. assumed to be unit-length
        glm::vec3 dir;
  };

#endif
