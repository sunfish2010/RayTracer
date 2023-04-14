#ifndef CAMERA_H
#define CAMERA_H

#include "common/constants.hh"
#include "common/ray.hh"

class Camera {
   public:
    explicit Camera(const double aspect_ratio = 16.0 / 9, const double viewport_height = 2.0, const double focal_length = 1.0) {
        focal_length_ = focal_length;
        const auto viewport_width = aspect_ratio * viewport_height;
        origin_ = Point3(0, 0, 0);
        horizontal_ = Vec3(viewport_width, 0.0, 0.0);
        vertical_ = Vec3(0.0, viewport_height, 0.0);
        lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - Vec3(0, 0, focal_length);
    }

    Ray get_ray(double u, double v) const {
        return Ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
    }

    double focal_length() const { return focal_length_; }

   private:
    double focal_length_;
    Point3 origin_;
    Point3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
};
#endif