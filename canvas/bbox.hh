#ifndef BBOX
#define BBOX

#include "canvas/ray.hh"

// Axis aligned bounding box.
class Bbox {
   public:
    Bbox() {}
    Bbox(const Point3& ll, const Point3& ur) : lower_left_(ll), upper_right_(ur) {}

    Point3 lower_left() const { return lower_left_; }

    Point3 upper_right() const { return upper_right_; }

    bool hit(const Ray& r, double t_min, double t_max) const;

   private:
    Point3 lower_left_;
    Point3 upper_right_;
};

Bbox surrounding_box(const Bbox& box0, const Bbox& box1);

#endif