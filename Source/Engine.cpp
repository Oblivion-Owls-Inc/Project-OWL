/// @file       Engine.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Engine class
/// @version    0.1
/// @date       2023-09-05
///
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Engine.h"

#include "basics.h"
#include <rapidjson/document.h>
#include <fstream>

#include "PlatformSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"
#include "RenderSystem.h"
#include "AudioSystem.h"
#include "SandboxSystem.h"
#include "XinoScene.h"
#include "CollisionSystem.h"
#include "InputSystem.h"
#include "EntitySystem.h"
#include "PlayerController.h"

#include "BehaviorSystem.h"
#include "RigidBody.h"
#include "MovementAI.h"
#include "Animation.h"
#include "CameraSystem.h"


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  Starts running the engine.
    void Engine::Run()
    {

        load();

        init();

        while (
            Input()->GetKeyTriggered(GLFW_KEY_ESCAPE) == false &&
            PlatformSystem::GetInstance()->WindowClosing() == false
        )
        {
            update();
        }

        exit();
    }

    /// @brief  Flags the engine to close once it finishes this loop
    void Engine::Close()
    {
        m_ShouldExit = true;
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the duration of each fixed frame
    /// @return the amount of time in seconds that each fixed frame lasts
    float Engine::GetFixedFrameDuration() const
    {
        return m_FixedFrameDuration;
    }

    /// @brief  sets the duration of each fixed frame
    /// @param  fixedFrameDuration  the amount of time in seconds that each fixed frame lasts
    void Engine::SetFixedFrameDuration( float fixedFrameDuration )
    {
        m_FixedFrameDuration = fixedFrameDuration;
    }

    /// @brief  Gets the array of all Systems in the engine.
    /// @return the array of all Systems in the engine
    std::vector< System* > const& Engine::GetSystems() const
    {
        return m_Systems;
    }

//-----------------------------------------------------------------------------
// public: reading
//-----------------------------------------------------------------------------

    /// @brief  gets the map of Engine read methods
    /// @return the map of Engine read methods
    ReadMethodMap< Engine > const& Engine::GetReadMethods() const
    {
        return s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the fixedFrameDuration
    /// @param  stream  the stream to read the data from
    void Engine::readFixedFrameDuration( Stream stream )
    {
        m_FixedFrameDuration = stream.Read<float>();
    }

    /// @brief  reads the systems
    /// @param  stream  the stream to read the data from
    void Engine::readSystems( Stream stream )
    {
        for ( auto& systemData : stream.GetObject() )
        {
            // get the appropriate System creation method
            auto addSystemMethod = s_AddSystemMethods.find(
                systemData.name.GetString()
            );

            // throw an error if token not found
            if ( addSystemMethod == s_AddSystemMethods.end() )
            {
                throw std::runtime_error(
                    (
                        std::stringstream() <<
                        "unrecognized token \"" <<
                        systemData.name.GetString() <<
                        "\" encountered while reading Systems in Engine"
                    ).str()
                );
            }
           
            System* system = ( this->*addSystemMethod->second )(); // create and add the System to the Engine
            Stream( systemData.value ).Read( system ); // have the System load itself
        }
    }

    /// @brief  map containing Engine read methods
    ReadMethodMap< Engine > const Engine::s_ReadMethods = {
        { "FixedFrameDuration", &readFixedFrameDuration },
        { "Systems",            &readSystems            }
    };

    /// @brief  Adds a System to the Engine.
    /// @tparam SystemType  The type of system to add the Engine
    template < class SystemType >
    System* Engine::addSystem()
    {
        System* system = SystemType::GetInstance();
        m_Systems.push_back( system );
        return system;
    }

/// @brief contains the function for adding each System type to the Engine. Used for Loading systems from config.
std::map< std::string, System* (Engine::*)()> const Engine::s_AddSystemMethods = {
    { "PlatformSystem",             &addSystem< PlatformSystem >                },
	{ "CollisionSystem",            &addSystem< CollisionSystem >               },
    { "CameraSystem",               &addSystem< CameraSystem >                  },  
    { "InputSystem",                &addSystem< InputSystem >                   },
    { "SceneSystem",                &addSystem< SceneSystem >                   },
    { "RenderSystem",               &addSystem< RenderSystem >                  },
//#ifndef NDEBUG
    { "DebugSystem",                &addSystem< DebugSystem >                   },
//#endif // !Debug
    { "AudioSystem",                &addSystem< AudioSystem >                   },
    { "EntitySystem",               &addSystem< EntitySystem >                  },
    { "SandboxSystem",              &addSystem< SandboxSystem >                 },
    { "XinoScene",                  &addSystem< XinoScene >                     },
    { "BehaviorSystem<PlayerController>", &addSystem< BehaviorSystem< PlayerController > >  },
    { "BehaviorSystem<MovementAI>", &addSystem< BehaviorSystem< MovementAI > >  },
    { "BehaviorSystem<RigidBody>",  &addSystem< BehaviorSystem< RigidBody > >   },
    { "BehaviorSystem<Animation>",  &addSystem< BehaviorSystem< Animation > >   }
};

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  Loads the engine config from "Data/EngineConfig.json"
    void Engine::load()
    {
        rapidjson::Document document = Stream::ReadFromJSON( "Data/EngineConfig.json" );

        try
        {
            Stream( document ).Read( this );
        }
        catch ( std::runtime_error error )
        {
            std::cerr << error.what();
        }
    }

    /// @brief  Initializes the engine and all Systems in the Engine
    void Engine::init()
    {
        // initialize the time values
        m_PreviousTime = glfwGetTime();
        m_PreviousFixedTime =  m_PreviousTime;

        // initialize all the systems
        for (System * system :  m_Systems)
        {
            system->OnInit();
        }

        // TODO: move the below code out of the engine into its own systems

        // this will go in GraphicsSystem
            // Set the clear color (background color)
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }

    /// @brief  Updates the engine each frame
    void Engine::update()
    {

        double currentTime = glfwGetTime();

        updateSystems( static_cast<float>(currentTime - m_PreviousTime) );

        if (currentTime - m_PreviousFixedTime > m_FixedFrameDuration)
        {
            fixedUpdateSystems();
            m_PreviousFixedTime += m_FixedFrameDuration;
        }
    
        m_PreviousTime = currentTime;


        // TODO: move the below code out of Engine and into its own Systems
        
        // this goes to GraphicsSystem
            
            GLFWwindow* window = PlatformSystem::GetInstance()->GetWindowHandle();

            // ensure viewport size matches window size
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            // Swap front and back buffers
            glfwSwapBuffers(window);
            glClear(GL_COLOR_BUFFER_BIT);

        // this goes to InputSystem
            
            // Poll for and process events
            glfwPollEvents();

        // TODO: move the above code out of Engine and into its own System
    }

    /// @brief  Calls all Systems' OnUpdate function
    /// @param  dt  the amount of time the frame lasted
    void Engine::updateSystems( float dt )
    {
        for ( System * system : m_Systems )
        {
            system->OnUpdate(dt);
        }
    }

    /// @brief  Calls all Systems' OnFixedUpdate function
    void Engine::fixedUpdateSystems()
    {
        for ( System * system : m_Systems )
        {
            system->OnFixedUpdate();
        }
    }

    /// @brief  Calls all Systems' OnExit function
    void Engine::exit()
    {
        for ( System * system : m_Systems )
        {
            system->OnExit();
        }
    }


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  The singleton instance of the Engine
    Engine * Engine::s_Instance;

    /// @brief  Constructs a new Engine
    Engine::Engine() :
        m_ShouldExit( false ),
        m_FixedFrameDuration( 1.0f / 60.0f ),
        m_PreviousFixedTime( 0.0 ),
        m_PreviousTime( 0.0 )
    {}

    /// @brief  gets the singleton instance of the Engine
    /// @return the singleton instance of the Engine
    Engine* Engine::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new Engine();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
