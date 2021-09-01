#include "ssr/rasterizer.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

#include "ssr/math.hpp"

using namespace ssr;

float edge_func(const float3& a, const float3& b, const float3& c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

float3 ndc_to_raster(const float4& ndc, const int2& resolution) {
    return {(ndc.x + 1) / 2 * resolution.x, (1 - ndc.y) / 2 * resolution.y, -ndc.z};
}

const void rasterizer::render_pixmap(std::ostream& color_ppm, std::ostream& depth_ppm) {
    std::vector<std::vector<float3>> image_buffer;
    std::vector<std::vector<float>> depth_buffer;

    image_buffer.resize(m_resolution.x, std::vector<float3>(m_resolution.y, {0, 0, 0}));
    depth_buffer.resize(m_resolution.x, std::vector<float>(m_resolution.y, m_camera.m_clip_far));

    for (const auto& object : m_objects) {
        const float4x4 mvp = mul(m_camera.projection_matrix(m_resolution), m_camera.m_view, object.m_transform);

        const auto mesh = object.m_mesh;
        for (auto i = 0; i < mesh->m_indices.size(); ++i) {
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
            v0_raster.z /= 1;
            v1_raster.z /= 1;
            v2_raster.z /= 1;

            auto area = edge_func(v0_raster, v1_raster, v2_raster);

            // TODO: Use 2d bounds of triangle as extents
            for (auto x = 0; x < m_resolution.x; x++) {
                for (auto y = 0; y < m_resolution.y; y++) {
                    float3 sample = {x + .5, y + .5, 0};

                    auto w0 = edge_func(v1_raster, v2_raster, sample);
                    auto w1 = edge_func(v2_raster, v0_raster, sample);
                    auto w2 = edge_func(v0_raster, v1_raster, sample);

                    if (w0 < 0 || w1 < 0 || w2 < 0)
                        continue;

                    // TODO: some kind of normalization? - review
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    auto reciprocal_z = v0_raster.z * w0 + v1_raster.z * w1 + v2_raster.z * w2;
                    auto z            = 1.0f / reciprocal_z;

                    if (z >= depth_buffer[x][y])
                        continue;

                    depth_buffer[x][y] = z;
                    image_buffer[x][y] = color;
                }
            }
        }
    }

    color_ppm << "P3\n";
    color_ppm << m_resolution.x << " " << m_resolution.y << "\n255\n";

    for (auto y = 0; y < m_resolution.y; y++) {
        for (auto x = 0; x < m_resolution.x; x++) {
            auto color = image_buffer[x][y];
            // TODO: use buffer instead of stream operations
            color_ppm << int(255 * color[0]) << " " << int(255 * color[1]) << " " << int(255 * color[2]) << "\n";
        }
    }

    depth_ppm << "P3\n";
    depth_ppm << m_resolution.x << " " << m_resolution.y << "\n255\n";

    for (auto y = 0; y < m_resolution.y; y++) {
        for (auto x = 0; x < m_resolution.x; x++) {
            auto depth = depth_buffer[x][y] / m_camera.m_clip_far;
            // TODO: use buffer instead of stream operations
            depth_ppm << int(255 * depth) << " " << int(255 * depth) << " " << int(255 * depth) << "\n";
        }
    }
}
