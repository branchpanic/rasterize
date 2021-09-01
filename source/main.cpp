#include <fstream>
#include <iostream>
#include <linalg.h>
#include <memory>

#include "ssr/camera.hpp"
#include "ssr/math.hpp"
#include "ssr/mesh.hpp"
#include "ssr/rasterizer.hpp"

using namespace ssr;

int main(int argc, char const* argv[]) {
    const auto unit_cube = std::make_shared<ssr::mesh>(ssr::mesh::make_cube());

    rasterizer rasterizer(int2{800, 600}, camera{lookat_matrix<float>({1, 1, 2}, {0, 0, 0}, {0, 1, 0})},
        {
            ssr::object{unit_cube},
            ssr::object{unit_cube, pose_matrix<float>({1, 0, 0, 0}, {0, 2, 0})},
            ssr::object{unit_cube, pose_matrix<float>({1, 0, 0, 0}, {2, 0, 0})},
            ssr::object{unit_cube, pose_matrix<float>({1, 0, 0, 0}, {0, 0, -2})},
        });

    std::ofstream out_color("out.color.ppm");
    std::ofstream out_depth("out.depth.ppm");

    rasterizer.render_pixmap(out_color, out_depth);

    out_color.close();
    out_depth.close();

    std::cout << "OK!" << std::endl;
}