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

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


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
