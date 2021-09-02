#ifndef SSR_RASTERIZER_H_
#define SSR_RASTERIZER_H_

#include <ostream>
#include <vector>

#include "ssr/camera.hpp"
#include "ssr/math.hpp"
#include "ssr/mesh.hpp"
#include "ssr/object.hpp"

namespace ssr
{

class rasterizer
{
  private:
    std::vector<object> m_objects;

  public:
    int2 m_resolution;
    camera m_camera;

  public:
    rasterizer(int2 resolution, camera camera, std::vector<object> objects = {})
        : m_resolution(resolution), m_camera(camera), m_objects(objects)
    {
    }

    const void render_pixmap(std::ostream &, std::ostream &);
};

} // namespace ssr

#endif
