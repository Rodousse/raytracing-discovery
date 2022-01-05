#pragma once
#include "EngineSymbols.h"
#include "engine/data/Material.hpp"
#include "engine/geometry/CommonTypes.hpp"

#include <Eigen/Dense>
#include <array>
#include <memory>
#include <vector>
namespace engine
{
/// Used for mesh vertex indexing
using FaceIndex = std::size_t;

/// Used for mesh vertex indexing
using VertexIndex = std::size_t;

/// Triangle face vertex indices
using FaceIndices = std::array<VertexIndex, 3>;

/**
 * @brief Axis aligned bounding box
 */
struct ENGINE_API AABoundingBox
{
    Vector3 min{
      std::numeric_limits<Floating>::max(), std::numeric_limits<Floating>::max(), std::numeric_limits<Floating>::max()};
    Vector3 max{std::numeric_limits<Floating>::lowest(),
                std::numeric_limits<Floating>::lowest(),
                std::numeric_limits<Floating>::lowest()};
};

/**
 * @brief Vertex description
 */
struct ENGINE_API Vertex
{
    Vector3 pos{};
    Vector3 normal{};
    Vector2 uv{};
};

/**
 * @brief Vertex description
 */
struct ENGINE_API Face
{
    Vector3 normal{};
    FaceIndices indices{};
};

/**
 * @brief Mesh description
 */
struct ENGINE_API Mesh
{
    std::vector<Vertex> vertices{};
    std::vector<Face> faces{};
    std::string name{};
    std::string path{};
    AABoundingBox aabb{};
    std::shared_ptr<Material> material{nullptr};
    bool hasVertexNormals{false};

    void refreshBoundingBox();
};

} // namespace engine
