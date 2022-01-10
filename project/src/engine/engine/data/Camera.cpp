#include "engine/data/Camera.hpp"

#include <Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdexcept>

namespace engine
{
Camera::Camera():
  m_position(Vector3::Zero()),
  m_nearPlaneDistance(0.1f),
  m_farPlaneDistance(100.0f),
  m_camToWorldMat(Matrix4::Identity()),
  m_worldToCamMat(Matrix4::Identity()),
  m_widthRender(640),
  m_heightRender(480)
{
    refreshMatrices();
}

/********************************** PRIVATE ****************************/

void Camera::refreshMatrices()
{
    // x,y,z axis of camera space in world space
    m_right = m_up.cross(m_forward);

    m_rotationMat.block<3, 1>(0, 0) = m_right;
    m_rotationMat.block<3, 1>(0, 1) = m_up;
    m_rotationMat.block<3, 1>(0, 2) = m_forward;

    Matrix3 rotationMatInv = m_rotationMat.inverse();

    m_camToWorldMat.block<3, 3>(0, 0) = m_rotationMat;
    m_camToWorldMat.block<3, 1>(0, 3) = m_position;
    m_camToWorldMat(3, 3) = 1;

    m_worldToCamMat.block<3, 3>(0, 0) = rotationMatInv;
    m_worldToCamMat.block<3, 1>(0, 3) = rotationMatInv * (-m_position);
    m_worldToCamMat(3, 3) = 1;
}

/********************************** PUBLIC ****************************/

void Camera::setPosition(const Vector3& pos)
{
    m_position = pos;
    refreshMatrices();
}

const Vector3& Camera::position() const
{
    return m_position;
}

void Camera::setUp(const Vector3& up)
{
    m_up = up;
    refreshMatrices();
}

const Vector3& Camera::up() const
{
    return m_upWorld;
}

void Camera::setForward(const Vector3& forward)
{
    m_forward = forward;
    refreshMatrices();
}

const Vector3& Camera::forward() const
{
    return m_forward;
}

void Camera::setNearPlaneDistance(float dist)
{
    if(dist < 0.00001)
    {
        throw(std::invalid_argument("Near Plane Distance can't be negative or null : " + std::to_string(dist)));
    }

    m_nearPlaneDistance = dist;
}

float Camera::nearPlaneDistance() const
{
    return m_nearPlaneDistance;
}

void Camera::setFarPlaneDistance(float dist)
{
    if(dist < 0.00001 || dist < m_nearPlaneDistance)
    {
        throw(std::invalid_argument("Far Plane Distance can't be negative or behind the Near Plane : " +
                                    std::to_string(dist)));
    }

    m_farPlaneDistance = dist;
}

float Camera::farPlaneDistance() const
{
    return m_farPlaneDistance;
}

void Camera::setRenderDimensions(unsigned int width, unsigned int height)
{
    setWidthRenderDimensions(width);
    setHeightRenderDimensions(height);
}

const Vector2ui Camera::renderDimensions() const
{
    return Vector2ui(m_widthRender, m_heightRender);
}

// Set the dimensions of the rendered image in pixels
void Camera::setWidthRenderDimensions(unsigned int width)
{
    m_widthRender = width;
    resizeWorldDimensions();
}

unsigned int Camera::widthRenderDimensions() const
{
    return m_widthRender;
}

void Camera::setHeightRenderDimensions(unsigned int height)
{
    m_heightRender = height;
    resizeWorldDimensions();
}

unsigned int Camera::heightRenderDimensions() const
{
    return m_heightRender;
}

} // namespace engine
