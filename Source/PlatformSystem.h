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

    /// @brief Shuts down window / GLFW.
	virtual void OnExit() override;



    /// @brief reads the window width from JSON
    /// @param jsonValue the JSON value
    void ReadWindowWidth( rapidjson::Value const& jsonValue );

    /// @brief reads the window height from JSON
    /// @param jsonValue the JSON value
    void ReadWindowHeight( rapidjson::Value const& jsonValue );

    /// @brief the Read Methods used in this System
    static std::map< std::string, ReadMethod< PlatformSystem > > const ReadMethods;

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;



	// Unused virtuals
    virtual void OnUpdate(float dt) override {}
	virtual void OnFixedUpdate() override {}
	virtual void OnSceneLoad() override {}
	virtual void OnSceneInit() override {}
	virtual void OnSceneExit() override {}

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
