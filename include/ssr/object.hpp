//
// object.hpp: Scene object
//

#ifndef SSR_OBJECT_H_
#define SSR_OBJECT_H_

#include <memory>

#include "ssr/material.hpp"
#include "ssr/math.hpp"
#include "ssr/mesh.hpp"

namespace ssr
{

struct object
{
    std::shared_ptr<mesh> m_mesh{nullptr};
    std::shared_ptr<material> m_material{nullptr};
    float3 m_position{0, 0, 0};
    float4 m_rotation{0, 0, 0, 1};
    float3 m_scale{1, 1, 1};

    const float4x4 transform() const;
};

} // namespace ssr

#endif
