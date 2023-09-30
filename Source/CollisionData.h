/// @file       CircleCollider.h
/// @author     Steve Bukowinski (jax.clayton@digipen.edu)
/// @brief      struct that holds collision information
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include <glm/glm.hpp>

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
