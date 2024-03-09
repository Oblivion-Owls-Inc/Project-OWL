/// @file       CircleCollider.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Circle-shaped collider
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Collider.h"
#include "Stream.h"

/// @class  CircleCollider
/// @brief  Circle-shaped Collider
class CircleCollider : public Collider
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    CircleCollider();


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets this CircleCollider's radius
    /// @return this CircleCollider's radius
    float GetRadius() const;

    /// @brief  sets this CircleCollider's radius
    /// @param  radius  the radius
    void SetRadius( float radius );


    /// @brief  gets whether this CircleCollider has changed and its position in the CollisionSystem needs to update
    /// @return whether this CircleCollider has changed and its position in the CollisionSystem needs to update
    bool GetHasChanged() const;

    /// @brief  clears the HasChanged flag
    /// @brief  SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    void ClearHasChanged();


//-----------------------------------------------------------------------------
public: // virtual overrides
//-----------------------------------------------------------------------------


    /// @brief  called when this Component's Entity enters the Scene
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    virtual void OnExit() override;


    /// @brief  draws the collision shape of this collider for debug purposes
    virtual void DebugDraw() const override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  The radius of this CircleCollider
    float m_Radius = 1.0f;


    /// @brief  whether this CircleCollider has changed and its position in the CollisionSystem needs to update
    bool m_HasChanged = false;

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspector for this CircleCollider
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  Reads the radius
    /// @param  stream  The json data to read from
    void readRadius( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief Write all CircleCollider component data to a JSON file.
    /// @return The JSON file containing the CircleCollider component data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  virtual component clone function
    /// @return new clone of component
    virtual CircleCollider* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    CircleCollider( CircleCollider const& other );


    // disable assignment operator
    void operator =( CircleCollider const& ) = delete;

    
//-----------------------------------------------------------------------------
};
