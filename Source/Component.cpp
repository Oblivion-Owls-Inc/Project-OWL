/******************************************************************************
filename    Component.cpp
author      Tyler Birdsall
DP email    tyler.birdsall@digipen.edu
course      GAM200
section     A
Project		Dig Deeper

Brief Description:
  Function declerations for the componnt class.

******************************************************************************/

#include "Component.h"

/// @brief Default constructor for Component.
/// @param type_ type of component being created.
Component::Component( std::type_index type_ ) :
    type( type_ ),
    parent( nullptr )
{}