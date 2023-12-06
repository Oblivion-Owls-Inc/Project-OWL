#pragma once

#include <Sol/sol.hpp>
#include "System.h"
#include "Script.h"

class Script;

class ScriptingSystem : public System
{

    //-----------------------------------------------------------------------------
public: // virtual methods
    //-----------------------------------------------------------------------------

        /// @brief  Gets called when system initializes
    virtual void OnInit() override;

    /// @brief  Gets called when system initializes
    virtual void OnExit() override;

    /// @brief fixed update for input, must be called for input to function
    virtual void OnFixedUpdate() override;

    /// @brief update for input, must be called for input to function
    virtual void OnUpdate(float dt) override;

//-----------------------------------------------------------------------------
private: // Private methods
//-----------------------------------------------------------------------------

    void LoadMainScript();
    sol::function LoadInitFunction(sol::table MainTable);
    sol::function LoadUpdateFunction(sol::table MainTable);
    sol::function LoadExitFunction(sol::table MainTable);

//-----------------------------------------------------------------------------
public: // Public methods
//-----------------------------------------------------------------------------

    void AddScript(Script* script);
    void LoadScript(std::string const& scriptName);
    void RemoveScript(Script* script);
    sol::state* GetLuaInstance() const { return m_LuaInstance; }

//-----------------------------------------------------------------------------
private: // constructor
//-----------------------------------------------------------------------------

    ScriptingSystem() :
        System("ScriptingSystem"),
        m_LuaInstance(nullptr)
    {}

    //-----------------------------------------------------------------------------
private: // Members
    //-----------------------------------------------------------------------------

        /// @brief The singleton instance of InputSystem
    static ScriptingSystem* s_Instance;
    std::vector<Script*> m_Scripts;
    sol::state* m_LuaInstance;

    //-----------------------------------------------------------------------------
public: // Singleton
    //-----------------------------------------------------------------------------

    static ScriptingSystem* GetInstance();

    // Prevent copying
    ScriptingSystem(ScriptingSystem& other) = delete;
    void operator=(const ScriptingSystem&) = delete;
};

__inline ScriptingSystem* Lua()
{
    return ScriptingSystem::GetInstance();
}
