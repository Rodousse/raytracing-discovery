#include "engine/geometry/sampling.hpp"

#include "engine/geometry/Constants.hpp"

#include <random>

namespace engine
{
namespace sampling
{
Vector3 generateRandomUniformRayDir(const Vector3& normal)
{
    static std::random_device rd;  // Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<Floating> dis(-1.0, 1.0);
    Vector3 dir{};
    do
    {
        dir = {dis(gen), dis(gen), dis(gen)};
    } while(dir.norm() * dir.norm() > 1.0f);
    if(dir.dot(normal) < 0)
    {
        dir = -dir;
    }
    dir.normalize();
    return dir;
}

} // namespace sampling

} // namespace engine
