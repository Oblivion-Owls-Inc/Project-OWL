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
#include "ComponentFactory.h"

#include "Transform.h"
#include "TilemapSprite.h"
#include "RigidBody.h"
#include "MovementAI.h"
#include "CircleCollider.h"
#include "EnemyBehavior.h"
#include "AudioPlayer.h"
#include "AudioListener.h"
#include "BulletBehavior.h"
#include "Animation.h"
#include "TurretBehavior.h"
#include "PlayerController.h"
#include "EffectAnimator.h"
#include "Text.h"
#include "Tilemap.h"
#include "Pathfinder.h"
#include "TilemapCollider.h"
#include "StaticBody.h"
#include "ConstructionBehavior.h"
#include "Camera.h"
#include "WavesBehavior.h"
#include "EmitterSprite.h"
#include "Emitter.h"
#include "BaseBehavior.h"
#include "Health.h"
#include "GeneratorBehavior.h"
#include "UiElement.h"
#include "MiningLaser.h"
#include "ItemComponent.h"
#include "TilemapItemDropper.h"
#include "Inventory.h"
#include "ItemCollector.h"
#include "Light.h"
#include "UiBarSprite.h"
#include "HealthBar.h"
#include "CameraBehavior.h"
#include "UiButton.h"
#include "TilemapTextureConnector.h"
#include "TilemapEditor.h"
#include "EditorCameraController.h"


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  creates a new Component with the specified type
    /// @param  typeName    the type name of Component to create
    /// @return the created component
    Component* ComponentFactory::Create( std::string const& typeName )
    {
        return GetComponentInfo( typeName ).second();
    }

    /// @brief  gets the std::type_index of the Component type with the specified name
    /// @param  typeName    the type name of the Component type to get
    /// @return the std::type_index of the Component type
    std::type_index ComponentFactory::GetTypeId( std::string const& typeName )
    {
        return GetComponentInfo( typeName ).first;
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
    std::pair< std::type_index, Component* (*)() > const& ComponentFactory::GetComponentInfo( std::string const& typeName )
    {
        auto iterator = s_ComponentTypes.find( typeName );
        if ( iterator == s_ComponentTypes.end() )
        {
            throw std::runtime_error(
                std::string() +
                "Error: could not find Component Type with name \"" +
                typeName + '\"'
            );
        }
        return iterator->second;
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
        { "Transform"              , ComponentInfo< Transform >()               },
        { "Sprite"                 , ComponentInfo< Sprite >()                  },
        { "RigidBody"              , ComponentInfo< RigidBody >()               },
        { "MovementAI"             , ComponentInfo< MovementAI >()              },
        { "CircleCollider"         , ComponentInfo< CircleCollider >()          },
        { "AudioPlayer"            , ComponentInfo< AudioPlayer >()             },
        { "AudioListener"          , ComponentInfo< AudioListener >()           },
        { "Animation"              , ComponentInfo< Animation >()               },
        { "Text"                   , ComponentInfo< Text >()                    },
        { "PlayerController"       , ComponentInfo< PlayerController >()        },
		{ "EnemyBehavior"          , ComponentInfo< EnemyBehavior >()           },
		{ "BulletBehavior"         , ComponentInfo< BulletBehavior >()          },
        { "TurretBehavior"         , ComponentInfo< TurretBehavior >()          },
        { "EffectAnimator"         , ComponentInfo< EffectAnimator >()          },
        { "Tilemap<int>"           , ComponentInfo< Tilemap< int > >()          },
        { "Tilemap<Entity*>"       , ComponentInfo< Tilemap< Entity* > >()      },
        { "TilemapSprite"          , ComponentInfo< TilemapSprite >()           },
        { "Pathfinder"             , ComponentInfo< Pathfinder >()              },
        { "TilemapCollider"        , ComponentInfo< TilemapCollider >()         },
        { "StaticBody"             , ComponentInfo< StaticBody >()              },
        { "ConstructionBehavior"   , ComponentInfo< ConstructionBehavior >()    },
        { "Camera"                 , ComponentInfo< Camera >()                  },
        { "WavesBehavior"          , ComponentInfo< WavesBehavior >()           },
        { "Emitter"                , ComponentInfo< Emitter >()                 },
        { "EmitterSprite"          , ComponentInfo< EmitterSprite >()           },
        { "BaseBehavior"           , ComponentInfo< BaseBehavior >()            },
        { "Health"                 , ComponentInfo< Health >()                  },
        { "UiElement"              , ComponentInfo< UiElement >()               },
        { "MiningLaser"            , ComponentInfo< MiningLaser >()             },
        { "ItemComponent"          , ComponentInfo< ItemComponent >()           },
        { "TilemapItemDropper"     , ComponentInfo< TilemapItemDropper >()      },
        { "Inventory"              , ComponentInfo< Inventory >()               },
        { "ItemCollector"          , ComponentInfo< ItemCollector >()           },
        { "HealthBar"              , ComponentInfo< HealthBar >()               },
        { "UiBarSprite"            , ComponentInfo< UiBarSprite >()             },
        { "UiButton"               , ComponentInfo< UiButton >()                },
        { "Light"                  , ComponentInfo< Light >()                   },
        { "CameraBehavior"         , ComponentInfo< CameraBehavior >()          },
        { "TilemapTextureConnector", ComponentInfo< TilemapTextureConnector >() },
        { "GeneratorBehavior"      , ComponentInfo< GeneratorBehavior >()       },
        { "TilemapEditor"          , ComponentInfo< TilemapEditor >()           },
        { "EditorCameraController" , ComponentInfo< EditorCameraController >()  }
    };

//-----------------------------------------------------------------------------
