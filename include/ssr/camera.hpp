//
// camera.hpp: Camera data structure
//

#ifndef SSR_CAMERA_H_
#define SSR_CAMERA_H_

#include "ssr/math.hpp"

namespace ssr {

    struct camera {
        float4x4 m_view{identity};
        float m_fov{90.0_deg};
        float m_clip_near{0.01f};
        float m_clip_far{1000.0f};

        const float4x4 projection_matrix(const int2& output_resolution);
    };

} // namespace ssr

#endif