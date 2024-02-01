/// @file       EventSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that handles Events and EventListeners
/// @version    0.1
/// @date       2024-01-31
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

    #include "EventSystem.h"

    #include "EventListener.h"

    #include <algorithm>

    #include "DebugSystem.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  broadcasts an event to all event listeners of the specified event type
    /// @tparam EventType   the type of event to broadcast
    /// @param  event       the event to broadcast
    template < typename EventType >
    void EventSystem::BroadcastEvent( EventType const& event )
    {
        auto iterator = m_EventListeners.find( typeid( event ) );
        if ( iterator == m_EventListeners.end() )
        {
            return;
        }

        for ( EventListenerBase* listener : iterator->second )
        {
            static_cast< EventListener< EventType >* >( listener )->ProcessEvent( event );
        }
    }

    
    /// @brief  adds an event listener to the event system
    /// @tparam EventType       the type of event the listener listens for
    /// @param  eventListener   the event listener to add to the event system
    template < typename EventType >
    void EventSystem::AddEventListener( EventListener< EventType >* eventListener )
    {
        // map [] operator will return existing OR create new, if necessary
        std::vector< EventListenerBase* >& listeners = m_EventListeners[ typeid( EventType ) ];
        listeners.push_back( eventListener );
    }

    /// @brief  removes an event listener to the event system
    /// @tparam EventType       the type of event the listener listens for
    /// @param  eventListener   the event listener to remove from the event system
    template < typename EventType >
    void EventSystem::RemoveEventListener( EventListener< EventType >* eventListener )
    {
        // find the appropriate listener array
        auto mapIterator = m_EventListeners.find( typeid( EventType ) );
        if ( mapIterator == m_EventListeners.end() )
        {
            Debug() << "WARNING: there are no EventListeners of type \"" << typeid( EventType ).name() << "\" to remove" << std::endl;
            return;
        }
        std::vector< EventListenerBase* >& listeners = mapIterator->second;

        // find the appropriate listener in the array
        auto vecIterator = std::find(
            listeners.begin(),
            listeners.end(),
            static_cast< EventListener< EventType >* >( eventListener )
        );
        if ( vecIterator == listeners.end() )
        {
            Debug() << "WARNING: EventListener of type \"" << typeid( EventType ).name() << "\" could not be found to remove" << std::endl;
            return;
        }

        // remove the listener
        listeners.erase( vecIterator );

        if ( listeners.empty() )
        {
            m_EventListeners.erase( mapIterator );
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of EventSystem
    /// @return the instance of the EventSystem
    EventSystem* EventSystem::GetInstance()
    {
        static EventSystem* instance = nullptr;
        if ( instance == nullptr )
        {
            instance = new EventSystem();
        }
        return instance;
    }


//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EventSystem
    EventSystem::EventSystem() :
        System( "EventSystem" )
    {}


//-----------------------------------------------------------------------------
