/// @file       Interactor.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Behavior that can interact with nearby Interactable Components
/// @version    0.1
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h"

#include "Interactor.h"

#include "BehaviorSystem.h"

#include "ComponentReference.t.h"
#include "Transform.h"
#include "Interactable.h"

#include "Sprite.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    Interactor::Interactor() :
        Behavior( typeid( Interactor ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  tries to interact with the currently targeted Interactable
    void Interactor::TryInteract()
    {
        if ( m_TargetedInteractable != nullptr )
        {
            m_TargetedInteractable->Interact( this );
        }
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Transform Component attached to this Interactor
    /// @return the Transform Component attached to this Interactor
    Transform const* Interactor::GetTransform() const
    {
        return m_Transform;
    }


    /// @brief  gets the Interactable Component currently being targeted by this Interactor
    /// @return the Interactable Component currently being targeted by this Interactor
    Interactable* Interactor::GetTargetedInteractable()
    {
        return m_TargetedInteractable;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void Interactor::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_TargetedInteractable.SetOnConnectCallback( [ this ]() {
            Sprite* sprite = m_TargetedInteractable->GetPromptSprite();
            if ( sprite != nullptr )
            {
                sprite->SetOpacity( 1.0f );
            }
        } );
        m_TargetedInteractable.SetOnDisconnectCallback( [ this ]() {
            Sprite* sprite = m_TargetedInteractable->GetPromptSprite();
            if ( sprite != nullptr )
            {
                sprite->SetOpacity( 0.0f );
            }
        } );

        m_Transform.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void Interactor::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_Transform.Exit();
        m_TargetedInteractable.Exit();
    }


    /// @brief  called every simulation frame
    void Interactor::OnFixedUpdate()
    {
        updateTargetedInteractable();

        if ( m_TargetedInteractable == nullptr )
        {
            return;
        }

        Action const* action = m_TargetedInteractable->GetInteractAction();
        if ( action == nullptr )
        {
            return;
        }

        if ( action->GetReleased() )
        {
            m_TargetedInteractable->Interact( this );
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  targets the nearest Interactable within range
    void Interactor::updateTargetedInteractable()
    {
        if ( m_Transform == nullptr )
        {
            return;
        }
        glm::vec2 const& pos = m_Transform->GetTranslation();

        Interactable* nearest = nullptr;
        float nearestSqrDist = INFINITY;
        for ( Interactable* interactable : Components< Interactable >()->GetComponents() )
        {
            if ( interactable->GetEnabled() == false || interactable->GetTransform() == nullptr )
            {
                continue;
            }

            glm::vec2 offset = pos - interactable->GetTransform()->GetTranslation();
            float sqrDist = glm::dot( offset, offset );
            float radius = interactable->GetInteractionRadius();

            if ( sqrDist >= nearestSqrDist || sqrDist > radius * radius )
            {
                continue;
            }

            nearestSqrDist = sqrDist;
            nearest = interactable;
        }

        m_TargetedInteractable = nearest;
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    // /// @brief  shows the inspector for Interactor
    // void Interactor::Inspector()
    // {
    // 
    // }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    // /// @brief  gets the map of read methods for this Interactor
    // /// @return the map of read methods for this Interactor
    // ReadMethodMap< ISerializable > const& Interactor::GetReadMethods() const
    // {
    //     static ReadMethodMap< Interactor > const readMethods = {
    // 
    //     };
    // 
    //     return (ReadMethodMap< ISerializable > const&)readMethods;
    // }
    // 
    // 
    // /// @brief  writes this Interactor to JSON
    // /// @return the JSON data of this Interactor
    // nlohmann::ordered_json Interactor::Write() const
    // {
    //     nlohmann::ordered_json json = nlohmann::ordered_json::object();
    // 
    //     return json;
    // }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Interactor
    /// @return the newly created clone of this Interactor
    Interactor* Interactor::Clone() const
    {
        return new Interactor( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Interactor
    /// @param  other   the other Interactor to copy
    Interactor::Interactor( Interactor const& other ) :
        Behavior( other )
    {}


//-----------------------------------------------------------------------------
