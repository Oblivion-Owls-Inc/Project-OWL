/// @file     Light.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Light source component. It shines.
#include "LightingSystem.h" // + Light.h
#include "imgui.h"


/// @brief   Default constructor
Light::Light() : Component(typeid(Light)) {}


/// @brief   Clones this component
Component* Light::Clone() const { return new Light(*this); }


//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Initialization: light adds itself to the lighting system
void Light::OnInit() { m_Index = Lights()->AddLightSource(this); }

/// @brief  Exit: light removes itself from lighting system
void Light::OnExit() { Lights()->RemoveLightSource(m_Index); }

/// @brief  Tweak properties in debug window
void Light::Inspector()
{
    ImGui::DragFloat("Radius", &m_Radius, 0.01f, 0.0f, 20.0f);
    ImGui::DragFloat("Strength", &m_Strength, 0.005f, 0.0f, 10.0f);
    ImGui::DragFloat2("Offset", &m_Offset.x, 0.01f);
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief       Reads in the radius of the light
/// @param data  json data
void Light::readRadius(nlohmann::ordered_json const& data)
{
    m_Radius = Stream::Read<float>(data);
}

/// @brief       Reads in the strength of the light
/// @param data  json data
void Light::readStrength(nlohmann::ordered_json const& data)
{
    m_Strength = Stream::Read<float>(data);
}

/// @brief       Reads in the position offset
/// @param data  json data
void Light::readOffset(nlohmann::ordered_json const& data)
{
    m_Offset = Stream::Read< 2, float >(data);
}

/// @brief  map of read methods
ReadMethodMap< Light > const Light::s_ReadMethods = {
    { "Radius",   &readRadius   },
    { "Strength", &readStrength },
    { "Offset",   &readOffset   }
};


/// @brief    Writes component data to a JSON file.
/// @return   JSON data to write
nlohmann::ordered_json Light::Write() const
{
    nlohmann::ordered_json data;

    data["Radius"] = m_Radius;
    data["Strength"] = m_Strength;
    
    nlohmann::ordered_json& offsetVec = data["Offset"];
    offsetVec.push_back(m_Offset.x);
    offsetVec.push_back(m_Offset.y);

    return data;
}
