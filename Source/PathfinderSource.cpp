/// @file       PathfinderSource.cpp
/// @author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief      Defines parent entity as the map for pathfinding.
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology

#include "pch.h" // precompiled header has to be included first
#include "PathfinderSource.h"
#include "PathfindSystem.h"



/// @brief   Default constructor
PathfinderSource::PathfinderSource() : Component(typeid(PathfinderSource)) {}

/// @brief   Copy ctor
PathfinderSource::PathfinderSource(PathfinderSource const& other) : 
    Component(typeid(PathfinderSource)),
    m_Walkables(other.m_Walkables) 
{}

/// @brief   Clones this component
PathfinderSource* PathfinderSource::Clone() const { return new PathfinderSource(*this); }


/// @brief   Sets this component's parent entity as the active tilemap for pathfinding.
void PathfinderSource::SetActive() const 
{ 
    Pathfinder()->SetActiveTilemap(GetEntity());
    Pathfinder()->SetWalkables(m_Walkables);
}


//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Initialization: Sets this component's parent as active tilemap
void PathfinderSource::OnInit() { SetActive(); }


/// @brief  Tweak properties in debug window
void PathfinderSource::Inspector()
{
    if ( ImGui::Button("RELOAD / ACTIVATE") )
        SetActive();
    
    inspectWalkables();
}


/// @brief  inspector for choosing which tiles are walkable
void PathfinderSource::inspectWalkables()
{
    if (ImGui::BeginListBox("walkable tiles", ImVec2(82.0f, (float)m_Walkables.size() * 24.0f + 26.0f)) == false)
    {
        return;
    }

    for (int i = 0; i < m_Walkables.size(); ++i)
    {
        ImGui::PushID(i);

        ImGui::PushItemWidth(50);
        ImGui::DragInt("", &m_Walkables[i], 0.05f, -1, INT_MAX);

        ImGui::SameLine();

        if (ImGui::Button("X", ImVec2(20, 20)))
        {
            m_Walkables.erase(m_Walkables.begin() + i);
        }

        ImGui::PopID();
    }

    if (ImGui::Button("+", ImVec2(50, 20)))
    {
        m_Walkables.push_back(0);
    }

    ImGui::EndListBox();
}




//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief       Reads in the radius of the Pathfinder
/// @param data  json data
void PathfinderSource::readWalkables(nlohmann::ordered_json const& data)
{
    m_Walkables.clear();

    for (int i = 0; i < data.size(); ++i)
    {
        int x = Stream::Read<int>( data[i] );
        m_Walkables.push_back(x);
    }
}

/// @brief  map of read methods
ReadMethodMap< PathfinderSource > const PathfinderSource::s_ReadMethods = {
    { "Walkables",   &readWalkables   }
};


/// @brief    Writes component data to a JSON file.
/// @return   JSON data to write
nlohmann::ordered_json PathfinderSource::Write() const
{
    nlohmann::ordered_json data;

    data["Walkables"] = m_Walkables;

    return data;
}
