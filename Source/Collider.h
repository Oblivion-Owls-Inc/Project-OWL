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
#include "Transform.h"

#include <map>

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------

struct CollisionData;

class CircleCollider;
class TilemapCollider;

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
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets this Collider's Transform
    /// @return this Collider's Transform
    __inline Transform* GetTransform() const { return m_Transform; }

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called when this Component's Entity enters the Scene
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
    
    /// @brief The transform of this Collider's Entity
    Transform* m_Transform;

//-----------------------------------------------------------------------------
};
