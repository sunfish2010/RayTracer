#include "canvas/bvh.hh"

BVHNode::BVHNode(const std::vector<std::shared_ptr<Object>>& src_objects,
                 size_t start, size_t end, double time0, double time1) {
    auto objects = src_objects;

    int axis = random_number<int>(0, 2);

    std::cout << "Axis is " << axis << std::endl;

    auto comparator = [&axis](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
        return box_compare(a, b, axis);
    };

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(objects, start, mid, time0, time1);
        right = std::make_shared<BVHNode>(objects, mid, end, time0, time1);
    }

    Bbox box_left, box_right;

    if (!left->maybe_get_bbox(time0, time1, box_left) || !right->maybe_get_bbox(time0, time1, box_right))
        std::cerr << "No bounding box in bounding volume hiearchies.\n";

    box = surrounding_box(box_left, box_right);
}

bool BVHNode::maybe_get_bbox(double time0, double time1, Bbox& output_box) const {
    output_box = box;
    return true;
}

bool BVHNode::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}
