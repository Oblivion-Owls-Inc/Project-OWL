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

/// @brief  bit flags of which layers a collider collides with
using CollisionLayerFlags = unsigned;

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


    /// @brief  makes a CollisionLayerFlags for a set of layer names
    /// @param  layerNames  the names of the layers to include in the flags
    CollisionLayerFlags GetLayerFlags( std::vector< std::string > const& layerNames ) const;

    /// @brief  gets the collision layer ID with the specified name
    /// @param  layerName   the name of the layer to get
    /// @return the collision layer ID
    unsigned GetCollisionLayerId( std::string const& layerName ) const;


    /// @brief  gets the names of the layers in a CollisionLayerFlags
    /// @param  layerFlags  the layer flags to get the names of
    /// @return the names of the layers
    std::vector< std::string > GetLayerNames( CollisionLayerFlags layerFlags ) const;

    /// @brief  gets the name of the specified layer
    /// @param  layerId the ID of the layer to get the name of
    /// @return the name of the layer
    std::string const& GetLayerName( unsigned layerId ) const;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------
    
    /// @brief  gets the collision layer names
    /// @return the array of collision layer names
    std::vector< std::string > const& GetLayerNames() const { return m_CollisionLayerNames; }

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override;

    /// @brief  gets called once whenever a scene exits
    virtual void OnSceneExit() override;

    /// @brief  creates the debug window for the CollisionSystem
    virtual void DebugWindow() override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief Checks and handles all Collisions
    void checkCollisions();

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief  all Colliders in the Scene
    std::vector< Collider* > m_Colliders = {};

    /// @brief  the names of each collision layer
    std::vector< std::string > m_CollisionLayerNames = {};

//-----------------------------------------------------------------------------
private: // static methods
//-----------------------------------------------------------------------------

    /// @brief  checks a collision between two colliders of unknown type
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    static void CheckCollision( Collider* colliderA, Collider* colliderB );

    /// @brief  checks a collision between two circle colliders
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    static bool CheckCircleCircle( Collider const* colliderA, Collider const* colliderB, CollisionData* collisionData );

    /// @brief  checks a collision between a circle and tilemap collider
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    static bool CheckCircleTilemap( Collider const* colliderA, Collider const* colliderB, CollisionData* collisionData );



    /// @brief  helper function which checks a circle against an AABB
    /// @param  circlePos       the position of the circle
    /// @param  circleRadius    the radius of the circle
    /// @param  aabbMin         the min pos of the AABB
    /// @param  aabbMax         the max pos of the AABB
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two shapes are colliding
    /// @note   ASSUMES THAT THE AABB OF THE CIRCLE IS KNOWN TO OVERLAP THE RECTANGLE
    static bool CheckCircleAABB( glm::vec2 circlePos, float circleRadius, glm::vec2 aabbMin, glm::vec2 aabbMax, CollisionData* collisionData );

    /// @brief  helper function which checks a circle against a point
    /// @param  circlePos       the position of the circle
    /// @param  circleRadius    the radius of the circle
    /// @param  point           the pos position of the point
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two shapes are colliding
    static bool CheckCirclePoint( glm::vec2 circlePos, float circleRadius, glm::vec2 point, CollisionData* collisionData );


//-----------------------------------------------------------------------------
private: // static members
//-----------------------------------------------------------------------------

    /// @brief map that stores the CollisionCheckMethods between each Collider type
    static CollisionFunctionMap const s_CollisionFunctions;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the collision layer names from json
    /// @param  data    the json data to read from
    void readCollisionLayerNames( nlohmann::ordered_json const& data );

    /// @brief map of the CollisionSystem read methods
    static ReadMethodMap< CollisionSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief  writes the CollisionSystem config to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const override;

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

/// @brief  shorthand method for getting the collision system
/// @return the collision system instance
__inline CollisionSystem* Collisions()
{
    return CollisionSystem::GetInstance();
}
