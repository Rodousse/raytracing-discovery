#include "EngineSymbols.h"
#include "engine/geometry/CommonTypes.hpp"

#include <random>
namespace engine
{
namespace sampling
{
class ENGINE_API UniformSampler
{
  public:
    UniformSampler();
    Floating sample();

  private:
    std::mt19937 m_generator; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<Floating> m_distribution;
};

ENGINE_API Vector3 generateRandomUniformHemisphereRayDir(const Vector3& normal, UniformSampler& sampler);

} // namespace sampling
} // namespace engine
