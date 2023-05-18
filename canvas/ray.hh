#ifndef RAY_H
#define RAY_H

#include "common/vec3.hh"

class Ray {
   public:
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction, const double time = 0.)
        : orig_(origin), dir_(direction), time_(time) {}

    Point3 origin() const { return orig_; }
    Vec3 direction() const { return dir_; }
    double time() const { return time_; }

    Point3 at(double t) const {
        return orig_ + t * dir_;
    }

   private:
    Point3 orig_;
    Vec3 dir_;
    double time_;
};

#endif