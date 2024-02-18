/////////////////////////////////////////////////////////////////////////////////
/// @file       SceneChangeButton.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      DESCRIPTION HERE
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////


#include "SceneChangeButton.h"
#include "InputSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
SceneChangeButton::SceneChangeButton() :
    Component(typeid(SceneChangeButton))
{}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
void SceneChangeButton::OnInit()
{
    /// Set the filter function for the listener
    m_Listener.SetFilterFunction([&](std::string const& EventName) -> bool
	{
            Debug() << "EventName: " << EventName << " m_SceneName: " << m_SceneName << std::endl;
            return EventName == m_SceneName;
	});

    /// Set the Callback function for the listener
    m_Listener.SetResponseFunction([&](std::string const& EventName)
    {
         Debug() << "EventName: " << EventName << " m_SceneName: " << m_SceneName << std::endl;

         onButtonClick(EventName);
         
    });

    m_Listener.Init();
}

/// @brief  called once when exiting the scene
void SceneChangeButton::OnExit()
{
    m_Listener.Exit();
}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  called when the button is clicked to change the scene
void SceneChangeButton::onButtonClick(std::string const&) const
{
	// Change the scene
    Scenes()->SetNextScene(m_SceneName);
}

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


/// @brief  shows the inspector for SceneChangeButton
void SceneChangeButton::Inspector()
{
    static std::string name = m_SceneName;
    ImGui::InputText("Scene Name", &name);
    if ( ImGui::IsItemFocused() && Input()->GetKeyTriggered(GLFW_KEY_ENTER) )
	{
        // if the name is not empty
        if (!name.empty())
        {
            // Set the visaual name to the scene name
            m_SceneName = name;

            m_Listener.SetFilterFunction([&](std::string const& EventName) -> bool
			{
				return EventName == name;
			});
        }
	}
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    void SceneChangeButton::readSceneName(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_SceneName, data);
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this SceneChangeButton
    /// @return the map of read methods for this SceneChangeButton
    ReadMethodMap< ISerializable > const& SceneChangeButton::GetReadMethods() const
    {
        static ReadMethodMap< SceneChangeButton > const readMethods = {
		    { "SceneName", &SceneChangeButton::readSceneName }

        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all SceneChangeButton data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json SceneChangeButton::Write() const
    {
        nlohmann::ordered_json json;

        json["SceneName"] = m_SceneName;

        return json;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this SceneChangeButton
    /// @return the newly created clone of this SceneChangeButton
    SceneChangeButton* SceneChangeButton::Clone() const
    {
        return new SceneChangeButton(*this);
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the SceneChangeButton
    /// @param  other   the other SceneChangeButton to copy
    SceneChangeButton::SceneChangeButton(SceneChangeButton const& other) :
        Component(other)
    {}


//-----------------------------------------------------------------------------
