#include "hittable_list.h"


HittableList::HittableList(): hittables() {}


void HittableList::clear() {
    hittables.clear();
}


void HittableList::add(std::shared_ptr<Hittable> obj) {
    hittables.emplace_back(obj);
}


bool HittableList::hit(const Ray &r, const Interval &ray_t, HitRecord &rec) const {
    HitRecord tmp_rec;
    bool hit_any = false;
    Interval hit_t = ray_t;
    
    for (const auto &obj : hittables) {
        if (obj->hit(r, hit_t, tmp_rec)) {
            hit_any = true;
            hit_t.max = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_any;
}
