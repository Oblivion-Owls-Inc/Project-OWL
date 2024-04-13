///*****************************************************************/
/// @file	    HomeBase.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      HomeBase class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#pragma once
#include "Component.h"

#include "EntityReference.h"
#include "ComponentReference.h"
#include "EventListener.h"
class Health;
class SceneTransition;

class HomeBase : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------


    /// @brief  constructor
    HomeBase();

 //-----------------------------------------------------------------------------
public: // accessors
 //-----------------------------------------------------------------------------

    /// @brief  gets the Health Component attached to this Entity
    /// @return the Health Component attached to this Entity
    Health* GetHealth();

    /// @brief  gets the current win state
    /// @return bool if we can win the game
    bool CanWin();

//-----------------------------------------------------------------------------
public: // public method
//-----------------------------------------------------------------------------


    /// @brief destroy the base
    void Destroy();
    
    
//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    
    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    
    /// @brief  the name of the scene to transition to when the base dies
    std::string m_GameOverSceneName = "Gameover";


    /// @brief  the SceneTransition Component responsible for changing scenes
    ComponentReference< SceneTransition > m_SceneTransition;

    /// @brief  the Entity the SceneTransition Component is attached to
    EntityReference m_SceneTransitionEntity = EntityReference( { &m_SceneTransition } );


    /// @brief  the Health Component attached to this Entity
    ComponentReference< Health > m_Health;

    /// @brief Listener for the start event
    EventListener<std::string> m_ListenerBegin;

    /// @brief  the name of the event to listen for
    std::string m_EventNameBegin;

    /// @brief Listener for the end event
    EventListener<std::string> m_ListenerEnd;

    /// @brief  the name of the event to listen for
    std::string m_EventNameEnd;

    bool m_CanWin = false;

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspector for this HomeBase
    virtual void Inspector() override;

    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads the name of the scene to transition to when the base dies
    /// @param  data    the JSON data to read from
    void readGameOverSceneName( nlohmann::ordered_json const& data );

    /// @brief  reads the Entity the SceneTransition Component is attached to
    /// @param  data    the JSON data to read from
    void readSceneTransitionEntity( nlohmann::ordered_json const& data );

    /// @brief  reads the EventNameBegin from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameBegin(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameEnd from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameEnd(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this HomeBase
    /// @return the map of read methods for this HomeBase
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief Base clone
    virtual HomeBase* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------
    

    /// @brief copy ctor
    HomeBase( HomeBase const& other);


    void operator =( HomeBase const& ) = delete;


//-----------------------------------------------------------------------------
};


