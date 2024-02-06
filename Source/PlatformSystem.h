/// @file         PlatformSystem.h
/// @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief        Patform System class: initializes window and graphics (GLFW and GLEW),
///               shuts them down on exit.
///
/// @version      0.1
/// @copyright    Copyright (c) 2023

#pragma once

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

#include "System.h"

#include <glew.h>

#include <glm/glm.hpp>
#include <string>

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------

struct GLFWwindow;	// forward reference for GetWindowHandle()

// #define GLAPIENTRY _stdcall
// typedef unsigned int GLenum;
// typedef unsigned int GLuint;
// typedef int GLsizei;
// typedef char GLchar;

//-----------------------------------------------------------------------------

class PlatformSystem : public System
{
//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  adds a callback to get called whenever the window resizes
    /// @param  ownerId     ID of the owner of the callback (used for removing the callback later)
    /// @param  callback    the to callback to add
    void AddOnWindowResizeCallback( unsigned ownerId, std::function< void( glm::ivec2 const& size ) > callback );

    /// @brief  removes an OnWindowResize callback
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnWindowResizeCallback( unsigned ownerId );


    /// @brief  sets whether the window is fullscreen
    /// @param  fullscreen  whether to set the window to fullscreen or windowed
    void SetFullscreen( bool fullscreen );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief    Returns the window handle.
    /// @return   GLFWwindow pointer: Current window handle.
	GLFWwindow* GetWindowHandle();


    /// @brief    Returns window dimensions in pixels
    /// @return   glm ivec2: x = width, y = height.
	glm::ivec2 GetWindowDimensions() const;

    /// @brief   Returns the window name
    /// @return  std::string: The name of the window.
    std::string GetImguiWindowName() const;

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Initializes the PlatformSystem
    virtual void OnInit() override;

    /// @brief Shuts down window / GLFW.
    virtual void OnExit() override;


    /// @brief  called every graphics frame
    /// @param  dt  the duration of the frame in seconds
    virtual void OnUpdate( float dt ) override;


    /// @brief Debug window for PlatformSystem
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the size in pixels of the window
	glm::ivec2 m_WindowSize = { 800, 600 };

    /// @brief  the name of the window
    std::string m_WindowName = "Default Window";

    /// @brief  whether the window is fullscreen
    bool m_IsFullscreen = false;

    bool m_VSync = true;
    
	/// @brief  handle to the GLFW window
	GLFWwindow* m_Window = nullptr;


    /// @brief  callbacks to call whenever the window resizes
    std::map< unsigned, std::function< void ( glm::ivec2 const& ) > > m_OnWindowResizedCallbacks = {};
    

//-----------------------------------------------------------------------------
private: // callbacks
//-----------------------------------------------------------------------------


    /// @brief            (callback) Gets called when there's some OpenGL error
    /// @param  message     Error message (text)
    /// @param  severity    Severity of the error.
    /// @param  others      They don't really matter. Can be used for more advanced debugging.
    static void GLAPIENTRY openGlErrorCallback(
        GLenum source, GLenum m_Type, GLuint id, GLenum severity, 
        GLsizei length, const GLchar* message, const void* userparam
    );


    /// @brief  callback called whenever the GLFW window resizes
    /// @param  window  handle to the window that was resized
    /// @param  width   the new width of the window
    /// @param  height  the new height of the window
    static void glfwWindowResizeCallback( GLFWwindow* window, int width, int height );

    /// @brief  callback called whenever the GLFW window closes
    /// @param  window  the window that was just closed
    static void glfwWindowCloseCallback( GLFWwindow* window );


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief reads the window size
    /// @param stream the data to read from
    void readWindowSize( nlohmann::ordered_json const& data );

    /// @brief reads the window name
    /// @param stream the data to read from
    void readWindowName( nlohmann::ordered_json const& data );

    /// @brief reads the vsync option
    /// @param stream the data to read from
    void readVSync( nlohmann::ordered_json const& data );


    /// @brief map of the PlatformSystem read methods
    static ReadMethodMap< PlatformSystem > const s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading writing
//-----------------------------------------------------------------------------
    
    
    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


    /// @brief  writes this System config
    /// @return the writting System config
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief Constructor
    PlatformSystem();


    /// @brief The singleton instance of the PlatformSystem
    static PlatformSystem* instance;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the PlatformSystem instance, creating one if it doesn't exist
    /// @return a pointer to the PlatformSystem instance
    static PlatformSystem* GetInstance();
    

    // Prevent copying
    PlatformSystem( PlatformSystem& other ) = delete;
    void operator=( const PlatformSystem& ) = delete;


//-----------------------------------------------------------------------------
};

/// @brief  shorthand method for getting the PlatformSystem instance
/// @return the PlatformSystem instance
__inline PlatformSystem* Platform()
{
    return PlatformSystem::GetInstance();
}

//-----------------------------------------------------------------------------
