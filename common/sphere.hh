#ifndef SPHERE_H
#define SPHERE_H

#include "common/object.hh"

class Sphere final : public Object {
   public:
    Sphere() {}
    Sphere(Point3 cen, double r) : center_(cen), radius_(r){};

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

   private:
    Point3 center_;
    double radius_;
};

#endif