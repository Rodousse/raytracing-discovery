#pragma once
#include "engine/data/Camera.hpp"

namespace engine
{
class ENGINE_API PerspectiveCamera : public Camera
{
  protected:
    float m_fov;

    void resizeWorldDimensions() override;

  public:
    PerspectiveCamera();
    ~PerspectiveCamera() override = default;
    [[nodiscard]] Ray generateRay(const Eigen::Vector2i& pixelPos) const override;

    /********************************* Setter / Getter ***************************************/
    void setNearPlaneDistance(float dist) override;

    void setFov(float fov);
    float fov() const;
};

} // namespace engine
