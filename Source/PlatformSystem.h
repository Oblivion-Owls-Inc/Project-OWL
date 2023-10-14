/// @file         PlatformSystem.h
/// @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief        Patform System class: initializes window and graphics (GLFW and GLEW),
///               shuts them down on exit.
///
/// @version      0.1
/// @copyright    Copyright (c) 2023

#pragma once

#include "System.h"
#include "glm/vec2.hpp"
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
    /// @return   glm vec2: x = width, y = height.
	glm::vec2 GetWindowDimensions() const;

    /// @brief  Gets Used by the DebugSystem to display debug information
    virtual void DebugWindow() override;

private:

    // Window data - dimensions and pointer
	int windowWidth, windowHeight;
    std::string windowName;
	GLFWwindow* window;

    /// @brief Constructor
    PlatformSystem();

    /// @brief Initializes the PlatformSystem
    virtual void OnInit() override;

    /// @brief Shuts down window / GLFW.
	virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief reads the window width
    /// @param stream the data to read from
    void readWindowWidth( nlohmann::json const& data );

    /// @brief reads the window width
    /// @param stream the data to read from
    void readWindowHeight( nlohmann::json const& data );

    /// @brief map of the PlatformSystem read methods
    static ReadMethodMap< PlatformSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


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
