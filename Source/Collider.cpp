/// @file       Collider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Base component for detecting collisions
/// @version    0.1
/// @date       2023-09-29
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology


#include "Collider.h"

#include "Entity.h"

#include "CollisionSystem.h"

//-----------------------------------------------------------------------------
// protected: constructor
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    /// @param  type    the type of Component
    Collider::Collider( std::type_index type ) :
        Component( type ),
        m_Transform( nullptr )
    {}

    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    Collider::Collider( Collider const& other ) :
        Component( other ),
        m_Transform( nullptr )
    {}

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called when this Component's Entity enters the Scene
    void Collider::OnInit()
    {
        CollisionSystem::GetInstance()->addCollider(this);
        m_Transform = GetParent()->GetComponent<Transform>();
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    void Collider::OnExit()
    {
        CollisionSystem::GetInstance()->removeCollider(this);
    }

//-----------------------------------------------------------------------------
