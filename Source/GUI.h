///*****************************************************************/
/// @file	 GUI.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   GUI class header
/// @details This class is to be used for creating GUI elements with the 
///           DebugSystem
///*****************************************************************/
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
    virtual void SetInactive();

    /**
     * @brief Set the GUI element as active.
     */
    virtual void SetActive();

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
    void SetActive() override;

    /**
     * @brief Render function for DebugMenu.
     * @details Overrides the base class function to render the DebugMenu.
     */
    void Render() override;

private:
    bool active; 
    float color[5]; /**< An array representing the color Picker. */
};
