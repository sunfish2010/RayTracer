#include "canvas/object.hh"

bool Object::scatter(
    const Ray& r_in, const HitRecord& rec, Ray& scattered) const {
    switch (texture_.material) {
        case Material::DIFFUSIVE: {
            auto scatter_direction = rec.normal + unit_vector(random_in_unit_sphere());
            // Catch degenerate scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = Ray(rec.p, scatter_direction);
            return true;
        }
        case Material::METAL: {
            auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected);
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        default:
            break;
    }
    return false;
}

bool HittableObjectLists::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects_) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
