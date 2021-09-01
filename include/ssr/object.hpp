//
// object.hpp: Scene object
//

#ifndef SSR_OBJECT_H_
#define SSR_OBJECT_H_

#include <memory>

#include "ssr/math.hpp"
#include "ssr/mesh.hpp"

namespace ssr {
    struct object {
        std::shared_ptr<mesh> m_mesh{nullptr};
        float4x4 m_transform{identity};
    };
} // namespace ssr

#endif