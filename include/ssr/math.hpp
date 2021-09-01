//
// math.hpp: Math utilities and linalg namespace directives
//

#ifndef SSR_MATH_H_
#define SSR_MATH_H_

#include <linalg.h>

namespace ssr {

    constexpr double pi = 3.14159265358979323846;

    // We use linalg types almost everywhere, so add them to our project namespace
    using namespace linalg;
    using namespace linalg::aliases;

    // Degree literal (converts to radians, our conventional angle format)
    constexpr long double operator"" _deg(const long double x) {
        return x * ssr::pi / 180.0f;
    }

} // namespace ssr

#endif
