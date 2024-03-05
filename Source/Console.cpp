///*****************************************************************/
/// @file Console.cpp
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief Consol Class For Debugging
/// 
/// @details This class provides functionality for debugging in Editor,
///			 It will allow you to call commands, and display warnings 
///			 while in Editor 
///*****************************************************************/

#include "Console.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief Adds a log to the console 
    /// @param log  - A string to add to the console
    void DebugConsole::AddLog(const std::string& log) 
    {
        static std::string lastLog;

        // Check if the log ends with a newline
        if (!log.empty() && (log.back() == '\n' || log.back() == '\r')) 
        {
            // If lastLog is not empty, append it to the log before pushing.
            if (!lastLog.empty()) 
            {
                m_Items.push_back(lastLog + log);
                lastLog.clear(); // Clear lastLog since it's now pushed
            }
            else 
            {
                // Directly push the log if lastLog is empty.
                m_Items.push_back(log);
            }
        }
        else 
        {
            // Append the log to lastLog if it doesn't end with a newline.
            lastLog += log;
        }
    }

    /// @brief Shows the console window
    void DebugConsole::Inspect()
    {
        static bool p_open = true;
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Console", &p_open))
        {
            ImGui::End();
            return;
        }

        /// Open a Context Menu to Close the Console
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
            {
                p_open = false;
            }

            ImGui::EndPopup();
        }


    #ifdef TEST
        if (ImGui::SmallButton("Add Debug Warning")) 
        { 
            AddLog("Warning: Don't Do that\n");
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("Add Debug Error")) 
        { 
            AddLog("Error: something went wrong\n"); 
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("Add Debug Message")) 
	    { 
		    AddLog("Debug: This is a debug message\n"); 
	    }

        ImGui::SameLine();
    #endif // TEST


        if (ImGui::SmallButton("Clear")) 
        { 
            ClearLog(); 
        }
        ImGui::SameLine();

        bool copy_to_clipboard = ImGui::SmallButton("Copy");

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        if (ImGui::Button("Options"))
        {
            ImGui::OpenPopup("Options");
        }

        ImGui::SameLine();

        m_Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);

        ImGui::Separator();


        // Reserve enough left-over height for 1 separator + 1 input text
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) 
                {
                    ClearLog();
                }

                ImGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
            {
                ImGui::LogToClipboard();
            }

            for (const std::string& item : m_Items) {
                // If you don't have a filter, remove this conditional
                if (!m_Filter.PassFilter(item.c_str()))
                    continue;

                ImVec4 color;
                bool has_color = false;
                if (item.find("Warning:") != std::string::npos || item.find("WARNING:") != std::string::npos)
                {
                    /// Make the text yellow
                    color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
                    has_color = true;
                }
			    if (item.find("Error:") != std::string::npos) 
			    {
				    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
				    has_color = true;
			    }
                if (item.rfind("# ", 0) == 0) 
                { // Efficient check for prefix
                    color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                    has_color = true;
                }
                if(item.rfind("OpenGL Error:", 0) == 0)
				{
                    continue;
				}

                if (has_color)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                }

                ImGui::TextUnformatted(item.c_str());

                if (has_color)
                {
                    ImGui::PopStyleColor();
                }
            }
            if (copy_to_clipboard)
            {
                ImGui::LogFinish();
            }

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (m_AutoScroll || ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }

            m_AutoScroll = false;

            ImGui::PopStyleVar();
        }

        ImGui::EndChild();
        ImGui::End();
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief Adds the commands to the console
    void DebugConsole::addCommands()
    {
        // Add commands
	    m_Commands.push_back("HELP");
	    m_Commands.push_back("HISTORY");
	    m_Commands.push_back("CLEAR");
	    m_Commands.push_back("CLASS");
	    m_Commands.push_back("EXIT");
    }

    /// @brief Clears the console log
    void DebugConsole::ClearLog()
    {
	    m_Items.clear();
    }

    /// @brief Calls a Command given a string
    /// @param command - The command to call
    void DebugConsole::CallCommand(std::string const& command)
    {
        if (command.empty())
	    {
		    return;
	    }
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------




    /// @brief  gets this System's read methods
        /// @return this System's read methods
    ReadMethodMap< ISerializable > const& DebugConsole::GetReadMethods() const
    {
        static ReadMethodMap< DebugConsole > const readMethods = {};
        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this DebugConsole to JSON
    /// @return the JSON data of this DebugConsole
    nlohmann::ordered_json DebugConsole::Write() const
    {
        nlohmann::ordered_json json;
        return json;
    }


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


        /// @brief  gets the instance of DebugConsole
        /// @return the instance of the DebugConsole
    DebugConsole* DebugConsole::GetInstance()
    {
        static std::unique_ptr < DebugConsole > s_Instance = nullptr;
        if (s_Instance == nullptr)
        {
            s_Instance.reset( new DebugConsole() );
        }
        return s_Instance.get();
    }


//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


        /// @brief  Constructs the DebugConsole
    DebugConsole::DebugConsole()
    {
        addCommands();
    }

//-----------------------------------------------------------------------------

