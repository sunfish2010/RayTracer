#ifndef CAMERA_H
#define CAMERA_H

#include "canvas/ray.hh"
#include "common/constants.hh"

class Camera {
   public:
    explicit Camera(const Point3& camera_center,
                    const Point3& world_center,
                    const Vec3& world_up,
                    const double fov,  // vertical field of view in degrees
                    const double aperture,
                    const double focus_dist,
                    const double aspect_ratio = 16.0 / 9, const double t0 = 0, const double t1 = 0) {
        const auto viewport_height = 2.0 * tan(degrees_to_radians(fov) / 2);
        const auto viewport_width = aspect_ratio * viewport_height;
        w_ = unit_vector(camera_center - world_center);  // Pointing into the camera as positive.
        u_ = unit_vector(cross(world_up, w_));           // Width
        v_ = cross(w_, u_);                              // Height.
        origin_ = camera_center;
        horizontal_ = focus_dist * viewport_width * u_;
        vertical_ = focus_dist * viewport_height * v_;
        lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - focus_dist * w_;
        lens_radius_ = aperture / 2;
        time0_ = t0;
        time1_ = t1;
    }

    Ray get_ray(double u, double v) const {
        Vec3 blur = lens_radius_ * random_in_unit_disk();
        Vec3 offset = u_ * blur.x() + v_ * blur.y();
        return Ray(origin_ + offset, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_ - offset, random_number(time0_, time1_));
    }

    // double focal_length() const { return focal_length_; }

   private:
    Point3 origin_;
    Point3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 u_, v_, w_;
    double lens_radius_;
    // Shutter open and close times.
    double time0_, time1_;
};
#endif