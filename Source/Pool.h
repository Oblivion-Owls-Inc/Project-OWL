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


template <typename Value>
class Pool : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

	///@brief constructor
	Pool(std::string name = std::string("Pool"), Value value = 0, bool active = 0);

	Pool(const Pool& other);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------
 	
	/// @brief Gets the name of the pool
	/// @return Name of the Pool
	__inline std::string const& GetName() const;

 	/// ///@brief get the current value
	///@return the current value
	__inline Value const& GetCurrent() const;

	///@brief get the default value
	///@return the default value
	__inline Value const& GetDefault() const;

	///@brief get the active state
	///@return the active state
	__inline bool const& GetActive() const;

	/// @brief set the current value
	/// @param value - the new current value 
	__inline void SetCurrent(Value value);

	/// @brief Changes the base value
	/// @param value - the new base value
	__inline void SetDefault(Value value);
	
	__inline void DecreasePoolTime(Value value);
	/// @brief Changes if the pool is active
	/// @param active - the new active state
	__inline void SetActive(bool active);

	/// @brief Changes the name of the pool
	/// @param name - The New Name of the pool
	__inline void SetName(std::string name);

	/// @brief Resets the pool to its default value
	__inline void Reset() { m_CurrentValue = m_DefaultValue; }

//-----------------------------------------------------------------------------
public: // virtual methods
//-----------------------------------------------------------------------------
	/// @brief Used by the Debug System to display information about this Component
	virtual void Inspector();

	/// @brief called when this Component's Entity is added to the Scene
	virtual void OnInit() { Reset(); }

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
	/// @brief the name of the pool
	std::string m_Name;

	/// @brief the current value of the pool
	Value m_CurrentValue;

	/// @brief the default value of the pool
	Value m_DefaultValue;

	/// @brief the active state of the pool
	bool m_Active;

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

	/// @brief  map of the read methods
	static ReadMethodMap< Pool > const s_ReadMethods;

	/// @brief Reads the Name of the pool from the json data
	/// @param data - the json data to read from
	void readName(nlohmann::ordered_json const& data);

	/// @brief Reads the Base Value from the json data
	/// @param data - the json data to read from
	void readBaseValue(nlohmann::ordered_json const& data);

	/// @brief Reads the Current Value from the json data
	/// @param data - the json data to read from
	void readCurrentValue(nlohmann::ordered_json const& data);

	/// @brief Reads the Active State from the json data
	/// @param data - the json data to read from
	void readActive(nlohmann::ordered_json const& data);

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

	/// @brief converts the pool to a bool
	operator bool() const { return m_Active; }

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------
	///@brief write the data to json
	///@return the written json data
	virtual nlohmann::ordered_json Write() const override;
};

#ifndef POOL_C
#include "Pool.cpp"
#endif


