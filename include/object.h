#ifndef OBJECT_H_
#define OBJECT_H_

#include "glm/glm.hpp"

#include "hittable.h"
#include "materials.h"
#include "inertial.h"

class Object : public Hittable
{
public:
    Object(std::shared_ptr<Material> material, InertialFrame inertial_frame);

protected:
    std::shared_ptr<Material> mat;
    InertialFrame frame;
};

#endif
