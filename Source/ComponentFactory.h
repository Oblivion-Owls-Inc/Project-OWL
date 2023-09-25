#pragma once

#include <map>    // std::map
#include <string> // std::string
#include <typeindex>

class Component;

class ComponentFactory
{
public:

    ComponentFactory() = delete;
    ~ComponentFactory() = delete;

public:

    static Component* Create( std::string key );

    static std::type_index GetTypeId( std::string typeName );

private:

    template < typename ComponentType >
    static Component* Creator();

    static std::map<std::string, Component* (*)()> componentCreators;
    static std::map<std::string, std::type_index> componentTypes;
};

