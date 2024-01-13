#include "ScriptingSystem.h"
#include "DebugSystem.h"
#include "EntitySystem.h"
#include "BehaviorSystem.h"
#include "basics.h"
#include <filesystem>

///=============================================================================
/// Statics
///=============================================================================
ScriptingSystem* ScriptingSystem::s_Instance = nullptr;


/// @brief A custom print function for lua
/// @param message - Captured message from lua
static void CustomPrint(const std::string& message)
{
    Debug() << "[LUA]: " << message << std::endl;
}


///-----------------------------------------------------------------------------
/// Virtual methods
///-----------------------------------------------------------------------------

void ScriptingSystem::OnInit()
{
    m_LuaInstance = new sol::state();
    m_LuaInstance->open_libraries(
        sol::lib::base,	     /// for Base Lua functions like print(), 
        sol::lib::package,	/// for require()
        sol::lib::math,    /// for math functions like sin(),
        sol::lib::string, /// for string functions like len(),
        sol::lib::table, /// for table functions like insert(),
        sol::lib::debug /// for debug functions like traceback()
    );

}

void ScriptingSystem::OnExit()
{
    delete m_LuaInstance;
}

void ScriptingSystem::OnSceneInit()
{
    /// Loads The Main Script
    LoadScripts();
    AddFunctionsToLua();
    m_LuaInstance->set_function("print", &CustomPrint);
    for (auto& script : m_Scripts)
    {    
		script->OnInit();
    }
}

void ScriptingSystem::OnSceneExit()
{
    for (auto& script : m_Scripts)
    {
		script->OnExit();
	}

    m_Scripts.clear();
}

void ScriptingSystem::OnFixedUpdate()
{
    for (auto& script : m_Scripts)
    {
        script->OnFixedUpdate();
    }
}

void ScriptingSystem::OnUpdate(float dt)
{
}


void ScriptingSystem::LoadScripts()
{
    std::string folderPath = "LuaScripts/";
    /// I HATE THE LUA GARBAGE COLLECTOR
    /// ITS NICE IN SOME ASPECTS BUT IT MAKES IT SO HARD TO DO ANYTHING
    ///Lua Garbage Collection destroys the Entity before the script can be removed

    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.path().extension() == ".lua")
        {
            LoadScript(entry.path().string());
        }

        std::string filename = entry.path().stem().string();

        Debug() << "Loaded script: " << filename << std::endl;

        sol::table mainTable = (*m_LuaInstance)[filename];

        sol::function Init_function = LoadInitFunction(mainTable);
        if (Init_function == sol::lua_nil)
        {
            return;
        }

        sol::function Update_function = LoadUpdateFunction(mainTable);
        if (Update_function == sol::lua_nil)
        {
            return;
        }

        sol::function Exit_function = LoadExitFunction(mainTable);
        if (Exit_function == sol::lua_nil)
        {
            return;
        }

        Entity* entity = new Entity();

        entity->SetName(filename);

        Script* script = new Script(entry.path().filename().string(), Init_function, Update_function, Exit_function);

        entity->AddComponent(script);

        AddScript(script);
        Entities()->AddEntity(entity);
    }
}

void ScriptingSystem::LoadBehaviorsToLua()
{

}

sol::function ScriptingSystem::LoadInitFunction(sol::table MainTable)
{
    sol::optional<sol::table> InitExist = MainTable[1]; /// Looks for the first table in the main table

    /// Checks if the scripts table was not found and returns if so
    if (InitExist == sol::nullopt)
    {
        Debug() << "Error loading main.lua: Init table not found" << std::endl;
        return sol::lua_nil;
    }

    sol::table Init_table = MainTable[1];
    sol::function Init_function = Init_table["Init"];

    return Init_function;
}

sol::function ScriptingSystem::LoadUpdateFunction(sol::table MainTable)
{
    sol::optional<sol::table> UpdateExist = MainTable[2]; /// Looks for the second table in the main table

    /// Checks if the scripts table was not found and returns if so
    if (UpdateExist == sol::nullopt)
    {
        Debug() << "Error loading main.lua: Update table not found" << std::endl;
        
        return sol::lua_nil;
    }

    sol::table Update_table = MainTable[2];
    sol::function Update_function = Update_table["Update"];

    return Update_function;
}

sol::function ScriptingSystem::LoadExitFunction(sol::table MainTable)
{
    sol::optional<sol::table> ExitExist = MainTable[3]; /// Looks for the third table in the main table

    /// Checks if the scripts table was not found and returns if so
    if (ExitExist == sol::nullopt)
    {
        Debug() << "Error loading main.lua: Exit table not found" << std::endl;
        return sol::lua_nil;
    }

    sol::table Exit_table = MainTable[3];
    sol::function Exit_function = Exit_table["Exit"];

    return Exit_function;
}

void ScriptingSystem::AddFunctionsToLua()
{

}

void ScriptingSystem::AddScript(Script* script)
{
    m_Scripts.push_back(script);
}

void ScriptingSystem::LoadScript(std::string const& scriptName)
{
    Debug() << "Loading script: " << scriptName << std::endl;
    try
    {
        m_LuaInstance->safe_script_file(scriptName);
    }
    catch (const sol::error & e)
    {
        Debug() << "Error loading script: " << scriptName << std::endl;
        Debug() << e.what() << std::endl;
    }
}

void ScriptingSystem::RemoveScript(Script* script)
{
    m_Scripts.erase(
        std::remove(m_Scripts.begin(), m_Scripts.end(), script), m_Scripts.end()
    );
}


///-----------------------------------------------------------------------------
/// Singleton
///-----------------------------------------------------------------------------

ScriptingSystem* ScriptingSystem::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new ScriptingSystem();
    }
    return s_Instance;
}
