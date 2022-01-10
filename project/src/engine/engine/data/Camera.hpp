#pragma once
#include "EngineSymbols.h"
#include "engine/geometry/CommonTypes.hpp"
#include "engine/geometry/Ray.hpp"

#include <Eigen/Dense>
#include <memory>
#include <vector>

namespace engine
{
class ENGINE_API Camera
{
  protected:
    Vector3 m_position;
    Matrix3 m_rotationMat;

    // Frustum information
    float m_nearPlaneDistance;
    float m_farPlaneDistance;

    // Transformation matrice for ray generation
    Matrix4 m_camToWorldMat;
    Matrix4 m_worldToCamMat;

    Vector3 m_forward;
    Vector3 m_right;
    Vector3 m_up;
    Vector3 m_upWorld;

    // World space dimensions
    float m_widthWorld;
    float m_heightWorld;

    // Dimension of the Rendered image
    unsigned int m_widthRender;
    unsigned int m_heightRender;

    // Compute dimension of the canvas in the world space
    virtual void resizeWorldDimensions() = 0;

    void refreshMatrices();

  public:
    Camera();
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;
    Camera& operator=(const Camera&) = default;
    Camera& operator=(Camera&&) = default;
    virtual ~Camera() = default;

    [[nodiscard]] virtual Ray generateRay(const Vector2ui& pos) const = 0;

    /********************************* Setter / Getter ***************************************/

    void setPosition(const Vector3& pos);
    const Vector3& position() const;

    void setUp(const Vector3& up);
    const Vector3& up() const;

    void setForward(const Vector3& forward);
    const Vector3& forward() const;

    virtual void setNearPlaneDistance(float dist);
    float nearPlaneDistance() const;

    void setFarPlaneDistance(float dist);
    float farPlaneDistance() const;

    void setRenderDimensions(unsigned int width, unsigned int height);
    const Vector2ui renderDimensions() const;

    // Set the dimensions of the rendered image in pixels
    void setWidthRenderDimensions(unsigned int width);
    unsigned int widthRenderDimensions() const;

    void setHeightRenderDimensions(unsigned int height);
    unsigned int heightRenderDimensions() const;
};

} // namespace engine
