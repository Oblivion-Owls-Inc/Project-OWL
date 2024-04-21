///*****************************************************************/
/// @file FileExplorer.h
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief Debug System Class For Debugging
/// @brief The Debug Console for verbose debugging
/// 
/// @details This class provides functionality for debugging 
///          and using ImGui with other Systems.
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/
#include "pch.h" // precompiled header has to be included first
#include "FileExplorer.h"
#include "DebugSystem.h"


void FileExplorer::Explore(bool* open)
{
	static std::string path = std::filesystem::current_path().string(); /// The current path
    static std::string selectedFilePath;  /// path of the selected file
    static std::string currentPath = std::filesystem::current_path().string(); /// The current path that is being displayed
    static std::string fileContents; /// The contents of the file

    ImGui::Begin("Load File", open, ImGuiWindowFlags_MenuBar);
    if (!*open)
    {
		ImGui::End();
		return;
	}

    if (ImGui::Button("<"))
    {
        if (currentPath.ends_with("./"))
        {
            currentPath += "../";
        }
        else
        {
            for (auto i = currentPath.rbegin(); i != currentPath.rend(); ++i)
            {
                if (*i == '\\')
                {
					currentPath = std::string(&*currentPath.begin(), &*i);
					break;
				}
			}
        }

        fileContents.clear();
        path = currentPath;
    }
    ImGui::SameLine();
    
    ImGui::InputText("##path", &currentPath);

    ImGui::SameLine();

    if (ImGui::Button("GO"))
    {
		path = currentPath;
	}

    ImGui::SameLine();

    if (ImGui::Button("Root"))
    {
        currentPath = std::filesystem::current_path().string();
        path = currentPath;
    }

    try
    {
        auto directory_iterator = std::filesystem::directory_iterator(path);
        
        if (ImGui::BeginTable("##FileSystemViewer", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("FileName", ImGuiTableColumnFlags_WidthFixed, 200.0f);
            ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 200.0f);
            ImGui::TableHeadersRow();

            int i = 0;
            static int ID = 0;

            for (auto& directory : directory_iterator)
            {
                ImGui::PushID(i);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();

                std::string current_path = directory.path().string();

                ImGui::TextUnformatted(current_path.substr(path.size() + 1).c_str());

                ImGui::TableSetColumnIndex(1);

                if (directory.is_directory())
                {
                    if (ImGui::Button(">"))
                    {
                        currentPath = current_path;
                        path = current_path;

                        ImGui::PopID();
                        break;
                    }
                }
                else
                {
                    std::string ext = directory.path().extension().string();
                    if ((ext == ".csv") || (ext == ".json") || (ext == ".cpp") || (ext == ".txt") || (ext == ".h") ||
                        (ext == ".ini"))
                    {
                        if (ImGui::Button(("Open##" + std::to_string(i)).c_str()))
                        {
                            std::ifstream file(current_path);
                            if (file.is_open())
                            {
                                ID = ImGui::TableGetRowIndex();
                                fileContents.clear();
                                fileContents.assign((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
                                file.close();
                            }
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Save"))
                        {
							std::ofstream file(current_path);
                            if (file.is_open())
                            {
								file << fileContents;
								file.close();
							}
						}
                    }
                }

                ImGui::PopID();
                ++i;
            }

            ImGui::EndTable();
		}

        if (!fileContents.empty())
        {
            ImGui::Begin("File Contents");

            // Calculate an estimated size for the text box
            size_t lineCount = std::count(fileContents.begin(), fileContents.end(), '\n');
            float lineHeight = ImGui::GetTextLineHeightWithSpacing();
            ImVec2 textBoxSize = ImVec2(-FLT_MIN, lineHeight * (lineCount + 1)); // +1 for some extra space

            ImGui::InputTextMultiline("##Contents", &fileContents[0], fileContents.size() + 1,
                textBoxSize, ImGuiInputTextFlags_AllowTabInput);

            ImGui::End();
        }

    }
    catch (std::exception e)
    {
		Debug() << e.what() << std::endl;
        path = std::filesystem::current_path().string();
	}

    ImGui::End();
}
