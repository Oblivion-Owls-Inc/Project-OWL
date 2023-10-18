/// @file       CircleCollider.h
/// @author     Steve Bukowinski (jax.clayton@digipen.edu)
/// @brief      struct that holds collision information
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include <glm/glm.hpp>

class Collider;

/// @struct CollisionData
/// @brief  Struct that holds data about collisions
struct CollisionData
{
    /// @brief  The position of the collision
    glm::vec2 position;

    /// @brief  The normal of the collision
    glm::vec2 normal;

    /// @brief How deep the collision penetrated
    float depth;
};

/// @struct RayCastHit
/// @brief  the result of a raycast
struct RayCastHit
{
    /// @brief  the collider that was hit
    Collider* colliderHit = nullptr;

    /// @brief  the normal of the collider where the ray hit it
    glm::vec2 normal = { 0, 0 };

    /// @brief  the distance the ray travelled
    float distance = 100.0f;

    /// @brief  implicit conversion to bool
    operator bool() const { return colliderHit != nullptr; }
};