/**
 * @file GUI.h
 * @author Jax Clayton (jax.clayton@digipen.edu)
 * @brief Base class for user interface elements.
 * @version 0.1
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glfw/glfw3.h"

 /**
  * @class GUI
  * @brief Base class for user interface elements.
  */
class GUI
{
public:
    /**
     * @brief Default constructor for GUI.
     */
    GUI();

    /**
     * @brief Constructor for GUI with window title and initial visibility.
     * @param windowTitle The title of the GUI window.
     * @param initialVisibility The initial visibility state (default is true).
     */
    GUI(const char* windowTitle, bool initialVisibility = true);

    /**
     * @brief Virtual destructor for GUI to support proper inheritance.
     */
    virtual ~GUI();

    /**
     * @brief Render function for the GUI element.
     * @details This function is responsible for rendering the GUI element.
     */
    virtual void Render();

    /**
     * @brief Check if the GUI element is visible.
     * @return True if the GUI element is visible, false otherwise.
     */
    virtual bool IsVisible() const;

    /**
     * @brief Set the GUI element as inactive.
     */
    virtual void setInactive();

    /**
     * @brief Set the GUI element as active.
     */
    virtual void setActive();

    /**
     * @brief Get the window title of the GUI element.
     * @return The window title as a const char pointer.
     */
    const char* GetWindowTitle() const;

private:
    bool active; /**< Flag to track the visibility state of the GUI element. */
    const char* windowTitle; /**< The title of the GUI window. */
};

/**
 * @class DebugMenu
 * @brief Debugging menu class derived from GUI.
 */
class DebugMenu : public GUI
{
public:
    /**
     * @brief Default constructor for DebugMenu.
     */
    DebugMenu();

    /**
     * @brief Destructor for DebugMenu.
     */
    ~DebugMenu();

    /**
     * @brief Set the DebugMenu as active.
     * @details Overrides the base class function to set the DebugMenu as active.
     */
    void setActive() override;

    /**
     * @brief Render function for DebugMenu.
     * @details Overrides the base class function to render the DebugMenu.
     */
    void Render() override;

private:
    bool active; /**< Flag to track the visibility state of the DebugMenu. */
    float color[5]; /**< An array representing the color Picker. */
};
