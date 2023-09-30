/// @file CollisionSystem.h
/// @author Jax Clayton  (jax.clayton@digipen.edu)
/// @brief The CollisionSystem is responsible for checking for collisions between Different Colliders
/// @version 0.1
/// @date 2023-09-15
/// 
/// @copyright  Digipen LLC (c) 2023

#pragma once

//-----------------------------------------------------------------------------
// Includes:
//-----------------------------------------------------------------------------

#include "System.h"
#include <vector>
#include <typeindex>

//-----------------------------------------------------------------------------
// Forward references:
//-----------------------------------------------------------------------------

class Collider;
struct CollisionData;

//-----------------------------------------------------------------------------
// typedefs
//-----------------------------------------------------------------------------

/// @brief  function pointer to function that checks collisions between Collider types
using CollisionCheckFunction = bool (*)( Collider const* colliderA, Collider const* colliderB, CollisionData* collisionData );

/// @brief  map that stores the CollisionCheckMethods between each Collider type
using CollisionFunctionMap = std::map< std::pair< std::type_index, std::type_index >, CollisionCheckFunction >;

//-----------------------------------------------------------------------------
// class
//-----------------------------------------------------------------------------

/// @class  CollisionSystem
/// @brief  responsible for checking collsisions between all Colliders
class CollisionSystem : public System
{

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------
    
    /// @brief  adds a Collider to the CollisionSystem
    /// @param  collider    the collider to add
    void addCollider( Collider* collider );

    /// @brief  removes a Collider from this System
    /// @param  collider    the collider to remove
    void removeCollider( Collider* collider );

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief Checks and handles all Collisions
    void checkCollisions();

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief all Colliders in the Scene
    std::vector< Collider* > m_Colliders;

//-----------------------------------------------------------------------------
private: // static methods
//-----------------------------------------------------------------------------

    /// @brief  checks a collision between two colliders of unknown type
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    static void CheckCollision( Collider const* colliderA, Collider const* colliderB );

    /// @brief  checks a collision between two circle colliders
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    static bool CheckCircleCircle( Collider const* colliderA, Collider const* colliderB, CollisionData* collisionData );

//-----------------------------------------------------------------------------
private: // static members
//-----------------------------------------------------------------------------

    /// @brief map that stores the CollisionCheckMethods between each Collider type
    static CollisionFunctionMap const s_CollisionFunctions;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the CollisionSystem read methods
    static ReadMethodMap< CollisionSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the CollisionSystem
    CollisionSystem();

    /// @brief The singleton s_Instance of CollisionSystem
    static CollisionSystem* s_Instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief gets the instance of CollisionSystem
    /// @return the instance of the CollisionSystem
    static CollisionSystem* GetInstance();

    // Prevent copying
    CollisionSystem(CollisionSystem& other) = delete;
    void operator=(const CollisionSystem&) = delete;

//-----------------------------------------------------------------------------
};
