#include "canvas/bbox.hh"

bool Bbox::hit(const Ray& r, double t_min, double t_max) const {
    for (int i = 0; i < 3; ++i) {
        auto invD = 1.0 / r.direction()[i];
        auto t0 = (lower_left_[i] - r.origin()[i]) * invD;
        auto t1 = (upper_right_[i] - r.origin()[i]) * invD;
        // Swap the interval start and end when ray is travelling in the negative direction.
        if (invD < 0.0) {
            std::swap(t0, t1);
        }
        // Find overlap between intervals.
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min) {
            return false;
        }
    }
    return true;
}

Bbox surrounding_box(const Bbox& box0, const Bbox& box1) {
    Point3 ll(fmin(box0.lower_left().x(), box1.lower_left().x()),
              fmin(box0.lower_left().y(), box1.lower_left().y()),
              fmin(box0.lower_left().z(), box1.lower_left().z()));

    Point3 ur(fmax(box0.upper_right().x(), box1.upper_right().x()),
              fmax(box0.upper_right().y(), box1.upper_right().y()),
              fmax(box0.upper_right().z(), box1.upper_right().z()));

    return Bbox(ll, ur);
}
