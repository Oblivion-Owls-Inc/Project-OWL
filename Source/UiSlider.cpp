/// @file       UiSlider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui Slider component
/// @version    0.1
/// @date       2024-03-26
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "pch.h"

#include "UiSlider.h"

#include "ComponentReference.t.h"
#include "UiBarSprite.h"

#include "UiElement.h"

#include "BehaviorSystem.h"

#include "RenderSystem.h"
#include "InputSystem.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    UiSlider::UiSlider() :
        Behavior( typeid( UiSlider ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  adds a callback to call whenever the slider is moved
    /// @param  ownerId     the ID of the owner of the callback to add
    /// @param  callback    the callback to add
    void UiSlider::AddOnSliderValueChangedCallback( unsigned ownerId, std::function< void( float newValue ) > const& callback )
    {
        m_OnSliderValueChangedCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes a callback to call whenever the slider is moved
    /// @param  ownerId the ID of the owner of the callback to remove
    void UiSlider::RemoveOnSliderValueChangedCallback( unsigned ownerId )
    {
        auto it = m_OnSliderValueChangedCallbacks.find( ownerId );
        if ( it == m_OnSliderValueChangedCallbacks.end() )
        {
            Debug() << "ERROR: unable to find OnSliderValueChangedCallback with ownerId " << ownerId <<
                " attached to " << GetName() << std::endl;
            return;
        }

        m_OnSliderValueChangedCallbacks.erase( it );
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the current value of the slider from 0.0f to 1.0f
    /// @return the current value of the slider from 0.0f to 1.0f
    float UiSlider::GetValue() const
    {
        return m_Value;
    }

    /// @brief  sets the current value of the slider from 0.0f to 1.0f
    /// @param  value   the value from 0.0f to 1.0f to set the slider to
    void UiSlider::SetValue( float value )
    {
        m_Value = value;

        updateVisuals();
        callOnSliderValueChangedCallbacks();
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void UiSlider::OnInit()
    {
        Behaviors< UiSlider >()->AddComponent( this );

        m_DragSliderAction.Init();

        m_UiBarSprite.Init( GetEntity() );
        m_UiElement  .Init( GetEntity() );

        if ( GetEntity()->GetChildren().size() == 0 )
        {
            return;
        }

        m_HandleSprite   .Init( GetEntity()->GetChildren()[ 0 ] );
        m_HandleUiElement.Init( GetEntity()->GetChildren()[ 0 ] );
    }

    /// @brief  called once when exiting the scene
    void UiSlider::OnExit()
    {
        Behaviors< UiSlider >()->RemoveComponent( this );

        m_DragSliderAction.Exit();

        m_UiBarSprite    .Exit();
        m_UiElement      .Exit();

        m_HandleSprite   .Exit();
        m_HandleUiElement.Exit();
    }


    /// @brief  called every simulation frame
    void UiSlider::OnFixedUpdate()
    {
        if ( m_UiElement == nullptr || m_UiBarSprite == nullptr || m_DragSliderAction == nullptr )
        {
            return;
        }

        if ( m_IsBeingDragged == false && m_DragSliderAction->GetTriggered() == false )
        {
            return;
        }

        if ( m_DragSliderAction->GetReleased() )
        {
            m_IsBeingDragged = false;
            return;
        }

        Sprite const* hoveredSprite = Renderer()->GetMouseOverSprite();
        bool hovered = hoveredSprite == m_UiBarSprite || ( m_HandleSprite != nullptr && hoveredSprite == m_HandleSprite );
        if ( hovered == false && m_IsBeingDragged == false )
        {
            return;
        }

        m_IsBeingDragged = true;

        float position = m_UiElement->UiPosToLocalAnchor( Input()->GetMousePosUI() ).x * 0.5f + 0.5f;
        glm::vec2 const& borderPositions = m_UiBarSprite->GetBorderPositions();
        position = (position - borderPositions[ 0 ]) / (borderPositions[ 1 ] - borderPositions[ 0 ]);
        position = std::clamp( position, 0.0f, 1.0f );

        SetValue( position );
    }


    /// @brief  called after a child is added to this Entity
    /// @param  child   the child that was added
    void UiSlider::OnAddChild( Entity* child )
    {
        // only attach the first child
        if ( GetEntity()->GetChildren().size() == 1 )
        {
            m_HandleSprite   .Init( child );
            m_HandleUiElement.Init( child );
        }
    }

    /// @brief  called when a child is removed from this Entity
    /// @param  child   the child that is being removed
    void UiSlider::OnRemoveChild( Entity* child )
    {
        if ( m_HandleSprite.GetEntity() == child )
        {
            m_HandleSprite   .Exit();
            m_HandleUiElement.Exit();
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  updates the UiBarSprite and handle position
    void UiSlider::updateVisuals()
    {
        // adjust UiBarSprite
        if ( m_UiElement == nullptr || m_UiBarSprite == nullptr )
        {
            return;
        }

        std::vector< UiBarSprite::UiBarSection >& sections = m_UiBarSprite->GetSections();
        if ( sections.size() == 0 )
        {
            Debug() << "WARNING: UiBarSprite has 0 sections. expected at least 1 (" << GetName() << ")." << std::endl;
            return;
        }

        sections[ 0 ].M_Value = m_Value;


        // adjust handle position
        if ( m_HandleUiElement == nullptr )
        {
            return;
        }

        glm::vec2 const& borderPositions = m_UiBarSprite->GetBorderPositions();
        float position = (m_Value * (borderPositions[ 1 ] - borderPositions[ 0 ]) + borderPositions[ 0 ]) * 2.0f - 1.0f;

        glm::vec2 anchor = m_HandleUiElement->GetAnchor();
        anchor.x = position;
        m_HandleUiElement->SetAnchor( anchor );
    }


    /// @brief  calls the OnSliderValueChanded callbacks attached to this UiSlider
    void UiSlider::callOnSliderValueChangedCallbacks()
    {
        for ( auto& [ id, callback ] : m_OnSliderValueChangedCallbacks )
        {
            callback( m_Value );
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for UiSlider
    void UiSlider::Inspector()
    {
        if ( m_UiElement == nullptr )
        {
            ImGui::Text( "WARNING: no UiElement Component attached" );
        }
        if ( m_UiBarSprite == nullptr )
        {
            ImGui::Text( "WARNING: no UiBarSprite Component attached" );
        }

        if ( ImGui::DragFloat( "value", &m_Value, 0.01f, 0.0f, 1.0f ) )
        {
            updateVisuals();
            callOnSliderValueChangedCallbacks();
        }

        m_DragSliderAction.Inspect( "Drag Slider Control" );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the current value of the slider
    /// @param  data    the JSON data to read from
    void UiSlider::readValue( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Value, data );
    }

    /// @brief  reads the control action used to drag the slider
    /// @param  data    the JSON data to read from
    void UiSlider::readDragSliderAction( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_DragSliderAction, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this UiSlider
    /// @return the map of read methods for this UiSlider
    ReadMethodMap< ISerializable > const& UiSlider::GetReadMethods() const
    {
        static ReadMethodMap< UiSlider > const readMethods = {
            { "Value"           , &UiSlider::readValue            },
            { "DragSliderAction", &UiSlider::readDragSliderAction }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this UiSlider to JSON
    /// @return the JSON data of this UiSlider
    nlohmann::ordered_json UiSlider::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "Value"            ] = Stream::Write( m_Value            );
        json[ "DragSliderAction" ] = Stream::Write( m_DragSliderAction );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this UiSlider
    /// @return the newly created clone of this UiSlider
    UiSlider* UiSlider::Clone() const
    {
        return new UiSlider( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the UiSlider
    /// @param  other   the other UiSlider to copy
    UiSlider::UiSlider( UiSlider const& other ) :
        Behavior( other ),
        m_Value           ( other.m_Value            ),
        m_DragSliderAction( other.m_DragSliderAction )
    {}


//-----------------------------------------------------------------------------
