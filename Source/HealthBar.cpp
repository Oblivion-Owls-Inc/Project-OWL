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

        Entity* parent = GetEntity()->GetParent();
        if ( parent == nullptr )
        {
            Debug() << "WARNING: HealthBar Component has no parent Entity" << std::endl;
            return;
        }

        m_ParentTransform = parent->GetComponent< Transform >();
        m_ParentHealth    = parent->GetComponent< Health    >();

        if ( m_ParentHealth == nullptr )
        {
            Debug() << "WARNING: HealthBar Component's parent does not have a Health component" << std::endl;
            return;
        }

        m_ParentHealth->AddOnHealthChangedCallback(
            GetId(),
            std::bind( &HealthBar::onHealthChangedCallback, this )
        );
    }

    /// @brief  called once when exiting the scene
    void HealthBar::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        if ( m_ParentHealth != nullptr )
        {
            m_ParentHealth->RemoveOnHealthChangedCallback( GetId() );
        }
    }


    /// @brief  called once per graphics frame
    /// @param  dt  the length of time the frame lasts
    void HealthBar::OnUpdate( float dt )
    {
        updateRecentHealth( dt );

        updateVisuals();
    }


    /// @brief  called every time after the Entity this Component is attached to's heirarchy changes
    void HealthBar::OnHeirarchyChange()
    {
        Entity* parent = GetEntity()->GetParent();
        if ( parent == nullptr )
        {
            Debug() << "WARNING: HealthBar Component has no parent Entity" << std::endl;
            m_ParentTransform = nullptr;
            m_ParentHealth = nullptr;
            return;
        }

        m_ParentTransform = parent->GetComponent< Transform >();
        m_ParentHealth    = parent->GetComponent< Health    >();

        if ( m_ParentHealth == nullptr )
        {
            Debug() << "WARNING: HealthBar Component's parent does not have a Health component" << std::endl;
            return;
        }

        m_ParentHealth->AddOnHealthChangedCallback(
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
        Pool< int > const& health = *m_ParentHealth->GetHealth();
        m_CurrentHealthPortion = (float)health.GetCurrent() / health.GetDefault();

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
    void HealthBar::updateVisuals()
    {
        if ( m_ParentTransform == nullptr )
        {
            return;
        }

        glm::vec2 position = m_ParentTransform->GetTranslation() + m_Offset;
        m_Transform->SetTranslation( position );

        std::vector< UiBarSprite::UiBarSection >& barSections = m_UiBarSprite->GetSections();
        if ( barSections.size() < 2 )
        {
            Debug() << "WARNING: HealthBar UiBarSprite has less than two sections" << std::endl;
            return;
        }

        barSections[ 0 ].M_Value = m_CurrentHealthPortion;
        barSections[ 1 ].M_Value = m_RecentHealthPortion;
    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this HealthBar in the Inspector
    void HealthBar::Inspector()
    {
        ImGui::DragFloat2( "offset", &m_Offset[ 0 ], 0.05f );

        ImGui::Checkbox( "hide when full", &m_HideWhenFull );

        ImGui::DragFloat( "recent health depletion rate", &m_RecentHealthAcceleration, 0.05f, 0.0f, INFINITY );
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


    /// @brief  reads the acceleration of how quickly the recent health display depletes
    /// @param  data    the json data to read from
    void HealthBar::readRecentHealthAcceleration( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_RecentHealthAcceleration, data );
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
            { "RecentHealthAcceleration", &HealthBar::readRecentHealthAcceleration }
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
        json[ "RecentHealthAcceleration" ] = Stream::Write( m_RecentHealthAcceleration );

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
