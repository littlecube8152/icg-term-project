#include "objects.h"
#include "constants.h"

Cube::Cube(const glm::vec3 &_center, const float &side_length, std::shared_ptr<Material> material, InertialFrame _frame)
    : Object(material, _frame)
{
    this->corner = frame.transformCoordinateFrom(object_space_frame, 0.0f, _center).second - glm::vec3(side_length / 2.0f);
    this->x_axis = glm::vec3(side_length, 0.0f, 0.0f);
    this->y_axis = glm::vec3(0.0f, side_length, 0.0f);
    this->z_axis = glm::vec3(0.0f, 0.0f, side_length);
}

Cube::Cube(const glm::vec3 &center, const float &side_length, std::shared_ptr<Material> material)
    : Cube(center, side_length, material, object_space_frame) {};

bool Cube::hit(Ray &ray, HitRecord &record) const
{

    Ray observed_ray = ray;
    observed_ray.transformFrame(frame);

    // currently not used anywhere else; perform abstraction if needed
    auto test_hit_surface = [&observed_ray, &record, this](glm::vec3 surface_corner, glm::vec3 vec_u, glm::vec3 vec_v) -> bool
    {
        glm::vec3 normal = glm::normalize(glm::cross(vec_u, vec_v));
        float surface_level = glm::dot(surface_corner, normal);
        float origin_level = glm::dot((glm::vec3)observed_ray.origin(), normal);
        float alpha_level = glm::dot(observed_ray.direction(), normal);

        if (fabs(alpha_level) < kEpsilon)
        {
            // pretend there's no intersection
            return false;
        }
        float alpha = (surface_level - origin_level) / alpha_level;
        if (!observed_ray.interval.surrounds(alpha))
            return false;
        glm::vec4 hit_point = observed_ray.at(alpha);
        glm::vec3 surface_hit_vector = glm::vec3(hit_point) - surface_corner;
        if (glm::dot(glm::cross(vec_u, surface_hit_vector), normal) >= 0.0f &&
            glm::dot(glm::cross(vec_v, surface_hit_vector), normal) <= 0.0f &&
            glm::dot(glm::cross(vec_u, surface_hit_vector - vec_v), normal) <= 0.0f &&
            glm::dot(glm::cross(vec_v, surface_hit_vector - vec_u), normal) >= 0.0f)
        {
            observed_ray.interval.max = alpha;

            record.ray = Ray(hit_point, observed_ray.direction(), observed_ray.referenceFramePtr());
            record.alpha = alpha;
            record.setFaceNormal(normal);
            record.has_scattered = mat.get() ? mat->scatter(record) : false;
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

void Cube::toUniform(CubeUniform &cube_uniform) const {
    cube_uniform = (CubeUniform) {
        .center = glm::vec4(corner + (x_axis + y_axis + z_axis) / 2.0f, 0),
        .side_length = x_axis.x,
        .material_id = mat.get() ? mat->getId() : -1,
        .iframe = glm::vec4(frame.frame_velocity, 1.0f),
    };
}
