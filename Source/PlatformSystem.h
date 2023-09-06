/**
* @file         PlatformSystem.h
* @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
* @brief        Patform System class: initializes window and graphics (GLFW and GLEW),
                shuts them down on exit.
*
* @version      0.1
* @copyright    Copyright (c) 2023
*/
#pragma once
#include "System.h"
#include "glm/vec2.hpp"

struct GLFWwindow;	// forward reference for GetWindowHandle()


class PlatformSystem : public System
{
public:
    /**
    * @brief    Returns the window handle.
    * @return   GLFWwindow pointer: Current window handle.
    */
	GLFWwindow* GetWindowHandle() const;

    /**
    * @brief    Checks if the window is closing.
    * @return   bool: true if the window is closing.
    */
	bool WindowClosing() const;

    /**
    * @brief    Returns window dimensions as a vec2.
    * @return   glm vec2: x = width, y = height.
    */
	glm::vec2 GetWindowDimensions() const;



private:
    // Window data - dimensions and pointer
	int _width, _height;
	GLFWwindow* _window;

    /**
    * @brief            Constructor: initializes GLFW window and GLEW, enables error callback.
    * @param w_name     Window name.
    * @param w_width    Window width.
    * @param w_height   Window height.
    */
    PlatformSystem(const char* w_name = "Prototype", int w_width = 800, int w_height = 600);

    /**
    * @brief    Shuts down window / GLFW.
    */
	virtual void OnExit() override;


	// Unused virtuals
    virtual void OnInit() override {}
    virtual void OnUpdate(float dt) override {}
	virtual void OnFixedUpdate() override {}
	virtual void OnSceneLoad() override {}
	virtual void OnSceneInit() override {}
	virtual void OnSceneExit() override {}

public:

    /**
     * @brief       gets the PlatformSystem instance, creating one if it doesn't exist
     * @return      a pointer to the PlatformSystem instance
    */
    static PlatformSystem * getInstance();

    /**
     * @brief           gets the PlatformSystem instance, creating one if it doesn't exist
     * @param w_name    the name of the window to create
     * @param w_width   the width of the window to create
     * @param w_height  the height of the window to create
     * @return          a pointer to the PlatformSystem instance
    */
    static PlatformSystem * getInstance( const char* w_name, int w_width, int w_height );
    
    // Prevent copying
    PlatformSystem(PlatformSystem& other) = delete;
    void operator=(const PlatformSystem&) = delete;
};
