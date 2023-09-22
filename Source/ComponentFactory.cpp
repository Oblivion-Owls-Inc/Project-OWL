#include "ComponentFactory.h"
// 
#include "Transform.h"
#include "Sprite.h"
#include "RigidBody.h"

template < typename ComponentType >
Component* ComponentFactory::Creator() {
    return new ComponentType();
}

// Fill the map with constructors from the different components.
std::map<std::string, Component* (*)()> ComponentFactory::componentCreators = {
    {"Transform", Creator<Transform>},
    {"Sprite", Creator<Sprite>},
    {"RigidBody", Creator<RigidBody>},
};


std::map<std::string, std::type_index> ComponentFactory::componentTypes = {
    {"Transform", typeid(Transform)},
    {"Sprite", typeid(Sprite)},
    {"RigidBody", typeid(RigidBody)},
};

/// @brief Creates a component of a specified type.
/// @param key - the type of component being created.
/// @return A pointer to the new component.
Component* ComponentFactory::Create( std::string key )
{
    return componentCreators.at(key)();
}

std::type_index ComponentFactory::GetTypeId( std::string typeName )
{
    return componentTypes.at( typeName );
}