#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "canvas/camera.hh"
#include "canvas/color.hh"
#include "canvas/sphere.hh"
#include "common/constants.hh"

namespace {

constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr double FOCAL_LENGTH = 1.0;
constexpr double EPS = 0.001;
const Color BLACK_COLOR = Color(0, 0, 0);

Color ray_color(const Ray& r, const HittableObjectLists& world, int depth) {
    if (depth <= 0) {
        return BLACK_COLOR;
    }
    HitRecord rec;
    if (world.hit(r, EPS, infinity, rec)) {
        Ray scattered;
        if (rec.hit_object->scatter(r, rec, scattered)) {
            return rec.hit_object->albedo() * ray_color(scattered, world, depth - 1);
        }
        return BLACK_COLOR;
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
    const int max_depth = 50;

    // Camera
    Camera cam(ASPECT_RATIO, view_port_height, FOCAL_LENGTH);

    // Add a sphere in the middle of the scene scene.
    HittableObjectLists world;
    // Ground.
    // const auto ground_texture = ;
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -FOCAL_LENGTH), 100.0, Texture{.material = Material::DIFFUSIVE, .albedo = Color(0.8, 0.8, 0.8)}));
    // Center.
    world.add(std::make_shared<Sphere>(Point3(0, 0, -FOCAL_LENGTH), 0.5, Texture{.material = Material::DIFFUSIVE, .albedo = Color(0.7, 0.3, 0.3)}));

    std::cerr << "Number of threads available to use " << std::thread::hardware_concurrency() << std::endl;

    // Render
    std::cout
        << "P3\n"
        << image_width << ' ' << image_height << "\n255\n";
    // std::vector<std::future<Color>> results(samples_per_pixel);
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            // Costly way of anti-aliasing.
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_number<double>(0., 1.)) / (image_width - 1);
                auto v = (j + random_number<double>(0., 1.)) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
                // results[s] = std::async(std::launch::async, ray_color, r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
            // for (auto& result : results) {
            //     pixel_color += result.get();
            // }
        }
    }
    std::cerr << "\nDone.\n";
}