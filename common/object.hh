#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>

#include "common/ray.hh"

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Object {
   public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

class HittableObjectLists : Object {
   public:
    explicit HittableObjectLists(const std::shared_ptr<Object> object) : objects{object} {}

    HittableObjectLists() = default;

    void clear() { objects.clear(); }

    void add(const std::shared_ptr<Object> object) { objects.push_back(object); }

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

   private:
    std::vector<std::shared_ptr<Object>> objects;
};

#endif