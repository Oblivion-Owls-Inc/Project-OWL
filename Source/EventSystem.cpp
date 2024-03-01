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
        static std::unique_ptr< EventSystem > s_Instance = nullptr;

        if (s_Instance == nullptr )
        {
            s_Instance.reset( new EventSystem() );
        }
        return s_Instance.get();
    }

//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EventSystem
    EventSystem::EventSystem() :
        System( "EventSystem" )
    {}


//-----------------------------------------------------------------------------
