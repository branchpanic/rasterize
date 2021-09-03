#include "ssr/rasterizer.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

#include "linalg.h"
#include "ssr/math.hpp"

using namespace ssr;

// Pineda edge function - determine if point c lies on the right or left of the line formed by a and b
// >0 when on the right, <0 when on the left
// Line direction is a -> b, so triangles should be wound clockwise
// Divide by area to get barycentric coordinates
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
float edge_func(const float3 &a, const float3 &b, const float3 &c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

float3 ndc_to_raster(const float4 &ndc, const int2 &resolution)
{
    return {(ndc.x + 1) / 2 * resolution.x, (1 - ndc.y) / 2 * resolution.y, -ndc.z};
}

struct bounds
{
    int2 min;
    int2 max;
};

// Compute bounds of triangle in raster space
bounds triangle_bounds(const float3 &v1, const float3 &v2, const float3 &v3, const int2 &resolution)
{
    auto x_bounds = std::minmax({v1.x, v2.x, v3.x});
    auto y_bounds = std::minmax({v1.y, v2.y, v3.y});

    return bounds{
        .min = {std::max(0, (int)x_bounds.first), std::max(0, (int)y_bounds.first)},
        .max = {std::min(resolution.x - 1, (int)x_bounds.second), std::min(resolution.y - 1, (int)y_bounds.second)},
    };
}

const void rasterizer::render_pixmap(std::ostream &color_ppm, std::ostream &depth_ppm)
{
    std::vector<std::vector<float3>> image_buffer;
    std::vector<std::vector<float>> depth_buffer;

    auto objects = m_scene.m_objects;
    auto camera = m_scene.m_camera;

    image_buffer.resize(m_resolution.x, std::vector<float3>(m_resolution.y, m_background_color));
    depth_buffer.resize(m_resolution.x, std::vector<float>(m_resolution.y, camera.m_clip_far));

    for (const auto &object : objects)
    {
        const float4x4 mv = mul(camera.m_view, object.transform());
        const float4x4 mvp = mul(camera.projection_matrix(m_resolution), camera.m_view, object.transform());

        const auto mesh = object.m_mesh;
        for (auto i = 0; i < mesh->m_indices.size(); ++i)
        {
            const float3 color = {1., 1., 1.};

            int3 triangle = mesh->m_indices[i];

            float4 v0_ndc{mesh->m_vertices[triangle[0]], 1};
            float4 v1_ndc{mesh->m_vertices[triangle[1]], 1};
            float4 v2_ndc{mesh->m_vertices[triangle[2]], 1};

            // Local space -> NDC space (+ perspective divide)
            v0_ndc = mul(mvp, v0_ndc);
            v1_ndc = mul(mvp, v1_ndc);
            v2_ndc = mul(mvp, v2_ndc);
            v0_ndc /= v0_ndc.w;
            v1_ndc /= v1_ndc.w;
            v2_ndc /= v2_ndc.w;

            // NDC space -> Raster space
            auto v0_raster = ndc_to_raster(v0_ndc, m_resolution);
            auto v1_raster = ndc_to_raster(v1_ndc, m_resolution);
            auto v2_raster = ndc_to_raster(v2_ndc, m_resolution);

            // 1/z used for interpolation later
            v0_raster.z /= 1;
            v1_raster.z /= 1;
            v2_raster.z /= 1;

            auto area = edge_func(v0_raster, v1_raster, v2_raster);
            auto bounds = triangle_bounds(v0_raster, v1_raster, v2_raster, m_resolution);

            for (auto x = bounds.min.x; x <= bounds.max.x; x++)
            {
                for (auto y = bounds.min.y; y <= bounds.max.y; y++)
                {
                    float3 sample = {x + .5, y + .5, 0};

                    auto w0 = edge_func(v1_raster, v2_raster, sample);
                    auto w1 = edge_func(v2_raster, v0_raster, sample);
                    auto w2 = edge_func(v0_raster, v1_raster, sample);

                    if (w0 < 0 || w1 < 0 || w2 < 0)
                        continue;

                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    // Interpolate z coordinate on the triangle surface at this pixel
                    // Note that v[0-2]_raster.z is reciprocal
                    auto z = 1.0f / (v0_raster.z * w0 + v1_raster.z * w1 + v2_raster.z * w2);

                    if (z >= depth_buffer[x][y])
                        continue;

                    depth_buffer[x][y] = z;
                    image_buffer[x][y] = color;
                }
            }
        }
    }

    // TODO: Don't use stream operations to write buffer contents (slow)

    color_ppm << "P3\n";
    color_ppm << m_resolution.x << " " << m_resolution.y << "\n255\n";

    for (auto y = 0; y < m_resolution.y; y++)
    {
        for (auto x = 0; x < m_resolution.x; x++)
        {
            auto color = image_buffer[x][y];
            color_ppm << int(255 * color[0]) << " " << int(255 * color[1]) << " " << int(255 * color[2]) << "\n";
        }
    }

    depth_ppm << "P3\n";
    depth_ppm << m_resolution.x << " " << m_resolution.y << "\n255\n";

    for (auto y = 0; y < m_resolution.y; y++)
    {
        for (auto x = 0; x < m_resolution.x; x++)
        {
            auto depth = depth_buffer[x][y] / camera.m_clip_far;
            depth_ppm << int(255 * depth) << " " << int(255 * depth) << " " << int(255 * depth) << "\n";
        }
    }
}
