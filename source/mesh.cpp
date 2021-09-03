#include "ssr/mesh.hpp"

#include <memory>
#include <string>
#include <vector>

#include "ssr/math.hpp"

using namespace ssr;

std::shared_ptr<mesh> mesh::make_cube(const float size)
{
    auto extent = size * 0.5f;
    return std::make_shared<mesh>(
        std::vector<float3>{
            {extent, -extent, -extent},
            {-extent, -extent, -extent},
            {extent, -extent, extent},
            {-extent, -extent, extent},
            {-extent, extent, extent},
            {extent, extent, extent},
            {-extent, extent, -extent},
            {extent, extent, -extent},
        },
        std::vector<int3>{
            {0, 2, 1},
            {2, 3, 1},
            {4, 3, 5},
            {5, 3, 2},
            {4, 6, 3},
            {3, 6, 1},
            {2, 0, 5},
            {5, 0, 7},
            {1, 6, 0},
            {0, 6, 7},
            {6, 4, 7},
            {7, 4, 5},
        });
}

std::shared_ptr<mesh> mesh::load_obj(std::istream &source)
{
    std::vector<float3> vertices;
    std::vector<int3> indices;

    std::string command;
    while (source >> command)
    {
        if (command == "v")
        {
            float3 v;

            if (!(source >> v.x) || !(source >> v.y) || !(source >> v.z))
            {
                return nullptr;
            }

            vertices.push_back(v);
        }

        if (command == "f")
        {
            std::string i0, i1, i2;

            if (!(source >> i0) || !(source >> i1) || !(source >> i2))
            {
                return nullptr;
            }

            indices.push_back({std::stoi(i0, nullptr) - 1, std::stoi(i1, nullptr) - 1, std::stoi(i2, nullptr) - 1});
        }
    }

    return std::make_shared<mesh>(vertices, indices);
}

void mesh::write_obj(std::ostream &out)
{
    for (const auto &v : m_vertices)
    {
        out << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }

    for (const auto &f : m_indices)
    {
        out << "f " << f.x + 1 << " " << f.y + 1 << " " << f.z + 1 << "\n";
    }
}
