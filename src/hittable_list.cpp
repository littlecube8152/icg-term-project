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
bool HittableList::hit(Ray &ray, const InertialFrame &frame, HitRecord &record) const {

    ray.transformFrame(object_space_frame);

    HitRecord hit_result;
    bool hit_any = false;
    
    for (const auto &obj : hittables) {
        if (obj->hit(ray, frame, hit_result)) {
            hit_any = true;
            ray.interval.max = hit_result.alpha;
            record = hit_result;
        }
    }

    return hit_any;
}
