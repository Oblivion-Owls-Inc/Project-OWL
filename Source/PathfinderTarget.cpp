/// @file       PathfinderTarget.cpp
/// @author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief      Marks parent entity as a target for the enemies.
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology


#include "pch.h" // precompiled header has to be included first
#include "PathfinderTarget.h"
#include "PathfindSystem.h"

/// @brief   Default constructor
PathfinderTarget::PathfinderTarget() : Component(typeid(PathfinderTarget)) {}

/// @brief        Copy ctor
/// @param other  component to copy
PathfinderTarget::PathfinderTarget(PathfinderTarget const& other) : 
    Component(other), 
    m_Active(other.m_Active),
    m_Priority(other.m_Priority)
{}

/// @brief   Clones this component
PathfinderTarget* PathfinderTarget::Clone() const { return new PathfinderTarget(*this); }


//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Initialization: PathfinderTarget adds itself to the PathfinderTargeting system
void PathfinderTarget::OnInit()
{
    m_ParentTransform.Init(GetEntity());
    Pathfinder()->AddComponent(this);
    Pathfinder()->AddTransformCallback(m_ParentTransform);
}

/// @brief  Exit: PathfinderTarget removes itself from PathfinderTargeting system
void PathfinderTarget::OnExit()
{
    Pathfinder()->RemoveTransformCallback(m_ParentTransform);
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
    { "Priority",   &readPriority  },
    { "Active",     &readActive    }
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
