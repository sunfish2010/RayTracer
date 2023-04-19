#ifndef SPHERE_H
#define SPHERE_H

#include "canvas/object.hh"

class Sphere final : public Object, public std::enable_shared_from_this<Sphere> {
   public:
    Sphere() {}
    Sphere(const Point3& cen, double r, const Texture& texture) : center_(cen), radius_(r) {
        this->set_texture(texture);
    };

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

   private:
    Point3 center_;
    double radius_;
};

#endif