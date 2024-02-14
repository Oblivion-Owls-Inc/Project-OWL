/// @file       HealthBar.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      health bar UI that displays offset from an Entity with a Health component
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "HealthBar.h"

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

        m_Transform   = GetEntity()->GetComponent< Transform   >();
        m_UiBarSprite = GetEntity()->GetComponent< UiBarSprite >();


        if ( m_TargetEntityName == "" )
        {
            Entity* parent = GetEntity()->GetParent();
            if ( parent == nullptr )
            {
                Debug() << "WARNING: HealthBar does not have a target or parent" << std::endl;
                return;
            }
            m_ParentTransform = parent->GetComponent< Transform >();
            m_TargetHealth    = parent->GetComponent< Health    >();
        }
        else
        {
            Entity* target = Entities()->GetEntity( m_TargetEntityName );
            if ( target == nullptr )
            {
                Debug() << "WARNING: healthbar's target could not be found" << std::endl;
                return;
            }
            m_TargetHealth = target->GetComponent< Health >();
        }


        if ( m_TargetHealth == nullptr )
        {
            Debug() << "WARNING: HealthBar does not have a target with a Health component" << std::endl;
            return;
        }

        m_TargetHealth->AddOnHealthChangedCallback(
            GetId(),
            std::bind( &HealthBar::onHealthChangedCallback, this )
        );
    }

    /// @brief  called once when exiting the scene
    void HealthBar::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        if ( m_TargetHealth != nullptr )
        {
            m_TargetHealth->RemoveOnHealthChangedCallback( GetId() );
        }
    }


    /// @brief  called once per graphics frame
    /// @param  dt  the length of time the frame lasts
    void HealthBar::OnUpdate( float dt )
    {
        updateRecentHealth( dt );

        updateVisuals( dt );
    }


    /// @brief  called every time after the Entity this Component is attached to's heirarchy changes
    void HealthBar::OnHeirarchyChange()
    {
        if ( m_TargetEntityName == "" )
        {
            return;
        }

        if ( m_TargetHealth != nullptr )
        {
            m_TargetHealth->RemoveOnHealthChangedCallback( GetId() );
        }


        Entity* parent = GetEntity()->GetParent();
        if ( parent == nullptr )
        {
            Debug() << "WARNING: HealthBar Component has no target or parent Entity" << std::endl;
            m_ParentTransform = nullptr;
            m_TargetHealth = nullptr;
            return;
        }

        m_ParentTransform = parent->GetComponent< Transform >();
        m_TargetHealth    = parent->GetComponent< Health    >();

        if ( m_TargetHealth == nullptr )
        {
            Debug() << "WARNING: HealthBar Component's parent does not have a Health component" << std::endl;
            return;
        }

        m_TargetHealth->AddOnHealthChangedCallback(
            GetId(),
            std::bind( &HealthBar::onHealthChangedCallback, this )
        );
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
        else
        {
            int breakpoint = 2;
        }
    }


    /// @brief  updates the Sprite and Transform attached to the HealthBar
    /// @param  dt  the length of time the frame lasts
    void HealthBar::updateVisuals( float dt )
    {
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

        if ( m_ParentTransform != nullptr && m_TargetEntityName.empty() )
        {
            glm::vec2 position = m_ParentTransform->GetTranslation() + m_Offset;
            m_Transform->SetTranslation( position );
        }

    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this HealthBar in the Inspector
    void HealthBar::Inspector()
    {
        ImGui::DragFloat2( "offset", &m_Offset[ 0 ], 0.05f );

        ImGui::Checkbox( "hide when full", &m_HideWhenFull );

        if ( m_HideWhenFull )
        {
            ImGui::DragFloat( "opacity animation time", &m_OpacityAnimationTime, 0.05f, 0.0f, INFINITY );
        }

        ImGui::DragFloat( "max opacity", &m_MaxOpacity, 0.05f, 0.0f, 1.0f );

        ImGui::DragFloat( "recent health depletion rate", &m_RecentHealthAcceleration, 0.05f, 0.0f, INFINITY );

        Entity* targetEntity = m_TargetEntityName.empty() ? nullptr : m_TargetHealth->GetEntity();
        if ( Inspection::SelectEntityFromScene( "target entity", &targetEntity, &m_TargetEntityName ) )
        {
            if ( m_TargetHealth != nullptr )
            {
                m_TargetHealth->RemoveOnHealthChangedCallback( GetId() );
            }

            m_TargetHealth = targetEntity->GetComponent< Health >();

            if ( m_TargetHealth != nullptr )
            {
                m_TargetHealth->AddOnHealthChangedCallback(
                    GetId(),
                    std::bind( &HealthBar::onHealthChangedCallback, this )
                );
            }
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


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
    void HealthBar::readTargetEntityName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TargetEntityName, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& HealthBar::GetReadMethods() const
    {
        static ReadMethodMap< HealthBar > const readMethods = {
            { "Offset"                  , &HealthBar::readOffset                   },
            { "HideWhenFull"            , &HealthBar::readHideWhenFull             },
            { "OpacityAnimationTime"    , &HealthBar::readOpacityAnimationTime     },
            { "MaxOpacity"              , &HealthBar::readMaxOpacity               },
            { "RecentHealthAcceleration", &HealthBar::readRecentHealthAcceleration },
            { "TargetEntityName"        , &HealthBar::readTargetEntityName         }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all HealthBar data to a JSON file.
    /// @return The JSON file containing the HealthBar data.
    nlohmann::ordered_json HealthBar::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Offset"                   ] = Stream::Write( m_Offset                   );
        json[ "HideWhenFull"             ] = Stream::Write( m_HideWhenFull             );
        json[ "OpacityAnimationTime"     ] = Stream::Write( m_OpacityAnimationTime     );
        json[ "MaxOpacity"               ] = Stream::Write( m_MaxOpacity               );
        json[ "RecentHealthAcceleration" ] = Stream::Write( m_RecentHealthAcceleration );
        json[ "TargetEntityName"         ] = Stream::Write( m_TargetEntityName         );

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
