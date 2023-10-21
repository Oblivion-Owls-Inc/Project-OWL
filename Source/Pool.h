




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
	Pool(std::string name);

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
private: // reading
//-----------------------------------------------------------------------------

	/// @brief  map of the read methods
	static ReadMethodMap< Pool > const s_ReadMethods;

	/// @brief Reads the Name of the pool from the json data
	/// @param data - the json data to read from
	void readName(nlohmann::ordered_json const& data);

	/// @brief Reads the Base Value from the json data
	/// @param data - the json data to read from
	void readBaseValue(nlohmann::ordered_json const& data);

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
public: // writing
//-----------------------------------------------------------------------------
	///@brief write the data to json
	///@return the written json data
	virtual nlohmann::ordered_json Write() const override;
};

#ifndef POOL_C
#include "Pool.cpp"
#endif


