#ifndef BVH_H
#define BVH_H

#include "canvas/object.hh"

class BVHNode final : public Object {
   public:
    BVHNode() = default;

    BVHNode(const HittableObjectLists& list, double time0, double time1)
        : BVHNode(list.objects(), 0, list.objects().size(), time0, time1) {}

    BVHNode(
        const std::vector<std::shared_ptr<Object>>& src_objects,
        size_t start, size_t end, double time0, double time1);

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool maybe_get_bbox(double time0, double time1, Bbox& output_box) const override;

   public:
    std::shared_ptr<Object> left;
    std::shared_ptr<Object> right;
    Bbox box;
};

#endif