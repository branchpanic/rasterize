//
// mesh.hpp: Mesh data structure and utilities
//

#ifndef SSR_MESH_H_
#define SSR_MESH_H_

#include <iostream>
#include <vector>

#include "ssr/math.hpp"

namespace ssr
{

struct mesh
{
    std::vector<float3> m_vertices{};
    std::vector<float3> m_vertex_normals{};
    std::vector<int3> m_indices{};

    mesh(std::vector<float3> vertices, std::vector<int3> indices) : m_vertices(vertices), m_indices(indices)
    {
    }

    void write_obj(std::ostream &out);

    static std::shared_ptr<mesh> make_cube(const float size = 1);

    // TODO: WARNING: no error checking in this function
    // Vertices must not contain w component and faces need exactly 3 vertices
    // Unknown commands will be ignored
    //
    // Perhaps we can switch to assimp or gltf or something, because improving this parser is out of scope for this
    // project
    static std::shared_ptr<mesh> load_obj(std::istream &text);

  private:
    void calculate_vertex_normals();
};

} // namespace ssr

#endif
