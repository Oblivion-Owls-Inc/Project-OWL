/// @file       TileInfoSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System to get info about given tile indices
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023


#include "TileInfoSystem.h"

#include "Inspection.h"


//-----------------------------------------------------------------------------
// public: TileInfo class
//-----------------------------------------------------------------------------
    
    //-----------------------------------------------------------------------------
    // public: accessors
    //-----------------------------------------------------------------------------


        /// @brief  gets the Loot Table of the tile type
        /// @return the loot table of the tile type
        LootTable const& TileInfoSystem::TileInfo::GetLootTable() const
        {
            return m_LootTable;
        }


    //-----------------------------------------------------------------------------
    // public: inspection
    //-----------------------------------------------------------------------------

        
        /// @brief  inspects this TileInfo
        /// @return whether the TileInfo was changed
        bool TileInfoSystem::TileInfo::Inspect()
        {
            bool changed = false;

            if ( ImGui::TreeNode( "loot table" ) )
            {
                changed |= m_LootTable.Inspect();
                ImGui::TreePop();
            }

            return changed;
        }


    //-----------------------------------------------------------------------------
    // private: reading
    //-----------------------------------------------------------------------------


        /// @brief  reads the loot table of the tile type
        /// @param  data    - the json data to read from
        void TileInfoSystem::TileInfo::readLootTable( nlohmann::ordered_json const& data )
        {
            Stream::Read( m_LootTable, data );
        }


    //-----------------------------------------------------------------------------
    // public: reading
    //-----------------------------------------------------------------------------


        /// @brief  gets this System's read methods
        /// @return this System's read methods
        ReadMethodMap< ISerializable > const& TileInfoSystem::TileInfo::GetReadMethods() const
        {
            static ReadMethodMap< TileInfo > const readMethods = {
                { "LootTable", &TileInfo::readLootTable }
            };

            return (ReadMethodMap< ISerializable > const&)readMethods;
        }


        /// @brief  Write all TileInfo data to JSON.
        /// @return The JSON containing the TileInfo data.
        nlohmann::ordered_json TileInfoSystem::TileInfo::Write() const
        {
            nlohmann::ordered_json json;

            json[ "LootTable" ] = Stream::Write( m_LootTable );

            return json;
        }


    //-----------------------------------------------------------------------------
    

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    
    /// @brief  gets the info about the specified tile type
    /// @param  tileId  - the ID of the tile type to get the info of
    /// @return the tile info of the specified tile type
    TileInfoSystem::TileInfo const* TileInfoSystem::GetInfo( int tileId ) const
    {
        
        if ( tileId < 0 || tileId >= m_TileInfo.size() )
        {
            return nullptr;
        }

        return &m_TileInfo[ tileId ];

    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  inspects this System
    void TileInfoSystem::DebugWindow()
    {
        bool windowShown = GetDebugEnabled();

        if ( ImGui::Begin( "Tile Info System", &windowShown ) == false )
        {
            ImGui::End();
            SetDebugEnable( windowShown );
            return;
        }

        Inspection::InspectArray< TileInfo >(
            "tile info",
            &m_TileInfo,
            []( TileInfo* tileInfo ) -> bool
            {
                return tileInfo->Inspect();
            }
        );

        ImGui::End();
        SetDebugEnable( windowShown );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the info about each tile type
    /// @param  data    - the json data to read from
    void TileInfoSystem::readTileInfo( nlohmann::ordered_json const& data )
    {
        m_TileInfo.clear();
        for ( auto& tileInfoData : data )
        {
            TileInfo tileInfo;
            Stream::Read( tileInfo, tileInfoData );
            m_TileInfo.push_back( tileInfo );
        }
    }

    
//-----------------------------------------------------------------------------
// public: reading
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< ISerializable > const& TileInfoSystem::GetReadMethods() const
    {
        static ReadMethodMap< TileInfoSystem > const readMethods = {
            { "TileInfo", &TileInfoSystem::readTileInfo }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all TileInfoSystem data to JSON.
    /// @return The JSON containing the TileInfoSystem data.
    nlohmann::ordered_json TileInfoSystem::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& tileInfoData = json[ "TileInfo" ];
        for ( TileInfo const& tileInfo : m_TileInfo )
        {
            tileInfoData.push_back( tileInfo.Write() );
        }

        return json;
    }

//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the TileInfoSystem
    TileInfoSystem::TileInfoSystem() :
        System( "TileInfoSystem" )
    {}


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of TileInfoSystem
    /// @return the instance of the TileInfoSystem
    TileInfoSystem* TileInfoSystem::GetInstance()
    {
        static std::unique_ptr < TileInfoSystem > s_Instance = nullptr;

        if (s_Instance == nullptr )
        {
            s_Instance.reset( new TileInfoSystem());
        }

        return s_Instance.get();
    }

//-----------------------------------------------------------------------------
