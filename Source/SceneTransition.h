/// @file       SceneTransition.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      fades a texture out and in when transitioning between scenes
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once




class Sprite;

/// @brief  DESCRIPTION HERE
class SceneTransition : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    SceneTransition();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    
    /// @brief  starts transitioning to the specified scene
    /// @param  nextSceneName   the name of the scene to transition to
    void StartTransition( std::string const& nextSceneName );

    /// @brief  starts transitioning to the next scene
    void StartTransition();


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the scene this SceneTransition will transition to
    /// @return the name of the scene this SceneTransition will transition to
    std::string const& GetNextSceneName() const;

    /// @brief  sets the name of the scene this SceneTransition will transition to
    /// @param  nextSceneName   the name of the scene this SceneTransition will transition to
    void SetNextSceneName( std::string const& nextSceneName );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called once every simulation frame
    virtual void OnFixedUpdate() override;

    
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


    /// @brief  the current state of transitioning
    enum class State
    {
        In,     /// @brief  switching into the current scene
        Idle,   /// @brief  not transitioning
        Out     /// @brief  switching out of the current scene
    };


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  how long the transition out of/into this frame lasts
    float m_TransitionDuration = 0.5f;

    /// @brief  the name of the scene this SceneTransition will transition to
    std::string m_NextSceneName = "";


    /// @brief  how far along the transition we currently are
    float m_TransitionTimer = 0.0f;

    /// @brief  whether currently transitioning in, out, or not at all
    State m_State = State::In;


    /// @brief  the Sprite Component attached to this Entity
    ComponentReference< Sprite > m_Sprite;

    /// @brief  whether this scene has lighting
    bool m_HasLighting = false;

    /// @brief  the lighting layer of the this scene
    int m_LightingLayer = 0;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for SceneTransition
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads how long the transition out of/into this frame lasts
    /// @param  data    the JSON data to read from
    void readTransitionDuration( nlohmann::ordered_json const& data );

    /// @brief  reads the name of the scene this SceneTransition will transition to
    /// @param  data    the JSON data to read from
    void readNextSceneName( nlohmann::ordered_json const& data );

    /// @brief  reads whether the scene has lighting
    /// @param  data    the JSON data to read from
    void readHasLighting(nlohmann::ordered_json const& data);

    /// @brief  reads the lighting layer of the next scene
    /// @param  data    the JSON data to read from
    void readLightingLayer(nlohmann::ordered_json const& data);


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this SceneTransition
    /// @return the map of read methods for this SceneTransition
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this SceneTransition to JSON
    /// @return the JSON data of this SceneTransition
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this SceneTransition
    /// @return the newly created clone of this SceneTransition
    virtual SceneTransition* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the SceneTransition
    /// @param  other   the other SceneTransition to copy
    SceneTransition( SceneTransition const& other );


    // disable assignment operator
    void operator =( SceneTransition const& ) = delete;


//-----------------------------------------------------------------------------
};
