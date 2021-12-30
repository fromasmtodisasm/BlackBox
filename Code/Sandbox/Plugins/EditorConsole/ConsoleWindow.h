#pragma once
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <mutex>
#include "imgui.h"

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, we are using a more C++ like approach of declaring a class to hold both data and functions.
namespace gui
{
struct Console : public IOutputPrintSink, public IEditorNotifyListener, public IPane
{
	char				  InputBuf[256];
	ImVector<char*>		  Items;
	ImVector<const char*> Commands;
	ImVector<char*>		  History;
	int					  HistoryPos; // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter		  Filter;
	bool				  AutoScroll;
	bool				  ScrollToBottom;
	std::mutex			  ConsoleLock;

	Console();
	~Console();

	// Portable helpers
	static int Stricmp(const char* s1, const char* s2);
	static int	 Strnicmp(const char* s1, const char* s2, int n);
	static char* Strdup(const char* s);
	static void	 Strtrim(char* s);

	void ClearLog();

	void AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		// FIXME-OPT
		char	buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
	}

	void Draw(const char* title, bool* p_open);

	void ExecCommand(const char* command_line);

	// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);

	int TextEditCallback(ImGuiInputTextCallbackData* data);

	// Inherited via IOutputPrintSink
	virtual void Print(const char* inszText) override;

	// Inherited via IEditorNotifyListener
	virtual void OnEditorNotifyEvent(EEditorNotifyEvent event) override;
};
}
