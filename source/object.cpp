#include "ssr/object.hpp"
#include "linalg.h"

using namespace ssr;

const float4x4 object::transform() const
{
    return mul(linalg::translation_matrix(m_position), linalg::rotation_matrix(m_rotation),
               linalg::scaling_matrix(m_scale));
}
