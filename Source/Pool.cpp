/// @file       Pool.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief       
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology
#define POOL_C

#ifndef POOL_H
#include "Pool.h"
#endif // !POOL_H
#pragma once

#include "basics.h"
#include "DebugSystem.h"

template<typename Value>
Pool<Value>::Pool(std::string name, Value value, bool active): 
	m_CurrentValue(value), 
	m_DefaultValue(value), 
	m_Active(active),
	m_Name(std::string( ( name + std::string(" Pool") ) ) )
{
}

template<typename Value>
Pool<Value>::Pool(const Pool& other): 
	m_CurrentValue(other.m_CurrentValue), 
	m_DefaultValue(other.m_DefaultValue), 
	m_Active(other.m_Active),
	m_Name(other.m_Name)
{
}

template<typename Value>
std::string const& Pool<Value>::GetName() const
{
	return m_Name;
}

template<typename Value>
Value const& Pool<Value>::GetCurrent() const
{
	return m_CurrentValue;
}

template<typename Value>
Value const& Pool<Value>::GetDefault() const
{
	return m_DefaultValue;
}

template<typename Value>
bool const& Pool<Value>::GetActive() const
{
	return m_Active;
}

template<typename Value>
void Pool<Value>::SetCurrent(Value value)
{
	m_CurrentValue = value;
}

template<typename Value>
void Pool<Value>::SetDefault(Value value)
{
	m_DefaultValue = value;
}

template<typename Value>
void Pool<Value>::DecreasePoolTime(Value value)
{
	m_CurrentValue -= value;

	if (m_CurrentValue <= 0)
	{
		m_CurrentValue = 0;
		m_Active = false;
	}
}

template<typename Value>
void Pool<Value>::SetActive(bool active)
{
	m_Active = active;
}

template<typename Value>
void Pool<Value>::SetName(std::string name)
{
	m_Name = name;
}


template<typename Value>
void Pool<Value>::Inspector()
{
	ImGui::Text("Pool Name: %s", m_Name.c_str());

	float currentValue = static_cast<float>(m_CurrentValue);
	if (ImGui::DragFloat("Current Value", &currentValue))
	{
		m_CurrentValue = static_cast<Value>(currentValue);
	}

	float defaultValue = static_cast<float>(m_DefaultValue);
	if (ImGui::DragFloat("Default Value", &defaultValue))
	{
		m_DefaultValue = static_cast<Value>(defaultValue);
	}

	if (ImGui::Checkbox("Active", &m_Active))
	{
		m_Active = !m_Active;
	}

	if (ImGui::Button("Reset"))
	{
		Reset();
	}
}

template<typename Value>
void Pool<Value>::readName(nlohmann::ordered_json const& data)
{
	m_Name = Stream::Read<std::string>(data);
}

template<typename Value>
void Pool<Value>::readBaseValue(nlohmann::ordered_json const& data)
{
	m_DefaultValue = Stream::Read<Value>(data);
}


template<typename Value>
void Pool<Value>::readCurrentValue(nlohmann::ordered_json const& data)
{
	m_CurrentValue = Stream::Read<Value>(data);
}

template<typename Value>
void Pool<Value>::readActive(nlohmann::ordered_json const& data)
{
	m_Active = Stream::Read<bool>(data);
}

template<typename Value>
ReadMethodMap< Pool < Value > > const Pool<Value>::s_ReadMethods = {
	{ "Name"         , &Pool<Value>::readName         },	
	{ "BaseValue"    , &Pool<Value>::readBaseValue    },
	{ "CurrentValue" , &Pool<Value>::readCurrentValue },
	{ "Active"       , &Pool<Value>::readActive       }
};

template<typename Value>
nlohmann::ordered_json Pool<Value>::Write() const
{
	nlohmann::ordered_json data;

	data["Name"] = m_Name;
	data["BaseValue"] = m_DefaultValue;
	data["CurrentValue"] = m_CurrentValue;
	data["Active"] = m_Active;

	return data;
}

template <typename Value>
Pool<Value> operator+(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue += rhs.m_CurrentValue;
	return result;
}

template <typename Value>
Pool<Value> operator-(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue -= rhs.m_CurrentValue;
	return result;
}

template <typename Value>
Pool<Value> operator*(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue *= rhs.m_CurrentValue;
	return result;
}

template <typename Value>
Pool<Value> operator/(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue /= rhs.m_CurrentValue;
	return result;
}

template <typename Value>
bool operator==(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return lhs.m_CurrentValue == rhs.m_CurrentValue;
}

template <typename Value>
bool operator!=(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return !(lhs == rhs);
}

template <typename Value>
Pool<Value>& Pool<Value>::operator+=(const Value& value)
{
	m_CurrentValue += value;
	if (m_CurrentValue > 0)
	{
		m_Active = true;
	}
	return *this;
}

template <typename Value>
Pool<Value>& Pool<Value>::operator-=(const Value& value)
{
	m_CurrentValue -= value;

	if (m_CurrentValue <= 0)
	{
		m_CurrentValue = 0;
		m_Active = false;
	}

	return *this;
}

template <typename Value>
Pool<Value>& Pool<Value>::operator*=(const Value& value)
{
	m_CurrentValue *= value;

	if (m_CurrentValue <= 0)
	{
		m_CurrentValue = 0;
		m_Active = false;
	}
	
	return *this;
}

template <typename Value>
Pool<Value>& Pool<Value>::operator/=(const Value& value)
{
	m_CurrentValue /= value;

	if (m_CurrentValue <= 0)
	{
		m_CurrentValue = 0;
		m_Active = false;
	}

	return *this;
}

template <typename Value>
bool operator>(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return lhs.m_CurrentValue > rhs.m_CurrentValue;
}

template <typename Value>
bool operator<(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return lhs.m_CurrentValue < rhs.m_CurrentValue;
}

template <typename Value>
bool operator>=(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return !(lhs < rhs);
}

template <typename Value>
bool operator<=(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return !(lhs > rhs);
}