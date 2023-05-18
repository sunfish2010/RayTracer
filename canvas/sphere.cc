#include "canvas/sphere.hh"

#include <memory>

namespace {
bool hit_sphere(const Point3& center, double radius, const Ray& r, double t_min, double t_max, HitRecord& rec) {
    Vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    const auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
}  // namespace

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    const bool hit_object = hit_sphere(center_, radius_, r, t_min, t_max, rec);
    if (hit_object) {
        rec.hit_object = shared_from_this();
    }

    return hit_object;
}

bool Sphere::maybe_get_bbox(double time0, double time1, Bbox& output_box) const {
    output_box = Bbox(
        center_ - Vec3(radius_, radius_, radius_),
        center_ + Vec3(radius_, radius_, radius_));
    return true;
}

Point3 MovingSphere::center(double time) const {
    return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}

bool MovingSphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    const bool hit_object = hit_sphere(center(r.time()), radius_, r, t_min, t_max, rec);
    if (hit_object) {
        rec.hit_object = shared_from_this();
    }
    return hit_object;
}

bool MovingSphere::maybe_get_bbox(double time0, double time1, Bbox& output_box) const {
    Bbox box0(
        center(time0) - Vec3(radius_, radius_, radius_),
        center(time0) + Vec3(radius_, radius_, radius_));
    Bbox box1(
        center(time1) - Vec3(radius_, radius_, radius_),
        center(time1) + Vec3(radius_, radius_, radius_));
    output_box = surrounding_box(box0, box1);
    return true;
}
