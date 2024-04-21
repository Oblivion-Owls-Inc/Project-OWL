/// @file       LootTable.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      class for dropping configurable loot
/// @version    0.1
/// @date       2024-01-17
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h" // precompiled header has to be included first
#include "LootTable.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// private: TableEntry type
//-----------------------------------------------------------------------------


    //-------------------------------------------------------------------------
    // public: methods
    //-------------------------------------------------------------------------


        /// @brief  generates loot from this table entry
        /// @return the generated loot
        ItemStack LootTable::TableEntry::GetLoot() const
        {
            if ( m_ItemIds.size() == 0 || m_Probability <= random( 0.0f, 1.0f ) )
            {
                return ItemStack( -1, 0 );
            }

            int itemIndex = random( 0, (int)m_ItemIds.size() - 1 );
            int itemCount = random( m_MinCount, m_MaxCount );
            return ItemStack( m_ItemIds[ itemIndex ], itemCount );
        }


    //-------------------------------------------------------------------------
    // public: accessors
    //-------------------------------------------------------------------------


        /// @brief  gets how likely this entry is to be rolled, if random rolls are enabled
        /// @return how likely this entry is to be rolled, if random rolls are enabled
        float LootTable::TableEntry::GetWeight() const
        {
            return m_Weight;
        }

        /// @brief  gets whether this entry can be rolled multiple times, if random rolls are enabled
        /// @return whether this entry can be rolled multiple times, if random rolls are enabled
        bool LootTable::TableEntry::GetAllowMultipleRolls() const
        {
            return m_Weight;
        }


    //-------------------------------------------------------------------------
    // public: inspection
    //-------------------------------------------------------------------------


        /// @brief  inspects this TableEntry
        /// @param  randomRollsEnabled  - hides non-relevant members when random rolls are disabled
        /// @return whether the entry was modified
        bool LootTable::TableEntry::Inspect( bool randomRollsEnabled )
        {
            bool hasChanged = false;

            hasChanged = Inspection::InspectArray< int >(
                "item IDs",
                &m_ItemIds,
                []( int* itemId ) -> bool
                {
                    return ImGui::DragInt( "", itemId, 0.05f );
                }
            );

            if ( ImGui::DragInt( "min count", &m_MinCount, 0.05f, 0, INT_MAX ) )
            {
                hasChanged = true;
                m_MaxCount = std::max( m_MaxCount, m_MinCount );
            }

            if ( ImGui::DragInt( "max count", &m_MaxCount, 0.0f, 0, INT_MAX ) )
            {
                hasChanged = true;
                m_MinCount = std::min( m_MinCount, m_MaxCount );
            }

            hasChanged = ImGui::DragFloat( "probability", &m_Weight, 0.05f, 0.0f, 1.0f ) || hasChanged;

            if ( randomRollsEnabled == false )
            {
                return hasChanged;
            }

            hasChanged = ImGui::DragFloat( "weight", &m_Weight, 0.05f, 0.0f, INFINITY ) || hasChanged;

            hasChanged = ImGui::Checkbox( "allow multiple rolls", &m_AllowMultipleRolls ) || hasChanged;

            return hasChanged;
        }


    //-------------------------------------------------------------------------
    // private: reading
    //-------------------------------------------------------------------------


        /// @brief  reads the ItemIds for this TableEntry
        /// @param  data    the json data to read from
        void LootTable::TableEntry::readItemIds( nlohmann::ordered_json const& data )
        {
            Stream::Read( m_ItemIds, data );
        }

        /// @brief  reads the MinCount for this TableEntry
        /// @param  data    the json data to read from
        void LootTable::TableEntry::readMinCount( nlohmann::ordered_json const& data )
        {
            Stream::Read( m_MinCount, data );
        }

        /// @brief  reads the MaxCount for this TableEntry
        /// @param  data    the json data to read from
        void LootTable::TableEntry::readMaxCount( nlohmann::ordered_json const& data )
        {
            Stream::Read( m_MaxCount, data );
        }

        /// @brief  reads the Probability for this TableEntry
        /// @param  data    the json data to read from
        void LootTable::TableEntry::readProbability( nlohmann::ordered_json const& data )
        {
            Stream::Read( m_Probability, data );
        }

        /// @brief  reads the Weight for this TableEntry
        /// @param  data    the json data to read from
        void LootTable::TableEntry::readWeight( nlohmann::ordered_json const& data )
        {
            Stream::Read( m_Weight, data );
        }

        /// @brief  reads the AllowMultiplRolls for this TableEntry
        /// @param  data    the json data to read from
        void LootTable::TableEntry::readAllowMultipleRolls( nlohmann::ordered_json const& data )
        {
            Stream::Read( m_AllowMultipleRolls, data );
        }
        

    //-------------------------------------------------------------------------
    // public: reading / writing
    //-------------------------------------------------------------------------


        /// @brief  Gets the map of read methods for this object
        /// @return the map of read methods for htis object
        ReadMethodMap< ISerializable > const& LootTable::TableEntry::GetReadMethods() const
        {
            static ReadMethodMap< TableEntry > const readMethods = {
                { "ItemIds"             , &LootTable::TableEntry::readItemIds              },
                { "MinCount"            , &LootTable::TableEntry::readMinCount             },
                { "MaxCount"            , &LootTable::TableEntry::readMaxCount             },
                { "Probability"         , &LootTable::TableEntry::readProbability          },
                { "Weight"              , &LootTable::TableEntry::readWeight               },
                { "AllowMultipleRolls"  , &LootTable::TableEntry::readAllowMultipleRolls   }
            };

            return (ReadMethodMap< ISerializable > const&)readMethods;
        }


        /// @brief  Write all LootTableEntry data to JSON.
        /// @return The JSON containing the LootTableEntry data.
        nlohmann::ordered_json LootTable::TableEntry::Write() const
        {
            nlohmann::ordered_json json;

            json[ "ItemIds"            ] = Stream::Write( m_ItemIds            );
            json[ "MinCount"           ] = Stream::Write( m_MinCount           );
            json[ "MaxCount"           ] = Stream::Write( m_MaxCount           );
            json[ "Probability"        ] = Stream::Write( m_Probability        );
            json[ "Weight"             ] = Stream::Write( m_Weight             );
            json[ "AllowMultipleRolls" ] = Stream::Write( m_AllowMultipleRolls );

            return json;
        }


    //-------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets whether random rolls are enabled
    /// @return whether random rolls are enabled
    /// @details
    /// if random rolls are enabled, the the loot table will pick a random entry each time
    /// if random rolls are disabled, the loot table will use each of its entries once
    bool LootTable::GetRandomRollsEnabled() const
    {
        return m_RandomRollsEnabled;
    }

    /// @brief  sets whether random rolls are enabled
    /// @param  randomRollsEnabled  whether random rolls should enabled
    /// @details
    /// if random rolls are enabled, the the loot table will pick a random entry each time
    /// if random rolls are disabled, the loot table will use each of its entries once
    void LootTable::SetRandomRollsEnabled( bool randomRollsEnabled )
    {
        m_RandomRollsEnabled = randomRollsEnabled;
    }


    /// @brief  gets the minimum number of rolls into the table to make when random rolls are enabled
    /// @return the minimum number of rolls into the table to make when random rolls are enabled
    int LootTable::GetMinRolls() const
    {
        return m_MinRolls;
    }

    /// @brief  sets the minimum number of rolls into the table to make when random rolls are enabled
    /// @param  minRolls    - the minimum number of rolls into the table to make when random rolls are enabled
    void LootTable::SetMinRolls( int minRolls )
    {
        m_MinRolls = minRolls;
    }


    /// @brief  gets the maximum number of rolls into the table to make when random rolls are enabled
    /// @return the maximum number of rolls into the table to make when random rolls are enabled
    int LootTable::GetMaxRolls() const
    {
        return m_MaxRolls;
    }

    /// @brief  sets the maximum number of rolls into the table to make when random rolls are enabled
    /// @param  maxRolls    - the maximum number of rolls into the table to make when random rolls are enabled
    void LootTable::SetMaxRolls( int maxRolls )
    {
        m_MaxRolls = maxRolls;
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  uses the loot table to generate a roll of loot
    /// @return 
    std::vector< ItemStack > LootTable::GenerateLoot() const
    {
        if ( m_RandomRollsEnabled )
        {
            return generateLootWithRandomRolls();
        }
        else
        {
            return generateLootWithFixedRolls();
        }
    }
    
//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    
    /// @brief  generates loot using random entries
    /// @return the generated loot
    std::vector< ItemStack > LootTable::generateLootWithRandomRolls() const
    {
        float totalWeight = 0.0f;

        for ( TableEntry const& entry : m_Entries )
        {
            totalWeight += entry.GetWeight();
        }

        std::vector< bool > bannedEntries( m_Entries.size(), false );

        std::vector< ItemStack > loot = {};

        int numRolls = random( m_MinRolls, m_MaxRolls );
        for ( int i = 0; i < numRolls && totalWeight > 0.0f; ++i )
        {
            float targetWeight = random( 0.0f, totalWeight );

            for ( int j = 0; j < m_Entries.size(); ++j )
            {
                if ( bannedEntries[j] )
                {
                    continue;
                }

                targetWeight -= m_Entries[ j ].GetWeight();
                if ( targetWeight >= 0 )
                {
                    continue;
                }

                addLoot( loot, m_Entries[ j ].GetLoot() );

                if ( m_Entries[ j ].GetAllowMultipleRolls() == false )
                {
                    bannedEntries[ j ] = true;
                    totalWeight -= m_Entries[ j ].GetWeight();
                }

                break;
            }
        }

        return loot;

    }

    /// @brief  generates loot using each entry once
    /// @return the generated loot
    std::vector< ItemStack > LootTable::generateLootWithFixedRolls() const
    {
        std::vector< ItemStack > loot = {};

        for ( TableEntry const& entry : m_Entries )
        {
            addLoot( loot, entry.GetLoot() );
        }

        return loot;
    }


    /// @brief  adds loot to a collection
    /// @param  totalLoot   the collection of total loot
    /// @param  addLoot     the loot to add to the collection
    void LootTable::addLoot( std::vector< ItemStack >& totalLoot, ItemStack const& addLoot )
    {
        if ( addLoot.M_Count == 0 )
        {
            return;
        }

        int itemId = addLoot.M_ItemId;
        auto foundStack = std::find_if(
            totalLoot.begin(),
            totalLoot.end(),
            [ itemId ]( ItemStack const& stack ) -> bool
            {
                return stack.M_ItemId == itemId;
            }
        );

        if ( foundStack == totalLoot.end() )
        {
            totalLoot.push_back( addLoot );
        }
        else
        {
            foundStack->M_Count += addLoot.M_Count;
        }
    }

    
//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this loot table
    /// @return whether the table was modified
    bool LootTable::Inspect()
    {
        bool changed = false;

        bool randomRollsEnabled = m_RandomRollsEnabled;
        changed = Inspection::InspectArray< TableEntry >(
            "entries",
            &m_Entries,
            [ randomRollsEnabled ]( TableEntry* entry ) -> bool
            {
                if ( !ImGui::TreeNode( "entry" ) )
                {
                    return false;
                }

                bool changed = entry->Inspect( randomRollsEnabled );

                ImGui::TreePop();
                
                return changed;
            }
        );

        changed = ImGui::Checkbox( "enable random rolls", &m_RandomRollsEnabled ) || changed;

        if ( m_RandomRollsEnabled == false )
        {
            return changed;
        }

        if ( ImGui::DragInt( "min rolls", &m_MinRolls, 0.05f, 0, INT_MAX ) )
        {
            changed = true;
            m_MaxRolls = std::max( m_MaxRolls, m_MinRolls );
        }

        if ( ImGui::DragInt( "max rolls", &m_MaxRolls, 0.05f, 0, INT_MAX ) )
        {
            changed = true;
            m_MinRolls = std::min( m_MinRolls, m_MaxRolls );
        }

        return changed;
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads each entry in the loot table
    /// @param  data    the json data to read from
    void LootTable::readEntries( nlohmann::ordered_json const& data )
    {
        m_Entries.clear();
        for ( auto& entryData : data )
        {
            TableEntry entry;
            Stream::Read( entry, entryData );
            m_Entries.push_back( entry );
        }
    }

    /// @brief  reads whether random rolls are enabled for this loot table
    /// @param  data    the json data to read from
    void LootTable::readRandomRollsEnabled( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_RandomRollsEnabled, data );
    }

    /// @brief  reads the minimum number of rolls into the table to make
    /// @param  data    the json data to read from
    void LootTable::readMinRolls( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MinRolls, data );
    }

    /// @brief  reads the maximum number of rolls into the table to make
    /// @param  data    the json data to read from
    void LootTable::readMaxRolls( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MaxRolls, data );
    }


//-----------------------------------------------------------------------------
// public: reading
//-----------------------------------------------------------------------------

    
    /// @brief  Gets the map of read methods for this object
    /// @return the map of read methods for htis object
    ReadMethodMap< ISerializable > const& LootTable::GetReadMethods() const
    {
        static ReadMethodMap< LootTable > readMethods = {
            { "Entries"             , &LootTable::readEntries              },
            { "RandomRollsEnabled"  , &LootTable::readRandomRollsEnabled   },
            { "MinRolls"            , &LootTable::readMinRolls             },
            { "MaxRolls"            , &LootTable::readMaxRolls             }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all LootTable data to JSON.
    /// @return The JSON containing the LootTable data.
    nlohmann::ordered_json LootTable::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& entriesData = json[ "Entries" ];
        for ( TableEntry const& entry : m_Entries )
        {
            entriesData.push_back( Stream::Write( entry ) );
        }

        json[ "RandomRollsEnabled" ] = Stream::Write( m_RandomRollsEnabled );
        json[ "MinRolls"           ] = Stream::Write( m_MinRolls           );
        json[ "MaxRolls"           ] = Stream::Write( m_MaxRolls           );

        return json;
    }


//-----------------------------------------------------------------------------
