/*

.obj file format

A .obj file comprises multiple blocks, possibly with repeating types. Each block
starts with a line `<block_type> <n>`, indicating a block of type block_type with
n entries in the following n lines. Each entry is of format:

- `vertex` blocks: 3 floats `<x> <y> <z>` representing the coordinate of a vertex
- `colored_surface` blocks: 3 floats `<r> <g> <b>` with each component in range
  [0, 1] representing a face color used in this model
- `triangle` blocks: 4 integers `<v1> <v2> <v3> <c>` representing a triangle formed
  by the v1-th, v2-th, and v3-th vertices, with the c-th color

Indices are 0-indexed.

*/

#include "objects.h"

#include <fstream>
#include <algorithm>
#include <iostream>

ObjMesh::ObjMesh(const std::filesystem::path &_file_path, glm::vec3 _translation, float _scale, InertialFrame inertial_frame)
    : Object(nullptr, inertial_frame), file_path(_file_path), translation(_translation), scale(_scale) {}

bool ObjMesh::hit(Ray& ray, HitRecord& record) const {
    [ray, record](...){}();
    return false;
}

void ObjMesh::toUniform(SceneUniformCollector &collector) const {
    uint iframe_id = static_cast<uint>(collector.iframes.size());
    collector.iframes.emplace_back(glm::vec4(frame.frame_velocity, 0));

    int base_material_index = static_cast<int>(collector.materials.size());
    int base_vertex_index = static_cast<int>(collector.vertices.size());
    int base_triangle_index = static_cast<int>(collector.triangles.size());

    std::ifstream obj_stream(file_path);
    bool parse_failed = false;

    while (true) {
        std::string block_type;
        int n_entries;
        obj_stream >> block_type >> n_entries;
        if (obj_stream.eof()) break;

        if (n_entries < 0) {
            parse_failed = true;
            break;
        }

        while (n_entries--) {
            if (block_type == "vertex") {
                float x, y, z;
                obj_stream >> x >> y >> z;
                glm::vec3 coord = glm::vec3(x, y, z) * scale + translation;
                collector.vertices.emplace_back(glm::vec4(coord, 0));
            } else if (block_type == "colored_surface") {
                float r, g, b;
                obj_stream >> r >> g >> b;
                Lambertian colored_material(glm::vec4(r, g, b, 1.0));
                collector.materials.emplace_back();
                colored_material.toUniform(collector.materials.back());
            } else if (block_type == "triangle") {
                int i1, i2, i3, c;
                obj_stream >> i1 >> i2 >> i3 >> c;
                collector.triangles.emplace_back(glm::ivec4(
                    base_vertex_index + i1,
                    base_vertex_index + i2,
                    base_vertex_index + i3,
                    ((base_material_index + c) << 16) + iframe_id
                ));
            } else {
                parse_failed = true;
                break;
            }
        }

        if (parse_failed) break;
    }

    int end_material_index = static_cast<int>(collector.materials.size());
    int end_vertex_index = static_cast<int>(collector.vertices.size());
    for (size_t i = base_triangle_index; i < collector.triangles.size(); i++) {
        auto &tri = collector.triangles[i];
        int mat_id = tri.w >> 16;
        if (tri.x != std::clamp(tri.x, base_vertex_index, end_vertex_index) ||
            tri.y != std::clamp(tri.y, base_vertex_index, end_vertex_index) ||
            tri.z != std::clamp(tri.z, base_vertex_index, end_vertex_index) ||
            mat_id != std::clamp(mat_id, base_material_index, end_material_index)) {
            parse_failed = true;
            break;
        }
    }

    if (parse_failed) {
        throw std::runtime_error("Failed to load obj file: bad file format");
    }
}
