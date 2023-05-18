#include <algorithm>
// #include <execution> Not supported by apple clang https://en.cppreference.com/w/cpp/compiler_support/17#C.2B.2B17_library_features
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

// Generate random world of spheres.
HittableObjectLists generate_random_scene() {
    HittableObjectLists world;
    // Ground.
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000.0, Texture(Material::DIFFUSIVE, Color(0.5, 0.5, 0.5))));

    const int range = 11;
    const auto focus = Point3(4, 0.2, 0);

    for (int i = -range; i <= range; ++i) {
        for (int j = -range; j <= range; ++j) {
            const auto choose_material = random_number<double>(0, 1);
            Point3 center(i + 0.9 * random_number<double>(0, 1), 0.2, j + 0.9 * random_number<double>(0, 1));
            if ((center - focus).length() > 0.9) {
                if (choose_material < 0.8) {
                    // Diffiuse.
                    Color albedo(random_number<double>(0, 0.5), random_number<double>(0, 0.5), random_number<double>(0, 0.5));
                    const auto center2 = center + Vec3(0, random_number<double>(0, 0.5), 0);
                    world.add(std::make_shared<MovingSphere>(center, center2, 0, 1, 0.2, Texture(Material::DIFFUSIVE, albedo)));
                } else if (choose_material < 0.95) {
                    // Metal.
                    Color albedo(random_number<double>(0.5, 1), random_number<double>(0.5, 1), random_number<double>(0.5, 1));
                    const double fuzz = random_number<double>(0, 0.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, Texture(Material::METAL, albedo, fuzz)));
                } else {
                    // Glass.
                    world.add(std::make_shared<Sphere>(center, 0.2, Texture(Material::DIELECTRIC, Color(1, 1, 1), 1, 1.5)));
                }
            }
        }
    }
    // Large spheres near the center.
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, Texture(Material::DIELECTRIC, Color(1, 1, 1), 1, 1.5)));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, Texture(Material::DIFFUSIVE, Color(0.4, 0.2, 0.1))));
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, Texture(Material::METAL, Color(0.7, 0.6, 0.5), 0.0)));

    return world;
}

}  // namespace

int main() {
    // Image
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / ASPECT_RATIO);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // Camera
    Vec3 camera_center(13, 2, 3);
    Vec3 world_center(0, 0, 0);
    Camera cam(camera_center, world_center, Vec3(0, 1, 0), 20, 0.1, 10., ASPECT_RATIO, 0, 1);

    const auto world = generate_random_scene();

    std::cerr << "Number of threads available to use " << std::thread::hardware_concurrency() << std::endl;

    // Render
    std::cout
        << "P3\n"
        << image_width << ' ' << image_height << "\n255\n";
    std::vector<int> samples(samples_per_pixel);
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            const auto compute_color_for_pixel = [&cam, &world, &i, &j](Color sum_so_far, int s) {
                auto u = (i + random_number<double>(0., 1.)) / (image_width - 1);
                auto v = (j + random_number<double>(0., 1.)) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                return std::move(sum_so_far) + ray_color(r, world, max_depth);
            };

            Color pixel_color = std::accumulate(samples.begin(), samples.end(), Color(0, 0, 0), compute_color_for_pixel);
            // std::transform(std::execution::par_unseq, samples.begin(), samples.end(), results.begin(), compute_color_for_pixel);
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}