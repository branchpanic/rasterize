#include "ssr/camera.hpp"

#include "ssr/math.hpp"

using namespace ssr;

const float4x4 camera::projection_matrix(const int2 &output_resolution)
{
    return perspective_matrix(m_fov, output_resolution.x / (float)output_resolution.y, m_clip_near, m_clip_far);
}
