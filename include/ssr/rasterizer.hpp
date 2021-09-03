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

  public:
    rasterizer(int2 resolution, scene scene, float3 background_color = {0, 0, 0})
        : m_resolution(resolution), m_scene(scene), m_background_color(background_color)
    {
    }

    const void render_pixmap(std::ostream &, std::ostream &);
};

} // namespace ssr

#endif
