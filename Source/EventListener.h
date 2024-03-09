/// @file       EventListener.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      class that listens for specific events from the EventSystem
/// @version    0.1
/// @date       2024-01-31
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

    #include "pch.h" 
    #include "EventSystem.h"


//-----------------------------------------------------------------------------


/// @brief  unified base class for templatized EventListeners
class EventListenerBase
{
//-----------------------------------------------------------------------------
public: // destructor
//-----------------------------------------------------------------------------

    /// @brief  virtual destructor for polymorphic class
    virtual ~EventListenerBase() = default;

//-----------------------------------------------------------------------------
};


/// @brief  Event Listener that listens for a specific kind of event
/// @tparam EventType   the type of the event to listen for
template < typename EventType >
class EventListener : public EventListenerBase
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------
    

    /// @brief  constructor
    /// @param  response    the response to the event
    /// @param  filter      
    EventListener(
        std::function< void ( EventType const& ) > response = []( EventType const& event ) {},
        std::function< bool ( EventType const& ) > filter = []( EventType const& event ) -> bool { return true; }
    ) :
        m_ResponseFunction( response ),
        m_FilterFunction( filter )
    {}


//-----------------------------------------------------------------------------
public: // Init / Exit
//-----------------------------------------------------------------------------


    /// @brief  initializes this EventListener
    void Init()
    {
        Events()->AddEventListener< EventType >( this );
    }

    /// @brief  exits this EventListener
    void Exit()
    {
        Events()->RemoveEventListener< EventType >( this );
    }


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------
    

    /// @brief  processes the specified event
    /// @param  event   the event to process
    void ProcessEvent( EventType const& event )
    {
        if ( m_FilterFunction( event ) )
        {
            m_ResponseFunction( event );
        }
    }


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  sets the function that this EventListener responds to events with
    /// @param  response    the function that this EventListener should respond to events with
    void SetResponseFunction( std::function< void ( EventType const& ) > response )
    {
        m_ResponseFunction = response;
    }

    /// @brief  sets the function that this EventListener filters events with
    /// @param  filter  the function that this EventListener should filter events with
    void SetFilterFunction( std::function< bool ( EventType const& ) > filter )
    {
        m_FilterFunction = filter;
    }


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  function that responds to an event
    /// @param  event   the event to respond to
    std::function< void ( EventType const& ) > m_ResponseFunction = []( EventType const& event ) {};


    /// @brief  function that filters which events of the specified type this EventListener should respond to
    /// @param  event   the event to check whether to respond to
    /// @return whether to respond to the event
    std::function< bool ( EventType const& ) > m_FilterFunction = []( EventType const& event ) -> bool { return true; };


//-----------------------------------------------------------------------------
};
