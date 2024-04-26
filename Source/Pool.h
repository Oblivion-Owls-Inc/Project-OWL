/// @file       Pool.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Used for Pools like Time or Health 
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
#pragma once


#define POOL_H

#include "DebugSystem.h"


/// @brief The pool framework is what we use to represent health in our game
/// @tparam Value - the type of data in the pool.
template <typename Value>
class Pool : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

	///@brief constructor
	Pool(Value value = 0);

	/// @brief Copy constructor
	/// @param other - A reference to another pool
	Pool(const Pool& other);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------
 	
 	/// @brief get the current value
	/// @return the current value
	Value const& GetCurrent() const;

	/// @brief get the default value
	/// @return the default value
	Value const& GetMaximum() const;

	/// @brief set the current value
	/// @param value - the new current value 
	void SetCurrent(Value value);

	/// @brief Changes the base value
	/// @param value - the new base value
	void SetMaximum(Value value);
	
	/// @brief Decreases the value of the pool
	/// @param value - The value to decrease by.
	void DecreasePoolTime(Value value);
	
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

/// @brief  Templated Inspector for the pool framework
/// @return Was the value of the pool changed in the inspector.
template<>
bool Pool< int >::Inspect();

/// @brief Constructor for the pool framework
/// @tparam Value - the data type of the pool.
/// @param value  - the magnitude of the data in the pool.
template<typename Value>
Pool<Value>::Pool(Value value) :
	m_CurrentValue(value),
	m_MaximumValue(value)
{
}

/// @brief Copy Constructor for the pool framework.
/// @tparam Value - the data type of the pool.
/// @param other  - a reference to another pool.
template<typename Value>
Pool<Value>::Pool(const Pool& other) :
	m_CurrentValue(other.m_CurrentValue),
	m_MaximumValue(other.m_MaximumValue)
{
}

/// @brief Get the current value of the pool.
/// @tparam Value - the type of data in the pool.
/// @return The current value of the data in the pool (Value).
template<typename Value>
Value const& Pool<Value>::GetCurrent() const { return m_CurrentValue; }

/// @brief Get the maximum value of the pool.
/// @tparam Value - the type of data in the pool.
/// @return The maximum value of the data in the pool.
template<typename Value>
Value const& Pool<Value>::GetMaximum() const { return m_MaximumValue; }

/// @brief Set the current value of the pool.
/// @tparam Value - the type of data in the pool.
/// @param value  - the value to set the current to.
template<typename Value>
void Pool<Value>::SetCurrent(Value value) { m_CurrentValue = value; }

/// @brief Set the maximum value of the pool.
/// @tparam Value - the type of data in the pool.
/// @param value  - the value to set the maximum value to.
template<typename Value>
void Pool<Value>::SetMaximum(Value value) { m_MaximumValue = value; }

template<typename Value>
void Pool<Value>::DecreasePoolTime(Value value)
{
	m_CurrentValue -= value;

	if (m_CurrentValue <= 0)
	{
		m_CurrentValue = 0;
	}
}

/// @brief The inspector for the pool framework.
/// @tparam Value - the type of data in the pool.
/// @return Was the data in the pool changed in the inspector (bool).
template<typename Value>
bool Pool<Value>::Inspect()
{
	bool valueChanged = false;
	float currentValue = static_cast<float>(m_CurrentValue);

	// Display & edit the current value in the inspector.
	if (ImGui::DragFloat("Current Value", &currentValue))
	{
		m_CurrentValue = static_cast<Value>(currentValue);
		valueChanged = true;
	}

	// Display & edit the maximum value in the inspector.
	float maximumValue = static_cast<float>(m_MaximumValue);
	if (ImGui::DragFloat("Maximum Value", &maximumValue))
	{
		m_MaximumValue = static_cast<Value>(maximumValue);
		valueChanged = true;
	}

	// Allows the reset function to be utilised in the inspector.
	if (ImGui::Button("Reset"))
	{
		Reset();
        valueChanged = true;
	}

	return valueChanged;
}

/// @brief  Reads the Current Value from the json data
/// @tparam Value - the type of Value in this Pool
/// @param  data  - the json data to read from
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


/// @brief A map of all the functions responsible for reading in data for the pool framework
/// @tparam Value - the type of data in the pool.
template<typename Value>
ReadMethodMap< Pool < Value > > const Pool<Value>::s_ReadMethods = {
	{ "BaseValue"   , &Pool<Value>::readMaximumValue },
	{ "CurrentValue", &Pool<Value>::readCurrentValue }
};

/// @brief Writes all the data of the pool to a JSON file.
/// @tparam Value - the type of data in the pool.
/// @return The JSON file it was written to.
template<typename Value>
nlohmann::ordered_json Pool<Value>::Write() const
{
	nlohmann::ordered_json data;

	// Write the current and maximum value to a JSON file.
	data["BaseValue"] = Stream::Write(m_MaximumValue);
	data["CurrentValue"] = Stream::Write(m_CurrentValue);

	return data;
}

/// @brief Addition overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the +
/// @param rhs    - the pool to the right of the +
/// @return The sum of the two pools.
template <typename Value>
Pool<Value> operator+(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue += rhs.m_CurrentValue;
	return result;
}

/// @brief Subtraction overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the -
/// @param rhs    - the pool to the right of the -
/// @return The difference between the two pools.
template <typename Value>
Pool<Value> operator-(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue -= rhs.m_CurrentValue;
	return result;
}

/// @brief Multiplicaton overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the *
/// @param rhs    - the pool to the right of the *
/// @return The product of the two pools.
template <typename Value>
Pool<Value> operator*(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue *= rhs.m_CurrentValue;
	return result;
}

/// @brief Division overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the /
/// @param rhs    - the pool to the right of the /
/// @return The quotient of the two pools.
template <typename Value>
Pool<Value> operator/(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	Pool<Value> result(lhs);
	result.m_CurrentValue /= rhs.m_CurrentValue;
	return result;
}

/// @brief Equality overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the ==
/// @param rhs    - the pool to the right of the ==
/// @return Are the values of the pools equal (bool).
template <typename Value>
bool operator==(const Pool<Value>& lhs, const Pool<Value>& rhs) { return lhs.m_CurrentValue == rhs.m_CurrentValue; }

/// @brief Inequality overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the !=
/// @param rhs    - the pool to the right of the !=
/// @return Are the values of the pools not equal (bool).
template <typename Value>
bool operator!=(const Pool<Value>& lhs, const Pool<Value>& rhs) { return !(lhs == rhs); }

/// @brief compound addition overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param value  - the value to add to the current value of the pool.
/// @return A reference to the pool
template <typename Value>
Pool<Value>& Pool<Value>::operator+=(const Value& value)
{
	m_CurrentValue += value;
	if (m_CurrentValue > 0)
	{

	}
	return *this;
}

/// @brief compound subtraction overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param value  - the value to subtract from the current value of the pool.
/// @return A reference to the pool
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

/// @brief compound multiplication overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param value  - the value to multiply with the current value of the pool.
/// @return A reference to the pool
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

/// @brief compound division overload for the pool framework
/// @tparam Value - the type of data in the pool.
/// @param value  - the value to divide the current value of the pool by.
/// @return A reference to the pool
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

/// @brief Greater than ovrload for the pool framework.
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the >.
/// @param rhs    - the pool to the right of the >.
/// @return Whether or not the left pool is greater than the right pool.
template <typename Value>
bool operator>(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return lhs.m_CurrentValue > rhs.m_CurrentValue;
}

/// @brief Lesser than ovrload for the pool framework.
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the <.
/// @param rhs    - the pool to the right of the <.
/// @return Whether or not the left pool is less than the right pool.
template <typename Value>
bool operator<(const Pool<Value>& lhs, const Pool<Value>& rhs)
{
	return lhs.m_CurrentValue < rhs.m_CurrentValue;
}

/// @brief Greater than eqal to ovrload for the pool framework.
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the >=.
/// @param rhs    - the pool to the right of the >=.
/// @return Whether or not the left pool is greater than or equal to the right pool.
template <typename Value>
bool operator>=(const Pool<Value>& lhs, const Pool<Value>& rhs) { return !(lhs < rhs); }

/// @brief Lesser than eqal to ovrload for the pool framework.
/// @tparam Value - the type of data in the pool.
/// @param lhs    - the pool to the left of the <=.
/// @param rhs    - the pool to the right of the <=.
/// @return Whether or not the left pool is lesser than or equal to the right pool.
template <typename Value>
bool operator<=(const Pool<Value>& lhs, const Pool<Value>& rhs) { return !(lhs > rhs); }
