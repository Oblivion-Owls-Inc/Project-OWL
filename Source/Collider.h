/// @file       Collider.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Base component for detecting collisions
/// @version    0.1
/// @date       2023-09-29
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------

#include "Component.h"

#include <map>
#include <functional>

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------

struct CollisionData;

class Transform;

//-----------------------------------------------------------------------------
// typedefs
//-----------------------------------------------------------------------------

using CollisionLayerFlags = unsigned;

//-----------------------------------------------------------------------------
// class
//-----------------------------------------------------------------------------

/// @class  Collider
/// @brief  component which is used for detecting collisions and information about them
class Collider : public Component
{
//-----------------------------------------------------------------------------
protected: // constructors
//-----------------------------------------------------------------------------
	
    /// @brief  default constructor
    /// @param  type    the type of Component
    Collider( std::type_index type );

    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    Collider( Collider const& other );

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  adds a callback function to be called when this collider collides
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void AddOnCollisionCallback( unsigned ownerId, std::function< void ( Collider*, CollisionData const& ) > callback );

    /// @brief  removes a callback function to be called when this collider collides
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnCollisionCallback( unsigned ownerId );

    /// @brief  calls all OnCollision callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    /// @param  collisionData   the collisionData of the collision
    void CallOnCollisionCallbacks( Collider* other, CollisionData const& collisionData );


    /// @brief  sets the collision layer of this Collider
    /// @param  layerName   the name of the collision layer to set
    void SetCollisionLayer( std::string const& layerName );

    /// @brief  sets the collision layer of this Collider
    /// @param  layerNames  the name of the collision layer to set
    void SetCollisionLayerNames( std::vector< std::string > const& layerNames );
    
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets this Collider's Transform
    /// @return this Collider's Transform
    Transform* GetTransform() const { return m_Transform; }


    /// @brief  gets the collision layer of this Collider
    /// @return the collision layer of this Collider
    unsigned GetCollisionLayerId() const { return m_CollisionLayerId; }

    /// @brief  sets the collision layer of this Collider
    /// @param  layerId the collision layer to set
    void SetCollisionLayerId( unsigned layerId ) { m_CollisionLayerId = layerId; }


    /// @brief  gets the flags of which layers this Collider collides with
    /// @return the flags of which layers this Collider collides with
    CollisionLayerFlags GetCollisionLayerFlags() const { return m_CollisionLayerFlags; }

    /// @brief  sets the flags of which layers this Collider collides with
    /// @param  layerFlags  the flags of which layers this Collider collides with
    void SetCollisionLayerFlags( CollisionLayerFlags layerFlags ) {m_CollisionLayerFlags = layerFlags; }


//-----------------------------------------------------------------------------
protected: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called when this Component's Entity enters the Scene
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit() override;

    /// @brief  debug inspector for Colliders
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
    
    /// @brief The transform of this Collider's Entity
    Transform* m_Transform = nullptr;

    /// @brief  the collision layer of this Collider
    unsigned m_CollisionLayerId = 0;

    /// @brief  flags of which layers this Collider collides with
    CollisionLayerFlags m_CollisionLayerFlags = 0;

    /// @brief  callbacks which get called whenever this collider collides
    std::map< unsigned, std::function< void ( Collider*, CollisionData const& ) > > m_OnCollisionCallbacks = {};

//-----------------------------------------------------------------------------
protected: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the collision layer from json
    /// @param  data    the json data to read from
    void readCollisionLayer( nlohmann::ordered_json const& data );

    /// @brief  reads the collision layer flags from json
    /// @param  data    the json data to read from
    void readCollisionLayerFlags( nlohmann::ordered_json const& data );
    
//-----------------------------------------------------------------------------
};
