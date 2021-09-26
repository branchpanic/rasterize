//
// rasterizer.hpp: Main rasterizer object
//

#ifndef SSR_RASTERIZER_H_
#define SSR_RASTERIZER_H_

#include <ostream>
#include <vector>

#include "ssr/camera.hpp"
#include "ssr/math.hpp"
#include "ssr/mesh.hpp"
#include "ssr/object.hpp"
#include "ssr/scene.hpp"

namespace ssr
{

class rasterizer
{
  public:
    int2 m_resolution;
    scene m_scene;
    float3 m_background_color;
    bool m_smooth_shading;

  public:
    rasterizer(int2 resolution, scene scene, float3 background_color = {0, 0, 0}, bool smooth_shading = false)
        : m_resolution(resolution), m_scene(scene), m_background_color(background_color),
          m_smooth_shading(smooth_shading)
    {
    }

    void render_pixmap(std::ostream &, std::ostream &) const;
};

} // namespace ssr

#endif
