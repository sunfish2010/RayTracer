#include <iostream>
#include <memory>
#include <thread>

#include "common/camera.hh"
#include "common/color.hh"
#include "common/constants.hh"
#include "common/sphere.hh"

namespace {

constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr double FOCAL_LENGTH = 1.0;

Color ray_color(const Ray& r, const HittableObjectLists& world) {
    HitRecord rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * Color(Vec3(1, 1, 1) + rec.normal);
    }
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    // Linear blend of blue and white color.
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

}  // namespace

int main() {
    // Image
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / ASPECT_RATIO);
    const int samples_per_pixel = 100;
    const int view_port_height = 2.0;

    // Camera
    Camera cam(ASPECT_RATIO, view_port_height, FOCAL_LENGTH);

    // Add a sphere in the middle of the scene scene.
    HittableObjectLists world;
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -FOCAL_LENGTH), 100.0));
    world.add(std::make_shared<Sphere>(Point3(0, 0, -FOCAL_LENGTH), 0.5));

    std::cerr << "Number of threads available to use " << std::thread::hardware_concurrency() << std::endl;

    // Render
    std::cout
        << "P3\n"
        << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_number<double>(0., 1.)) / (image_width - 1);
                auto v = (j + random_number<double>(0., 1.)) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}