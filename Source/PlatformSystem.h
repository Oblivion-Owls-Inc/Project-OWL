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

private:

    // Window data - dimensions and pointer
	int windowWidth, windowHeight;
    std::string windowName;
	GLFWwindow* window;

    /// @brief Constructor
    PlatformSystem();

    /// @brief Initializes the PlatformSystem
    virtual void OnInit() override;

    /// @brief    Shuts down window / GLFW.
	virtual void OnExit() override;

	// Unused virtuals
    virtual void OnUpdate(float dt) override {}
	virtual void OnFixedUpdate() override {}
	virtual void OnSceneLoad() override {}
	virtual void OnSceneInit() override {}
	virtual void OnSceneExit() override {}
    virtual void Load( rapidjson::Value const& ) {}

    /// @brief The singleton instance of the PlatformSystem
    static PlatformSystem * instance;

public:

    /// @brief       gets the PlatformSystem instance, creating one if it doesn't exist
    /// @return      a pointer to the PlatformSystem instance
    static PlatformSystem * getInstance();
    
    // Prevent copying
    PlatformSystem(PlatformSystem& other) = delete;
    void operator=(const PlatformSystem&) = delete;
};
