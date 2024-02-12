///*****************************************************************/
/// @file	    BaseBehavior.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      BaseBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#pragma once
#include "BasicEntityBehavior.h"
#include "Pool.h"


class AudioPlayer;

class BaseBehavior :
    public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    BaseBehavior();
    /// @brief dtor
    ~BaseBehavior();
    /// @brief Base clone
    Component* Clone() const override;

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief called when base initializes
    virtual void OnInit() override;

    /// @brief called when base exits
    virtual void OnExit() override;

//-----------------------------------------------------------------------------
public: // public method
//-----------------------------------------------------------------------------

    /// @brief destroy the base
    void Destroy();

    /// @breif  gets the health of the base
    /// @return health of the base
    Pool<int>* GetHealth();
    
//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------
    
    /// @brief copy ctor
    BaseBehavior(const BaseBehavior& other);

//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
};


