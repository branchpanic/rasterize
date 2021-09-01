#include "ssr/mesh.hpp"

#include <string>

#include "ssr/math.hpp"

using namespace ssr;

mesh mesh::make_cube(const float size) {
    auto extent = size * 0.5f;

    return mesh{.m_vertices =
                    {
                        {-extent, -extent, -extent},
                        {-extent, -extent, extent},
                        {extent, -extent, extent},
                        {extent, -extent, -extent},
                        {-extent, extent, -extent},
                        {-extent, extent, extent},
                        {extent, extent, extent},
                        {extent, extent, -extent},
                    },
        .m_indices = {
            {3, 0, 2},
            {2, 0, 3},

            {5, 1, 6},
            {6, 1, 2},

            {5, 4, 1},
            {1, 4, 0},

            {2, 3, 6},
            {6, 3, 7},

            {0, 4, 3},
            {3, 4, 7},

            {4, 5, 7},
            {7, 5, 6},
        }};
}

bool mesh::try_load_obj(std::istream& source, mesh& out) {
    std::vector<float3> vertices;
    std::vector<int3> indices;

    std::string command;
    while (source >> command) {
        if (command == "v") {
            float3 v;

            if (!(source >> v.x) || !(source >> v.y) || !(source >> v.z))
                return false;

            vertices.push_back(v);
        }

        if (command == "f") {
            std::string i0, i1, i2;

            if (!(source >> i0) || !(source >> i1) || !(source >> i2))
                return false;

            indices.push_back({std::stoi(i0, nullptr), std::stoi(i1, nullptr), std::stoi(i2, nullptr)});
        }
    }

    out = mesh{.m_vertices = vertices, .m_indices = indices};
    return true;
}