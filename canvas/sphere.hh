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

    virtual bool maybe_get_bbox(double time0, double time1, Bbox& output_box) const override;

    Point3 center() const { return center_; }

    double radius() const { return radius_; }

   private:
    Point3 center_;
    double radius_;
};

class MovingSphere final : public Object, public std::enable_shared_from_this<MovingSphere> {
   public:
    MovingSphere() {}
    MovingSphere(const Point3& cen0, const Point3& cen1, double t0, double t1, double r, const Texture& texture)
        : center0_(cen0), center1_(cen1), time0_(t0), time1_(t1), radius_(r) {
        this->set_texture(texture);
    }

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    // Get the bbox that covers the sphere from time0 to time1.
    virtual bool maybe_get_bbox(double time0, double time1, Bbox& output_box) const override;

    Point3 center(double time) const;

   private:
    Point3 center0_, center1_;
    double time0_, time1_;
    double radius_;
};

#endif