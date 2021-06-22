#include "engine/geometry/sampling.hpp"

#include "engine/geometry/Constants.hpp"

namespace engine
{
namespace sampling
{
UniformSampler::UniformSampler(): m_generator(std::random_device{}()), m_distribution(-1.0, 1.0) {}

Floating UniformSampler::sample()
{
    return m_distribution(m_generator);
}

Vector3 generateRandomUniformHemisphereRayDir(const Vector3& normal, UniformSampler& sampler)
{
    Vector3 dir{};
    do
    {
        dir = {sampler.sample(), sampler.sample(), sampler.sample()};
    } while(dir.norm() > 1.0f);
    if(dir.dot(normal) < 0)
    {
        dir = -dir;
    }
    dir.normalize();
    return dir;
}

} // namespace sampling

} // namespace engine
