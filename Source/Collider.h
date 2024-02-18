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

#include "ComponentReference.h"
#include "Transform.h"

#include "CollisionLayerFlags.h"

#include <map>
#include <functional>

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------

struct CollisionData;

//-----------------------------------------------------------------------------
// class
//-----------------------------------------------------------------------------

/// @class  Collider
/// @brief  component which is used for detecting collisions and information about them
class Collider : public Component
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------


    /// @brief  Callback called whenever this Collider detects a collision
    /// @param  collider        - the other Collider that this Collider collided with
    /// @param  collisionData   - physics information about the collision between the two objects
    using OnCollisionCallback = std::function< void ( Collider* collider, CollisionData const& collisionData ) >;

    /// @brief  Callback called whenever this Collider enters or exits a collision
    /// @param  collider        - the other Collider that this Collider collided with
    using OnCollisionStateChangeCallback = std::function< void ( Collider* collider ) >;

    
//-----------------------------------------------------------------------------
protected: // constructor
//-----------------------------------------------------------------------------
    

    /// @brief  default constructor
    /// @param  type    the type of Component
    Collider( std::type_index type );

    /// @brief  copy constructor
    /// @param  other   the collider to copy
    Collider( Collider const& other );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets this Collider's Transform
    /// @return this Collider's Transform
    Transform* GetTransform() const;


    /// @brief  gets the collision layer of this Collider
    /// @return the collision layer of this Collider
    unsigned GetCollisionLayer() const;

    /// @brief  sets the collision layer of this Collider
    /// @param  layerId the collision layer to set
    void SetCollisionLayer( unsigned layerId );


    /// @brief  gets the flags of which layers this Collider collides with
    /// @return the flags of which layers this Collider collides with
    CollisionLayerFlags GetCollisionLayerFlags() const;

    /// @brief  sets the flags of which layers this Collider collides with
    /// @param  layerFlags  the flags of which layers this Collider should collide with
    void SetCollisionLayerFlags( CollisionLayerFlags layerFlags );


    /// @brief  gets the list of colliders this Collider is currently colliding with
    /// @return the list of colliders this Collider is currently colliding with
    std::map< Collider*, int > const& GetContacts() const;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  checks if the collider is colliding with another collider
    /// @param  other   the other collider to check if is colliding with this one
    /// @return whether the colliders are colliding
    bool IsColliding( Collider* other );


    /// @brief  adds a callback function to be called when this collider collides;
    ///         may be called multiple times per frame for the same collider
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void AddOnCollisionCallback( unsigned ownerId, OnCollisionCallback callback );

    /// @brief  removes a callback function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback to remove
    void RemoveOnCollisionCallback( unsigned ownerId );


    /// @brief  adds a callback function to be called when a collision begins
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void AddOnCollisionEnterCallback( unsigned ownerId, OnCollisionStateChangeCallback callback );

    /// @brief  adds a callback function to be called when a collision begins
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnCollisionEnterCallback( unsigned ownerId );


    /// @brief  adds a callback function to be called when a collision ends
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void AddOnCollisionExitCallback( unsigned ownerId, OnCollisionStateChangeCallback callback );

    /// @brief  adds a callback function to be called when a collision ends
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnCollisionExitCallback( unsigned ownerId );


    /// @brief  calls all OnCollision callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    /// @param  collisionData   the collisionData of the collision
    /// @note   SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    void CallOnCollisionCallbacks( Collider* other, CollisionData const& collisionData );


    /// @brief  tries to add a contact to this Collider's contacts array
    /// @param  other           the contact to add
    /// @param  currentFrame    the frame the Contact was added on
    /// @note   SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    void TryAddContact( Collider* other, int currentFrame );


    /// @brief  removes all outdated contacts from this Collider and calls OnCollisionExit callbacks
    void RemoveOutdatedContacts();


//-----------------------------------------------------------------------------
protected: // member variables
//-----------------------------------------------------------------------------

    
    /// @brief The transform of this Collider's Entity
    ComponentReference< Transform > m_Transform;


    /// @brief  the collision layer of this Collider
    unsigned m_CollisionLayerId = 0;

    /// @brief  flags of which layers this Collider collides with
    CollisionLayerFlags m_CollisionLayerFlags = 0;


    /// @brief  all of the Colliders this Collider is currently colliding with, and the last frame that they were colliding
    std::map< Collider*, int > m_Contacts = {};


    /// @brief  callbacks which get called whenever this collider collides
    std::map< unsigned, OnCollisionCallback > m_OnCollisionCallbacks = {};

    /// @brief  callbacks which get called whenever a collision begins
    std::map< unsigned, OnCollisionStateChangeCallback > m_OnCollisionEnterCallbacks = {};

    /// @brief  callbacks which get called whenever a collision ends
    std::map< unsigned, OnCollisionStateChangeCallback > m_OnCollisionExitCallbacks = {};

    
//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  calls all OnCollisionEnter callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    void CallOnCollisionEnterCallbacks( Collider* other );

    /// @brief  calls all OnCollisionExit callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    void CallOnCollisionExitCallbacks( Collider* other );


//-----------------------------------------------------------------------------
protected: // inspection
//-----------------------------------------------------------------------------


    /// @brief  debug inspector for Colliders
    virtual void Inspector() override;


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
