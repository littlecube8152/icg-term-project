#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

#include <vector>
#include <memory>

#include "hittable.h"
#include "inertial.h"


class HittableList: public Hittable {
public:
    HittableList(std::shared_ptr<InertialFrame> frame);

    void clear();
    void add(std::shared_ptr<Hittable> obj);
    bool hit(Ray &ray, HitRecord &record) const override;

private:
    std::shared_ptr<InertialFrame> frame;
    std::vector<std::shared_ptr<Hittable>> hittables;
};

#endif
