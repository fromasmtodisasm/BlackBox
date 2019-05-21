#pragma once
#include <BlackBox/IConsole.hpp>

#include <imgui.h>

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, here we are using a more C++ like approach of declaring a class to hold the data and the functions.
class CConsole : public IConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

public:
    CConsole();
    ~CConsole();

    // Portable helpers
    static int   Stricmp(const char* str1, const char* str2);
    static int   Strnicmp(const char* str1, const char* str2, int n);
    static char* Strdup(const char* str);
    static void  Strtrim(char* str);

    void    ClearLog();

    void    AddLog(const char* fmt, ...);

    void    Draw(const char* title, bool* p_open);

    void    ExecCommand(const char* command_line);

    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);

    int     TextEditCallback(ImGuiInputTextCallbackData* data);
};