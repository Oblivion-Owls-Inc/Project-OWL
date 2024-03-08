///*****************************************************************/
/// @file Console.h
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief Consol Class For Debugging
/// 
/// @details This class provides functionality for debugging in Editor,
///			 It will allow you to call commands, and display warnings 
///			 while in Editor 
///*****************************************************************/

#pragma once
#include "basics.h"
#include "DebugSystem.h"
#include <string>
#include <map>

typedef void (*CheatFunction)(void); // function pointer type


/// @brief The DebugConsole class is a singleton that provides a console for debugging
///        It Shows Warning Messages, Errors, and allows you to call commands
class DebugConsole: public ISerializable
{
//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------
    
    /// @brief Adds a log to the console
    /// @param log - A string to add to the console
    void AddLog(std::string const& log);

    /// @brief Shows the console window
    void Inspect();

    /// @brief Clears the console log
    void ClearLog();

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief  the filter for the console
    ImGuiTextFilter  m_Filter;

    /// @brief  the buffer for the console
    std::string m_Buffer;

    /// @brief The list of items in the console
    std::vector<std::string> m_Items;

    /// @brief commands history, filter
    std::vector<std::string> m_Commands;

    /// @brief A map of cheat codes.
    std::unordered_map<std::string, CheatFunction> m_CheatMap;

    /// @brief Chached user input text
    std::vector<std::string> m_History;

    /// @brief If we are currently scrolling to the bottom to keep up with new entries.
    bool m_AutoScroll = true;

    /// @brief -1: new line, 0..History.Size-1 browsing history.
    int m_HistoryPos = -1;    
//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief Adds the commands to the console
    void addCommands();

    /// @brief Calls a Command given a string
    /// @param command - The command to call
    void CallCommand(std::string const& command);

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief Reads all the console commands from a JSON file.
    /// @param data - The JSON file to read from.
    void readCommands(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this DebugConsole's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this DebugConsole to JSON
    /// @return the JSON data of this DebugConsole
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

	/// @brief  gets the instance of the DebugConsole
    /// @return the instance of the DebugConsole
    static DebugConsole* GetInstance();

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  creates the DebugConsole
    DebugConsole();

	// Prevent Copying
	DebugConsole(DebugConsole const&) = delete;
	void operator =(DebugConsole const&) = delete;

};


/// @brief  gets the instance of the DebugConsole
/// @return the instance of the DebugConsole
__inline DebugConsole* Console() {
    return DebugConsole::GetInstance();
}
 