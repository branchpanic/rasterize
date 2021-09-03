//
// scene.hpp: Scene graph abstraction
//

#ifndef SSR_SCENE_H_
#define SSR_SCENE_H_

#include "ssr/camera.hpp"
#include "ssr/object.hpp"

#include <vector>

namespace ssr
{

struct scene
{
    camera m_camera;
    std::vector<object> m_objects;
};

} // namespace ssr

#endif
