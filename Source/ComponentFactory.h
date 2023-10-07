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

    static std::map< std::string, std::pair< std::type_index, Component* (*)() > > const& GetComponentTypes() { return s_ComponentTypes; }
private:

    template < typename ComponentType >
    static Component* Creator();

    template < typename ComponentType >
    static __inline std::pair< std::type_index, Component* (*)() > ComponentInfo() {
        return { typeid( ComponentType ), Creator< ComponentType > };
    }

    static std::map< std::string, std::pair< std::type_index, Component* (*)() > > const s_ComponentTypes;
};

