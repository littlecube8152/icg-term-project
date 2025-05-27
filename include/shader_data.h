#ifndef SHADER_DATA_H_
#define SHADER_DATA_H_

#include <vector>

#include "glm/glm.hpp"

#include "shaders/compute/macros.h"

// NOTE:
// vec3 intentionally replaced with vec4 to avoid alignment issues
// https://stackoverflow.com/questions/38172696/should-i-ever-use-a-vec3-inside-of-a-uniform-buffer-or-shader-storage-buffer-o
struct alignas(16) CameraUniform {
    alignas(4)  int sqrt_samples_per_pixel;
    alignas(4)  float pixel_samples_scale;
    alignas(4)  float pixel_samples_delta;
    alignas(16) glm::vec4 lookfrom;
    alignas(16) glm::vec4 iframe;
    alignas(16) glm::vec4 viewport_lower_left;
    alignas(16) glm::vec4 viewport_dx;
    alignas(16) glm::vec4 viewport_dy;
};

struct alignas(16) SphereUniform {
    alignas(16) glm::vec4 center;
    alignas(4)  float radius;
    alignas(4)  int material_id;
    alignas(16) glm::vec4 iframe;
};

struct alignas(16) MetalUniform {
    alignas(16) glm::vec4 albedo;
    alignas(4)  float fuzz;
};

struct alignas(16) LambertianUniform {
    alignas(16) glm::vec4 albedo;
};

struct alignas(16) DielectricUniform {
    alignas(4) float eta;
};

struct alignas(16) MaterialUniform {
    alignas(4)  int material_type;
    alignas(16) MetalUniform metal;
    alignas(16) LambertianUniform lambertian;
    alignas(16) DielectricUniform dielectric;
};

struct alignas(16) SceneUniform {
    alignas(4)  int n_spheres;
    alignas(4)  int n_triangles;
    alignas(4)  float scene_time;
    alignas(16) glm::vec4 world_iframe;
    alignas(16) CameraUniform camera_uniform;
    alignas(16) glm::vec4 u_iframes[MAX_IFRAMES];
    alignas(16) SphereUniform u_spheres[MAX_OBJECTS];
    alignas(16) MaterialUniform u_materials[MAX_MATERIALS];
    alignas(16) glm::vec4 u_vertices[MAX_VERTICES];
    alignas(16) glm::ivec4 u_triangles[MAX_TRIANGLES];
};

struct SceneUniformCollector {
    std::vector<glm::vec4> iframes;
    std::vector<SphereUniform> spheres;
    std::vector<MaterialUniform> materials;
    std::vector<glm::vec4> vertices;
    std::vector<glm::ivec4> triangles;
};

#endif
