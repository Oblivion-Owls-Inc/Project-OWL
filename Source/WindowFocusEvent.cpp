/// @file       WindowFocusEvent.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Broadcasts events when the window gains/loses focus
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h"

#include "WindowFocusEvent.h"

#include "PlatformSystem.h"
#include "EventSystem.h"
#include "PauseSystem.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    WindowFocusEvent::WindowFocusEvent() :
        Component( typeid( WindowFocusEvent ) )
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
    void WindowFocusEvent::OnInit()
    {
        Platform()->AddOnFocusChangedCallback( GetId(), [this]( bool focused )
        {
            if ( m_DisableWhenPaused && Pause()->GetRunning() == false )
            {
                return;
            }

            if ( focused )
            {
                if ( m_GainFocusEvent.empty() == false )
                {
                    Events()->BroadcastEvent( m_GainFocusEvent );
                }
            }
            else
            {
                if ( m_LoseFocusEvent.empty() == false )
                {
                    Events()->BroadcastEvent( m_LoseFocusEvent );
                }
            }
        } );
    }

    /// @brief  called once when exiting the scene
    void WindowFocusEvent::OnExit()
    {
        Platform()->RemoveOnFocusChangedCallback( GetId() );
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for WindowFocusEvent
    void WindowFocusEvent::Inspector()
    {
        ImGui::InputText( "gain focus event", &m_GainFocusEvent );
        ImGui::InputText( "lose focus event", &m_LoseFocusEvent );

        ImGui::Checkbox( "disable when paused", &m_DisableWhenPaused );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  event to broadcast when the window gains focus
    /// @param  data    the JSON data to read from
    void WindowFocusEvent::readGainFocusEvent( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_GainFocusEvent, data );
    }

    /// @brief  event to broadcast when the window loses focus
    /// @param  data    the JSON data to read from
    void WindowFocusEvent::readLoseFocusEvent( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_LoseFocusEvent, data );
    }

    /// @brief  reads whether to disable to broadcasting of events while the game is paused
    /// @param  data    the JSON data to read from
    void WindowFocusEvent::readDisableWhenPaused( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_DisableWhenPaused, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this WindowFocusEvent
    /// @return the map of read methods for this WindowFocusEvent
    ReadMethodMap< ISerializable > const& WindowFocusEvent::GetReadMethods() const
    {
        static ReadMethodMap< WindowFocusEvent > const readMethods = {
            { "GainFocusEvent"   , &WindowFocusEvent::readGainFocusEvent    },
            { "LoseFocusEvent"   , &WindowFocusEvent::readLoseFocusEvent    },
            { "DisableWhenPaused", &WindowFocusEvent::readDisableWhenPaused }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }
    
    
    /// @brief  writes this WindowFocusEvent to JSON
    /// @return the JSON data of this WindowFocusEvent
    nlohmann::ordered_json WindowFocusEvent::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "GainFocusEvent"    ] = Stream::Write( m_GainFocusEvent    );
        json[ "LoseFocusEvent"    ] = Stream::Write( m_LoseFocusEvent    );
        json[ "DisableWhenPaused" ] = Stream::Write( m_DisableWhenPaused );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this WindowFocusEvent
    /// @return the newly created clone of this WindowFocusEvent
    WindowFocusEvent* WindowFocusEvent::Clone() const
    {
        return new WindowFocusEvent( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the WindowFocusEvent
    /// @param  other   the other WindowFocusEvent to copy
    WindowFocusEvent::WindowFocusEvent( WindowFocusEvent const& other ) :
        Component( other ),
        m_GainFocusEvent   ( other.m_GainFocusEvent    ),
        m_LoseFocusEvent   ( other.m_LoseFocusEvent    ),
        m_DisableWhenPaused( other.m_DisableWhenPaused )
    {}


//-----------------------------------------------------------------------------