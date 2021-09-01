//
// mesh.hpp: Mesh data structure and utilities
//

#ifndef SSR_MESH_H_
#define SSR_MESH_H_

#include <iostream>
#include <vector>

#include "ssr/math.hpp"

namespace ssr {
    struct mesh {
        std::vector<float3> m_vertices{};
        std::vector<int3> m_indices{};

        static ssr::mesh make_cube(const float size = 1);
        static ssr::mesh load_obj(std::istream& text);

        void write_obj(std::ostream& out);
    };
} // namespace ssr

#endif
