#ifndef SSR_MATERIAL_H_
#define SSR_MATERIAL_H_

#include "linalg.h"
#include "ssr/math.hpp"
#include <array>

namespace ssr
{

struct shading_params
{
    float3 barycentric_coords;

    float3 face_normal;
    std::array<float3, 3> vertex_normals;

    float3 light_orientation;
};

class material
{
  public:
    virtual ~material()
    {
    }

    virtual float3 shade(const shading_params &in) const = 0;
};

class lambert : public material
{
  public:
    float3 m_color;
    bool m_smooth;

    lambert(float3 color, bool smooth = true) : m_color(color), m_smooth(smooth)
    {
    }

    float3 shade(const shading_params &in) const override;
};

} // namespace ssr

#endif
