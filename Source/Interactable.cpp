/// @file       Interactable.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component which can be interacted with by pressing a button when nearby
/// @version    0.1
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "pch.h"

#include "Interactable.h"

#include "ComponentSystem.h"
#include "EventSystem.h"

#include "ComponentReference.t.h"
#include "Transform.h"
#include "Sprite.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    Interactable::Interactable() :
        Component( typeid( Interactable ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    
    /// @brief  interacts with this Interactable
    void Interactable::Interact( Interactor* interactor )
    {
        if (m_EventCast != "")
        {
            Events()->BroadcastEvent< std::string >(m_EventCast);
            Debug() << "Event Emitted: " << m_EventCast << std::endl;
        }
        for ( auto& [ ownerId, callback ] : m_OnInteractCallbacks )
        {
            callback( interactor );
        }
    }


    /// @brief  adds a callback to be called when this Interactable Component is interacted with
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback to add
    void Interactable::AddOnInteractCallback( unsigned ownerId, std::function< void ( Interactor* interactor ) > callback )
    {
        m_OnInteractCallbacks.emplace_back( ownerId, callback );
    }

    /// @brief  removes an OnInteractCallback from this Interactable Component
    /// @param  ownerId     the ownerId of the callback to remove
    void Interactable::RemoveOnInteractCallback( unsigned ownerId )
    {
        auto it = std::find_if(
            m_OnInteractCallbacks.begin(), m_OnInteractCallbacks.end(),
            [ ownerId ]( std::pair< unsigned, std::function< void ( Interactor* interactor ) > > const& callbackPair ) -> bool
            {
                return callbackPair.first == ownerId;
            }
        );
        if ( it == m_OnInteractCallbacks.end() )
        {
            Debug() << "ERROR: could not find OnInteractCallback to remove with ownerId " << ownerId << " (" << GetName() << ")" << std::endl;
            return;
        }

        m_OnInteractCallbacks.erase( it );
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets whether the Interactable can be interacted with
    /// @return whether the Interactable can be interacted with
    bool Interactable::GetEnabled() const
    {
        return m_Enabled;
    }

    /// @brief  sets whether the Interactable can be interacted with
    /// @param  enabled whether the Interactable can be interacted with
    void Interactable::SetEnabled( bool enabled )
    {
        m_Enabled = enabled;
    }


    /// @brief  gets the radius at which this Interactable can be interacted with
    /// @return the radius at which this Interactable can be interacted with
    float Interactable::GetInteractionRadius() const
    {
        return m_InteractionRadius;
    }

    /// @brief  sets the radius at which this Interactable can be interacted with
    /// @param  radius  the radius at which this Interactable can be interacted with
    void Interactable::SetInteractionRadius( float radius )
    {
        m_InteractionRadius = radius;
    }


    /// @brief  gets the Transform attached to this Interactable
    /// @return the Transform attached to this Interactable
    Transform const* Interactable::GetTransform() const
    {
        return m_Transform;
    }

    /// @brief  gets the Sprite used to display the interact control prompt
    /// @return the Sprite used to display the interact control prompt
    Sprite* Interactable::GetPromptSprite()
    {
        return m_PromptSprite;
    }

    /// @brief  gets the Interact Action used to interact with this Interactable
    /// @return the Interact Action used to interact with this Interactable
    Action const* Interactable::GetInteractAction() const
    {
        return m_InteractAction;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void Interactable::OnInit()
    {
        Components< Interactable >()->AddComponent( this );

        m_Transform.SetOnConnectCallback( [ this ]()
        {
            if ( m_PromptTransform != nullptr )
            {
                m_PromptTransform->SetTranslation( m_Transform->GetTranslation() + m_PromptOffset );
            }
            m_Transform->AddOnTransformChangedCallback( GetId(), [ this ]()
            {
                if ( m_PromptTransform != nullptr )
                {
                    m_PromptTransform->SetTranslation( m_Transform->GetTranslation() + m_PromptOffset );
                }
            } );
        } );
        m_Transform.SetOnDisconnectCallback( [ this ]()
        {
            m_Transform->RemoveOnTransformChangedCallback( GetId() );
        } );

        m_PromptTransform.SetOnConnectCallback( [ this ]()
        {
            if ( m_Transform != nullptr )
            {
                m_PromptTransform->SetTranslation( m_Transform->GetTranslation() + m_PromptOffset );
            }
        } );

        m_Transform.Init( GetEntity() );

        if ( GetEntity()->GetChildren().size() > 0 )
        {
            m_PromptSprite   .Init( GetEntity()->GetChildren()[ 0 ] );
            m_PromptTransform.Init( GetEntity()->GetChildren()[ 0 ] );
        }

        m_InteractAction.Init();

        
        // m_PromptEntity.SetOwnerName( GetName() );
        // m_PromptEntity.Init();
    }


    /// @brief  called once when exiting the scene
    void Interactable::OnExit()
    {
        Components< Interactable >()->RemoveComponent( this );

        m_Transform      .Exit();
        m_PromptSprite   .Exit();
        m_PromptTransform.Exit();

        m_InteractAction.Exit();

        // m_PromptEntity.Exit();
    }


    /// @brief  called after a child is added to this Entity
    /// @param  child   the child Entity that was added
    void Interactable::OnAddChild( Entity* child )
    {
        if ( m_PromptSprite.GetEntity() == nullptr )
        {
            m_PromptSprite   .Init( child );
            m_PromptTransform.Init( child );
        }
    }

    /// @brief  called before a child is removed from this Entity
    /// @param  child   the child that will be removed
    void Interactable::OnRemoveChild( Entity* child )
    {
        if ( m_PromptSprite.GetEntity() == child )
        {
            m_PromptSprite   .Exit();
            m_PromptTransform.Exit();
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for Interactable
    void Interactable::Inspector()
    {
        ImGui::Checkbox( "Enabled", &m_Enabled );

        ImGui::DragFloat( "Interaction Radius", &m_InteractionRadius, 0.05f, 0.0f, INFINITY );

        m_InteractAction.Inspect( "Interact Control Action" );

        ImGui::DragFloat2( "prompt offset", &m_PromptOffset[ 0 ], 0.05f );

        ImGui::InputText( "Cast Event", &m_EventCast );
        // m_PromptEntity.Inspect( "Prompt Entity" );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads whether the Interactable can be interacted with
    /// @param  data    the JSON data to read from
    void Interactable::readEnabled( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Enabled, data );
    }

    /// @brief  reads the radius at which this Interactable can be interacted with
    /// @param  data    the JSON data to read from
    void Interactable::readInteractionRadius( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_InteractionRadius, data );
    }

    /// @brief  reads the control Action used to interact with this Interactable
    /// @param  data    the JSON data to read from
    void Interactable::readInteractAction( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_InteractAction, data );
    }

    /// @brief  reads the offset position to display the prompt at
    /// @param  data    the JSON data to read from
    void Interactable::readPromptOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_PromptOffset, data );
    }

    /// @brief  reads the event to cast when interacted with
    /// @param  data    the JSON data to read from
    void Interactable::readEventCast(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_EventCast, data);
    }

    // /// @brief  reads the Entity used to display the interact control prompt
    // /// @param  data    the JSON data to read from
    // void Interactable::readPromptEntity( nlohmann::ordered_json const& data )
    // {
    //     Stream::Read( m_PromptEntity, data );
    // }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Interactable
    /// @return the map of read methods for this Interactable
    ReadMethodMap< ISerializable > const& Interactable::GetReadMethods() const
    {
        static ReadMethodMap< Interactable > const readMethods = {
            { "Enabled"          , &Interactable::readEnabled           },
            { "InteractionRadius", &Interactable::readInteractionRadius },
            { "InteractAction"   , &Interactable::readInteractAction    },
            { "PromptOffset"     , &Interactable::readPromptOffset      },
            { "EventCast"        , &Interactable::readEventCast         }

            // { "PromptEntity"     , &Interactable::readPromptEntity      }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this Interactable to JSON
    /// @return the JSON data of this Interactable
    nlohmann::ordered_json Interactable::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "Enabled"           ] = Stream::Write( m_Enabled           );
        json[ "InteractionRadius" ] = Stream::Write( m_InteractionRadius );
        json[ "InteractAction"    ] = Stream::Write( m_InteractAction    );
        json[ "PromptOffset"      ] = Stream::Write( m_PromptOffset      );
        json[ "EventCast"         ] = Stream::Write( m_EventCast         );

        // json[ "PromptEntity"      ] = Stream::Write( m_PromptEntity      );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Interactable
    /// @return the newly created clone of this Interactable
    Interactable* Interactable::Clone() const
    {
        return new Interactable( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Interactable
    /// @param  other   the other Interactable to copy
    Interactable::Interactable( Interactable const& other ) :
        Component( other ),
        m_Enabled          ( other.m_Enabled           ),
        m_InteractionRadius( other.m_InteractionRadius ),
        m_InteractAction   ( other.m_InteractAction    ),
        m_PromptOffset     ( other.m_PromptOffset      ),
        m_EventCast        ( other.m_EventCast         )
        // m_PromptEntity     ( other.m_PromptEntity, { &m_PromptSprite } )
    {}


//-----------------------------------------------------------------------------
