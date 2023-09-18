/// @file DebugSystem.h
/// @brief Debug System Class For Debugging

#pragma once
#include "basics.h"
#include "GUI.h"
#include "System.h"
#include <vector>
#include <iostream>
#include <sstream>

/// @class DebugSystem
/// @brief Debug System Class For Debugging
/// @details This class provides functionality for debugging within your application.
class DebugSystem : public System
{
public: // methods

    /// @brief Show the Frames Per Second (FPS) in a debug window
    /// @details This function displays the Frames Per Second (FPS) in a debug window when called.
    void ToggleFPS();

    void ToggleDev();

    /// @brief Print a formatted message to the screen
    /// @param format The format string, similar to printf
    /// @details This function allows you to print a formatted message to the screen using ImGui.
    void ScreenPrint(const char* format, ...);

    void ShowDebugMenu();

public: // accessors

    /// @brief Set the GLFW window handle
    /// @param window The GLFW window handle to set
    __inline void SetWindowHandle( GLFWwindow* window ) { _window = window; }

    /// @brief gets the log buffer of the DebugSystem
    /// @return the log buffer of the DebugSystem
    __inline ImGuiTextBuffer& getLogBuffer() { return logBuffer; }

private: // virtual override methods

    /// @brief Initialize the DebugSystem
    void OnInit() override;

    /// @brief Update the DebugSystem
    /// @param dt The time elapsed since the last update
    void OnUpdate(float dt) override;

    /// @brief Exit and clean up the DebugSystem
    void OnExit() override;

    void OnFixedUpdate() override;

private: // member variables

    ImGuiTextBuffer logBuffer; /// @brief the TextBuffer for ImGui console
    std::vector<GUI*> windows; /// @brief A collection of GUI windows
    GLFWwindow* _window; /// @brief The GLFW window handle
    bool showFpsWindow; /// @brief Flag to control FPS display
    bool showDevWindow; /// @brief Flag to control dev display
    ImGuiIO* io; /// @brief Pointer to the ImGui Input/Output structure

private: // class-specific Read Methods

    void ReadShowFPSWindow( rapidjson::Value const& jsonValue );

private: // unused virtual overrides

    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}

    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}

    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override {}

private: // default Read method stuff

    /// @brief the Read Methods used in this System
    static std::map< std::string, ReadMethod< DebugSystem > > const ReadMethods;

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;

private: // singleton stuff

    /// @brief Constructs the DebugSystem
    DebugSystem();

    /// @brief The singleton instance of DebugSystem
    static DebugSystem * instance;

public: // singleton stuff

    /// @brief gets the instance of DebugSystem
    /// @return the instance of the DebugSystem
    static DebugSystem * getInstance();

    // Prevent copying
    DebugSystem(DebugSystem& other) = delete;
    void operator=(const DebugSystem&) = delete;

};

/// @brief shorthand method for DebugSystem::getInstance()
/// @return the DebugSystem instance
__inline DebugSystem* Debug() { return DebugSystem::getInstance(); }

class DebugConsole
{
    public:
        DebugConsole(DebugSystem& system) : _system(system) {}

        template <typename T>
        DebugConsole& operator<<(const T& value)
        {
            _messageStream << value;
            return *this;
        }

        ~DebugConsole()
        {
            _system.ScreenPrint(_messageStream.str().c_str());
        }

    private:
        std::stringstream _messageStream;
        DebugSystem& _system;
};
