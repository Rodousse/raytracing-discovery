#include "engine/data/PerspectiveCamera.hpp"

#include <iostream>
#include <numeric>
#include <stdexcept>

namespace engine
{
PerspectiveCamera::PerspectiveCamera(): Camera() {}

/********************************** PRIVATE ****************************/

void PerspectiveCamera::resizeWorldDimensions()
{
    m_heightWorld = 2.0 * tanf((m_fov) / 2.0) * nearPlaneDistance();
    m_widthWorld = m_heightWorld * widthRenderDimensions() / heightRenderDimensions();
}

const Ray PerspectiveCamera::generateRay(const Eigen::Vector2i& pixelPos) const
{
    Ray ray;

    // camera space position
    Vector2 cameraCoord;

    cameraCoord = (pixelPos.cast<Floating>() - (renderDimensions().cast<Floating>() / 2.0f));
    cameraCoord = cameraCoord.cwiseQuotient(renderDimensions().cast<Floating>());
    cameraCoord = -cameraCoord;

    ray.origin = position() + (m_right.normalized() * cameraCoord.x() * m_widthWorld) +
                 (m_up.normalized() * cameraCoord.y() * m_heightWorld) + (m_forward.normalized() * nearPlaneDistance());
    ray.dir = (ray.origin - position()).normalized();

    return ray;
}

/********************************** PUBLIC ****************************/

void PerspectiveCamera::setNearPlaneDistance(float dist)
{
    Camera::setNearPlaneDistance(dist);
    resizeWorldDimensions();
}

void PerspectiveCamera::setFov(float fov)
{
    if(fov > 172.0f || fov < std::numeric_limits<Floating>::epsilon())
    {
        throw(std::invalid_argument("FoV of perspective camera out of range : " + std::to_string(fov) +
                                    " should be in ]0.0;172.0] "));
    }

    m_fov = fov;
    resizeWorldDimensions();
}

float PerspectiveCamera::fov() const
{
    return m_fov;
}

} // namespace engine
