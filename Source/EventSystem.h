/// @file       EventSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that handles Events and EventListeners
/// @version    0.1
/// @date       2024-01-31
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

    #include "System.h"

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------

    class EventListenerBase;

    template < typename EventType >
    class EventListener;

//-----------------------------------------------------------------------------


/// @brief  System that handles Events and EventListeners
class EventSystem : public System
{   
//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  broadcasts an event to all event listeners of the specified event type
    /// @tparam EventType   the type of event to broadcast
    /// @param  event       the event to broadcast
    template < typename EventType >
    void BroadcastEvent( EventType const& event );

    
    /// @brief  adds an event listener to the event system
    /// @tparam EventType       the type of event the listener listens for
    /// @param  eventListener   the event listener to add to the event system
    template < typename EventType >
    void AddEventListener( EventListener< EventType >* eventListener );

    /// @brief  removes an event listener to the event system
    /// @tparam EventType       the type of event the listener listens for
    /// @param  eventListener   the event listener to remove from the event system
    template < typename EventType >
    void RemoveEventListener( EventListener< EventType >* eventListener );

    
//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  data structure of all active event listeners
    /// @details
    ///     Each key of the map represents a type of event
    ///     The cooresponding value at that key is all of the EventListeners that listen for that type of event
    std::map< std::type_index, std::vector< EventListenerBase* > > m_EventListeners;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of EventSystem
    /// @return the instance of the EventSystem
    static EventSystem* GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EventSystem
    EventSystem();

    // Prevent copying
    EventSystem( EventSystem const& ) = delete;
    void operator =( EventSystem const& ) = delete;


//-----------------------------------------------------------------------------
};

/// @brief  shorthand method for EventSystem::GetInstance()
/// @return the EventSystem instance
__inline EventSystem* Events()
{
    return EventSystem::GetInstance();
}
