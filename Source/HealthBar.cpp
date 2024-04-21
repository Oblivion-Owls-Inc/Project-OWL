/// @file       HealthBar.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      health bar UI that displays offset from an Entity with a Health component
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology



#include "pch.h" // precompiled header has to be included first
#include "HealthBar.h"

#include "ComponentReference.t.h"

#include "Transform.h"
#include "Health.h"
#include "UiBarSprite.h"

#include "Entity.h"

#include "EntitySystem.h"
#include "BehaviorSystem.h"
#include "RenderSystem.h"


//-----------------------------------------------------------------------------
// public: types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// private: types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void HealthBar::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_TargetHealth.SetOnConnectCallback(
            [ this ]()
            {
                m_TargetHealth->AddOnHealthChangedCallback(
                    GetId(),
                    std::bind( &HealthBar::onHealthChangedCallback, this )
                );
                onHealthChangedCallback();
            }
        );
        m_TargetHealth.SetOnDisconnectCallback(
            [ this ]()
            {
                m_TargetHealth->RemoveOnHealthChangedCallback( GetId() );
                m_CurrentHealthPortion = 0.0f;
            }
        );

        m_Transform  .Init( GetEntity() );
        m_UiBarSprite.Init( GetEntity() );


        m_TargetEntity.SetOwnerName( GetName() );

        if ( m_TargetEntity.GetName().empty() )
        {
            m_TargetEntity = GetEntity()->GetParent();
        }
        else
        {
            m_TargetEntity.Init();
        }

    }

    /// @brief  called once when exiting the scene
    void HealthBar::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_Transform  .Exit();
        m_UiBarSprite.Exit();

        m_TargetEntity.Exit();
    }


    /// @brief  called once per graphics frame
    /// @param  dt  the length of time the frame lasts
    void HealthBar::OnUpdate( float dt )
    {
        updateRecentHealth( dt );

        updateVisuals( dt );
    }


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


    /// @brief  callback called whenever the health component changes
    void HealthBar::onHealthChangedCallback()
    {
        Pool< int > const& health = *m_TargetHealth->GetHealth();
        m_CurrentHealthPortion = (float)health.GetCurrent() / health.GetMaximum();

        m_RecentHealthVelocity = 0.0f;
    }


    /// @brief  updates the recent health portion of the health bar
    /// @param  dt  the length of time the frame lasts
    void HealthBar::updateRecentHealth( float dt )
    {
        m_RecentHealthPortion -= m_RecentHealthVelocity * dt;
        m_RecentHealthVelocity += m_RecentHealthAcceleration * dt; // do this after to delay fall by an additional frame

        if ( m_RecentHealthPortion <= m_CurrentHealthPortion )
        {
            m_RecentHealthPortion = m_CurrentHealthPortion;
            m_RecentHealthVelocity = 0.0f;
        }
    }


    /// @brief  updates the Sprite and Transform attached to the HealthBar
    /// @param  dt  the length of time the frame lasts
    void HealthBar::updateVisuals( float dt )
    {
        if ( m_UiBarSprite == nullptr )
        {
            return;
        }

        std::vector< UiBarSprite::UiBarSection >& barSections = m_UiBarSprite->GetSections();
        if ( barSections.size() < 2 )
        {
            Debug() << "WARNING: HealthBar UiBarSprite has less than two sections" << std::endl;
            return;
        }
        barSections[ 0 ].M_Value = m_CurrentHealthPortion;
        barSections[ 1 ].M_Value = m_RecentHealthPortion;


        if ( m_HideWhenFull )
        {
            if ( m_CurrentHealthPortion >= 1.0f )
            {
                float opacity = m_UiBarSprite->GetOpacity();
                opacity -= m_MaxOpacity / m_OpacityAnimationTime * dt;
                opacity = std::max( 0.0f, opacity );
                m_UiBarSprite->SetOpacity( opacity );
            }
            else
            {
                float opacity = m_UiBarSprite->GetOpacity();
                opacity += m_MaxOpacity / m_OpacityAnimationTime * dt;
                opacity = std::min( 1.0f, opacity );
                m_UiBarSprite->SetOpacity( opacity );
            }
        }

        if ( m_FollowTargetEntity )
        {
            if ( m_Transform == nullptr || m_TargetTransform == nullptr )
            {
                return;
            }

            glm::vec2 position = m_TargetTransform->GetTranslation() + m_Offset;
            m_Transform->SetTranslation( position );
        }

    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this HealthBar in the Inspector
    void HealthBar::Inspector()
    {
        ImGui::Checkbox( "Follow Target Entity", &m_FollowTargetEntity );

        if ( m_FollowTargetEntity )
        {
            ImGui::DragFloat2( "offset", &m_Offset[ 0 ], 0.05f );
        }


        if ( ImGui::Checkbox( "hide when full", &m_HideWhenFull ) )
        {
            if ( m_HideWhenFull == false && m_UiBarSprite != nullptr )
            {
                m_UiBarSprite->SetOpacity( m_MaxOpacity );
            }
        }

        if ( m_HideWhenFull )
        {
            ImGui::DragFloat( "opacity animation time", &m_OpacityAnimationTime, 0.05f, 0.0f, INFINITY );
        }


        ImGui::DragFloat( "max opacity", &m_MaxOpacity, 0.05f, 0.0f, 1.0f );

        ImGui::DragFloat( "recent health depletion rate", &m_RecentHealthAcceleration, 0.05f, 0.0f, INFINITY );


        m_TargetEntity.Inspect( "Target Entity" );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads Whether this HealthBar should move to follow the TargetEntity
    /// @param  data    the json data to read from
    void HealthBar::readFollowTargetEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_FollowTargetEntity, data );
    }

    /// @brief  reads the offset from the target Entity to display the health bar
    /// @param  data    the json data to read from
    void HealthBar::readOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_Offset, data );
    }


    /// @brief  reads whether the bar should be hidden when the health is full
    /// @param  data    the json data to read from
    void HealthBar::readHideWhenFull( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_HideWhenFull, data );
    }

    /// @brief  how long the opacity takes to animate in and out when the health is full
    /// @param  data    the json data to read from
    void HealthBar::readOpacityAnimationTime( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_OpacityAnimationTime, data );
    }

    /// @brief  the maximum opacity of the health bar
    /// @param  data    the json data to read from
    void HealthBar::readMaxOpacity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MaxOpacity, data );
    }


    /// @brief  reads the acceleration of how quickly the recent health display depletes
    /// @param  data    the json data to read from
    void HealthBar::readRecentHealthAcceleration( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_RecentHealthAcceleration, data );
    }


    /// @brief  reads the name of the entity to track the health of
    /// @param  data    the json data to read from
    void HealthBar::readTargetEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TargetEntity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& HealthBar::GetReadMethods() const
    {
        static ReadMethodMap< HealthBar > const readMethods = {
            { "FollowTargetEntity"      , &HealthBar::readFollowTargetEntity       },
            { "Offset"                  , &HealthBar::readOffset                   },
            { "HideWhenFull"            , &HealthBar::readHideWhenFull             },
            { "OpacityAnimationTime"    , &HealthBar::readOpacityAnimationTime     },
            { "MaxOpacity"              , &HealthBar::readMaxOpacity               },
            { "RecentHealthAcceleration", &HealthBar::readRecentHealthAcceleration },
            { "TargetEntity"            , &HealthBar::readTargetEntity             }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all HealthBar data to a JSON file.
    /// @return The JSON file containing the HealthBar data.
    nlohmann::ordered_json HealthBar::Write() const
    {
        nlohmann::ordered_json json;

        json[ "FollowTargetEntity"       ] = Stream::Write( m_FollowTargetEntity       );
        json[ "Offset"                   ] = Stream::Write( m_Offset                   );
        json[ "HideWhenFull"             ] = Stream::Write( m_HideWhenFull             );
        json[ "OpacityAnimationTime"     ] = Stream::Write( m_OpacityAnimationTime     );
        json[ "MaxOpacity"               ] = Stream::Write( m_MaxOpacity               );
        json[ "RecentHealthAcceleration" ] = Stream::Write( m_RecentHealthAcceleration );
        json[ "TargetEntity"             ] = Stream::Write( m_TargetEntity             );

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    HealthBar::HealthBar() :
        Behavior( typeid( HealthBar ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this HealthBar
    /// @return the newly created clone of this HealthBar
    HealthBar* HealthBar::Clone() const
    {
        return new HealthBar( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the HealthBar
    /// @param  other   the other HealthBar to copy
    HealthBar::HealthBar( const HealthBar& other ) :
        Behavior( other ),
        m_Offset                  ( other.m_Offset                   ),
        m_HideWhenFull            ( other.m_HideWhenFull             ),
        m_RecentHealthAcceleration( other.m_RecentHealthAcceleration )
    {}


//-----------------------------------------------------------------------------
