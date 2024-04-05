///--------------------------------------------------------------------------//
/// @file   ComponentFactory.cpp
/// @brief  Definitions for the component factory class.
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

//-----------------------------------------------------------------------------
// Include Files:
//-----------------------------------------------------------------------------

#include "pch.h" 
#include "ComponentFactory.h"

#include "Transform.h"
#include "TilemapSprite.h"
#include "RigidBody.h"
#include "CircleCollider.h"
#include "EnemyBehavior.h"
#include "AudioPlayer.h"
#include "AudioListener.h"
#include "Bullet.h"
#include "BulletAoe.h"
#include "BulletAoePulse.h"
#include "Animation.h"
#include "TurretBehavior.h"
#include "PlayerController.h"
#include "EffectAnimator.h"
#include "Text.h"
#include "Tilemap.h"
#include "TilemapCollider.h"
#include "StaticBody.h"
#include "ConstructionBehavior.h"
#include "Camera.h"
#include "Lifetime.h"
#include "WavesBehavior.h"
#include "EmitterSprite.h"
#include "Emitter.h"
#include "HomeBase.h"
#include "Health.h"
#include "Generator.h"
#include "UiElement.h"
#include "MiningLaser.h"
#include "ItemComponent.h"
#include "TilemapItemDropper.h"
#include "Inventory.h"
#include "ItemCollector.h"
#include "Light.h"
#include "UiBarSprite.h"
#include "Nineslice.h"
#include "HealthBar.h"
#include "CameraBehavior.h"
#include "UiButton.h"
#include "TilemapTextureConnector.h"
#include "TilemapEditor.h"
#include "EditorCameraController.h"
#include "WinState.h"
#include "ResourceCounterUi.h"
#include "ResourcesUiManager.h"
#include "SceneChangeButton.h"
#include "SplashScreenController.h"
#include "HideableForeground.h"
#include "PathfinderTarget.h"
#include "PathfinderSource.h"
#include "Popup.h"
#include "pause.h"
#include "orbitbehavior.h"
#include "DestructibleTilemap.h"
#include "ControlPrompt.h"
#include "BuildingSelectionButton.h"
#include "DigEffect.h"
#include "UiSlider.h"
#include "Interactable.h"
#include "Interactor.h"
#include "LaserUpgrade.h"
#include "TurretUnlockUpgrade.h"
#include "SceneTransition.h"


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  creates a new Component with the specified type
    /// @param  typeName    the type name of Component to create
    /// @return the created component
    Component* ComponentFactory::Create( std::string const& typeName )
    {
        std::pair< std::type_index, Component* (*)() > const* componentInfo = getComponentInfo( typeName );
        if ( componentInfo == nullptr )
        {
            return nullptr;
        }

        return componentInfo->second();
    }

    /// @brief  gets the std::type_index of the Component type with the specified name
    /// @param  typeName    the type name of the Component type to get
    /// @return the std::type_index of the Component type
    std::type_index const* ComponentFactory::GetTypeId( std::string const& typeName )
    {
        std::pair< std::type_index, Component* (*)() > const* componentInfo = getComponentInfo( typeName );
        if ( componentInfo == nullptr )
        {
            return nullptr;
        }

        return &componentInfo->first;
    }

    /// @brief  gets the name of the Component type with the specified type_index
    /// @param  typeIndex    the type index of the Component type to get
    /// @return the name of the Component type
    std::string ComponentFactory::GetTypeName( std::type_index const& typeIndex )
    {
        for ( auto& [key, typeInfo] : s_ComponentTypes )
        {
            if ( typeInfo.first == typeIndex )
            {
                return key;
            }
        }

        std::cerr << "Error: could not find proper name of type \"" << typeIndex.name() << "\"" << std::endl;
        return "";
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  accesses the Component type map with error handling
    /// @param  typeName    the type of component to get info about
    /// @return the Component type info for the specified type name
    std::pair< std::type_index, Component* (*)() > const* ComponentFactory::getComponentInfo( std::string const& typeName )
    {
        auto iterator = s_ComponentTypes.find( typeName );
        if ( iterator == s_ComponentTypes.end() )
        {
            Debug() <<  "WARNING: could not create unrecognized Component type \""
                << typeName << "\"" << std::endl;
            return nullptr;
        }
        return &iterator->second;
    }

//-----------------------------------------------------------------------------
// private: data
//-----------------------------------------------------------------------------

    /// @brief  template method to create the specified type of Component
    /// @tparam ComponentType   the type of Component to create
    /// @return the created Component
    template < typename ComponentType >
    Component* ComponentFactory::Creator()
    {
        return new ComponentType();
    }

    /// @brief  template method for creating Type information
    /// @tparam ComponentType   the type of Component to create information for
    /// @return the Component information
    template < typename ComponentType >
    std::pair< std::type_index, Component* (*)() > ComponentFactory::ComponentInfo() {
        return { typeid( ComponentType ), Creator< ComponentType > };
    }

    /// @brief  map containing information on Component types by name
    std::map< std::string, std::pair< std::type_index, Component* (*)() > > const ComponentFactory::s_ComponentTypes = {
        { "Transform"              , ComponentInfo< Transform               >() },
        { "Sprite"                 , ComponentInfo< Sprite                  >() },
        { "RigidBody"              , ComponentInfo< RigidBody               >() },
        { "CircleCollider"         , ComponentInfo< CircleCollider          >() },
        { "AudioPlayer"            , ComponentInfo< AudioPlayer             >() },
        { "AudioListener"          , ComponentInfo< AudioListener           >() },
        { "Animation"              , ComponentInfo< Animation               >() },
        { "Text"                   , ComponentInfo< Text                    >() },
        { "PlayerController"       , ComponentInfo< PlayerController        >() },
		{ "EnemyBehavior"          , ComponentInfo< EnemyBehavior           >() },
		{ "Bullet"                 , ComponentInfo< Bullet                  >() },
        { "BulletAoe"              , ComponentInfo< BulletAoe               >() },
        { "BulletAoePulse"         , ComponentInfo< BulletAoePulse          >() },
        { "TurretBehavior"         , ComponentInfo< TurretBehavior          >() },
        { "EffectAnimator"         , ComponentInfo< EffectAnimator          >() },
        { "Tilemap<int>"           , ComponentInfo< Tilemap< int     >      >() },
        { "Tilemap<float>"         , ComponentInfo< Tilemap< float   >      >() },
        { "Tilemap<Entity*>"       , ComponentInfo< Tilemap< Entity* >      >() },
        { "TilemapSprite"          , ComponentInfo< TilemapSprite           >() },
        { "TilemapCollider"        , ComponentInfo< TilemapCollider         >() },
        { "StaticBody"             , ComponentInfo< StaticBody              >() },
        { "ConstructionBehavior"   , ComponentInfo< ConstructionBehavior    >() },
        { "Camera"                 , ComponentInfo< Camera                  >() },
        { "Lifetime"               , ComponentInfo< Lifetime                >() },
        { "WavesBehavior"          , ComponentInfo< WavesBehavior           >() },
        { "Emitter"                , ComponentInfo< Emitter                 >() },
        { "EmitterSprite"          , ComponentInfo< EmitterSprite           >() },
        { "HomeBase"               , ComponentInfo< HomeBase                >() },
        { "Health"                 , ComponentInfo< Health                  >() },
        { "UiElement"              , ComponentInfo< UiElement               >() },
        { "MiningLaser"            , ComponentInfo< MiningLaser             >() },
        { "ItemComponent"          , ComponentInfo< ItemComponent           >() },
        { "TilemapItemDropper"     , ComponentInfo< TilemapItemDropper      >() },
        { "Inventory"              , ComponentInfo< Inventory               >() },
        { "ItemCollector"          , ComponentInfo< ItemCollector           >() },
        { "HealthBar"              , ComponentInfo< HealthBar               >() },
        { "UiBarSprite"            , ComponentInfo< UiBarSprite             >() },
        { "UiButton"               , ComponentInfo< UiButton                >() },
        { "Light"                  , ComponentInfo< Light                   >() },
        { "CameraBehavior"         , ComponentInfo< CameraBehavior          >() },
        { "TilemapTextureConnector", ComponentInfo< TilemapTextureConnector >() },
        { "Generator"              , ComponentInfo< Generator               >() },
        { "TilemapEditor"          , ComponentInfo< TilemapEditor           >() },
        { "EditorCameraController" , ComponentInfo< EditorCameraController  >() },
        { "ResourceCounterUi"      , ComponentInfo< ResourceCounterUi       >() },
        { "ResourcesUiManager"     , ComponentInfo< ResourcesUiManager      >() },
        { "SceneChangeButton"      , ComponentInfo< SceneChangeButton       >() },
        { "WinState"               , ComponentInfo< WinState                >() },
        { "Nineslice"              , ComponentInfo< Nineslice               >() },
        { "SplashScreenController" , ComponentInfo< SplashScreenController  >() },
        { "HideableForeground"     , ComponentInfo< HideableForeground      >() },
        { "PathfinderTarget"       , ComponentInfo< PathfinderTarget        >() },
        { "PathfinderSource"       , ComponentInfo< PathfinderSource        >() },
        { "HideableForeground"     , ComponentInfo< HideableForeground      >() },
        { "Popup"                  , ComponentInfo< Popup                   >() },
        { "PauseComponent"         , ComponentInfo< PauseComponent          >() },
        { "OrbitBehavior"          , ComponentInfo< OrbitBehavior           >() },
        { "DestructibleTilemap"    , ComponentInfo< DestructibleTilemap     >() },
        { "ControlPrompt"          , ComponentInfo< ControlPrompt           >() },
        { "BuildingSelectionButton", ComponentInfo< BuildingSelectionButton >() },
        { "DigEffect"              , ComponentInfo< DigEffect               >() },
        { "UiSlider"               , ComponentInfo< UiSlider                >() },
        { "Interactable"           , ComponentInfo< Interactable            >() },
        { "Interactor"             , ComponentInfo< Interactor              >() },
        { "LaserUpgrade"           , ComponentInfo< LaserUpgrade            >() },
        { "TurretUnlockUpgrade"    , ComponentInfo< TurretUnlockUpgrade     >() },
        { "SceneTransition"        , ComponentInfo< SceneTransition         >() }
    };

//-----------------------------------------------------------------------------
