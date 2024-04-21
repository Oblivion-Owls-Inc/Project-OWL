/// \file         PlatformSystem.h
/// \author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// \brief        Patform System class: initializes window and graphics (GLFW and GLEW),
///               shuts them down on exit.
///
/// \version      0.1
/// \copyright    Copyright (c) 2024 Digipen Institute of Technology

#pragma once

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

#include "pch.h" // precompiled header has to be included first
#include "System.h"
#include "EventListener.h"


//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------

struct GLFWwindow;	// forward reference for GetWindowHandle()

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


    /// @brief  adds a callback to get called whenever the focus of this window changes
    /// @param  ownerId     ID of the owner of the callback (used for removing the callback later)
    /// @param  callback    the to callback to add
    void AddOnFocusChangedCallback( unsigned ownerId, std::function< void( bool focused ) > callback );

    /// @brief  removes an OnFocusChanged callback
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnFocusChangedCallback( unsigned ownerId );


    /// @brief  sets whether the window is fullscreen
    /// @param  fullscreen  whether to set the window to fullscreen or windowed
    void SetFullscreen( bool fullscreen );


    /// @brief  checks if a screen pos is within the game window
    /// @param  screenPos   the screen pos to check
    /// @return whether the position is within the window
    bool isPosWithinWindow( glm::vec2 const& screenPos ) const;


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


    /// @brief  gets the position of the gameWindow within the glfw window
    /// @return the position of the game window
    glm::vec2 const& GetGameWindowPos() const;

    /// @brief  gets if the game is in full screen
    /// @return is the game in fullscreen
    bool GetFullscreen() const;

    /// @brief  gets whether the game window is focused
    /// @return whether the game window is focused
    bool GetIsFocused() const;


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Initializes the PlatformSystem
    virtual void OnInit() override;

    /// @brief Shuts down window / GLFW.
    virtual void OnExit() override;

    /// @brief  Gets called whenever a scene is initialized
    virtual void OnSceneInit();

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit(); 

    /// @brief  called every graphics frame
    /// @param  dt  the duration of the frame in seconds
    virtual void OnUpdate( float dt ) override;


    /// @brief Debug window for PlatformSystem
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief Listens for the exit call
    EventListener<std::string> m_Listener;


    /// @brief  the size in pixels of the window
	glm::ivec2 m_WindowSize = { 800, 600 };

    /// @brief  the name of the window
    std::string m_WindowName = "Default Window";

    /// @brief  whether the window is fullscreen
    bool m_IsFullscreen = false;

    bool m_VSync = true;
    
	/// @brief  handle to the GLFW window
	GLFWwindow* m_Window = nullptr;

    /// @brief  the position of the game window within the GLFW window
    glm::vec2 m_WindowPos = glm::vec2( 0.0f, 0.0f );


    /// @brief  callbacks to call whenever the window resizes
    std::map< unsigned, std::function< void ( glm::ivec2 const& ) > > m_OnWindowResizedCallbacks = {};

    /// @brief  callbacks to call whenever the focus of the window changes
    std::map< unsigned, std::function< void ( bool focused ) > > m_OnFocusChangedCallbacks = {};
    

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


    /// @brief  GLFW callback called whenever the GLFW window resizes
    /// @param  window  handle to the window that was resized
    /// @param  width   the new width of the window
    /// @param  height  the new height of the window
    static void glfwWindowResizeCallback( GLFWwindow* window, int width, int height );

    /// @brief  GLFW callback called whenever the GLFW window closes
    /// @param  window  the window that was just closed
    static void glfwWindowCloseCallback( GLFWwindow* window );

    /// @brief  GLFW callback called whenever the window focus changes
    /// @param  window  handle to the window that focus was changed on
    /// @param  focused whether the window is focused
    static void glfwWindowFocusCallback( GLFWwindow* window, int focused );


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
