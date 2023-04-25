#include "canvas/object.hh"

namespace {

double reflectance(double cosine, double refraction) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - refraction) / (1 + refraction);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    double cos_theta = std::min(dot(-uv, n), 1.0);
    const bool can_refrac = (etai_over_etat * sqrt(1 - cos_theta * cos_theta)) <= 1.0;
    if (!can_refrac || reflectance(cos_theta, etai_over_etat) > random_number<double>(0, 1)) {
        return reflect(uv, n);
    }
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

}  // namespace

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
            break;
        }
        case Material::METAL: {
            auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + texture_.fuzz * random_in_unit_sphere());
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        case Material::DIELECTRIC: {
            double refraction_ratio = rec.front_face ? (1.0 / texture_.refraction) : texture_.refraction;
            Vec3 unit_direction = unit_vector(r_in.direction());
            Vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
            scattered = Ray(rec.p, refracted);
            break;
        }
        default:
            break;
    }
    return true;
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
