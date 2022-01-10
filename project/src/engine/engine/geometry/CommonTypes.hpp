#pragma once

#include <Eigen/Dense>

using Floating = float;
using Matrix4 = Eigen::Matrix<Floating, 4, 4>;
using Matrix3 = Eigen::Matrix<Floating, 3, 3>;
using Vector4 = Eigen::Matrix<Floating, 4, 1>;
using Vector3 = Eigen::Matrix<Floating, 3, 1>;
using Vector2 = Eigen::Matrix<Floating, 2, 1>;
using Vector2i = Eigen::Vector2i;
using Vector2ui = Eigen::Matrix<unsigned int, 2, 1>;

struct Rect
{
    Vector2ui origin;
    Vector2ui size;
};
