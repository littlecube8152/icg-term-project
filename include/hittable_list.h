#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

#include <vector>
#include <memory>

#include "hit_record.h"
#include "objects.h"
#include "inertial.h"


class HittableList {
public:
    HittableList(std::shared_ptr<InertialFrame> frame);

    void clear();
    void add(std::shared_ptr<Object> obj);
    bool hit(Ray &ray, HitRecord &record) const;
    const std::vector<std::shared_ptr<Object>>& getObjects() const;

private:
    std::shared_ptr<InertialFrame> frame;
    std::vector<std::shared_ptr<Object>> hittables;
};

#endif
