#pragma once
#include "Component.h"
#include "Stream.h"
#include "ScriptingSystem.h"

class Script : public Component
{

    //-----------------------------------------------------------------------------
public: // constructor / destructor
    //-----------------------------------------------------------------------------

    Script();
    Script(std::string scriptName,
        sol::protected_function onInit,
        sol::protected_function onUpdate,
        sol::protected_function onExit);
    ~Script();

    //-----------------------------------------------------------------------------
public: // methods
    //-----------------------------------------------------------------------------


    //-----------------------------------------------------------------------------
public: // accessors
    //-----------------------------------------------------------------------------

        /// @brief Sets the name of the script
        /// @param scriptName - the new name of the script 
    void SetScriptName(std::string const& scriptName) { m_ScriptName = scriptName; }

    /// @brief Gets the name of the script
    std::string GetScriptName() const { return m_ScriptName; }

    void SetOnInit(sol::protected_function const& onInit) { m_OnInit = onInit; }

    void SetOnUpdate(sol::protected_function const& onUpdate) { m_OnUpdate = onUpdate; }

    void SetOnExit(sol::protected_function const& onExit) { m_OnExit = onExit; }

    sol::protected_function const& GetOnInit() const { return m_OnInit; }

    sol::protected_function const& GetOnUpdate() const { return m_OnUpdate; }

    sol::protected_function const& GetOnExit() const { return m_OnExit; }

    //-----------------------------------------------------------------------------
public: // virtual override methods
    //-----------------------------------------------------------------------------


        /// @brief  gets called once when entering the scene
    virtual void OnInit() override;

    /// @brief  gets called once when exiting the scene
    virtual void OnExit() override;

    /// @brief	updates animation
    /// @param	dt  the time since the last graphics frame
    virtual void OnFixedUpdate();

    /// @brief  shows the Inspector for this Script
    virtual void Inspector() override;


    //-----------------------------------------------------------------------------
private: // member variables
    //-----------------------------------------------------------------------------

    std::string m_ScriptName;
    sol::protected_function m_OnInit;
    sol::protected_function m_OnUpdate;
    sol::protected_function m_OnExit;

    //-----------------------------------------------------------------------------
private: // reading
    //-----------------------------------------------------------------------------

    void readScriptName(nlohmann::ordered_json const& data);


    /// @brief  map of read methods
    static ReadMethodMap< Script > const s_ReadMethods;

    //-----------------------------------------------------------------------------
public: // reading / writing
    //-----------------------------------------------------------------------------


        /// @brief  gets the map of read methods
        /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


    /// @brief  Write all Script component data to a JSON file.
    /// @return The JSON file containing the Script component data.
    virtual nlohmann::ordered_json Write() const override;

    //-----------------------------------------------------------------------------
private: // copying
    //-----------------------------------------------------------------------------


        /// @brief  clones this RigidBody
        /// @return the newly created clone of this RigidBody
    virtual Component* Clone() const override;

    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    Script(const Script& other);

};
