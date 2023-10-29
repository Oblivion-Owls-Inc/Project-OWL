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
#include <fstream>

#include "PlatformSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"
#include "RenderSystem.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "InputSystem.h"
#include "EntitySystem.h"
#include "CameraSystem.h"

#include "AssetLibrarySystem.h"
#include "Sound.h"
#include "Texture.h"

#include "BehaviorSystem.h"
#include "EnemyBehavior.h"
#include "RigidBody.h"
#include "MovementAI.h"
#include "PlayerController.h"
#include "TurretBehavior.h"
#include "BulletBehavior.h"
#include "EffectAnimator.h"
#include "WavesBehavior.h"
#include "Animation.h"
#include "Tilemap.h"

#include "SandboxSystem.h"
#include "ParticleSystem.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  Starts running the engine.
    void Engine::Run()
    {

        load();

        init();

        while (
            // PlatformSystem::GetInstance()->WindowClosing() == false
            m_ShouldExit == false &&
            Input()->GetKeyTriggered(GLFW_KEY_ESCAPE) == false
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
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  Loads the engine config from "Data/EngineConfig.json"
    void Engine::load()
    {
        nlohmann::ordered_json json = Stream::ReadFromFile( "Data/EngineConfig.json" );

        try
        {
            Engine* self = this; // convert from rvalue into lvalue
            Stream::Read( self, json );
        }
        catch ( std::runtime_error error )
        {
            std::cerr << error.what();
        }
    }

    /// @brief  Initializes the engine and all Systems in the Engine
    void Engine::init()
    {
        Debug() << "Starting up..." << std::endl;
        // initialize the time values
        m_PreviousTime = glfwGetTime();
        m_PreviousFixedTime =  m_PreviousTime;

        // initialize all the systems
        for (System * system :  m_Systems)
        {
            system->OnInit();
            Debug() << "Starting " << system->GetName() << std::endl << std::endl;
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

            if ( currentTime - m_PreviousFixedTime <= -m_FixedFrameDuration )
            {
                m_PreviousFixedTime = currentTime;
            }
            else
            {
                m_PreviousFixedTime += m_FixedFrameDuration;
            }
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
            if ( system->GetEnabled() )
            {
                system->OnUpdate( dt );
            }
        }
    }

    /// @brief  Calls all Systems' OnFixedUpdate function
    void Engine::fixedUpdateSystems()
    {
        for ( System * system : m_Systems )
        {
            if ( system->GetEnabled() )
            {
                system->OnFixedUpdate();
            }
        }
    }

    /// @brief  Calls all Systems' OnExit function
    void Engine::exit()
    {
        Debug() << std::endl << std::endl << "Exiting..."
            << std::endl << std::endl;
        for ( System * system : m_Systems )
        {
            system->OnExit();

            Debug() << "Exiting " << system->GetName() 
                << std::endl << std::endl;
        }

        Debug() << "\nShutdown complete." << std::endl;

    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the fixedFrameDuration
    /// @param  data    the stream to read the data from
    void Engine::readFixedFrameDuration( nlohmann::ordered_json const& data )
    {
        m_FixedFrameDuration = Stream::Read<float>( data );
    }

    /// @brief  reads the systems
    /// @param  data    the stream to read the data from
    void Engine::readSystems( nlohmann::ordered_json const& data )
    {
        for ( auto& [ key, value ] : data.items() )
        {
            // get the appropriate System creation method
            auto addSystemMethod = s_AddSystemMethods.find( key );

            // throw an error if token not found
            if ( addSystemMethod == s_AddSystemMethods.end() )
            {
                throw std::runtime_error(
                    (
                        std::stringstream() <<
                        "unrecognized token \"" <<
                        key <<
                        "\" encountered while reading Systems in Engine"
                    ).str()
                );
            }
           
            System* system = ( this->*addSystemMethod->second )(); // create and add the System to the Engine
            Stream::Read( system, value ); // have the System load itself
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

        { "PlatformSystem",                        &addSystem< PlatformSystem >                           },
	    { "CollisionSystem",                       &addSystem< CollisionSystem >                          },
        { "CameraSystem",                          &addSystem< CameraSystem >                             },  
        { "InputSystem",                           &addSystem< InputSystem >                              },
        { "SceneSystem",                           &addSystem< SceneSystem >                              },
        { "RenderSystem",                          &addSystem< RenderSystem >                             },
        { "DebugSystem",                           &addSystem< DebugSystem >                              },
        { "AudioSystem",                           &addSystem< AudioSystem >                              },
        { "EntitySystem",                          &addSystem< EntitySystem >                             },
                                                                                                          
        { "BehaviorSystem<RigidBody>",             &addSystem< BehaviorSystem< RigidBody > >              },
        { "BehaviorSystem<Behavior>",              &addSystem< BehaviorSystem< Behavior > >               },
        { "BehaviorSystem<BulletBehavior>",        &addSystem< BehaviorSystem< BulletBehavior > >         },
        { "BehaviorSystem<Animation>",             &addSystem< BehaviorSystem< Animation > >              },
        { "BehaviorSystem<EffectAnimator>",        &addSystem< BehaviorSystem< EffectAnimator > >         },
        { "BehaviorSystem<Tilemap<int>>",          &addSystem< BehaviorSystem< Tilemap<int> > >           },
        { "BehaviorSystem<TilemapBase>",           &addSystem< BehaviorSystem< TilemapBase > >            },
        { "BehaviorSystem<WavesBehavior>",         &addSystem< BehaviorSystem< WavesBehavior > >          },
                                                                                                          
        { "AssetLibrarySystem<Entity>",            &addSystem< AssetLibrarySystem< Entity > >             },
        { "AssetLibrarySystem<Sound>",             &addSystem< AssetLibrarySystem< Sound > >              },
        { "AssetLibrarySystem<Texture>",           &addSystem< AssetLibrarySystem< Texture > >            },
        { "AssetLibrarySystem<TransformAnimation>",&addSystem< AssetLibrarySystem< TransformAnimation > > },
        { "AssetLibrarySystem<AnimationAsset>",    &addSystem< AssetLibrarySystem< AnimationAsset > >     },

        { "SandboxSystem",                         &addSystem< SandboxSystem >                            },
        { "ParticleSystem",                        &addSystem< ParticleSystem >                           }
       // { "BehaviorSystem<PlayerController>",   &addSystem< BehaviorSystem< PlayerController > >    },
       //{ "BehaviorSystem<MovementAI>",         &addSystem< BehaviorSystem< MovementAI > >           },
       //{ "BehaviorSystem<TurretBehavior>",     &addSystem< BehaviorSystem< TurretBehavior > >       },
       // { "BehaviorSystem<EnemyBehavior>",      &addSystem< BehaviorSystem< EnemyBehavior> >        },

    };

    /// @brief  writes the Engine config to json
    /// @return the written json data
    nlohmann::ordered_json Engine::Write() const
    {
        nlohmann::ordered_json json;

        json[ "FixedFrameDuration" ] = m_FixedFrameDuration;

        nlohmann::ordered_json& systems = json[ "Systems" ];
        for ( System* system : m_Systems )
        {
            systems[ system->GetName() ] = system->Write();
        }

        return json;
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
