/// @file       Pool.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Used for Pools like Time or Health 
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology
#pragma once


#define POOL_H
#include "ISerializable.h"
#include "DebugSystem.h"


template <typename Value>
class Pool : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

	///@brief constructor
	Pool(Value value = 0);

	Pool(const Pool& other);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------
 	
 	/// ///@brief get the current value
	///@return the current value
	__inline Value const& GetCurrent() const;

	///@brief get the default value
	///@return the default value
	__inline Value const& GetMaximum() const;

	/// @brief set the current value
	/// @param value - the new current value 
	__inline void SetCurrent(Value value);

	/// @brief Changes the base value
	/// @param value - the new base value
	__inline void SetMaximum(Value value);
	
	__inline void DecreasePoolTime(Value value);
	

	/// @brief Resets the pool to its default value
	__inline void Reset() { m_CurrentValue = m_MaximumValue; }

//-----------------------------------------------------------------------------
public: // virtual methods
//-----------------------------------------------------------------------------
	/// @brief Used by the Debug System to display information about this Component
	bool Inspect();

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
	
	/// @brief the current value of the pool
	Value m_CurrentValue;

	/// @brief the default value of the pool
	Value m_MaximumValue;

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

	/// @brief  map of the read methods
	static ReadMethodMap< Pool > const s_ReadMethods;

	/// @brief Reads the Base Value from the json data
	/// @param data - the json data to read from
	void readMaximumValue(nlohmann::ordered_json const& data);

	/// @brief Reads the Current Value from the json data
	/// @param data - the json data to read from
	void readCurrentValue(nlohmann::ordered_json const& data);

	/// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // Operators
//-----------------------------------------------------------------------------

	/// @brief - adds two pools together
	/// @param lhs - the left hand side of the addition
	/// @param rhs - the right hand side of the addition
	/// @return - the new pool value
	friend Pool operator+(const Pool& lhs, const Pool& rhs);

	/// @brief - subtracts two pools together
	/// @param lhs - the left hand side of the subtraction
	/// @param rhs - the right hand side of the subtraction
	/// @return - the new pool value
	friend Pool operator-(const Pool& lhs, const Pool& rhs);

	/// @brief - multiplies two pools together
	/// @param lhs - the left hand side of the multiplication
	/// @param rhs - the right hand side of the multiplication
	/// @return - the new pool value
	friend Pool operator*(const Pool& lhs, const Pool& rhs);

	/// @brief - divides two pools together
	/// @param lhs - the left hand side of the division
	/// @param rhs - the right hand side of the division
	/// @return - the new pool value
	friend Pool operator/(const Pool& lhs, const Pool& rhs);

	/// @brief - compares two pools for equality
	/// @param lhs -  the left hand side of the comparison
	/// @param rhs - the right hand side of the comparison
	/// @return - true if the pools are equal
	friend bool operator==(const Pool& lhs, const Pool& rhs);

	/// @brief - compares two pools for inequality
	/// @param lhs - the left hand side of the comparison
	/// @param rhs - the right hand side of the comparison
	/// @return - true if the pools are not equal
	friend bool operator!=(const Pool& lhs, const Pool& rhs);

	/// @brief - adds a value to the pool
	/// @param value - the value to add
	/// @return - the new pool value
	Pool& operator+=(const Value& value);

	/// @brief - subtracts a value from the pool
	/// @param value - the value to subtract
	/// @return - the new pool value
	Pool& operator-=(const Value& value);

	/// @brief - multiplies the pool by a value
	/// @param value - the value to multiply by
	/// @return - the new pool value
	Pool& operator*=(const Value& value);

	/// @brief - divides the pool by a value
	/// @param value - the value to divide by
	/// @return - the new pool value
	Pool& operator/=(const Value& value);

	/// @brief - compares two pools
	/// @param lhs - the left hand side of the comparison
	/// @param rhs - the right hand side of the comparison
	/// @return - true if the left hand side is greater than the right hand side
	friend bool operator>(const Pool& lhs, const Pool& rhs);

	/// @brief - compares two pools
	/// @param lhs - the left hand side of the comparison
	/// @param rhs - the right hand side of the comparison
	/// @return - true if the left hand side is less than the right hand side
	friend bool operator<(const Pool& lhs, const Pool& rhs);

	/// @brief - compares two pools
	/// @param lhs - the left hand side of the comparison
	/// @param rhs - the right hand side of the comparison
	/// @return - true if the left hand side is greater than or equal to the right hand side
	friend bool operator>=(const Pool& lhs, const Pool& rhs);

	/// @brief - compares two pools
	/// @param lhs - the left hand side of the comparison
	/// @param rhs - the right hand side of the comparison
	/// @return - true if the left hand side is less than or equal to the right hand side
	friend bool operator<=(const Pool& lhs, const Pool& rhs);

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------
	///@brief write the data to json
	///@return the written json data
	virtual nlohmann::ordered_json Write() const override;
};


template<>
bool Pool< int >::Inspect();

template<typename Value>
Pool<Value>::Pool(Value value) :
	m_CurrentValue(value),
	m_MaximumValue(value)
{
}

template<typename Value>
Pool<Value>::Pool(const Pool& other) :
	m_CurrentValue(other.m_CurrentValue),
	m_MaximumValue(other.m_MaximumValue)
{
}

template<typename Value>
Value const& Pool<Value>::GetCurrent() const
{
	return m_CurrentValue;
}

template<typename Value>
Value const& Pool<Value>::GetMaximum() const
{
	return m_MaximumValue;
}

template<typename Value>
void Pool<Value>::SetCurrent(Value value)
{
	m_CurrentValue = value;
}

template<typename Value>
void Pool<Value>::SetMaximum(Value value)
{
	m_MaximumValue = value;
}

template<typename Value>
void Pool<Value>::DecreasePoolTime(Value value)
{
	m_CurrentValue -= value;

	if (m_CurrentValue <= 0)
	{
		m_CurrentValue = 0;
	}
}

template<typename Value>
bool Pool<Value>::Inspect()
{
	bool valueChanged = false;
	float currentValue = static_cast<float>(m_CurrentValue);
	if (ImGui::DragFloat("Current Value", &currentValue))
	{
		m_CurrentValue = static_cast<Value>(currentValue);
		valueChanged = true;
	}

	float maximumValue = static_cast<float>(m_MaximumValue);
	if (ImGui::DragFloat("Maximum Value", &maximumValue))
	{
		m_MaximumValue = static_cast<Value>(maximumValue);
		valueChanged = true;
	}

	if (ImGui::Button("Reset"))
	{
		Reset();
	}

	return valueChanged;
}

/// @brief  Reads the Current Value from the json data
/// @tparam Value   the type of Value in this Pool
/// @param  data    the json data to read from
template<typename Value>
void Pool<Value>::readCurrentValue(nlohmann::ordered_json const& data)
{
	Stream::Read(m_CurrentValue, data);
}

/// @brief  Reads the Maaximum Value from the json data
/// @tparam Value   the type of Value in this Pool
/// @param  data    the json data to read from
template<typename Value>
void Pool<Value>::readMaximumValue(nlohmann::ordered_json const& data)
{
	Stream::Read(m_MaximumValue, data);
}

template<typename Value>
ReadMethodMap< Pool < Value > > const Pool<Value>::s_ReadMethods = {
	{ "BaseValue"   , &Pool<Value>::readMaximumValue },
	{ "CurrentValue", &Pool<Value>::readCurrentValue }
};

template<typename Value>
nlohmann::ordered_json Pool<Value>::Write() const
{
	nlohmann::ordered_json data;

	data["BaseValue"] = Stream::Write(m_MaximumValue);
	data["CurrentValue"] = Stream::Write(m_CurrentValue);

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
