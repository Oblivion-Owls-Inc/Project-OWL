#include "Script.h"
#include "DebugSystem.h"

Script::Script() : Component(typeid(Script)), m_ScriptName("NULL"),
m_OnInit(), m_OnUpdate(), m_OnExit()
{
}

Script::Script(std::string scriptName,
	sol::protected_function onInit,
	sol::protected_function onUpdate,
	sol::protected_function onExit) :
	Component(typeid(Script))
{
	m_ScriptName = scriptName;
	m_OnInit = onInit;
	m_OnUpdate = onUpdate;
	m_OnExit = onExit;
}

Script::~Script()
{
}

void Script::OnInit()
{
	Lua()->AddScript(this);
}

void Script::OnExit()
{
	Lua()->RemoveScript(this);
}

void Script::OnFixedUpdate()
{
}

void Script::Inspector()
{
	ImGui::Text("Script Name: %s", m_ScriptName.c_str());
}

ReadMethodMap< Script > const Script::s_ReadMethods = {
	{ "ScriptName", &readScriptName }
};

void Script::readScriptName(nlohmann::ordered_json const& data)
{
	m_ScriptName = data["ScriptName"];
}

nlohmann::ordered_json Script::Write() const
{
	nlohmann::ordered_json data;
	data["ScriptName"] = m_ScriptName;
	return data;
}

Component* Script::Clone() const
{
	return new Script(*this);
}

Script::Script(const Script& other) :
	Component(other),
	m_ScriptName(other.m_ScriptName),
	m_OnInit(other.m_OnInit),
	m_OnUpdate(other.m_OnUpdate),
	m_OnExit(other.m_OnExit)
{
}
