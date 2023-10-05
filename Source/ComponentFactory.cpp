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

/// @brief     Creates a component of a specified type.
/// @param key The type of component being created.
/// @return    A pointer to the new component.
Component* ComponentFactory::Create( std::string const& key )
{
    return s_ComponentTypes.at( key ).second();
}

std::type_index ComponentFactory::GetTypeId( std::string const& typeName )
{
    return s_ComponentTypes.at( typeName ).first;
}

template < typename ComponentType >
Component* ComponentFactory::Creator()
{
    return new ComponentType();
}

// Fill the map with constructors from the different components.
std::map< std::string, std::pair< std::type_index, Component* (*)() > > const ComponentFactory::s_ComponentTypes = {
    { "Transform"        , ComponentInfo<Transform>()        },
    { "Sprite"           , ComponentInfo<Sprite>()           },
    { "RigidBody"        , ComponentInfo<RigidBody>()        },
    { "MovementAI"       , ComponentInfo<MovementAI>()       },
    { "LineCollider"     , ComponentInfo<LineCollider>()     },
    { "CircleCollider"   , ComponentInfo<CircleCollider>()   },
    { "AudioPlayer"      , ComponentInfo<AudioPlayer>()      },
    { "Animation"        , ComponentInfo<Animation>()        },
    { "TextSprite"       , ComponentInfo<TextSprite>()       },
    { "PlayerController" , ComponentInfo<PlayerController>() },
};