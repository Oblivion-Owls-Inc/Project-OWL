/// @file     Light.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    
#include "LightingSystem.h" // +Light.h


/// @brief   Default constructor
Light::Light() : Component(typeid(Light)) {}


/// @brief   Clones this component
Component* Light::Clone() const { return new Light(*this); }


void Light::OnInit() { m_Index = Lights()->AddLightSource(this); }

void Light::OnExit() { Lights()->RemoveLightSource(m_Index); }


/// @brief  map of read methods
ReadMethodMap< Light > const Light::s_ReadMethods = {
    
};