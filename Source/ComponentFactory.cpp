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

template < typename ComponentType >
Component* ComponentFactory::Creator() {
    return new ComponentType();
}

/// @brief     Creates a component of a specified type.
/// @param key The type of component being created.
/// @return    A pointer to the new component.
Component* ComponentFactory::Create( std::string key )
{
    return componentCreators.at( key )();
}

/// @brief          Get the type ID of the component.
/// @param typeName A key to search the map for.
/// @return         The type ID of the component.
std::type_index ComponentFactory::getTypeId( std::string typeName )
{
    return componentTypes.at( typeName );
}

// Fill the map with constructors from the different components.
std::map<std::string, Component* (*)()> ComponentFactory::componentCreators = {
    { "Transform"      , Creator<Transform>      },
    { "Sprite"         , Creator<Sprite>         },
    { "RigidBody"      , Creator<RigidBody>      },
    { "MovementAI"     , Creator<MovementAI>     },
    { "LineCollider"   , Creator<LineCollider>   },
    { "CircleCollider" , Creator<CircleCollider> },
    { "AudioPlayer"    , Creator<AudioPlayer>    }
};

// A map of the component type IDs.
std::map<std::string, std::type_index> ComponentFactory::componentTypes = {
    { "Transform"      , typeid(Transform)      },
    { "Sprite"         , typeid(Sprite)         },
    { "RigidBody"      , typeid(RigidBody)      },
    { "MovementAI"     , typeid(MovementAI)     },
    { "LineCollider"   , typeid(LineCollider)   },
    { "CircleCollider" , typeid(CircleCollider) },
    { "AudioPlayer"    , typeid(AudioPlayer)    }
};