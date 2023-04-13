#ifndef RAY_H
#define RAY_H

#include "common/vec3.hh"

class Ray {
   public:
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction)
        : orig(origin), dir(direction) {}

    Point3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    Point3 at(double t) const {
        return orig + t * dir;
    }

   private:
    Point3 orig;
    Vec3 dir;
};

#endif