#pragma once
#include <fstream>
#include <streambuf>

#include <imgui.h>
#include <imfilebrowser.h>

#include <cassert>

class GLSLEditor
{
public:
    class TextEditor* editor = nullptr;
    static const char* fileToEdit;
	ImGui::FileBrowser fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
    GLSLEditor();
    ~GLSLEditor();

    void OpenFile(const std::string& name);
    void OpenFileDialog();
    void SaveFile(const std::string& name);

    void Update();
};
