#include "objects.h"

#include <fstream>

ObjMesh::ObjMesh(const std::filesystem::path &_file_path, glm::vec3 _translation, float _scale, std::shared_ptr<Material> material, InertialFrame inertial_frame)
    : Object(material, inertial_frame), file_path(_file_path), translation(_translation), scale(_scale) {}

bool ObjMesh::hit(Ray& ray, HitRecord& record) const {
    [ray, record](...){}();
    return false;
}

void ObjMesh::toUniform(SceneUniformCollector &collector) const {
    int material_iframe_ids = 0;
    if (mat.get()) {
        material_iframe_ids = static_cast<int>(collector.materials.size());
        collector.materials.emplace_back();
        mat->toUniform(collector.materials.back());
    }

    material_iframe_ids = (material_iframe_ids << 16) + static_cast<int>(collector.iframes.size()); // iframe id
    collector.iframes.emplace_back(glm::vec4(frame.frame_velocity, 0));

    std::ifstream obj(file_path);

    int vert_count;
    obj >> vert_count;
    int base_idx = static_cast<int>(collector.vertices.size());
    for (int i = 0; i < vert_count; i++) {
        float x, y, z;
        obj >> x >> y >> z;
        glm::vec3 coord = glm::vec3(x, y, z) * scale + translation;
        collector.vertices.emplace_back(glm::vec4(coord, 0));
    }

    int tri_count;
    obj >> tri_count;
    for (int i = 0; i < tri_count; i++) {
        int i1, i2, i3;
        obj >> i1 >> i2 >> i3;
        collector.triangles.emplace_back(glm::ivec4(base_idx + i1, base_idx + i2, base_idx + i3, material_iframe_ids));
    }
}
