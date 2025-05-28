#include "objects.h"
#include "constants.h"
#include "scene.h"

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

void Cube::toUniform(SceneUniformCollector &collector) const {
    int material_iframe_ids = 0;
    if (mat.get()) {
        material_iframe_ids = static_cast<int>(collector.materials.size());
        collector.materials.emplace_back();
        mat->toUniform(collector.materials.back());
    }

    material_iframe_ids = (material_iframe_ids << 16) + static_cast<int>(collector.iframes.size()); // iframe id
    collector.iframes.emplace_back(glm::vec4(frame.frame_velocity, 0));

    glm::vec3 vert[] = {
        corner,
        corner + x_axis,
        corner + x_axis + y_axis,
        corner + y_axis,
        corner + z_axis,
        corner + x_axis + z_axis,
        corner + x_axis + y_axis + z_axis,
        corner + y_axis + z_axis,
    };
    int base_idx = static_cast<int>(collector.vertices.size());
    for (const auto &v:vert)
        collector.vertices.emplace_back(glm::vec4(v, 0));

    glm::ivec3 indices[] = {
        {1, 3, 2},
        {1, 4, 3},
        {1, 6, 2},
        {1, 5, 6},
        {1, 8, 4},
        {1, 5, 8},
        {7, 2, 3},
        {7, 6, 2},
        {7, 5, 6},
        {7, 8, 5},
        {7, 4, 3},
        {7, 8, 4},
    };
    for (const auto &i:indices)
        collector.triangles.emplace_back(glm::ivec4(i + glm::ivec3(base_idx) - 1, material_iframe_ids));
}
