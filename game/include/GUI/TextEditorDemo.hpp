#pragma once
#include <fstream>
#include <streambuf>

#include <imgui.h>
#include <imfilebrowser.h>
#include "Texteditor.h"

#include <cassert>

class GLSLEditor
{
public:
    TextEditor* editor = nullptr;
    std::string fileToEdit = "TextEditorDemo.hpp";
	ImGui::FileBrowser fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CloseOnEsc);
    GLSLEditor();
    ~GLSLEditor();

    void OpenFile(const std::string& name);
    void OpenFileDialog();
	void Syntax();
    void SaveFile(const std::string& name);

    void Update();

    struct LanguageReflection
	{
		std::vector<TextEditor::LanguageDefinition> languages;	
        std::vector<const char*> names;
        std::vector<std::vector<const char*>> exts;
		int selectedItem = 0;
		LanguageReflection()
		{
			TextEditor::LanguageDefinition  defs[] = {
				TextEditor::LanguageDefinition::CPlusPlus(),
				TextEditor::LanguageDefinition::HLSL(),
				TextEditor::LanguageDefinition::GLSL(),
				TextEditor::LanguageDefinition::C(),
				TextEditor::LanguageDefinition::AngelScript(),
				TextEditor::LanguageDefinition::Lua()
			};
			static const char* items[]
			{
				"C++", "HLSL", "GLSL", "C", "AngelScript", "LUA"
			};
			static std::vector<const char*> ext[]
			{
				{"cpp", "hpp", "h", "cxx"}, 
				{"hlsl", "fx"}, 
				{"glsl", "frag", "vert", "geom"}, 
				{"C", "h"}, 
				{"as"}, 
				{"lua"}
			};


			for (auto & d : defs)
			{
				languages.emplace_back() = d;	
			}
			for (auto & n : items)
			{
				names.emplace_back() = n;
			}
			for (auto & e : ext)
			{
				exts.emplace_back() = e;
			}
        }

		void SetLang(TextEditor& ed, std::string_view ext)
		{
			size_t n = 0;
			for (const auto& e : exts)
			{
				if (auto it = std::find_if(e.begin(), e.end(), [&ext](const char* arg) {
					return 
						std::equal(
							arg, arg + strlen(arg), ext.begin(), ext.end(),[](char a, char b) { return tolower(a) == tolower(b); }
						);
					});	 it != e.end())
				{
					ed.SetLanguageDefinition(languages[n]);
					break;
				}
				n++;
			}
		}
    }languageReflection;
	int opened = true;
};
