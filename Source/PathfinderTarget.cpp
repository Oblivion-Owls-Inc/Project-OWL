/// @file     PathfinderTarget.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    
#include "PathfinderTarget.h"
#include "imgui.h"
#include "PathfindSystem.h"

/// @brief   Default constructor
PathfinderTarget::PathfinderTarget() : Component(typeid(PathfinderTarget)) {}

/// @brief        Copy ctor
/// @param other  component to copy
PathfinderTarget::PathfinderTarget(PathfinderTarget const& other) : Component(other) { }

/// @brief   Destructor : removes itself from system, if not removed already
PathfinderTarget::~PathfinderTarget() { Pathfinder()->RemoveComponent(this); }

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
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------


/// @brief  map of read methods
ReadMethodMap< PathfinderTarget > const PathfinderTarget::s_ReadMethods = {
//    { "stuff",   &stuff  }
};


/// @brief    Writes component data to a JSON file.
/// @return   JSON data to write
nlohmann::ordered_json PathfinderTarget::Write() const
{
    nlohmann::ordered_json data;

    //data["Stuff"] = m_Stuff;
    

    return data;
}
