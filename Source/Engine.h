/// @file       Engine.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Engine class
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once




#include "System.h"


class Engine : public ISerializable
{

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Starts running the engine. Code will not advance past this point until the engine stops running.
    void Run();

    /// @brief  Flags the engine to close once it finishes this loop
    void Close();


    // TODO: give this method a name that reflects that it opens a window, not directly saves the Engine Config
    /// @brief Used To Create a Window to Save the Engine Config
    /// @return - true if the window is still open, false if the window is closed
    bool SaveEngineConfig();


//-----------------------------------------------------------------------------
public: // update enum
//-----------------------------------------------------------------------------


    /// @brief  enum saying which update mode the engine is currently in
    enum class UpdateMode
    {
        fixedUpdate,
        update
    };


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the current update status
    /// @return the current status
    UpdateMode GetCurrentUpdate() const;


    /// @brief  gets the duration of each fixed frame
    /// @return the amount of time in seconds that each fixed frame lasts
    float GetFixedFrameDuration() const;

    /// @brief  sets the duration of each fixed frame
    /// @param  fixedFrameDuration  the amount of time in seconds that each fixed frame lasts
    void SetFixedFrameDuration( float fixedFrameDuration );


    /// @brief  gets the current graphics frame count
    /// @return the number of graphics frames that have elapsed since the Engine started
    int GetFrameCount() const;

    /// @brief  gets the current simulation frame count
    /// @return the number of simulation frames that have elapsed since the Engine started
    int GetFixedFrameCount() const;


    /// @brief  gets whether a fixed update took place this frame
    /// @return whether a fixed update took place this frame
    bool GetFixedUpdatedThisFrame() const;


    /// @brief  gets the array of all Systems in the engine.
    /// @return the array of all Systems in the engine
    std::vector< System * > const& GetSystems() const;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the fixedFrameDuration
    /// @param  stream  the stream to read the data from
    void readFixedFrameDuration( nlohmann::ordered_json const& data );

    /// @brief  reads the systems
    /// @param  stream  the stream to read the data from
    void readSystems( nlohmann::ordered_json const& data );

    /// @brief map containing Engine read methods
    static ReadMethodMap< Engine > const s_ReadMethods;


    /// @brief  Adds a System to the Engine.
    /// @tparam SystemType  The type of system to add the Engine
    template < class SystemType >
    System* addSystem();

    /// @brief  contains the function for adding each System type to the Engine. Used for Loading systems from config.
    static std::map< std::string, System* (Engine::*)() > const s_AddSystemMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------

    /// @brief  gets the map of Engine read methods
    /// @return the map of Engine read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief  writes the Engine config to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  Container of all the Systems in the engine
    std::vector< System * > m_Systems = {};


    /// @brief  flag set to true when the engine needs to exit
    bool m_ShouldExit = false;


    /// @brief  The timestamp of the previous frame
    double m_PreviousTime = 0.0;

    /// @brief  The timestamp of the previous fixed frame
    double m_PreviousFixedTime = 0.0;


    /// @brief  The duration of each fixed frame
    float m_FixedFrameDuration = 1.0f / 60.0f;


    /// @brief  the number of graphics frames that have elapsed since the Engine started
    int m_FrameCount = 0;

    /// @brief  the number of fixed frames that have elapsed since the Engine started
    int m_FixedFrameCount = 0;


    /// @brief  whether a fixed update took place this frame
    bool m_FixedUpdatedThisFrame = false;


    /// @brief  The current update we're in
    UpdateMode m_currentMode = UpdateMode::fixedUpdate;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  Loads the engine config from "Data/EngineConfig.json"
    void load();

    /// @brief  Initializes the engine and all Systems in the Engine.
    void init();

    /// @brief  Updates the engine each frame
    void update();

    /// @brief  Calls all Systems in the Engine's Update function
    void updateSystems( float dt );

    /// @brief  Calls all Systems in the Engine's FixedUpdate function
    void fixedUpdateSystems();

    /// @brief  exits and closes the Engine
    void exit();

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs a new Engine
    Engine();



//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  gets the singleton instance of the Engine
    /// @return the singleton instance of the Engine
    static Engine * GetInstance();

    // Prevent Engine from being copied
    Engine( Engine& other ) = delete;
    void operator=( const Engine& ) = delete;

//-----------------------------------------------------------------------------
};

__inline Engine* GameEngine()
{
	return Engine::GetInstance();
}
