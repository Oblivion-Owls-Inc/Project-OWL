///--------------------------------------------------------------------------//
/// @file   ComponentFactory.h
/// @brief  Declaration for the component factory class.
/// 
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @date   September 2023
///
/// @copyright © 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
#pragma once

//-----------------------------------------------------------------------------
// Include Files:
//-----------------------------------------------------------------------------

#include "pch.h" 

//-----------------------------------------------------------------------------
// Forward References:
//-----------------------------------------------------------------------------
class Component;

//-----------------------------------------------------------------------------
// Class: ComponentFactory
//-----------------------------------------------------------------------------
class ComponentFactory
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

    ComponentFactory() = delete;
    ~ComponentFactory() = delete;

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  creates a new Component with the specified type
    /// @param  typeName    the type name of Component to create
    /// @return the created component
    static Component* Create( std::string const& typeName );

    /// @brief  gets the std::type_index of the Component type with the specified name
    /// @param  typeName    the type name of the Component type to get
    /// @return the std::type_index of the Component type
    static std::type_index const* GetTypeId( std::string const& typeName );

    /// @brief  gets the name of the Component type with the specified type_index
    /// @param  typeIndex    the type index of the Component type to get
    /// @return the name of the Component type
    static std::string GetTypeName( std::type_index const& typeINdex );

    /// @brief  gets the map of Component types be name
    /// @return the map of Component types by name
    static std::map< std::string, std::pair< std::type_index, Component* (*)() > > const& GetComponentTypes() { return s_ComponentTypes; }

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------
    
    /// @brief  accesses the Component type map with error handling
    /// @param  typeName    the type of component to get info about
    /// @return the Component type info for the specified type name
    static std::pair< std::type_index, Component* (*)() > const* getComponentInfo( std::string const& typeName );

//-----------------------------------------------------------------------------
private: // data
//-----------------------------------------------------------------------------

    /// @brief  template method to create the specified type of Component
    /// @tparam ComponentType   the type of Component to create
    /// @return the created Component
    template < typename ComponentType >
    static Component* Creator();

    /// @brief  template method for creating Type information
    /// @tparam ComponentType   the type of Component to create information for
    /// @return the Component information
    template < typename ComponentType >
    static std::pair< std::type_index, Component* (*)() > ComponentInfo();

    /// @brief  map containing information on Component types by name
    static std::map< std::string, std::pair< std::type_index, Component* (*)() > > const s_ComponentTypes;

//-----------------------------------------------------------------------------
};
