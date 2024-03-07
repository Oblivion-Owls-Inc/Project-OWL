/// @file       Pool.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief       
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#define POOL_C

#ifndef POOL_H
#include "Pool.h"
#endif // !POOL_H
#pragma once

#include "basics.h"
#include "DebugSystem.h"


/// @brief The templated inspector the pool framework
/// @return Was the value of the pool changed by the inspector
template<>
bool Pool<int>::Inspect()
{
	bool valueChanged = false;
	
	if (ImGui::DragInt("Current Value", &m_CurrentValue))
	{
		valueChanged = true;
	}

	if (ImGui::DragInt("Maximum Value", &m_MaximumValue))
	{
		valueChanged = true;
	}

	if (ImGui::Button("Reset"))
	{
		Reset();
        valueChanged = true;
	}

	return valueChanged;
}