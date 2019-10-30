#include <BlackBox/Console.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Utils.hpp>
#include <BlackBox/Render/IFont.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Game/Game.hpp>

#include <string>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <functional>

#include <glm/glm.hpp>

#define strdup _strdup

bool isnumber(const char* s)
{
	auto p = s;
	while (*p)
	{
		if (!isdigit(*p++))
			return false;
	}
	return true;
}

void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);

	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

void findAndReplaceAll(std::string& data, std::string toSearch, std::function<std::string(int)> replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);

	// Repeat till end is reached
	int n = 0;
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		if ((pos + 1) < data.length() && std::isdigit(data[pos + 1]))
		{
			data.replace(pos, toSearch.size() + 1, replaceStr(data[pos + 1] - '0'));
			n++;
		}
		else
		{
			data.replace(pos, toSearch.size(), "");
		}
		// Get the next occurrence from the current position
		pos = data.find(toSearch, ++pos);
	}
}

class HelpCommand : public IConsoleCommand 
{
public:
	HelpCommand()
	{
		
	}
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		for (auto &cmd : cd.args)
			GetISystem()->GetIConsole()->Help(wstr_to_str(cmd).c_str());
		if (cd.args.size() == 0)
			GetISystem()->GetIConsole()->Help(nullptr);
		return true;
	}
};
class SetCommand : public IConsoleCommand 
{
	CConsole* m_Console;
public:
	SetCommand(CConsole *console) : m_Console(console)
	{
		
	}
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		m_Console->Set(cd);
		return true;
	}
};

class GetCommand : public IConsoleCommand 
{
	CConsole* m_Console;
public:
	GetCommand(CConsole *console) : m_Console(console)
	{
		
	}
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		m_Console->Get(cd);
		return true;
	}
};

class DumpCommand : public IConsoleCommand 
{
	CConsole* m_Console;
public:
	DumpCommand(CConsole *console) : m_Console(console)
	{
		
	}
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		m_Console->Dump();
		return true;
	}
};


void CConsole::SetImage(ITexture* pTexture)
{
  if (m_pBackGround != nullptr)
    delete m_pBackGround;
  m_pBackGround = pTexture;
}

void CConsole::Update()
{
	for (const auto& worker : m_workers)
	{
		worker->OnUpdate();
	}


	//=====================
	for (auto& worker : m_worker_to_delete)
	{
		m_workers.erase(worker);
	}
	m_worker_to_delete.clear();
}

void CConsole::Draw()
{
	if (!isOpened) return;
	auto deltatime = GetISystem()->GetIGame()->getDeltaTime();
	auto render = GetISystem()->GetIRender();
	height = (float)(render->GetHeight()) / 2;
	Animate(deltatime, render);
	size_t end;
	auto prompt = getPrompt();
	time += GetISystem()->GetIGame()->getDeltaTime();
	render->SetRenderTarget(0);
	render->DrawImage(0, 0, (float)render->GetWidth(), height, m_pBackGround->getId(), time * r_anim_speed->GetFVal(), 0, 0, 0, 0, 0, 0, transparency);
	CalcMetrics(end);
	m_Font->SetXPos(0);
	m_Font->SetYPos(16);
	for (on_line = 0; current_line < end; current_line++, on_line++)
	{
		printLine(current_line);
	}
	for (auto& element : prompt)
	{
		printText(element, line_count - 1);
	}
	char *cursor = needShowCursor() ? "*" : " ";
	//command_text.replace(command_text.size() - 1, 1, 1, cursor);
	//command_text[command.length()] = cursor;

	//printText(Text(std::string("cursor:<" + std::string(cursor) + ">\n"), textColor, 1.0f), 0);
	printText(Text(std::string("#"), glm::vec3(1.0, 0.3, 0.5), 1.0), 0);
	printText(Text(std::string(command_text), textColor, 1.0f), 0);
	drawCursor();
	/*m_Font->RenderText(
		command_text + "\n",
		m_Font->GetXPos(), height / 2 - line_count * line_height - line_height, 1.0f, textColor);
	*/
}

void CConsole::Animate(float deltatime, IRender* render)
{
  animate = false;
	if (animate)
	{
		curr_height += curr_speed * deltatime;
		if (curr_height >= height)
		{
			height = static_cast<float>(render->GetHeight());
			animate = false;
			curr_speed = speed;
			curr_height = 0.0f;
		}
		else
		{
			height = curr_height;
			curr_speed -= gravity * deltatime;
		}
	}
}

void CConsole::CalcMetrics(size_t& end)
{
	constexpr int MAGIC = 1;
	line_in_console = (int)((height)) / (int)line_height;
	auto num_all_lines = cmd_buffer.size();
	if (line_in_console > num_all_lines)
	{
		current_line = 0;
		line_count = num_all_lines;
		end = num_all_lines;
	}
	else
	{
		current_line = num_all_lines - line_in_console + MAGIC;
    if (page_up && current_line > 0)
      current_line--;
    else if (page_dn && current_line < cmd_buffer.size() - line_in_console)
    {
      current_line++;
    }
		line_count = line_in_console - MAGIC;
		end = num_all_lines;
	}
  page_up = false;
  page_dn = false;
}

void CConsole::AddCommand(const char* sName, IConsoleCommand* command, const char* help)
{
	CommandInfo cmdInfo;
	cmdInfo.Command = command;
	if (help) cmdInfo.help = help;
	cmdInfo.type = CommandInfo::Type::INTERFACE;
	m_Commands[str_to_wstr(std::string(sName))] = cmdInfo;
}

void CConsole::ExecuteString(const char* command)
{
	handleCommand(str_to_wstr(std::string(command)));
}

bool CConsole::OnInputEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		if (event.key.control)
		{
			auto it = m_keyBind.find(event.key.code);
			if (it != m_keyBind.end())
			{
				return handleCommand(it->second);
			}
		}
	if (!isOpened)
	{
		return false;
	}
	std::vector<std::wstring> completion;
	//m_World->getActiveScene()->setPostProcessor(postProcessors[4]);
	
	if (cmd_is_compete)
	{
		//SetInputLine("");
		ClearInputLine();
	}
	cmd_is_compete = false;
	input_trigered = false;

	switch (event.type)
	{
	case sf::Event::KeyPressed:
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Tab:
			completion = autocomplete(command);
			if (completion.size() > 0)
			{
				if (completion.size() == 1)
				{
					completeCommand(completion);
				}
				else
				{
					//SetInputLine("");
					//ClearInputLine();
					addToCommandBuffer(completion);
				}
			}
			return true;
		case sf::Keyboard::Num6:
		{
			if (event.key.control)
			{
				cursor.x = 0;
			}
			return true;
		}
		case sf::Keyboard::Num4:
		{
			if (event.key.control)
			{
				cursor.x = command.size();
			}
			return true;
		}
		case sf::Keyboard::Enter:
		case sf::Keyboard::M:
		{
			if (event.key.code != sf::Keyboard::Enter && !event.key.control)
				return false;
			handleEnterText(); 
			cursor.x = 0;
			return true;
		}
		case sf::Keyboard::Insert:
		{
			if (event.key.shift == true)
			{
				setBuffer();
				return true;
			}
			else if (event.key.control == true)
			{
				getBuffer();
			}

			return false;
		}
		case sf::Keyboard::Escape:
		{
			SetInputLine("");
			return true;
		}
		case sf::Keyboard::P:
		{
			if (event.key.control)
			{
				if (history_line > 0)
					--history_line;
				getHistoryElement();
				return true;
			}
			return false;
		}
		case sf::Keyboard::N:
		{
			if (event.key.control)
			{
				if (history_line < (cmd_buffer.size() - 1))
					++history_line;
				getHistoryElement();
				return true;
			}
			return false;
		}
		case sf::Keyboard::PageUp:
		case sf::Keyboard::PageDown:
		{
      pageUp(event.key.code == sf::Keyboard::PageUp);
      return true;
		}
		case sf::Keyboard::Left:
		case sf::Keyboard::Right:
		{
			moveCursor(event.key.code == sf::Keyboard::Left);
			return true;
		}
		case sf::Keyboard::L:
		case sf::Keyboard::B:
		{
			if (event.key.alt)
			{
				moveCursor(event.key.code == sf::Keyboard::B, true);
				return true;
			}
			return false;
		}
		case sf::Keyboard::Delete:
		{
			//TODO: rewrite erasing
			command.erase((int)cursor.x, 1);
			fillCommandText();
			return true;
		}
		default:
			return false;
		}

	}
		
	case sf::Event::TextEntered:
	{
		handleCommandTextEnter(event.text.unicode);
		return true;
	}
	default:
		return false;
	}

}

void CConsole::getHistoryElement()
{
	SetInputLine("");
	if (cmd_buffer.size() < 1)
		return;
	auto line_history = cmd_buffer[history_line];
	for (auto& element : line_history)
	{
		for (auto& ch : element.data)
		{
			handleCommandTextEnter(ch);
		}
	}
}

void CConsole::completeCommand(std::vector<std::wstring>& completion)
{
	SetInputLine("");
	for (auto& ch : completion[0])
	{
		handleCommandTextEnter(ch);
	}
	command += L" ";
}

void CConsole::setBuffer()
{
	std::wstring clipboard = sf::Clipboard::getString();
	if (clipboard.size() != 0)
	{
		for (auto& ch : clipboard)
		{
			handleCommandTextEnter(ch);
		}
	}
}

bool CConsole::handleEnterText()
{
	cmd_is_compete = true;
	CommandLine cmd;
	/*for (auto& element : getPrompt())
	{
		cmd.push_back(element);
	}*/
	cmd.push_back(Text(wstr_to_str(command) + "\n", textColor, 1.0));
	cmd_buffer.push_back(cmd);
	history_line = cmd_buffer.size();
	return handleCommand(command);
}

void CConsole::addToCommandBuffer(std::vector<std::wstring>& completion)
{
	cmd_buffer.push_back(getPrompt());
	cmd_buffer.push_back({ Text(std::string("\n"), textColor, 1.0f) });
	for (auto& cmd : completion)
	{
		addText(cmd);
	}
}

void CConsole::addText(std::wstring& cmd)
{
	cmd_buffer.push_back({ Text(wstr_to_str(cmd) + "\n", textColor, 1.0f) });
}

void CConsole::Set(CommandDesc& cd)
{
	if (cd.args.size() == 2)
	{
		auto name = wstr_to_str(cd.args[0]);
		auto value = wstr_to_str(cd.args[1]);
		auto var = m_mapVariables.find(name);
		if (var != m_mapVariables.end())
		{
			for (auto onChanger : varSinks)
			{
				if (onChanger->OnBeforeVarChange(var->second, value.c_str()))
				{
					return;
				}
			}
			SetInternal(var->second, value, name);
		}
		else
		{
			//PrintLine("Variable [%s] not found. Creating", name.c_str());
			CreateVariable(name.c_str(), value.c_str(), 0);
		}
	}
}

void CConsole::SetInternal(ICVar* pVar, std::string& value, std::string& name)
{
	switch (pVar->GetType())
	{
	case CVAR_INT:
		pVar->Set(static_cast<int>(std::atoi(value.c_str())));
		break;
	case CVAR_FLOAT:
		pVar->Set(static_cast<float>(std::atof(value.c_str())));
		break;
	case CVAR_STRING:
		pVar->Set(static_cast<const char*>(value.c_str()));
		break;
	default:
		PrintLine("Unknown type for [%s] variable", name.c_str());
	}
}

void CConsole::Get(CommandDesc& cd)
{
	if (cd.args.size() == 1)
	{
		auto name = wstr_to_str(cd.args[0]);
		auto var = m_mapVariables.find(name);
		if (var != m_mapVariables.end())
		{
			GetInternal(var->second, name);
		}
		else
		{
			PrintLine("Variable [%s] not found", name.c_str());
		}


	}
}

void CConsole::GetInternal(ICVar* pVar, std::string& name)
{
	switch (pVar->GetType())
	{
	case CVAR_INT:
		PrintLine("Variable = [%d]", pVar->GetIVal());
		break;
	case CVAR_FLOAT:
		PrintLine("Variable = [%f]", pVar->GetFVal());
		break;
	case CVAR_STRING:
		PrintLine("Variable = [%s]", pVar->GetString());
		break;
	default:
		PrintLine("Unknown type for [%s] variable", name.c_str());
	}
}

void CConsole::Dump()
{
	DumpCVars(this, 0);
}

void CConsole::getBuffer()
{
	std::string toClipBoard = "";
	for (auto& cmd_line : cmd_buffer)
	{
		for (auto& cmd : cmd_line)
		{
			toClipBoard += cmd.data;
		}

	}
	sf::Clipboard::setString(toClipBoard);
}

bool CConsole::needShowCursor()
{
	float dt = GetISystem()->GetIGame()->getDeltaTime();
	/*
	if (cursor_tick_tack)
		cursor_tick += dt;
	else
		cursor_tack += dt;
	if (cursor_tick - cursor_tack >= 0.9f)
	{
		cursor_tack = cursor_tick;
		cursor_tick_tack = false;
	}
	else
	{
		cursor_tick = cursor_tack;
		cursor_tick_tack = true;
	}
	*/	

	blinking += dt;

	if (blinking >= blinkTime->GetFVal())
	{
		blinking = 0.0f;
		needDrawCursor = !needDrawCursor;
	}
	return needDrawCursor;
}

void CConsole::pageUp(bool isPgUp)
{
  if (isPgUp)
    page_up = true;
  else
    page_dn = true;
}

void CConsole::drawCursor()
{
	if (!needShowCursor())
		return;
	auto curr_y = m_Font->GetYPos();
	m_Font->RenderText(
		cursor.data,
		m_Font->CharWidth('#') + m_Font->TextWidth(command_text.substr(0,static_cast<int>(cursor.x))), curr_y, 1.0f, &glm::vec4(cursor.color, 1.0)[0]);
}

void CConsole::moveCursor(bool left, bool wholeWord)
{
	if (left)
	{
		if (wholeWord)
		{
			std::size_t found = command_text.rfind(" ", cursor.x - 1);
			if (found != std::string::npos)
				cursor.x = std::min((size_t)cursor.x, found - 1);
		}
		else
		{
			cursor.x = std::max(0, (int)cursor.x - 1);
		}
	}
	else
	{
		if (wholeWord)
		{
			std::size_t found = command_text.find_first_of(" ", cursor.x);
			if (found != std::string::npos)
				cursor.x = std::min(command_text.size(), found + 1);
			else
				cursor.x = command_text.size();

		}
		else
		{
			cursor.x = std::min((int)command.size(), (int)cursor.x + 1);
		}
	}
}

void CConsole::initBind()
{
	{
		m_str2key[std::string("A")] = sf::Keyboard::A;
		m_str2key[std::string("B")] = sf::Keyboard::B;
		m_str2key[std::string("C")] = sf::Keyboard::C;
		m_str2key[std::string("D")] = sf::Keyboard::D;
		m_str2key[std::string("E")] = sf::Keyboard::E;
		m_str2key[std::string("F")] = sf::Keyboard::F;
		m_str2key[std::string("G")] = sf::Keyboard::G;
		m_str2key[std::string("H")] = sf::Keyboard::H;
		m_str2key[std::string("I")] = sf::Keyboard::I;
		m_str2key[std::string("J")] = sf::Keyboard::J;
		m_str2key[std::string("K")] = sf::Keyboard::K;
		m_str2key[std::string("L")] = sf::Keyboard::L;
		m_str2key[std::string("M")] = sf::Keyboard::M;
		m_str2key[std::string("N")] = sf::Keyboard::N;
		m_str2key[std::string("O")] = sf::Keyboard::O;
		m_str2key[std::string("P")] = sf::Keyboard::P;
		m_str2key[std::string("Q")] = sf::Keyboard::Q;
		m_str2key[std::string("R")] = sf::Keyboard::R;
		m_str2key[std::string("S")] = sf::Keyboard::S;
		m_str2key[std::string("T")] = sf::Keyboard::T;
		m_str2key[std::string("U")] = sf::Keyboard::U;
		m_str2key[std::string("V")] = sf::Keyboard::V;
		m_str2key[std::string("W")] = sf::Keyboard::W;
		m_str2key[std::string("X")] = sf::Keyboard::X;
		m_str2key[std::string("Y")] = sf::Keyboard::Y;
		m_str2key[std::string("Z")] = sf::Keyboard::Z;
		m_str2key[std::string("Num0")] = sf::Keyboard::Num0;
		m_str2key[std::string("Num1")] = sf::Keyboard::Num1;
		m_str2key[std::string("Num2")] = sf::Keyboard::Num2;
		m_str2key[std::string("Num3")] = sf::Keyboard::Num3;
		m_str2key[std::string("Num4")] = sf::Keyboard::Num4;
		m_str2key[std::string("Num5")] = sf::Keyboard::Num5;
		m_str2key[std::string("Num6")] = sf::Keyboard::Num6;
		m_str2key[std::string("Num7")] = sf::Keyboard::Num7;
		m_str2key[std::string("Num8")] = sf::Keyboard::Num8;
		m_str2key[std::string("Num9")] = sf::Keyboard::Num9;
		m_str2key[std::string("Escape")] = sf::Keyboard::Escape;
		m_str2key[std::string("LControl")] = sf::Keyboard::LControl;
		m_str2key[std::string("LShift")] = sf::Keyboard::LShift;
		m_str2key[std::string("LAlt")] = sf::Keyboard::LAlt;
		m_str2key[std::string("LSystem")] = sf::Keyboard::LSystem;
		m_str2key[std::string("RControl")] = sf::Keyboard::RControl;
		m_str2key[std::string("RShift")] = sf::Keyboard::RShift;
		m_str2key[std::string("RAlt")] = sf::Keyboard::RAlt;
		m_str2key[std::string("RSystem")] = sf::Keyboard::RSystem;
		m_str2key[std::string("Menu")] = sf::Keyboard::Menu;
		m_str2key[std::string("LBracket")] = sf::Keyboard::LBracket;
		m_str2key[std::string("RBracket")] = sf::Keyboard::RBracket;
		m_str2key[std::string("Semicolon")] = sf::Keyboard::Semicolon;
		m_str2key[std::string("Comma")] = sf::Keyboard::Comma;
		m_str2key[std::string("Period")] = sf::Keyboard::Period;
		m_str2key[std::string("Quote")] = sf::Keyboard::Quote;
		m_str2key[std::string("Slash")] = sf::Keyboard::Slash;
		m_str2key[std::string("Backslash")] = sf::Keyboard::Backslash;
		m_str2key[std::string("Tilde")] = sf::Keyboard::Tilde;
		m_str2key[std::string("Equal")] = sf::Keyboard::Equal;
		m_str2key[std::string("Hyphen")] = sf::Keyboard::Hyphen;
		m_str2key[std::string("Space")] = sf::Keyboard::Space;
		m_str2key[std::string("Enter")] = sf::Keyboard::Enter;
		m_str2key[std::string("Backspace")] = sf::Keyboard::Backspace;
		m_str2key[std::string("Tab")] = sf::Keyboard::Tab;
		m_str2key[std::string("PageUp")] = sf::Keyboard::PageUp;
		m_str2key[std::string("PageDown")] = sf::Keyboard::PageDown;
		m_str2key[std::string("End")] = sf::Keyboard::End;
		m_str2key[std::string("Home")] = sf::Keyboard::Home;
		m_str2key[std::string("Insert")] = sf::Keyboard::Insert;
		m_str2key[std::string("Delete")] = sf::Keyboard::Delete;
		m_str2key[std::string("Add")] = sf::Keyboard::Add;
		m_str2key[std::string("Subtract")] = sf::Keyboard::Subtract;
		m_str2key[std::string("Multiply")] = sf::Keyboard::Multiply;
		m_str2key[std::string("Divide")] = sf::Keyboard::Divide;
		m_str2key[std::string("Left")] = sf::Keyboard::Left;
		m_str2key[std::string("Right")] = sf::Keyboard::Right;
		m_str2key[std::string("Up")] = sf::Keyboard::Up;
		m_str2key[std::string("Down")] = sf::Keyboard::Down;
		m_str2key[std::string("Numpad0")] = sf::Keyboard::Numpad0;
		m_str2key[std::string("Numpad1")] = sf::Keyboard::Numpad1;
		m_str2key[std::string("Numpad2")] = sf::Keyboard::Numpad2;
		m_str2key[std::string("Numpad3")] = sf::Keyboard::Numpad3;
		m_str2key[std::string("Numpad4")] = sf::Keyboard::Numpad4;
		m_str2key[std::string("Numpad5")] = sf::Keyboard::Numpad5;
		m_str2key[std::string("Numpad6")] = sf::Keyboard::Numpad6;
		m_str2key[std::string("Numpad7")] = sf::Keyboard::Numpad7;
		m_str2key[std::string("Numpad8")] = sf::Keyboard::Numpad8;
		m_str2key[std::string("Numpad9")] = sf::Keyboard::Numpad9;
		m_str2key[std::string("F1")] = sf::Keyboard::F1;
		m_str2key[std::string("F2")] = sf::Keyboard::F2;
		m_str2key[std::string("F3")] = sf::Keyboard::F3;
		m_str2key[std::string("F4")] = sf::Keyboard::F4;
		m_str2key[std::string("F5")] = sf::Keyboard::F5;
		m_str2key[std::string("F6")] = sf::Keyboard::F6;
		m_str2key[std::string("F7")] = sf::Keyboard::F7;
		m_str2key[std::string("F8")] = sf::Keyboard::F8;
		m_str2key[std::string("F9")] = sf::Keyboard::F9;
		m_str2key[std::string("F10")] = sf::Keyboard::F10;
		m_str2key[std::string("F11")] = sf::Keyboard::F11;
		m_str2key[std::string("F12")] = sf::Keyboard::F12;
		m_str2key[std::string("F13")] = sf::Keyboard::F13;
		m_str2key[std::string("F14")] = sf::Keyboard::F14;
		m_str2key[std::string("F15")] = sf::Keyboard::F15;
		m_str2key[std::string("Pause")] = sf::Keyboard::Pause;
	}
}

void CConsole::CreateKeyBind(const char* key, const char* cmd)
{
	auto it = m_str2key.find(key);
	if (it != m_str2key.end())
	{
		m_keyBind[it->second] = str_to_wstr(cmd);
	}
}

void CConsole::SetInputLine(const char* szLine)
{
	command = str_to_wstr(szLine);
	cursor.x = 0;
}

void CConsole::ClearInputLine()
{
	command.clear();
	cursor.x = 0;
}

void CConsole::AddCommand(const char* sCommand, ConsoleCommandFunc func, int nFlags/* = 0*/, const char* help/* = NULL*/)
{
	CommandInfo cmdInfo;
	cmdInfo.Func = func;
	if (help) cmdInfo.help = help;
	cmdInfo.type = CommandInfo::Type::FUNC;
	m_Commands[str_to_wstr(std::string(sCommand))] = cmdInfo;
}

void CConsole::AddWorkerCommand(IWorkerCommand* cmd)
{
	m_workers.insert(cmd);
}

void CConsole::RemoveWorkerCommand(IWorkerCommand* cmd)
{
	m_worker_to_delete.push_back(cmd);
}

void CConsole::UnregisterVariable(const char* sVarName, bool bDelete/* = false*/)
{
	//TODO: implement this
}

char* CConsole::Register(const char* name, const char** src, const char* defaultvalue, int flags, const char* help/* = ""*/)
{
	ICVar* pCVar(nullptr);
	auto it = m_mapVariables.find(name);
	if (it != m_mapVariables.end())
	{
		GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(float): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
		gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
		return 0;
	}
	/*
	if (!allowModify)
		nFlags |= VF_CONST_CVAR;
	*/
	pCVar = new CCVarRef(name, src, help);
	*src = defaultvalue;
	RegisterVar(pCVar/*, pChangeFunc*/);
	return 0;
}

float CConsole::Register(const char* name, float* src, float defaultvalue, int flags/* = 0*/, const char* help/* = ""*/)
{
	ICVar* pCVar(nullptr);
	auto it = m_mapVariables.find(name);
	if (it != m_mapVariables.end())
	{
		GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(float): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
		gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
		return 0;
	}
	/*
	if (!allowModify)
		nFlags |= VF_CONST_CVAR;
	*/
	pCVar = new CCVarRef(name, src, help);
	*src = defaultvalue;
	RegisterVar(pCVar/*, pChangeFunc*/);
	return 0.0f;
}

int CConsole::Register(const char* name, int* src, int defaultvalue, int flags/* = 0*/, const char* help/* = ""*/)
{
	ICVar* pCVar(nullptr);
	auto it = m_mapVariables.find(name);
	if (it != m_mapVariables.end())
	{
		GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(float): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
		gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
		return 0;
	}
	/*
	if (!allowModify)
		nFlags |= VF_CONST_CVAR;
	*/
	pCVar = new CCVarRef(name, src, help);
	*src = defaultvalue;
	RegisterVar(pCVar/*, pChangeFunc*/);
	return 0;
}

void CConsole::AddCommand(const char* sName, const char* sScriptFunc, const uint32_t indwFlags/* = 0*/, const char* help/* = ""*/)
{
	CommandInfo cmdInfo;
	cmdInfo.Script.code = sScriptFunc;
	if (help) cmdInfo.help = help;
	cmdInfo.type = CommandInfo::Type::SCRIPT;
	m_Commands[str_to_wstr(std::string(sName))] = cmdInfo;
}

void CConsole::DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter)
{
	for (auto& var : m_mapVariables)
	{
		OnElementFound(var.second);
	}
}

void CConsole::OnElementFound(ICVar* pCVar)
{
	
	auto name = pCVar->GetName();
	auto helpString = pCVar->GetHelp();
	auto help = helpString[0] != '\0' ? ". Help: " + std::string(helpString) : "";
	switch (pCVar->GetType())
	{
	case CVAR_INT:
		PrintLine("%s = %d%s", name, pCVar->GetIVal(), help.c_str());
		break;
	case CVAR_FLOAT:
		PrintLine("%s = %f%s", name, pCVar->GetFVal(), help.c_str());
		break;
	case CVAR_STRING:
		PrintLine("%s = %s%s", name, pCVar->GetString(), help.c_str());
		break;
	default:
		//PrintLine("Unknown type for [%s] variable", name.c_str());
		break;
	}

}

void CConsole::AddConsoleVarSink(IConsoleVarSink* pSink)
{
	varSinks.push_back(pSink);
}

void CConsole::RemoveConsoleVarSink(IConsoleVarSink* pSink)
{
	auto var = varSinks.begin();
	for (; var != varSinks.end(); var++)
	{
		if (*var == pSink)
		{
			break;
		}
	}
	if (var != varSinks.end())
	{
		varSinks.erase(var);
	}
}

ICVar* CConsole::GetCVar(const char* name, const bool bCaseSensitive)
{
	std::string data = name;
	ICVar* pVar = nullptr;
	if (!bCaseSensitive)
	{
		std::transform(data.begin(), data.end(), data.begin(),
			[](unsigned char c) { return std::tolower(c); });
	}
	auto var = m_mapVariables.find(name);
	if (var != m_mapVariables.end())
	{
		pVar = var->second;
	}
	return pVar;
}

bool CConsole::Init(ISystem* pSystem)
{
	m_pSystem = pSystem;
	m_pScriptSystem = pSystem->GetIScriptSystem();
	m_Font = new FreeTypeFont();
	m_Font->Init("arial.ttf", 16, static_cast<unsigned int>(line_height));
	m_pBackGround = new Texture();
	const char* texture_path = "console_background2.jpg";
	ICVar* background = GetCVar("console_background");
	r_anim_speed = CreateVariable("r_anim_speed", 0.1f, 0);
	blinkTime = CreateVariable("btime", 1.0f, 0, "Time of cursor blinking");

	if (background != nullptr)
		texture_path = background->GetString();
	m_pBackGround->load(texture_path);
	initBind();
	return true;
}

bool CConsole::IsOpened()
{
	return isOpened;
}

ICVar* CConsole::CreateVariable(const char* sName, const char* sValue, int nFlags, const char* help)
{
	ICVar* var = new CCVar(strdup(sName), strdup(sValue), const_cast<char*>(help));
	if (var == nullptr) return var;
	m_mapVariables[sName] = var;
	return var;
}

ICVar* CConsole::CreateVariable(const char* sName, int iValue, int nFlags, const char* help)
{
	ICVar* var = new CCVar(strdup(sName), iValue, const_cast<char*>(help));
	if (var == nullptr) return var;
	m_mapVariables[sName] = var;
	return var;

}

ICVar* CConsole::CreateVariable(const char* sName, float fValue, int nFlags, const char* help)
{
	ICVar* var = new CCVar(strdup(sName), fValue, const_cast<char*>(help));
	if (var == nullptr) return var;
	m_mapVariables[sName] = var;
	return var;
}

void CConsole::handleCommandTextEnter(uint32_t ch)
{
	if (ch == 8)
	{
		if (command.size() > 0 && (int)cursor.x > 0)
		{
			//command.pop_back();
			command.erase(std::max(0, (int)cursor.x - 1), 1);
			moveCursor(true);
		}
	}
	else
	{
		if (iswgraph(ch) || (iswblank(ch) && ch != '\t'))
		{
			command.insert((int)cursor.x, 1, ch);// += ch;
			moveCursor(false);
		}
	}
	fillCommandText();
}

bool CConsole::handleCommand(std::wstring command)
{

#if !defined(RELEASE) || defined(ENABLE_DEVELOPER_CONSOLE_IN_RELEASE)
	///////////////////////////
	//Execute as string
	if (command[0] == '#' || command[0] == '@')
	{
		if (/*!con_restricted || *//*isOpened*/true)      // in restricted mode we allow only VF_RESTRICTEDMODE CVars&CCmd
		{
			std::string str = wstr_to_str(command);
			//PrintLine(str.c_str());

			if (m_pSystem->IsDevMode())
			{
				if (m_pSystem->GetIScriptSystem())
					m_pSystem->GetIScriptSystem()->ExecuteBuffer(str.c_str() + 1, str.length() - 1);
				//m_bDrawCursor = 0;
			}
			else
			{
				// Warning.
				// No Cheat warnings. ConsoleWarning("Console execution is cheat protected");
			}
			return true;
		}
	}
#endif

	bool result = false;
	auto cd = parseCommand(command);
	//cd.history = &history;

	auto cmd_it = m_Commands.find(cd.command);

	if (cmd_it != m_Commands.end())
	{
		if (cmd_it->second.type == CommandInfo::Type::INTERFACE)
		{
			result = cmd_it->second.Command->execute(cd);
		}
		else if (cmd_it->second.type == CommandInfo::Type::SCRIPT)
		{
			std::string code(cmd_it->second.Script.code);
			findAndReplaceAll(code, "%", [&cd](int n) -> std::string {
				return "\"" + wstr_to_str(cd.get(n - 1)) + "\"";
			});
			result = m_pScriptSystem->ExecuteBuffer(code.c_str(), code.length());
		}
		else if (cmd_it->second.type == CommandInfo::Type::FUNC)
		{
			result = cmd_it->second.Func(cd);
		}

	}
	else
	{
		auto var_it = m_mapVariables.find(wstr_to_str(cd.command));
		if (var_it != m_mapVariables.end())
		{
			CommandDesc desc;
			desc.args.push_back(cd.command);
			if (cd.args.size() == 0)
			{
				Get(desc);
			}
			else if (cd.args.size() == 1)
			{
				desc.args.push_back(cd.args[0]);
				Set(desc);
			}
		}
		else if (cd.command == L"close")
			isOpened = false;
	}
	//history.push_back(str_to_wstr(getPrompt()) + command);
	return result;
}

void CConsole::RegisterVar(ICVar* pCVar)
{
	m_mapVariables[pCVar->GetName()] = pCVar;
}

CommandDesc CConsole::parseCommand(std::wstring& command)
{
	enum {COMMAND, ARGS, INCMD, INSPACE, INARGSPACE, INARG, INSTRING} state1 = INSPACE;
	CommandDesc cd;
	int begin_cmd = 0, end_cmd = 0;
	int begin_args = 0, end_args = 0;
	std::wstring current_arg;
	std::wstring value;
	bool get_value = false;

	auto getVal = [this](std::wstring name) -> std::wstring {
		auto str = wstr_to_str(name);

		if (auto var = GetCVar(str.c_str()))
		{
			return str_to_wstr(var->GetString());
		}
		return std::wstring();
	};
	
	for (int i = begin_cmd; i < command.size(); i++)
	{
		switch (state1)
		{
		case COMMAND:
			if (command[i] != L' ')
			{
				if (command[i] == L'"')
				{
					state1 = INSTRING;
					break;
				}
				cd.command += command[i];
			}
			else
			{
				state1 = INARGSPACE;	
			}
			break;
		case INSTRING:
		{
			if (command[i] != L'"')
				current_arg += command[i];
			else
				state1 = ARGS;
			break;
		}
		case ARGS:
			if (command[i] != L' ')
			{
				if (command[i] == L'"')
				{
					state1 = INSTRING;
					break;
				}
				if (get_value)
					value += command[i];
				else
					current_arg += command[i];
			}
			else
			{
				state1 = INARGSPACE;	
				if (get_value)
				{
					cd.args.push_back(getVal(value));
					value.clear();
					get_value = false;
				}
				else
				{
					cd.args.push_back(current_arg);
					current_arg.clear();
				}
			}
			break;
		case INCMD:
			break;
		case INSPACE:
			if (command[i] != L' ')
			{
				state1 = COMMAND;
				cd.command += command[i];
			}
			break;
		case INARGSPACE:
			if (command[i] != L' ')
			{
				state1 = ARGS;
				if (command[i] == L'@')
				{
					get_value = true;
				}
				else
				{
					if (command[i] == L'"')
					{
						state1 = INSTRING;
						break;
					}
					else current_arg += command[i];
				}
			}
			break;
		case INARG:
			break;
		default:
			break;
		}
	}
	if (state1 == ARGS)
	{
		if (get_value)
			cd.args.push_back(getVal(value));
		else
			cd.args.push_back(current_arg);
	}

	return cd;
}

std::vector<std::wstring> CConsole::autocomplete(std::wstring cmd)
{
	std::vector<std::wstring> completion;
	for (auto& curr_cmd : m_Commands)
	{
		if (curr_cmd.first.substr(0, cmd.size()) == cmd)
		{
			completion.push_back(curr_cmd.first);
		}
	}
	for (auto& cur_var : m_mapVariables)
	{
		if (cur_var.first.substr(0, cmd.size()) == wstr_to_str(cmd))
		{
			completion.push_back(str_to_wstr(cur_var.first));
		}
	}
	return completion;
}

void CConsole::doFile(std::ifstream& cfg)
{
	std::string line;
	while (std::getline(cfg, line))
	{
		handleCommand(str_to_wstr(line));
	}
	cfg.clear();
	cfg.seekg(0, std::ios::beg);
}

void CConsole::ExecuteFile(const char* file)
{
	auto it = scripts.find(file);
	if (it == scripts.end())
	{
		std::ifstream f(file);
		if (!f.is_open())
			return;
		doFile(f);
		scripts["file"] = std::move(f);
	}
}

CConsole::CConsole() 
	:
	m_pBackGround(nullptr)
{
	//prompt = user + " #";
	AddCommand("help", new HelpCommand());
	AddCommand("set", new SetCommand(this));
	AddCommand("get", new GetCommand(this));
	AddCommand("dump", new DumpCommand(this));
	message_buffer.resize(MESSAGE_BUFFER_SIZE);
}
CConsole::~CConsole()
{
	if (m_Font) delete m_Font;
	if (m_pBackGround) delete m_pBackGround;
}

void CConsole::ShowConsole(bool show)
{
	if (isOpened != show)
	{
		isOpened = show;
		if (show)
		{
			input_trigered = true;
			animate = true;
		}
	}
}

void CConsole::fillCommandText()
{
	command_text.clear();
	for (auto ch : command)
	{
		command_text.push_back(static_cast<char>(ch));
	}
	//command_text.push_back(cursor);
}

void CConsole::setFont(IFont* font)
{
	m_Font = font;
}

CommandLine CConsole::getPrompt()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string time_str = std::ctime(&time);
	time_str[time_str.size() - 1] = 0;
	return { 
		Text(user + "@" + pc, glm::vec3(0.0, 1.0, 0.0), 1.0) , 
		Text(" " + env, glm::vec3(1.0, 0.0, 1.0), 1.0) , 
		Text(" " + cd, glm::vec3(1.0, 1.0, 0.0), 1.0) , 
		Text(std::string(" " + time_str), promptColor, 1.0),
		Text(" FPS: " + std::to_string(GetISystem()->GetIGame()->getFPS()) + "\n", glm::vec3(1.0, 0.3, 0.5), 1.0),
	};
}

void CConsole::printLine(size_t line)
{
	auto i = line;
	for (auto &element = cmd_buffer[line].begin(); element != cmd_buffer[line].end(); element++, i++)
	{
		printText(*element, line);
	}
}

void CConsole::printText(Text & element, size_t line)
{
	auto curr_y = m_Font->GetYPos();
	m_Font->RenderText(
		element.data,
		m_Font->GetXPos(), curr_y, 1.0f, &glm::vec4(element.color, 1.0)[0]);
	if (curr_y < m_Font->GetYPos())
	{
		// TODO: handle this
	}
}

void CConsole::AddArgumentCompletion(const char* cmd, const char* arg, int n)
{
}

void CConsole::Clear()
{
	cmd_buffer.clear();
	cursor.x = 0;
	cursor.y = 0;
	history_line = 0;
}

void CConsole::Help(const char *cmd)
{
	if (cmd != nullptr)
	{
		auto name = str_to_wstr(std::string(cmd));
		auto it = m_Commands.find(name);
		const char* help = nullptr;
		if (it != m_Commands.end())
		{
			help = it->second.help.c_str();
		}
		else
		{
			auto it = m_mapVariables.find(cmd);
			if (it != m_mapVariables.end())
				help = it->second->GetHelp();
			else
				return;
		}

		cmd_buffer.push_back({ Text(std::string(cmd) + ": " + help + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
	}
	else
	{
		for (auto& cmd : m_Commands)
		{
			if (cmd.second.help.size() > 0)
				cmd_buffer.push_back({ Text(std::string(wstr_to_str(cmd.first)) + ": " + cmd.second.help + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
		}
		for (auto& var : m_mapVariables)
		{
			cmd_buffer.push_back({ Text(var.first + ": " + var.second->GetHelp() + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
		}
	}
}

void CConsole::PrintLine(const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  vsnprintf(const_cast<char*>(message_buffer.data()), MESSAGE_BUFFER_SIZE, format, ptr);
  va_end(ptr);

	addText(str_to_wstr(message_buffer));
}

char* CCVar::GetString()
{
	int res = 0;
	if (type == CVAR_INT)
		value.s = strdup(std::to_string(value.i).c_str());
	else if (type == CVAR_FLOAT)
		value.s = strdup(std::to_string(value.f).c_str());
	type = CVAR_STRING;
	return value.s;
}

void CCVar::Set(const char* s)
{
	// TODO: fix it, to check if <s> is number
	if (type == CVAR_STRING)
	{
		if (value.s != nullptr)
			delete[] value.s;
	}
	else if (isnumber(s))
	{
		value.i = std::atoi(s);
		type = CVAR_INT;
	}
	else
	{
		value.s = strdup(s);
		type = CVAR_STRING;
	}
}

void CCVar::ForceSet(const char* s)
{
	if (value.s != nullptr)
		delete[] value.s;
	type = CVAR_STRING;
	value.s = const_cast<char*>(s);
}

void CCVar::Set(float f)
{
	if (type == CVAR_STRING)
	{
		delete[] value.s;
	}
	type = CVAR_FLOAT;
	value.f = f;
}

void CCVar::Set(int i)
{
	if (type == CVAR_STRING)
	{
		delete[] value.s;
	}
	type = CVAR_INT;
	value.i = i;
}

void CCVar::Refresh()
{
	value.i = 0;
}

void CCVar::ClearFlags(int flags)
{
}

int CCVar::GetFlags()
{
	return 0;
}

int CCVar::SetFlags(int flags)
{
	return 0;
}

int CCVar::GetType()
{
	return type;
}

const char* CCVar::GetName()
{
	return name;
}

const char* CCVar::GetHelp()
{
	return help;
}

void CCVar::Release()
{
}

int CCVar::GetIVal()
{
	int res = 0;
	if (type == CVAR_FLOAT)
		value.i = static_cast<int>(value.f);
	else if (type == CVAR_STRING)
		value.i = static_cast<int>(std::atoi(value.s));
	type = CVAR_INT;
	return value.i;
}

float CCVar::GetFVal()
{
	int res = 0;
	if (type == CVAR_INT)
		value.f = static_cast<float>(value.i);
	else if (type == CVAR_STRING)
		value.f = static_cast<float>(std::atof(value.s));
	type = CVAR_FLOAT;
	return value.f;
}

void CCVarRef::Release()
{
}

int CCVarRef::GetIVal()
{
	int res = 0;
	if (type == CVAR_FLOAT)
		*value.i = static_cast<int>(*value.f);
	else if (type == CVAR_STRING)
		*value.i = static_cast<int>(std::atoi(*value.s));
	type = CVAR_INT;
	return *value.i;
}

float CCVarRef::GetFVal()
{
	int res = 0;
	if (type == CVAR_INT)
		*value.f = static_cast<float>(*value.i);
	else if (type == CVAR_STRING)
		*value.f = static_cast<float>(std::atof(*value.s));
	type = CVAR_FLOAT;
	return *value.f;
}

char* CCVarRef::GetString()
{
	int res = 0;
	if (type == CVAR_INT)
		*value.s = strdup(std::to_string(*value.i).c_str());
	else if (type == CVAR_FLOAT)
		*value.s = strdup(std::to_string(*value.f).c_str());
	type = CVAR_STRING;
	return *value.s;
}

void CCVarRef::Set(const char* s)
{
	if (type != CVAR_STRING)
		return;
	if (*value.s != nullptr)
		delete[] *value.s;
	*value.s = strdup(s);
	type = CVAR_STRING;
}

void CCVarRef::ForceSet(const char* s)
{
	if (value.s != nullptr)
		delete[] *value.s;
	type = CVAR_STRING;
	*value.s = const_cast<char*>(s);
}

void CCVarRef::Set(float f)
{
	if (type == CVAR_STRING)
	{
		delete[] *value.s;
	}
	type = CVAR_FLOAT;
	*value.f = f;
}

void CCVarRef::Set(int i)
{
	if (type == CVAR_STRING)
	{
		delete[] *value.s;
	}
	type = CVAR_INT;
	*value.i = i;
}


void CCVarRef::Refresh()
{
	*value.i = 0;
}

void CCVarRef::ClearFlags(int flags)
{
}

int CCVarRef::GetFlags()
{
	return 0;
}

int CCVarRef::SetFlags(int flags)
{
	return 0;
}

int CCVarRef::GetType()
{
	return type;
}

const char* CCVarRef::GetName()
{
	return name;
}

const char* CCVarRef::GetHelp()
{
	return help;
}
