#include <iostream>

#include "common/color.hh"
#include "common/ray.hh"

namespace {

struct Sphere {
    point3 center;
    double radius;
};

bool hit_sphere(const Sphere& sphere, const ray& r) {
    vec3 oc = r.origin() - sphere.center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - sphere.radius * sphere.radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

color ray_color(const ray& r, const Sphere& sphere) {
    if (hit_sphere(sphere, r)) {
        return {0.8, 0., 0.};
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    // Linear blend of blue and white color.
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

constexpr double ASPECT_RATIO = 16.0 / 9.0;
}  // namespace

int main() {
    // Image

    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / ASPECT_RATIO);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = ASPECT_RATIO * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Add a sphere in the middle of the scene scene.
    Sphere sphere{.center = vec3(0, 0, -focal_length), 0.5};

    // Render
    std::cout
        << "P3\n"
        << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = static_cast<double>(i) / (image_width - 1);
            auto v = static_cast<double>(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r, sphere);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}