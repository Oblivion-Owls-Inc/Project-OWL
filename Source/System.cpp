/**
* @file System.cpp
* @author Steve Bukowinski (steve.bukowinski@digipen.edu)
* @brief Base class for all Systems
* @version 0.1
* @date 2023-09-05
* 
* @copyright Copyright (c) 2023
* 
*/

#pragma once

#include "System.h"


/**
* @brief Gets this System's index in the Engine
* @return The index of this System in the Engine
*/
int System::getIndex() const
{
    return index;
}

/**
* @brief Sets this System's internal index value. SHOULD ONLY BE CALLED BY THE ENGINE.
* @param index the index to be set to
*/
void System::setIndex( int _index )
{
    index = _index;
}

/**
* @brief Constructs a System
*/
System::System() :
    index(-1)
{}
