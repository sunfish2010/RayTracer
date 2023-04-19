#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>

#include "canvas/ray.hh"

enum Material {
    DIFFUSIVE = 0,
    METAL = 1
};

class Object;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<const Object> hit_object;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct Texture {
    Material material;
    Color albedo;
};

class Object {
   public:
    Object() = default;

    explicit Object(const Texture& texture) : texture_(texture) {}

    void set_texture(const Texture& texture) {
        texture_ = texture;
    }

    Color albedo() const { return texture_.albedo; }

    bool scatter(
        const Ray& r_in, const HitRecord& rec, Ray& scattered) const;

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;

   private:
    Texture texture_;
};

class HittableObjectLists : Object {
   public:
    explicit HittableObjectLists(const std::shared_ptr<Object> object) : objects_{object} {}

    HittableObjectLists() = default;

    void clear() { objects_.clear(); }

    void add(const std::shared_ptr<Object> object) { objects_.push_back(object); }

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

   private:
    std::vector<std::shared_ptr<Object>> objects_;
};

#endif