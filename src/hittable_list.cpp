#include "hittable_list.h"

HittableList::HittableList(): hittables() {}

// Clears the list.
void HittableList::clear() {
    hittables.clear();
}

// Add an object to the list.
void HittableList::add(std::shared_ptr<Hittable> obj) {
    hittables.emplace_back(obj);
}

// Test if the ray `ray` hits any element in the list.
// If true, returns `true` and store the HitRecord in `record`.
bool HittableList::hit(const Ray &ray, const InertialFrame &frame, const Interval &valid_interval, HitRecord &record) const {
    HitRecord hit_result;
    bool hit_any = false;
    Interval remaining_interval = valid_interval;
    
    for (const auto &obj : hittables) {
        if (obj->hit(ray, frame, remaining_interval, hit_result)) {
            hit_any = true;
            remaining_interval.max = hit_result.t;
            record = hit_result;
        }
    }

    return hit_any;
}
