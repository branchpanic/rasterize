#include "ssr/material.hpp"

using namespace ssr;

float3 lambert::shade(const shading_params &in) const
{
    float3 normal;
    if (m_smooth)
    {
        normal = normalize(in.barycentric_coords[0] * in.vertex_normals[0] +
                           in.barycentric_coords[1] * in.vertex_normals[1] +
                           in.barycentric_coords[2] * in.vertex_normals[2]);
    }
    else
    {
        normal = in.face_normal;
    }

    auto lambert = max(0, dot(normal, in.light_orientation));

    // TODO: Still half-lambert, should be moved elsewhere
    lambert = 0.5f * lambert + 0.5f;
    lambert *= lambert;

    return lambert * m_color;
}
