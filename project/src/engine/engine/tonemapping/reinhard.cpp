#include "engine/tonemapping/reinhard.hpp"

#include "engine/geometry/Constants.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stbipp/Color.hpp>

// http://www.cmap.polytechnique.fr/~peyre/cours/x2005signal/hdr_photographic.pdf
double rgbLuminance(const Vector3& val)
{
    return val.dot(Vector3{0.27, 0.67, 0.06});
}
namespace tone
{
void reinhard(stbipp::Image& image, Floating scale)
{
    double logAverageLum{0.0};
    double maxLum{0.0};
    for(const auto& col: image)
    {
        const auto lum = rgbLuminance(Vector3{col.r(), col.g(), col.b()});
        logAverageLum += std::log(EPSILON + lum);
        maxLum = std::max(lum, maxLum);
    }
    logAverageLum = std::exp(logAverageLum / static_cast<double>(image.width() * image.height()));
    std::cout << logAverageLum;
    for(auto& col: image)
    {
        const auto lum = rgbLuminance(Vector3{col.r(), col.g(), col.b()});
        const Floating luminanceScaled = (scale * lum) / logAverageLum;
        const auto reinOperator =
          luminanceScaled * (1.0 + (luminanceScaled / std::pow(maxLum, 2.0))) / (1.0 + luminanceScaled);
        col /= stbipp::Color4f(lum);
        col *= stbipp::Color4f(reinOperator);
    }
} // namespace tone

} // namespace tone
