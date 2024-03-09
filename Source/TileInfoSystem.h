/// @file       TileInfoSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System to get info about given tile indices
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023

#pragma once

#include "System.h"

#include "LootTable.h"


/// @brief  Example System meant to be copy-pasted when creating new Systems
class TileInfoSystem : public System
{
//-----------------------------------------------------------------------------
public: // TileInfo class
//-----------------------------------------------------------------------------
    
    /// @brief  holds info about a specific tile type
    class TileInfo : public ISerializable
    {
    //-----------------------------------------------------------------------------
    public: // accessors
    //-----------------------------------------------------------------------------


        /// @brief  gets the Loot Table of the tile type
        /// @return the loot table of the tile type
        LootTable const& GetLootTable() const;


    //-----------------------------------------------------------------------------
    private: // members
    //-----------------------------------------------------------------------------


        /// @brief  the loot table of the tile type
        LootTable m_LootTable;


    //-----------------------------------------------------------------------------
    public: // inspection
    //-----------------------------------------------------------------------------

        
        /// @brief  inspects this TileInfo
        /// @return whether the TileInfo was changed
        bool Inspect();


    //-----------------------------------------------------------------------------
    private: // reading
    //-----------------------------------------------------------------------------


        /// @brief  reads the loot table of the tile type
        /// @param  data    - the json data to read from
        void readLootTable( nlohmann::ordered_json const& data );


    //-----------------------------------------------------------------------------
    public: // reading / writing
    //-----------------------------------------------------------------------------


        /// @brief  gets this System's read methods
        /// @return this System's read methods
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;

        /// @brief  Write all TileInfo data to JSON.
        /// @return The JSON containing the TileInfo data.
        virtual nlohmann::ordered_json Write() const override;


    //-----------------------------------------------------------------------------
    };

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    
    /// @brief  gets the info about the specified tile type
    /// @param  tileId  - the ID of the tile type to get the info of
    /// @return the tile info
    TileInfo const* GetInfo( int tileId ) const;


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the info about each tile type
    std::vector< TileInfo > m_TileInfo;


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  inspects this System
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the info about each tile type
    /// @param  data    - the json data to read from
    void readTileInfo( nlohmann::ordered_json const& data );

    
//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all TileInfoSystem data to JSON.
    /// @return The JSON containing the TileInfoSystem data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the TileInfoSystem
    TileInfoSystem();


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of TileInfoSystem
    /// @return the instance of the TileInfoSystem
    static TileInfoSystem * GetInstance();

    // Prevent copying
    TileInfoSystem( TileInfoSystem const& ) = delete;
    void operator=( TileInfoSystem const& ) = delete;


//-----------------------------------------------------------------------------
};


/// @brief  shorthand method for getting the TileInfoSystem
/// @return 
__inline TileInfoSystem* Tiles()
{
    return TileInfoSystem::GetInstance();
}
