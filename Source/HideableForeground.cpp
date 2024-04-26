/// @file       HideableForeground.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that hides an attached Sprite when it overlaps something
/// @version    0.1
/// @date       2024-02-25
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology



#include "pch.h" // precompiled header has to be included first
#include "HideableForeground.h"

#include "BehaviorSystem.h"

#include "ComponentReference.t.h"




//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    HideableForeground::HideableForeground() :
        Behavior( typeid( HideableForeground ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void HideableForeground::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_Collider.SetOnConnectCallback(
            [ this ]()
            {
                m_Collider->AddOnCollisionEnterCallback(
                    GetId(),
                    [ this ]( Collider* collider )
                    {
                        onCollisionEnter( collider );
                    }
                );
                m_Collider->AddOnCollisionExitCallback(
                    GetId(),
                    [ this ]( Collider* collider )
                    {
                        onCollisionExit( collider );
                    }
                );
            }
        );
        m_Collider.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Collider->RemoveOnCollisionEnterCallback( GetId() );
                m_Collider->RemoveOnCollisionExitCallback ( GetId() );
            }
        );

        m_Sprite  .Init( GetEntity() );
        m_Collider.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void HideableForeground::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_Sprite  .Exit();
        m_Collider.Exit();
    }


    /// @brief  called every graphics frame
    /// @param  dt  the amount of time since the last frame
    void HideableForeground::OnUpdate( float dt )
    {
        if ( m_IsFading == false || m_Sprite == nullptr )
        {
            return;
        }

        float opacity = m_Sprite->GetOpacity();

        if ( m_CurrentlyOverlapping )
        {
            opacity -= m_FadeSpeed * dt;
            if ( opacity <= m_OverlappedOpacity )
            {
                opacity = m_OverlappedOpacity;
                m_IsFading = false;
            }
        }
        else
        {
            opacity += m_FadeSpeed * dt;
            if ( opacity >= m_DefaultOpacity )
            {
                opacity = m_DefaultOpacity;
                m_IsFading = false;
            }
        }

        m_Sprite->SetOpacity( opacity );
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  callback called when entering a collision
    /// @param  collider    the collider that was collided with
    void HideableForeground::onCollisionEnter( Collider* collider )
    {
        m_CurrentlyOverlapping = true;
        m_IsFading = true;
    }

    /// @brief  callback called when entering a collision
    /// @param  collider    the collider that was collided with
    void HideableForeground::onCollisionExit( Collider* collider )
    {
        m_CurrentlyOverlapping = false;
        m_IsFading = true;
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for HideableForeground
    void HideableForeground::Inspector()
    {
        if ( m_Sprite == nullptr )
        {
            ImGui::Text( "WARNING: no Sprite Component attached" );
        }
        if ( m_Collider == nullptr )
        {
            ImGui::Text( "WARNING: no Collider Component attached" );
        }

        ImGui::DragFloat( "overlapped opacity", &m_OverlappedOpacity, 0.05f, 0.0f, 1.0f );
        ImGui::DragFloat( "default opacity"   , &m_DefaultOpacity   , 0.05f, 0.0f, 1.0f );

        ImGui::DragFloat( "fade speed", &m_FadeSpeed, 0.05f, 0.0f, INFINITY );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the opacity when overlapping the player
    /// @param  data    the JSON data to read from
    void HideableForeground::readOverlappedOpacity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_OverlappedOpacity, data );
    }

    /// @brief  reads the opacity when not overlapping the player
    /// @param  data    the JSON data to read from
    void HideableForeground::readDefaultOpacity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_DefaultOpacity, data );
    }

    /// @brief  reads how quickly the opacity fades between the two states
    /// @param  data    the JSON data to read from
    void HideableForeground::readFadeSpeed( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_FadeSpeed, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this HideableForeground
    /// @return the map of read methods for this HideableForeground
    ReadMethodMap< ISerializable > const& HideableForeground::GetReadMethods() const
    {
        static ReadMethodMap< HideableForeground > const readMethods = {
            { "OverlappedOpacity", &HideableForeground::readOverlappedOpacity },
            { "DefaultOpacity"   , &HideableForeground::readDefaultOpacity    },
            { "FadeSpeed"        , &HideableForeground::readFadeSpeed         }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this HideableForeground to JSON
    /// @return the JSON data of this HideableForeground
    nlohmann::ordered_json HideableForeground::Write() const
    {
        nlohmann::ordered_json json;

        json[ "OverlappedOpacity" ] = Stream::Write( m_OverlappedOpacity );
        json[ "DefaultOpacity"    ] = Stream::Write( m_DefaultOpacity    );
        json[ "FadeSpeed"         ] = Stream::Write( m_FadeSpeed         );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this HideableForeground
    /// @return the newly created clone of this HideableForeground
    HideableForeground* HideableForeground::Clone() const
    {
        return new HideableForeground( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the HideableForeground
    /// @param  other   the other HideableForeground to copy
    HideableForeground::HideableForeground( HideableForeground const& other ) :
        Behavior( other ),
        m_OverlappedOpacity( m_OverlappedOpacity ),
        m_DefaultOpacity   ( m_DefaultOpacity    ),
        m_FadeSpeed        ( m_FadeSpeed         )
    {}


//-----------------------------------------------------------------------------
