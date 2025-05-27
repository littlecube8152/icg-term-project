#include "materials.h"

Material::Material() {
    static int material_id_counter = 1;
    material_id = material_id_counter++;
}

const int& Material::getId() const {
    return material_id;
}
