//
// camera.hpp: Camera data structure
//

#ifndef SSR_CAMERA_H_
#define SSR_CAMERA_H_

#include "ssr/math.hpp"

namespace ssr
{

struct camera
{
    float4x4 m_view{identity};
    float m_fov{90.0_deg};
    float m_clip_near{0.01f};
    float m_clip_far{1000.0f};

    const float4x4 projection_matrix(const int2 &output_resolution);

    camera()
    {
    }

    camera(float4x4 view, float fov, float clip_near, float clip_far)
        : m_view(view), m_fov(fov), m_clip_near(clip_near), m_clip_far(clip_far)
    {
    }

    static camera look_at(const float3 &eye, const float3 &center, const float3 &up = {0, 1, 0}, float fov = 90.0_deg,
                          float clip_near = 0.01f, float clip_far = 1000.0f)
    {
        return camera(lookat_matrix(eye, center, up), fov, clip_near, clip_far);
    }
};

} // namespace ssr

#endif
