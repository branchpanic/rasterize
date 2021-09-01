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

int main(int argc, char const* argv[]) {
    std::ifstream obj_file("object.obj");
    const auto obj_mesh = std::make_shared<ssr::mesh>(ssr::mesh::load_obj(obj_file));

    rasterizer rasterizer(int2{800, 600},
        camera{
            lookat_matrix<float>({0, 0, 1.5f}, {0, 0, 0}, {0, 1, 0}),
            110.0_deg,
        },
        {
            ssr::object{obj_mesh},
            ssr::object{obj_mesh, translation_matrix<float>({1, 0, 0})},
        });

    std::ofstream out_color("out.color.ppm");
    std::ofstream out_depth("out.depth.ppm");

    rasterizer.render_pixmap(out_color, out_depth);

    out_color.close();
    out_depth.close();

    std::cout << "OK!" << std::endl;
}
