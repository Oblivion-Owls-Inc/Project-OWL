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
#include "Sprite.h"
#include "RigidBody.h"
#include "MovementAI.h"
#include "LineCollider.h"
#include "CircleCollider.h"
#include "AudioPlayer.h"
#include "Animation.h"
#include "TextSprite.h"
#include "PlayerController.h"

template < typename ComponentType >
Component* ComponentFactory::Creator() {
    return new ComponentType();
}

/// @brief     Creates a component of a specified type.
/// @param key The type of component being created.
/// @return    A pointer to the new component.
Component* ComponentFactory::Create( std::string const& key )
{
    return componentCreators.at( key )();
}

std::type_index ComponentFactory::GetTypeId( std::string const& typeName )
{
    return s_ComponentTypes.at( typeName );
}

// Fill the map with constructors from the different components.
std::map<std::string, Component* (*)()> ComponentFactory::componentCreators = {
    { "Transform"        , { Creator<Transform>        },
    { "Sprite"           , { Creator<Sprite>           },
    { "RigidBody"        , { Creator<RigidBody>        },
    { "MovementAI"       , { Creator<MovementAI>       },
    { "LineCollider"     , { Creator<LineCollider>     },
    { "CircleCollider"   , { Creator<CircleCollider>   },
    { "AudioPlayer"      , { Creator<AudioPlayer>      },
    { "Animation"        , { Creator<Animation>        },
    { "TextSprite"       , { Creator<TextSprite>       },
    { "PlayerController" , { Creator<PlayerController> }
};