/// @file         PlatformSystem.cpp
/// @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief        Patform System class: initializes window and graphics (GLFW and GLEW), 
///               shuts them down on exit.
///
/// @version      0.1
/// @copyright    Copyright (c) 2023 Digipen Institute of Technology

#include "PlatformSystem.h"

#include "DebugSystem.h"
#include "RenderSystem.h"
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


    /// @brief  sets whether the window is fullscreen
    /// @param  fullscreen  whether to set the window to fullscreen or windowed
    void PlatformSystem::SetFullscreen( bool fullscreen )
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

            glfwSwapInterval(m_VSync ? 1 : 0); // enable vsync

            Debug() << "Fullscreen Mode" << std::endl;

            Debug().ResetViewport();
        }
        else
        {
            Debug().ResetViewport();

            glfwSwapInterval(m_VSync ? 1 : 0); // enable vsync

            // Update m_WindowSize to the new dimensions
            m_WindowSize = glm::ivec2( savedWidth, savedHeight );

            // Restore the window's previous size and position
            glfwSetWindowMonitor( m_Window, nullptr, savedPosX, savedPosY, savedWidth, savedHeight, 0 );

            Debug() << "Windowed Mode" << std::endl;
        }

    }


    /// @brief  checks if a screen pos is within the game window
    /// @param  screenPos   the screen pos to check
    /// @return whether the position is within the window
    bool PlatformSystem::isPosWithinWindow( glm::vec2 const& screenPos ) const
    {
        return (
            screenPos.x >= 0.0f && screenPos.x < m_WindowSize.x &&
            screenPos.y >= 0.0f && screenPos.y < m_WindowSize.y
        );
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

    /// @brief   Returns the window name
    /// @return  std::string: The name of the window.
    std::string PlatformSystem::GetImguiWindowName() const
    {
        /// Build the window name with the ID appended to it
        const std::string windowName = m_WindowName + "###" + std::to_string(GetId());

        return windowName;
    }


    /// @brief  gets the position of the gameWindow within the glfw window
    /// @return the position of the game window
    glm::vec2 const& PlatformSystem::GetGameWindowPos() const
    {
        return m_WindowPos;
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
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );    // OpenGL 4.3
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true ); // enable error callback
        glfwWindowHint( GLFW_RESIZABLE, true );            // allow window resizing


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
        glfwSwapInterval(m_VSync ? 1 : 0); // enable vsync

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

        glfwSetWindowSizeCallback( m_Window, glfwWindowResizeCallback );
        glfwSetWindowCloseCallback( m_Window, glfwWindowCloseCallback );


        #ifndef NDEBUG
            Renderer()->SetDrawToBuffer(true); // enable drawing to off-screen buffer
        #endif // NDEBUG

    }

    /// @brief    Shuts down the the platform.
    void PlatformSystem::OnExit()
    {
        glfwDestroyWindow( m_Window );
        glfwTerminate();
    }


    /// @brief  called every graphics frame
    /// @param  dt  the duration of the frame in seconds
    void PlatformSystem::OnUpdate(float dt)
    {
        /// Create a DockSpace on the Main Viewport for the Debug Window
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        dockspace_flags |= ImGuiDockNodeFlags_AutoHideTabBar;
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);

        #ifndef NDEBUG

            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
            /// Start the ImGui window with the window name
            ImGui::Begin( GetImguiWindowName().c_str() );

                /// Get the window width and height
                ImVec2 imGuiSize = ImGui::GetContentRegionAvail();
                ImVec2 pos = ImGui::GetWindowPos();
                ImVec2 max = ImVec2( pos.x + imGuiSize.x, pos.y + imGuiSize.y );

                #pragma warning( push )
                #pragma warning( disable: 4312 )
                ImGui::GetWindowDrawList()->AddImage(
                    (ImTextureID)Renderer()->GetBufferTextureID(),
                    pos,
                    max,
                    ImVec2(0, 1),
                    ImVec2(1, 0)
                );
                #pragma warning( pop )

            ImGui::End();
            ImGui::PopStyleVar();

            glm::ivec2 glfwWindowPos;
            glfwGetWindowPos( m_Window, &glfwWindowPos.x, &glfwWindowPos.y );

            m_WindowPos = glm::vec2( pos.x - glfwWindowPos.x, pos.y - glfwWindowPos.y );

            glm::ivec2 windowSize = glm::ivec2( (int)imGuiSize.x, (int)imGuiSize.y );
            if ( windowSize != m_WindowSize )
            {
                m_WindowSize = windowSize;

                for ( auto& [ key, callback ] : m_OnWindowResizedCallbacks )
                {
                    callback( m_WindowSize );
                }
            }

        #endif // NDEBUG  
    }


    /// @brief Debug window for PlatformSystem
    void PlatformSystem::DebugWindow()
    {
        ImGui::Text( "Window Size: %d x %d", m_WindowSize.x, m_WindowSize.y );
        ImGui::Text( "Window Name: %s", m_WindowName.c_str() );

        if ( ImGui::Button( m_IsFullscreen ? "Fullscreen Mode" : "Windowed Mode" ) )
        {
            SetFullscreen( !m_IsFullscreen );
        }
    }

//-----------------------------------------------------------------------------
// private: callbacks
//-----------------------------------------------------------------------------


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
           // Debug() << message << std::endl;
            assert(x);  // Keep a breakpoint here: when it's triggered, drag to skip assert,
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
    void PlatformSystem::glfwWindowResizeCallback( GLFWwindow* window, int width, int height )
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
    void PlatformSystem::glfwWindowCloseCallback( GLFWwindow* window )
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

    /// @brief reads the vsync option
    /// @param stream the data to read from
    void PlatformSystem::readVSync( nlohmann::ordered_json const& data )
    {
        m_VSync = Stream::Read< bool >( data );
    }


    /// @brief map of the PlatformSystem read methods
    ReadMethodMap< PlatformSystem > const PlatformSystem::s_ReadMethods = {
        { "WindowSize", &readWindowSize },
        { "WindowName", &readWindowName },
        { "VSync",      &readVSync }
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
        json[ "VSync" ] = m_VSync;

        return json;
    }


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------


    /// @brief Constructor
    PlatformSystem::PlatformSystem() :
        System( "PlatformSystem" )
    {}



    /// @brief    (Singleton) Gets the instance of this system.
    /// @return   PlatformSystem pointer: new or existing instance of this system.
    PlatformSystem* PlatformSystem::GetInstance()
    {
        static std::unique_ptr< PlatformSystem > instance = nullptr;

        if ( instance == nullptr )
        {
            instance.reset( new PlatformSystem() );
        }

        return instance.get();
    }


//-----------------------------------------------------------------------------
