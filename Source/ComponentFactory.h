///--------------------------------------------------------------------------//
/// @file   ComponentFactory.h
/// @brief  Declaration for the component factory class.
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
#pragma once

//-----------------------------------------------------------------------------
// Include Files:
//-----------------------------------------------------------------------------

#include <map>       // std::map
#include <string>    // std::string
#include <typeindex> // std::typeindex

//-----------------------------------------------------------------------------
// Forward References:
//-----------------------------------------------------------------------------
class Component;

//-----------------------------------------------------------------------------
// Class: ComponentFactory
//-----------------------------------------------------------------------------
class ComponentFactory
{
public:

    ComponentFactory() = delete;
    ~ComponentFactory() = delete;

public:
    // Creates a specified component.
    static Component* Create(std::string key);
    // Gets the matching ID of a specified component.
    static std::type_index getTypeId( std::string typeName );

private:

    template < typename ComponentType >
    static Component* Creator();
    // A map of the component constructors.
    static std::map<std::string, Component* (*)()> componentCreators;
    // A map of the component IDs.
    static std::map<std::string, std::type_index> componentTypes;
};

