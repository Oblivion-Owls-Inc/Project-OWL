/// @file       ConstructionBehavior.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the construction of towers and modification of terrain
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Behavior.h"
#include "basics.h"

#include "ItemStack.h"

#include <vector>

class Transform;
class Sprite;
class AudioPlayer;
class Inventory;

template< typename T >
class Tilemap;

/// @brief  Component that handles the construction of towers and modification of terrain
class ConstructionBehavior : public Behavior
{
//-----------------------------------------------------------------------------
private: // class BuildingInfo
//-----------------------------------------------------------------------------

    
    /// @brief  struct of info needed to construct buildings
    class BuildingInfo : public ISerializable
    {
    //-------------------------------------------------------------------------
    public: // members
    //-------------------------------------------------------------------------


        /// @brief  the archetype of the Building
        Entity const* M_Archetype = nullptr;
        
        /// @brief  the cost of the building
        std::vector< ItemStack > M_Cost = {};


    //-------------------------------------------------------------------------
    private: // members
    //-------------------------------------------------------------------------


        /// @brief  the name of the archetype
        std::string m_ArchetypeName;
        

    //-------------------------------------------------------------------------
    public: // methods
    //-------------------------------------------------------------------------


        /// @brief  initializes this BuildingInfo
        void Init();


    //-------------------------------------------------------------------------
    public: // inspection
    //-------------------------------------------------------------------------


        /// @brief  inspects this BuildingInfo
        /// @return whether this BuildingInfo changed
        bool Inspect();
    
    
    //-------------------------------------------------------------------------
    private: // reading
    //-------------------------------------------------------------------------

        
        /// @brief  reads the name of the archetype
        /// @param  data    the json data to read from
        void readArchetypeName( nlohmann::ordered_json const& data );

        /// @brief  reads the cost of the building
        /// @param  data    the json data to read from
        void readCost( nlohmann::ordered_json const& data );


    //-------------------------------------------------------------------------
    public: // reading / writing
    //-------------------------------------------------------------------------


        /// @brief  gets the map of read methods for this ConstructionBehavior
        /// @return the map of read methods for this ConstructionBehavior
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


        /// @brief  Write all ConstructionBehavior data to a JSON file.
        /// @return The JSON file containing the ConstructionBehavior data.
        virtual nlohmann::ordered_json Write() const override;


    //-------------------------------------------------------------------------
    };


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the placement range
    /// @return the placement range
    float GetPlacementRange() const { return m_PlacementRange; }

    /// @brief  sets the placement range
    /// @param  range   the placement range
    void SetPlacementRange( float range ) { m_PlacementRange = range; }


    /// @brief  gets the building index
    /// @return the building index
    int GetBuildingIndex() const { return m_BuildingIndex; }

    /// @brief  sets the building index
    /// @param  range   the building index
    void SetBuildingIndex( int range ) { m_BuildingIndex = range; }


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;

    /// @brief  called every simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  info needed to construct each building type
    std::vector< BuildingInfo > m_BuildingInfos = {};

    /// @brief  the index of the selected building to build
    int m_BuildingIndex = -1;


    /// @brief  how far the player can reach to place things
    float m_PlacementRange = 1.0f;

    /// @brief  how far past the placement range until the preview is fully faded out
    float m_PreviewFadeOutRadius = 2.0f;


    /// @brief  the current tile being targeted
    glm::ivec2 m_TargetTilePos = { -1, -1 };

    /// @brief  the current tile being mined
    glm::ivec2 m_CurrentMiningTilePos = { -1, -1 };

    /// @brief  the current position being targeted
    glm::vec2 m_TargetPos = { 0, 0 };


    /// @brief  color of the preview when the building is placeable
    glm::vec4 m_PreviewColorPlaceable = { 0, 0.5f, 0, 0 };

    /// @brief  color of the preview when the building is not placeable
    glm::vec4 m_PreviewColorNonPlaceable = { 0.5f, 0, 0, 0 };

    /// @brief  the default transparency the preview is shown with
    float m_PreviewAlpha = 0.5f;


    /// @brief  the name of the player entity
    std::string m_PlayerName;

    /// @brief  the transform of the player
    Transform const* m_PlayerTransform = nullptr;

    /// @brief  the Inventory of the player
    Inventory* m_PlayerInventory = nullptr;


    /// @brief  the name of the tilemap entity
    std::string m_TilemapName;

    /// @brief  the tilemap of the terrain
    Tilemap<int>* m_Tilemap = nullptr;

    /// @brief  tilemap of the placed buildings
    Tilemap< Entity* >* m_Buildings = nullptr;


    /// @brief  the transform of the preview sprite
    Transform* m_Transform = nullptr;

    /// @brief  the preview sprite
    Sprite* m_Sprite = nullptr;

    /// @brief  the sound played when mining.
    AudioPlayer* m_DiggingSound = nullptr;

    /// @brief  the sound played when placing a turret.
    AudioPlayer* m_TurretPlacementSound = nullptr;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------
    
    
    /// @brief  updates the targeted tile location
    void updateTargetLocation();

    /// @brief  updates which building is currently selected
    void updateSelectedBuilding();


    /// @brief  tries to place the currently selected building
    void tryPlaceBuilding();

    /// @brief  checks if the currently selected building is placeable
    bool isCurrentlyPlaceable() const;

    /// @brief  palces the currently selected building
    void placeBuilding();


    /// @brief  displays the building preview
    void showBuildingPreview();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  displays this ConstructionBehavior in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspector for the building list
    void inspectBuildingList();

    /// @brief  inspector for basic variables
    void inspectVariables();

    /// @brief  inspects the references to other entities
    void inspectEntityReferences();


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  read the buildings
    /// @param  data    the json data to read from
    void readBuildings( nlohmann::ordered_json const& data );

    /// @brief  read the building index
    /// @param  data    the json data to read from
    void readBuildingIndex( nlohmann::ordered_json const& data );


    /// @brief  read the placement range
    /// @param  data    the json data to read from
    void readPlacementRange( nlohmann::ordered_json const& data );

    /// @brief  read the preview fade out range
    /// @param  data    the json data to read from
    void readPreviewFadeOutRadius( nlohmann::ordered_json const& data );


    /// @brief  reads the preview color - placeable
    /// @param  data    the json data to read from
    void readPreviewColorPlaceable( nlohmann::ordered_json const& data );

    /// @brief  reads the preview color - nonplaceable
    /// @param  data    the json data to read from
    void readPreviewColorNonPlaceable( nlohmann::ordered_json const& data );

    /// @brief  reads the preview alpha
    /// @param  data    the json data to read from
    void readPreviewAlpha( nlohmann::ordered_json const& data );


    /// @brief  read the tilemap name
    /// @param  data    the json data to read from
    void readTilemapName( nlohmann::ordered_json const& data );

    /// @brief  read the player name
    /// @param  data    the json data to read from
    void readPlayerName( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all ConstructionBehavior data to a JSON file.
    /// @return The JSON file containing the ConstructionBehavior data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ConstructionBehavior();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ConstructionBehavior
    /// @return the newly created clone of this ConstructionBehavior
    virtual ConstructionBehavior* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ConstructionBehavior
    /// @param  other   the other ConstructionBehavior to copy
    ConstructionBehavior( const ConstructionBehavior& other );


    // diable = operator
    void operator =( const ConstructionBehavior& ) = delete;


//-----------------------------------------------------------------------------
};
