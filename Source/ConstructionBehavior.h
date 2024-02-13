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

#include "ComponentReference.h"
#include "Transform.h"
#include "Sprite.h"
#include "AudioPlayer.h"

#include "EntityReference.h"
#include "Inventory.h"
#include "Tilemap.h"


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
        AssetReference< Entity > M_Archetype;
        
        /// @brief  the cost of the building
        std::vector< ItemStack > M_Cost = {};
        

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
        void readArchetype( nlohmann::ordered_json const& data );

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
    float GetPlacementRange() const;

    /// @brief  sets the placement range
    /// @param  range   the placement range
    void SetPlacementRange( float range );


    /// @brief  gets the building index
    /// @return the building index
    int GetBuildingIndex() const;

    /// @brief  sets the building index
    /// @param  range   the building index
    void SetBuildingIndex( int range );


    /// @brief  gets whether buildings should be able to be constructed for free
    /// @return whether buildings should be able to be constructed for free
    bool GetIgnoreCosts() const;

    /// @brief  sets whether buildings should be able to be constructed for free
    /// @param  ignoreCosts whether buildings should be able to be constructed for free
    void SetIgnoreCosts( bool ignoreCosts );


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

    /// @brief  the current position being targeted
    glm::vec2 m_TargetPos = { 0, 0 };


    /// @brief  color of the preview when the building is placeable
    glm::vec4 m_PreviewColorPlaceable = { 0, 0.5f, 0, 1.0f };

    /// @brief  color of the preview when the building is not placeable
    glm::vec4 m_PreviewColorNonPlaceable = { 0.5f, 0, 0, 1.0f };

    /// @brief  the default transparency the preview is shown with
    float m_PreviewAlpha = 0.5f;


    /// @brief  whether buildings should be able to be constructed for free
    bool m_IgnoreCosts = false;


    /// @brief  the transform of the player
    ComponentReference< Transform > m_PlayerTransform;

    /// @brief  the Inventory of the player
    ComponentReference< Inventory > m_PlayerInventory;

    /// @brief  the player entity
    EntityReference m_PlayerEntity = EntityReference( { &m_PlayerTransform, &m_PlayerInventory } );


    /// @brief  the tilemap of the terrain
    ComponentReference< Tilemap< int > > m_Tilemap;

    /// @brief  tilemap of the placed buildings
    ComponentReference< Tilemap< Entity* > > m_Buildings;

    /// @brief  the tilemap entity
    EntityReference m_TilemapEntity = EntityReference( { &m_Tilemap, &m_Buildings } );


    /// @brief  the transform of the preview sprite
    ComponentReference< Transform > m_Transform;

    /// @brief  the preview sprite
    ComponentReference< Sprite > m_Sprite;

    /// @brief  the sound played when placing a turret.
    ComponentReference< AudioPlayer > m_TurretPlacementSound;


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


    /// @brief  read the tilemap entity
    /// @param  data    the json data to read from
    void readTilemapEntity( nlohmann::ordered_json const& data );

    /// @brief  read the player entity
    /// @param  data    the json data to read from
    void readPlayerEntity( nlohmann::ordered_json const& data );


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
    ConstructionBehavior( ConstructionBehavior const& other );


    // diable = operator
    void operator =( ConstructionBehavior const& ) = delete;


//-----------------------------------------------------------------------------
};
