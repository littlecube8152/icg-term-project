#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

#include <vector>
#include <memory>

#include "hittable.h"


class HittableList: public Hittable {
public:
    HittableList();

    void clear();
    void add(std::shared_ptr<Hittable> obj);
    bool hit(const Ray &r, const Interval &ray_t, HitRecord &rec) const override;

private:
    std::vector<std::shared_ptr<Hittable>> hittables;
};

#endif
