/// @file       LootTable.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      class for dropping configurable loot
/// @version    0.1
/// @date       2024-01-17
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "ISerializable.h"

#include "ItemStack.h"

/// @brief  class for dropping configurable loot
class LootTable : public ISerializable
{
//-----------------------------------------------------------------------------
private: // TableEntry type
//-----------------------------------------------------------------------------


    /// @brief  an entry into the loot table
    class TableEntry : public ISerializable
    {
    //-------------------------------------------------------------------------
    public: // methods
    //-------------------------------------------------------------------------


        /// @brief  generates loot from this table entry
        /// @return the generated loot
        ItemStack GetLoot() const;
    
    
    //-------------------------------------------------------------------------
    public: // accessors
    //-------------------------------------------------------------------------


        /// @brief  gets how likely this entry is to be rolled, if random rolls are enabled
        /// @return how likely this entry is to be rolled, if random rolls are enabled
        float GetWeight() const;

        /// @brief  gets whether this entry can be rolled multiple times, if random rolls are enabled
        /// @return whether this entry can be rolled multiple times, if random rolls are enabled
        bool GetAllowMultipleRolls() const;


    //-------------------------------------------------------------------------
    private: // members
    //-------------------------------------------------------------------------


        /// @brief  the item IDs that can be dropped from this entry of the loot table
        std::vector< int > m_ItemIds = {};

        /// @brief  the minimum number of items that can be dropped from this entry
        int m_MinCount = 1;
        /// @brief  the maximum number of items that can be dropped from this entry
        int m_MaxCount = 1;

        /// @brief  how likely this entry is to give any items
        float m_Probability = 1.0f;

        /// @brief  how likely this entry is to be rolled, if random rolls are enabled
        float m_Weight = 1.0f;

        /// @brief  whether this entry can be rolled multiple times, if random rolls are enabled
        bool m_AllowMultipleRolls = true;


    //-------------------------------------------------------------------------
    public: // inspection
    //-------------------------------------------------------------------------


        /// @brief  inspects this TableEntry
        /// @param  randomRollsEnabled  - hides non-relevant members when random rolls are disabled
        /// @return whether the entry was modified
        bool Inspect( bool randomRollsEnabled );


    //-------------------------------------------------------------------------
    private: // reading
    //-------------------------------------------------------------------------


        /// @brief  reads the ItemIds for this TableEntry
        /// @param  data    the json data to read from
        void readItemIds( nlohmann::ordered_json const& data );

        /// @brief  reads the MinCount for this TableEntry
        /// @param  data    the json data to read from
        void readMinCount( nlohmann::ordered_json const& data );

        /// @brief  reads the MaxCount for this TableEntry
        /// @param  data    the json data to read from
        void readMaxCount( nlohmann::ordered_json const& data );

        /// @brief  reads the Probability for this TableEntry
        /// @param  data    the json data to read from
        void readProbability( nlohmann::ordered_json const& data );

        /// @brief  reads the Weight for this TableEntry
        /// @param  data    the json data to read from
        void readWeight( nlohmann::ordered_json const& data );

        /// @brief  reads the AllowMultiplRolls for this TableEntry
        /// @param  data    the json data to read from
        void readAllowMultipleRolls( nlohmann::ordered_json const& data );
        

    //-------------------------------------------------------------------------
    public: // reading / writing
    //-------------------------------------------------------------------------


        /// @brief  Gets the map of read methods for this object
        /// @return the map of read methods for htis object
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const;


        /// @brief  Write all LootTableEntry data to JSON.
        /// @return The JSON containing the LootTableEntry data.
        virtual nlohmann::ordered_json Write() const override;


    //-------------------------------------------------------------------------
    };


//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    LootTable() = default;

    
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets whether random rolls are enabled
    /// @return whether random rolls are enabled
    /// @details
    /// if random rolls are enabled, the the loot table will pick a random entry each time
    /// if random rolls are disabled, the loot table will use each of its entries once
    bool GetRandomRollsEnabled() const;

    /// @brief  sets whether random rolls are enabled
    /// @param  randomRollsEnabled  whether random rolls should enabled
    /// @details
    /// if random rolls are enabled, the the loot table will pick a random entry each time
    /// if random rolls are disabled, the loot table will use each of its entries once
    void SetRandomRollsEnabled( bool randomRollsEnabled );


    /// @brief  gets the minimum number of rolls into the table to make when random rolls are enabled
    /// @return the minimum number of rolls into the table to make when random rolls are enabled
    int GetMinRolls() const;

    /// @brief  sets the minimum number of rolls into the table to make when random rolls are enabled
    /// @param  minRolls    - the minimum number of rolls into the table to make when random rolls are enabled
    void SetMinRolls( int minRolls );


    /// @brief  gets the maximum number of rolls into the table to make when random rolls are enabled
    /// @return the maximum number of rolls into the table to make when random rolls are enabled
    int GetMaxRolls() const;

    /// @brief  sets the maximum number of rolls into the table to make when random rolls are enabled
    /// @param  maxRolls    - the maximum number of rolls into the table to make when random rolls are enabled
    void SetMaxRolls( int maxRolls );


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  uses the loot table to generate a roll of loot
    /// @return the generated loot
    std::vector< ItemStack > GenerateLoot() const;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  each entry in the loot table
    std::vector< TableEntry > m_Entries = {};

    /// @brief  whether random rolls are enabled for this loot table
    bool m_RandomRollsEnabled = false;

    /// @brief  the minimum number of rolls into the table to make
    int m_MinRolls = 1;
    /// @brief  the maximum number of rolls into the table to make
    int m_MaxRolls = 1;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    
    /// @brief  generates loot using random entries
    /// @return the generated loot
    std::vector< ItemStack > generateLootWithRandomRolls() const;

    /// @brief  generates loot using each entry once
    /// @return the generated loot
    std::vector< ItemStack > generateLootWithFixedRolls() const;


    /// @brief  adds loot to a collection
    /// @param  totalLoot   the collection of total loot
    /// @param  addedLoot   the loot to add to the collection
    static void addLoot( std::vector< ItemStack >& totalLoot, ItemStack const& addedLoot );


    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this loot table
    /// @return whether the table was modified
    bool Inspect();


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads each entry in the loot table
    /// @param  data    the json data to read from
    void readEntries( nlohmann::ordered_json const& data );

    /// @brief  reads whether random rolls are enabled for this loot table
    /// @param  data    the json data to read from
    void readRandomRollsEnabled( nlohmann::ordered_json const& data );

    /// @brief  reads the minimum number of rolls into the table to make
    /// @param  data    the json data to read from
    void readMinRolls( nlohmann::ordered_json const& data );

    /// @brief  reads the maximum number of rolls into the table to make
    /// @param  data    the json data to read from
    void readMaxRolls( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------

    
    /// @brief  Gets the map of read methods for this object
    /// @return the map of read methods for htis object
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const;


    /// @brief  Write all LootTable data to JSON.
    /// @return The JSON containing the LootTable data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
