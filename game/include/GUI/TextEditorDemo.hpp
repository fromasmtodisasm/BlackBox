#pragma once
#include <fstream>
#include <streambuf>

#include "imgui.h"

#include <cassert>

class GLSLEditor
{
public:
    class TextEditor* editor = nullptr;
    static const char* fileToEdit;
    GLSLEditor();
    ~GLSLEditor();

    void Update();
};
