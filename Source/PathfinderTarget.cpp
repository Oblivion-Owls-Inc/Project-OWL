/// @file     PathfinderTarget.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Marks parent entity as a target for the enemies.
#include "PathfinderTarget.h"
#include "imgui.h"
#include "PathfindSystem.h"

/// @brief   Default constructor
PathfinderTarget::PathfinderTarget() : Component(typeid(PathfinderTarget)) {}

/// @brief        Copy ctor
/// @param other  component to copy
PathfinderTarget::PathfinderTarget(PathfinderTarget const& other) : Component(other) { }

/// @brief   Destructor : removes itself from system, if not removed already
//PathfinderTarget::~PathfinderTarget() { Pathfinder()->RemoveComponent(this); }

/// @brief   Clones this component
Component* PathfinderTarget::Clone() const { return new PathfinderTarget(*this); }

// TODO: issues may arise when trying to clone a component that has active references?


//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Initialization: PathfinderTarget adds itself to the PathfinderTargeting system
void PathfinderTarget::OnInit()
{
    m_ParentTransform.Init(GetEntity());
    Pathfinder()->AddComponent(this);
}

/// @brief  Exit: PathfinderTarget removes itself from PathfinderTargeting system
void PathfinderTarget::OnExit()
{
    Pathfinder()->RemoveComponent(this);
    m_ParentTransform.Exit();
}

/// @brief  Tweak properties in debug window
void PathfinderTarget::Inspector()
{
    ImGui::Checkbox("Active", &m_Active);

    ImGui::Spacing();

    const char* names[] = {"highest", "high", "mid", "low"};
    int ip = static_cast<int>( m_Priority );
    if (ImGui::SliderInt("Priority", &ip, 0, 3, names[ip]))
        m_Priority = static_cast<Priority>( ip );
    
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief          Read the priority for this target
/// @param  stream  The json to read from.
void PathfinderTarget::readPriority(nlohmann::ordered_json const& data)
{
    m_Priority = static_cast<Priority>( Stream::Read<int>(data) );
}


/// @brief          Read the active status for this target
/// @param  stream  The json to read from.
void PathfinderTarget::readActive(nlohmann::ordered_json const& data)
{
    m_Active = Stream::Read<bool>(data);
}

/// @brief  map of read methods
ReadMethodMap< PathfinderTarget > const PathfinderTarget::s_ReadMethods = {
    { "Priority",   &readPriority  }
};


/// @brief    Writes component data to a JSON file.
/// @return   JSON data to write
nlohmann::ordered_json PathfinderTarget::Write() const
{
    nlohmann::ordered_json data;

    data["Priority"] = static_cast<int>( m_Priority );
    data["Active"] = m_Active;

    return data;
}
