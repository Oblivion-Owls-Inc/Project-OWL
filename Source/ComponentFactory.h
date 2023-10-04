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

    static Component* Create( std::string const& key );

    static std::type_index GetTypeId( std::string const& typeName );

private:

    template < typename ComponentType >
    static Component* Creator();

    template < typename ComponentType >
    Component

    static std::map< std::string, std::pair< std::type_index, Component* (*)() > > const s_componentTypes;
};

