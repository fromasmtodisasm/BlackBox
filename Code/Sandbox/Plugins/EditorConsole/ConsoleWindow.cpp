#include <BlackBox/Renderer/IRender.hpp>
#include "ConsoleWindow.h"


#if 0
#	define IMGUI_DEMO_MARKER(section)                                                                                                     \
		do {                                                                                                                               \
			if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); \
		} while (0)
#else
#	define IMGUI_DEMO_MARKER(section)
#endif

namespace gui
{

struct ColorTable
{
	ColorB color;
	string name;
} ColorTable[10] =
	{
		ColorB(0x00, 0x00, 0x00), "black",
		ColorB(0xff, 0xff, 0xff), "white",
		ColorB(0x00, 0x00, 0xff), "blue",
		ColorB(0x00, 0xff, 0x00), "green",
		ColorB(0xff, 0x00, 0x00), "red",
		ColorB(0x00, 0xff, 0xff), "cyan",
		ColorB(0xff, 0xff, 0x00), "yellow",
		ColorB(0xff, 0x00, 0xff), "purple",
		ColorB(0xff, 0x80, 0x00), "orange",
		ColorB(0x8f, 0x8f, 0x8f), "grey"};

Console::Console()
{
	IMGUI_DEMO_MARKER("Examples/Console");
	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;

	// "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
	Commands.push_back("HELP");
	Commands.push_back("HISTORY");
	Commands.push_back("CLEAR");
	Commands.push_back("CLASSIFY");
	AutoScroll	   = true;
	ScrollToBottom = false;
	gEnv->pConsole->AddOutputPrintSink(this);
}

Console::~Console()
{
	gEnv->pConsole->RemoveOutputPrintSink(this);
	ClearLog();
	for (auto& i : Items)
	{
		free(i);
	}
	for (int i = 0; i < History.Size; i++)
		free(History[i]);
}

// Portable helpers

int Console::Stricmp(const char* s1, const char* s2)
{
	int d;
	while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
	{
		s1++;
		s2++;
	}
	return d;
}

int Console::Strnicmp(const char* s1, const char* s2, int n)
{
	int d = 0;
	while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
	{
		s1++;
		s2++;
		n--;
	}
	return d;
}

char* Console::Strdup(const char* s)
{
	IM_ASSERT(s);
	size_t len = strlen(s) + 1;
	void*  buf = malloc(len);
	IM_ASSERT(buf);
	return (char*)memcpy(buf, (const void*)s, len);
}

void Console::Strtrim(char* s)
{
	char* str_end = s + strlen(s);
	while (str_end > s && str_end[-1] == ' ') str_end--;
	*str_end = 0;
}

void Console::ClearLog()
{
	for (int i = 0; i < Items.Size; i++)
		free(Items[i]);
	Items.clear();
}

void Console::Draw(const char* title, bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}

	// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
	// So e.g. IsItemHovered() will return true when hovering the title bar.
	// Here we create a context menu only available from the title bar.
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Close Console"))
			*p_open = false;
		ImGui::EndPopup();
	}

	//ImGui::TextWrapped("Enter 'HELP' for help.");

	// TODO: display items starting from the bottom

	//if (ImGui::SmallButton("Clear"))           { ClearLog(); }
	ImGui::SameLine();
	bool copy_to_clipboard = false; // ImGui::SmallButton("Copy");
									//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

	//ImGui::Separator();

#if 0
        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
#endif
	ImGui::Separator();

	// Reserve enough left-over height for 1 separator + 1 input text
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}

	// Display every line as a separate entry so we can change their color or add custom widgets.
	// If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
	// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
	// to only process visible items. The clipper will automatically measure the height of your first item and then
	// "seek" to display only items in the visible area.
	// To use the clipper we can replace your standard loop:
	//      for (int i = 0; i < Items.Size; i++)
	//   With:
	//      ImGuiListClipper clipper;
	//      clipper.Begin(Items.Size);
	//      while (clipper.Step())
	//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
	// - That your items are evenly spaced (same height)
	// - That you have cheap random access to your elements (you can access them given their index,
	//   without processing all the ones before)
	// You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
	// We would need random-access on the post-filtered list.
	// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
	// or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
	// and appending newly elements as they are inserted. This is left as a task to the user until we can manage
	// to improve this example code!
	// If your items are of variable height:
	// - Split them into same height items would be simpler and facilitate random-seeking into your list.
	// - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
	if (copy_to_clipboard)
		ImGui::LogToClipboard();
	for (int i = 0; i < Items.Size; i++)
	{
		const char* item = Items[i];
		if (!Filter.PassFilter(item))
			continue;

		float		Color[4] = {1, 1, 1, 1};
		auto		text	 = std::string_view(item);
		const char* c;
		const char* end = text.data() + text.size();
		ImVec4		cur_c(1, 1, 1, 1);
		//ImGui::PushTextWrapPos();
		struct Chunk
		{
			std::string_view str;
			ImVec4			 color;
		};
		std::vector<Chunk> chunks;
		c						= text.data();
		const char* begin_chunk = c;
		const char* end_chunk	= c;
		ImVec4		color_chunk(1, 1, 1, 1);
		for (; c != end; c++)
		{
			// Normally you would store more information in your item than just a string.
			// (e.g. make Items[] an array of structure, store color/type etc.)
			ImVec4 color;
			bool   has_color = false;
			switch (*c)
			{
			case '\n':
			{
				continue;
			}
			case '$':
			{
				if ((c + 1) != end)
				{
					if (isdigit(*(++c)))
					{
						chunks.push_back({std::string_view(begin_chunk, end_chunk - begin_chunk), color_chunk});
						begin_chunk = end_chunk = c + 1;
						has_color				= true;
						int	   ColorIndex		= *c - '0';
						ColorB newColor			= ColorTable[ColorIndex].color;
						Color[1]				= newColor.g / 255.f;
						Color[2]				= newColor.b / 255.f;
						Color[3]				= 1.f;
						Color[0]				= newColor.r / 255.f;
#if 1
						color_chunk = ImVec4(
#endif
#if 1
							Color[0], // green
							Color[1], // blue
							Color[2], //alpha
							Color[3]  // red
#else
//1.f,0.f,255.f,0.f
#endif
						);
						continue;
					}
				}
			}
			default:
			{
				end_chunk++;
			}
			}
		}
		chunks.push_back({std::string_view(begin_chunk, end_chunk - begin_chunk), color_chunk});
		for (auto& chunk : chunks)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, chunk.color);
			ImGui::TextUnformatted(chunk.str.data(), chunk.str.data() + chunk.str.length());
			ImGui::SameLine(0, 0);
			ImGui::PopStyleColor();
		}
		ImGui::TextUnformatted("");
		//ImGui::PopTextWrapPos();
	}
	if (copy_to_clipboard)
		ImGui::LogFinish();

	if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		ImGui::SetScrollHereY(1.0f);
	ScrollToBottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	// Command-line
	bool				reclaim_focus	 = false;
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
	{
		char* s = InputBuf;
		Strtrim(s);
		if (s[0])
			ExecCommand(s);
		strcpy(s, "");
		reclaim_focus = true;
		gEnv->pConsole->ResetAutoCompletion();
	}

	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();
}

void Console::ExecCommand(const char* command_line)
{
	AddLog("# %s\n", command_line);
	if (strnicmp(command_line, "clear", 5) == 0)
	{
		ClearLog();
	}

	// Insert into history. First find match and delete it so it can be pushed to the back.
	// This isn't trying to be smart or optimal.
	HistoryPos = -1;
	for (int i = History.Size - 1; i >= 0; i--)
		if (Stricmp(History[i], command_line) == 0)
		{
			free(History[i]);
			History.erase(History.begin() + i);
			break;
		}
	History.push_back(Strdup(command_line));
	gEnv->pConsole->ExecuteString(command_line);
	gEnv->pConsole->AddCommandToHistory(command_line);
	// On command input, we scroll to bottom even if AutoScroll==false
	ScrollToBottom = true;
}

// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks

int Console::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	Console* console = (Console*)data->UserData;
	return console->TextEditCallback(data);
}

int Console::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackAlways:
	{
		gEnv->pConsole->ResetAutoCompletion();
		return 0;
	}
	case ImGuiInputTextFlags_CallbackCompletion:
	{
		// Example of TEXT COMPLETION

		// Locate beginning of current word
		const char* word_end   = data->Buf + data->CursorPos;
		const char* word_start = word_end;
		while (word_start > data->Buf)
		{
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		string		m_command(word_start, word_end - word_start);
		const char* pCompletion;
		pCompletion = gEnv->pConsole->ProcessCompletion(m_command.c_str());

		if (pCompletion)
		{
			m_command = pCompletion;
			AddLog(m_command.c_str());
		}
		return 0;

		// Build a list of candidates
		ImVector<const char*> candidates;
		for (int i = 0; i < Commands.Size; i++)
			if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
				candidates.push_back(Commands[i]);

		if (candidates.Size == 0)
		{
			// No match
			AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
		}
		else if (candidates.Size == 1)
		{
			// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
			data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
			data->InsertChars(data->CursorPos, candidates[0]);
			data->InsertChars(data->CursorPos, " ");
		}
		else
		{
			// Multiple matches. Complete as much as we can..
			// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
			int match_len = (int)(word_end - word_start);
			for (;;)
			{
				int	 c						= 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				if (!all_candidates_matches)
					break;
				match_len++;
			}

			if (match_len > 0)
			{
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
			}

			// List matches
			AddLog("Possible matches:\n");
			for (int i = 0; i < candidates.Size; i++)
				AddLog("- %s\n", candidates[i]);
		}

		break;
	}
	case ImGuiInputTextFlags_CallbackHistory:
	{
		// Example of HISTORY
		const int prev_history_pos = HistoryPos;
		auto	  history_element  = gEnv->pConsole->GetHistoryElement(ImGuiKey_UpArrow == data->EventKey);
		// A better implementation would preserve the data on the current input line along with cursor position.
		if (history_element)
		{
			//const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_element);
		}
	}
	}
	return 0;
}

// Inherited via IOutputPrintSink

void Console::Print(const char* inszText)
{
	auto lock = std::lock_guard(ConsoleLock);
	AddLog(inszText);
}

void Console::OnEditorNotifyEvent(EEditorNotifyEvent event)
{
	switch (event)
	{
	case eNotify_OnInit:
		break;
	case eNotify_OnQuit:
		break;
	case eNotify_OnIdleUpdate:
		static bool p_open;
		Draw("Console", &p_open);
		break;
	default:
		break;
	}
}

}
