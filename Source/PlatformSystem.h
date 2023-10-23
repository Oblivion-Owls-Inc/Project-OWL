/// @file         PlatformSystem.h
/// @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief        Patform System class: initializes window and graphics (GLFW and GLEW),
///               shuts them down on exit.
///
/// @version      0.1
/// @copyright    Copyright (c) 2023

#pragma once

#include "System.h"

#include <glm/glm.hpp>
#include <string>

struct GLFWwindow;	// forward reference for GetWindowHandle()

class PlatformSystem : public System
{
public:

    /// @brief    Returns the window handle.
    /// @return   GLFWwindow pointer: Current window handle.
	GLFWwindow* GetWindowHandle() const;

    /// @brief    Checks if the window is closing.
    /// @return   bool: true if the window is closing.
	bool WindowClosing() const;

    /// @brief    Returns window dimensions as a vec2.
    /// @return   glm ivec2: x = width, y = height.
	glm::ivec2 GetWindowDimensions() const;

private:

    // Window data - dimensions and pointer
	glm::ivec2 m_WindowSize = { 800, 600 };
    std::string m_WindowName = "Default Window";
	GLFWwindow* m_Window = nullptr;

    /// @brief Constructor
    PlatformSystem();

    /// @brief Initializes the PlatformSystem
    virtual void OnInit() override;

    /// @brief Shuts down window / GLFW.
	virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief reads the window size
    /// @param stream the data to read from
    void readWindowSize( nlohmann::ordered_json const& data );

    /// @brief reads the window name
    /// @param stream the data to read from
    void readWindowName( nlohmann::ordered_json const& data );

    /// @brief map of the PlatformSystem read methods
    static ReadMethodMap< PlatformSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief  writes this System config
    /// @return the writting System config
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief The singleton instance of the PlatformSystem
    static PlatformSystem * instance;

public:

    /// @brief       gets the PlatformSystem instance, creating one if it doesn't exist
    /// @return      a pointer to the PlatformSystem instance
    static PlatformSystem * GetInstance();
    
    // Prevent copying
    PlatformSystem(PlatformSystem& other) = delete;
    void operator=(const PlatformSystem&) = delete;
};

__inline PlatformSystem* Platform()
{
    return PlatformSystem::GetInstance();
}