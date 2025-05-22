#include "object.h"

Object::Object(std::shared_ptr<Material> material, InertialFrame inertial_frame)
{
    mat = material;
    frame = inertial_frame;
}
