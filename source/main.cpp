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

std::shared_ptr<mesh> load_sample(std::string path)
{
    std::ifstream obj_file(path);
    if (!obj_file.is_open())
        return nullptr;

    const auto obj = mesh::load_obj(obj_file);
    obj_file.close();
    return obj;
}

int main()
{
    const auto suzanne = load_sample("samples/suzanne.obj");
    if (suzanne == nullptr)
        return EXIT_FAILURE;

    const auto cube_sphere = load_sample("samples/cube_sphere.obj");
    if (cube_sphere == nullptr)
        return EXIT_FAILURE;

    const auto unit_cube = mesh::make_cube(.5f);

    scene scene{
        camera::look_at({.2f, .2f, 2.f}, {0, 0, 0}, {0, 1, 0}, 60.0_deg),
        std::vector<object>{
            object{suzanne},
        },
    };

    rasterizer rasterizer(int2{800, 600}, scene, float3{0.f}, true);

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
