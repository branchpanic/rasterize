#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <linalg.h>
#include <memory>

#include "ssr/camera.hpp"
#include "ssr/math.hpp"
#include "ssr/mesh.hpp"
#include "ssr/rasterizer.hpp"

using namespace ssr;
using namespace linalg::ostream_overloads;

int main(int argc, char const *argv[])
{
    std::ifstream obj_file("samples/suzanne.obj");

    const auto obj = mesh::load_obj(obj_file);
    if (obj == nullptr)
    {
        std::cerr << "Failed to load obj file" << std::endl;
        return EXIT_FAILURE;
    }

    const auto unit_cube = mesh::make_cube(.5f);

    scene scene{camera::look_at({0, 0, 1.5f}, {0, 0, 0}, {0, 1, 0}, 90.0_deg),
                std::vector<object>{
                    object{obj},
                    object{unit_cube, {1, 0, 0}, rotation_quat({0.f, .5f, .5f}, (float)45.0_deg)},
                    object{unit_cube, {-1, 0, 0}, {0, 0, 0, 1}, {0.5, 1.5, 0.5}},
                }};

    rasterizer rasterizer(int2{1280, 720}, scene, float3{.1f, .1f, .1f});

    std::ofstream out_color("out.color.ppm");
    std::ofstream out_depth("out.depth.ppm");

    auto start = std::chrono::steady_clock::now();

    rasterizer.render_pixmap(out_color, out_depth);

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Render finished in " << elapsed.count() << "ms" << std::endl;

    out_color.close();
    out_depth.close();
}
