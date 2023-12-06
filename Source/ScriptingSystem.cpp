#include "ScriptingSystem.h"
#include "DebugSystem.h"
#include "EntitySystem.h"

///=============================================================================
/// Statics
///=============================================================================
ScriptingSystem* ScriptingSystem::s_Instance = nullptr;


///-----------------------------------------------------------------------------
/// Virtual methods
///-----------------------------------------------------------------------------

void ScriptingSystem::OnInit()
{
	///SolState is not working you have to figure someothing out
	/// 
	m_LuaInstance->open_libraries(
		sol::lib::base,	     /// for Base Lua functions like print(), 
		sol::lib::package,	/// for require()
		sol::lib::math,    /// for math functions like sin(),
		sol::lib::string, /// for string functions like len(),
		sol::lib::table, /// for table functions like insert(),
		sol::lib::debug /// for debug functions like traceback()
	);

	/// Loads The Main Script
	LoadMainScript();
}

void ScriptingSystem::OnExit()
{
	for (auto& script : m_Scripts)
	{
		if (script->GetScriptName() != "main.lua")
		{
			continue;
		}

		auto err = script->GetOnExit()(script->GetEntity());
		if (!err.valid())
		{
			sol::error e = err;
			Debug() << "Error calling Update function in " << script->GetScriptName() << std::endl;
			Debug() << e.what() << std::endl;
		}
	}

	delete m_LuaInstance;
}

void ScriptingSystem::OnFixedUpdate()
{
	for (auto& script : m_Scripts)
	{
		if (script->GetScriptName() != "main.lua")
		{
			continue;
		}

		auto err = script->GetOnUpdate()(script->GetEntity());
		if (!err.valid())
		{
			sol::error e = err;
			Debug() << "Error calling Update function in " << script->GetScriptName() << std::endl;
			Debug() << e.what() << std::endl;
		}
	}
}

void ScriptingSystem::OnUpdate(float dt)
{
}

void ScriptingSystem::LoadMainScript()
{
	LoadScript("main.lua");

	sol::table mainTable;
	sol::optional<sol::table> optionalMainTable = (*m_LuaInstance)["main"];
	if (optionalMainTable) {
		mainTable = *optionalMainTable;
	}
	else {
		Debug() << "Error: 'main' is not a table" << std::endl;
		// Handle the error accordingly
	}
	
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

	entity->SetName("Main Script");

	Script* script = new Script("main.lua", Init_function, Update_function, Exit_function);

	entity->AddComponent(script);

	EntitySystem::GetInstance()->AddEntity(entity);

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

void ScriptingSystem::AddScript(Script* script)
{
	m_Scripts.push_back(script);
}

void ScriptingSystem::LoadScript(std::string const& scriptName)
{
	std::string scriptPath = "LuaScripts/" + scriptName;
	Debug() << "Loading script: " << scriptPath << std::endl;
	try
	{
		sol::protected_function_result res = m_LuaInstance->safe_script_file(scriptName);
	}
	catch (const sol::error & e)
	{
		Debug() << "Error loading script: " << scriptPath << std::endl;
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
