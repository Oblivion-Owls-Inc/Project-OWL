/// @file         PlatformSystem.cpp
/// @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief        Patform System class: initializes window and graphics (GLFW and GLEW), 
///               shuts them down on exit.
///
/// @version      0.1
/// @copyright    Copyright (c) 2023 Digipen Institute of Technology

#include "PlatformSystem.h"

#include "DebugSystem.h"
#include "Engine.h"

// #include <glew.h>       // initialize, error callback
#include <glfw3.h>      // initialize / shutdown
#include <glm/vec2.hpp> // for returning window dimensions
#include <iostream>     // cout
#include <cassert>


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  adds a callback to get called whenever the window resizes
    /// @param  ownerId     ID of the owner of the callback (used for removing the callback later)
    /// @param  callback    the to callback to add
    void PlatformSystem::AddOnWindowResizeCallback( unsigned ownerId, std::function< void( glm::ivec2 const& size ) > callback )
    {
        m_OnWindowResizedCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes an OnWindowResize callback
    /// @param  ownerId the ID of the owner of the callback to remove
    void PlatformSystem::RemoveOnWindowResizeCallback( unsigned ownerId )
    {
        m_OnWindowResizedCallbacks.erase( ownerId );
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief    Returns the window handle.
    /// @return   GLFWwindow pointer: Current window handle.
    GLFWwindow* PlatformSystem::GetWindowHandle()
    {
        return m_Window;
    }


    /// @brief    Returns window dimensions as a vec2.
    /// @return   glm ivec2: x = width, y = height.
    glm::ivec2 PlatformSystem::GetWindowDimensions() const
    {
        return m_WindowSize;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Initializes the PlatformSystem
    void PlatformSystem::OnInit()
    {
        // GLFW
        if ( !glfwInit() )
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );      // OpenGL 4.3
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true ); // enable error callback
        glfwWindowHint( GLFW_RESIZABLE, true );            // allow window resizing

        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); TODO: needed or nah?

        // Window
        m_Window = glfwCreateWindow( m_WindowSize.x, m_WindowSize.y, m_WindowName.c_str(), NULL, NULL);

        if (!m_Window)
        {
            #ifndef NDEBUG
                glfwTerminate();
                Debug() << "Failed to create GLFW window" << std::endl;
                assert(false);
            #else
                glfwTerminate();
                Debug() << "Failed to create GLFW window" << std::endl;
                exit(EXIT_FAILURE);  // Exit 
            #endif // !NDEBUG
        }
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1); // enable vsync

        // GLEW
        if (glewInit() != GLEW_OK)
        {
            #ifndef NDEBUG
                glfwTerminate();
                glfwDestroyWindow(m_Window);
                std::cerr << "Failed to initialize GLEW" << std::endl;
                assert(false);
            #else
                glfwTerminate();
                glfwDestroyWindow(m_Window);
                Debug() << "Failed to initialize GLEW" << std::endl;
                exit(EXIT_FAILURE);  // Exit 
            #endif // !NDEBUG
        }

        Debug() << "\nRunning OpenGL " << glGetString( GL_VERSION ) << '\n' << std::endl; // display OpenGL version

        glDebugMessageCallback( openGlErrorCallback, NULL ); // set OpenGL error callback func

        glfwSetWindowSizeCallback( m_Window, OnWindowResizeCallback );
        glfwSetWindowCloseCallback( m_Window, OnWindowCloseCallback );
    }

    /// @brief    Shuts down the the platform.
    void PlatformSystem::OnExit()
    {
        glfwDestroyWindow( m_Window );
        glfwTerminate();
    }


    /// @brief Debug window for PlatformSystem
    void PlatformSystem::DebugWindow()
    {
        ImGui::Text( "Window Size: %d x %d", m_WindowSize.x, m_WindowSize.y );
        ImGui::Text( "Window Name: %s", m_WindowName.c_str() );

        if ( ImGui::Button( m_IsFullscreen ? "Fullscreen Mode" : "Windowed Mode" ) )
        {
            setFullscreen( !m_IsFullscreen );
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  sets whether the window is fullscreen
    /// @param  fullscreen  whether to set the window to fullscreen or windowed
    void PlatformSystem::setFullscreen( bool fullscreen )
    {
        static int savedWidth = m_WindowSize.x;
        static int savedHeight = m_WindowSize.y;
        static int savedPosX, savedPosY;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        GLFWvidmode const* mode = glfwGetVideoMode( monitor );

        m_IsFullscreen = fullscreen;

        if ( m_IsFullscreen == true )
        {
            // Save the current window size and position before going fullscreen
            glfwGetWindowPos( m_Window, &savedPosX, &savedPosY );
            savedWidth = m_WindowSize.x;
            savedHeight = m_WindowSize.y;

            // Set the window to fullscreen
            glfwSetWindowMonitor( m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate );

            // Update m_WindowSize to the new dimensions
            m_WindowSize = glm::ivec2( mode->width, mode->height );

            glfwSwapInterval(1); // enable vsync
        }
        else
        {

            glfwSwapInterval(1); // enable vsync

            // Update m_WindowSize to the new dimensions
            m_WindowSize = glm::ivec2( savedWidth, savedHeight );

            // Restore the window's previous size and position
            glfwSetWindowMonitor( m_Window, nullptr, savedPosX, savedPosY, savedWidth, savedHeight, 0 );
        }

    }


    /// @brief            (callback) Gets called when there's some OpenGL error
    /// @param message    Error message (text)
    /// @param severity   Severity of the error.
    /// @param others     They don't really matter. Can be used for more advanced debugging.
    void GLAPIENTRY PlatformSystem::openGlErrorCallback(
        GLenum source, GLenum m_Type, GLuint id, GLenum severity, 
        GLsizei length, const GLchar* message, const void* userparam
    )
    {

        bool x = false;
    
        #ifndef NDEBUG
        if (severity == GL_DEBUG_SEVERITY_HIGH)
        {
            Debug() << message << std::endl;
            assert(x);  // Keep a breakpoint here: when it's triggered, change the x to true to skip assert,
            // and step outside this function to see where the error ocurred.
        }
        #endif // !NDEBUG
        // unused
        (void) source;
        (void) m_Type;
        (void) id;
        (void) length;
        (void) userparam;
    }


    /// @brief  callback called whenever the GLFW window resizes
    /// @param  window  handle to the window that was resized
    /// @param  width   the new width of the window
    /// @param  height  the new height of the window
    void PlatformSystem::OnWindowResizeCallback( GLFWwindow* window, int width, int height )
    {
        PlatformSystem* self = Platform();
        self->m_WindowSize = glm::ivec2( width, height );

        for ( auto& [ key, callback ] : self->m_OnWindowResizedCallbacks )
        {
            callback( self->m_WindowSize );
        }
    }

    /// @brief  callback called whenever the GLFW window closes
    /// @param  window  the window that was just closed
    void PlatformSystem::OnWindowCloseCallback( GLFWwindow* window )
    {
        Debug() << "Window Close Callback" << std::endl;
        Engine::GetInstance()->Close();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief reads the window size
    /// @param stream the data to read from
    void PlatformSystem::readWindowSize( nlohmann::ordered_json const& data )
    {
        m_WindowSize = Stream::Read< 2, int >( data );
    }

    /// @brief reads the window name
    /// @param stream the data to read from
    void PlatformSystem::readWindowName( nlohmann::ordered_json const& data )
    {
        m_WindowName = Stream::Read< std::string >( data );
    }


    /// @brief map of the PlatformSystem read methods
    ReadMethodMap< PlatformSystem > const PlatformSystem::s_ReadMethods = {
        { "WindowSize", &readWindowSize },
        { "WindowName", &readWindowName }
    };


//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------


    /// @brief  writes this System config
    /// @return the writting System config
    nlohmann::ordered_json PlatformSystem::Write() const
    {
        nlohmann::ordered_json json;

        json[ "WindowSize" ] = Stream::Write< 2, int >( m_WindowSize );
        json[ "WindowName" ] = m_WindowName;

        return json;
    }


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------


    /// @brief Constructor
    PlatformSystem::PlatformSystem() :
        System( "PlatformSystem" )
    {}


    /// @brief The singleton instance of ExampleSystem
    PlatformSystem* PlatformSystem::instance = nullptr;


    /// @brief    (Singleton) Gets the instance of this system.
    /// @return   PlatformSystem pointer: new or existing instance of this system.
    PlatformSystem* PlatformSystem::GetInstance()
    {
        if ( instance == nullptr )
        {
            instance = new PlatformSystem();
        }
        return instance;
    }


//-----------------------------------------------------------------------------
