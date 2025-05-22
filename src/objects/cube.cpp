#include "objects.h"
#include "constants.h"
#include "shaders/compute/objects/object_types.h"

#include "glm/gtx/projection.hpp"
#include "glm/gtx/norm.hpp"

#include <utility>
#include <iostream>

Cube::Cube(const glm::vec3 &corner_position, const float &side_length, std::shared_ptr<Material> material)
    : Object(material) {
    this->corner = corner_position - glm::vec3(side_length / 2.0f);
    this->x_axis = glm::vec3(side_length, 0.0f, 0.0f);
    this->y_axis = glm::vec3(0.0f, side_length, 0.0f);
    this->z_axis = glm::vec3(0.0f, 0.0f, side_length);
}

bool Cube::hit(Ray &ray, HitRecord &record) const
{
    // The closest point to the sphere on the ray

    // currently not used anywhere else; perform abstraction if needed
    auto test_hit_surface = [&ray, &record, this](glm::vec3 surface_corner, glm::vec3 vec_u, glm::vec3 vec_v) -> bool
    {
        glm::vec3 normal = glm::normalize(glm::cross(vec_u, vec_v));
        float surface_level = glm::dot(surface_corner, normal);
        float origin_level = glm::dot((glm::vec3)ray.origin(), normal);
        float alpha_level = glm::dot(ray.direction(), normal);

        if (fabs(alpha_level) < kEpsilon)
        {
            // pretend there's no intersection
            return false;
        }
        float alpha = (surface_level - origin_level) / alpha_level;
        if (!ray.interval.surrounds(alpha))
            return false;
        glm::vec4 hit_point = ray.at(alpha);
        glm::vec3 surface_hit_vector = glm::vec3(hit_point) - surface_corner;
        if (glm::dot(glm::cross(vec_u, surface_hit_vector), normal) >= 0.0f &&
            glm::dot(glm::cross(vec_v, surface_hit_vector), normal) <= 0.0f &&
            glm::dot(glm::cross(vec_u, surface_hit_vector - vec_v), normal) <= 0.0f &&
            glm::dot(glm::cross(vec_v, surface_hit_vector - vec_u), normal) >= 0.0f)
        {
            ray.interval.max = alpha;

            record.alpha = alpha;
            record.p = hit_point;

            record.setFaceNormal(ray, normal);
            record.has_scattered = mat.get() ? mat->scatter(ray, record) : false;
            return true;
        }
        return false;
    };

    bool hit = false;
    hit |= test_hit_surface(corner, x_axis, z_axis);
    hit |= test_hit_surface(corner, z_axis, y_axis);
    hit |= test_hit_surface(corner, y_axis, x_axis);
    hit |= test_hit_surface(corner + x_axis + y_axis + z_axis, -x_axis, -z_axis);
    hit |= test_hit_surface(corner + x_axis + y_axis + z_axis, -z_axis, -y_axis);
    hit |= test_hit_surface(corner + x_axis + y_axis + z_axis, -y_axis, -x_axis);

    return hit;
}

void Cube::toUniform(ObjectUniform &object_uniform) const {
    object_uniform.object_type = OBJECT_TYPE_CUBE;
    object_uniform.cube = (CubeUniform) {
        .center = glm::vec4(corner + (x_axis + y_axis + z_axis) / 2.0f, 0),
        .side_length = x_axis.x,
        .material_id = mat.get() ? mat->getId() : -1,
    };
}
