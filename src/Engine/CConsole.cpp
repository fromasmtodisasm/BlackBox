#include <BlackBox/CConsole.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Utils.hpp>
#include <BlackBox/Render/IFont.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Game/CGame.hpp>

#include <string>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <locale>

#include <glm/glm.hpp>

class HelpCommand : public IEditCommand 
{
public:
	HelpCommand()
	{
		
	}
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		for (auto &cmd : cd.args)
			GetIEngine()->getIConsole()->Help(wstr_to_str(cmd).c_str());
		if (cd.args.size() == 0)
			GetIEngine()->getIConsole()->Help(nullptr);
		return true;
	}
};
class SetCommand : public IEditCommand 
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

class GetCommand : public IEditCommand 
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

class DumpCommand : public IEditCommand 
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
}

void CConsole::Update()
{
}

void CConsole::Draw()
{
	if (!isOpened) return;
	auto deltatime = GetIEngine()->getIGame()->getDeltaTime();
	auto render = GetIEngine()->getIRender();
	height = render->GetHeight() / 2;
	Animate(deltatime, render);
	int begin, end;
	auto prompt = getPrompt();
	time += GetIEngine()->getIGame()->getDeltaTime();
	render->DrawImage(0, 0, render->GetWidth(), height, m_Texture->id, time * r_anim_speed->GetFVal(), 0, m_Texture->width, height, 0, 0, 0, 1.0);
	CalcMetrics(end);
	m_Font->SetXPos(0);
	m_Font->SetYPos(18);
	for (on_line = 0; current_line < end; current_line++, on_line++)
	{
		printLine(current_line);
	}
	for (auto& element : prompt)
	{
		printText(element, line_count - 1);
	}
	printText(Text(std::string(command_text + "\n"), textColor, 1.0f), 0);
	/*m_Font->RenderText(
		command_text + "\n",
		m_Font->GetXPos(), height / 2 - line_count * line_height - line_height, 1.0f, textColor);
	*/
}

void CConsole::Animate(float deltatime, IRender* render)
{
	if (animate = false)
	{
		curr_height += curr_speed * deltatime;
		if (curr_height >= height)
		{
			height = render->GetHeight();
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

void CConsole::CalcMetrics(int& end)
{
	line_in_console = (int)((height)) / (int)line_height;
	int num_all_lines = cmd_buffer.size();
	if (line_in_console > num_all_lines)
	{
		current_line = 0;
		line_count = num_all_lines;
		end = num_all_lines;
	}
	else
	{
		current_line = num_all_lines - line_in_console - 1;
		line_count = line_in_console;
		end = num_all_lines;
	}
}

void CConsole::AddCommand(const char* sName, IEditCommand* command, const char* help)
{
	CommandInfo cmdInfo;
	cmdInfo.Command = command;
	if (help) cmdInfo.help = help;
	m_Commands[str_to_wstr(std::string(sName))] = cmdInfo;
}

void CConsole::ExecuteString(const char* command)
{
	handleCommand(str_to_wstr(std::string(command)));
}

bool CConsole::OnInputEvent(sf::Event& event)
{
	if (!isOpened)
		return false;
	std::vector<std::wstring> completion;
	//m_World->getActiveScene()->setPostProcessor(postProcessors[4]);
	
		if (cmd_is_compete)
			command.clear();
		cmd_is_compete = false;
		input_trigered = false;

	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Tab:
			completion = autocomplete(command);
			if (completion.size() > 0)
			{
				command.clear();
				addToCommandBuffer(completion);
			}
			return true;
		case sf::Keyboard::Enter:
		{
			cmd_is_compete = true;
			CommandLine cmd;
			for (auto& element : getPrompt())
			{
				cmd.push_back(element);
			}
			cmd.push_back(Text(wstr_to_str(command) + "\n", textColor, 1.0));
			cmd_buffer.push_back(cmd);
			//m_World->getActiveScene()->setPostProcessor(nullptr);
			return handleCommand(command);
		}
		case sf::Keyboard::Insert:
		{
			if (event.key.shift == true)
			{
				std::wstring clipboard = sf::Clipboard::getString();
				if (clipboard.size() != 0)
				{
					for (auto& ch : clipboard)
					{
						handleCommandTextEnter(ch);
					}
				}
				return true;

			}
			return false;
		}
		default:
			return false;
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
		auto pVar = m_variables_map.find(name);
		if (pVar != m_variables_map.end())
		{
			switch (pVar->second->GetType())
			{
			case CVAR_INT:
				pVar->second->Set(static_cast<int>(std::atoi(value.c_str())));
				break;
			case CVAR_FLOAT:
				pVar->second->Set(static_cast<float>(std::atof(value.c_str())));
				break;
			case CVAR_STRING:
				pVar->second->Set(static_cast<const char*>(value.c_str()));
				break;
			default:
				PrintLine("Unknown type for [%s] variable", name.c_str());
			}
		}
		else
		{
			PrintLine("Variable [%s] not found. Creating", name.c_str());
			CreateVariable(name.c_str(), value.c_str(), 0);
		}


	}
}

void CConsole::Get(CommandDesc& cd)
{
	if (cd.args.size() == 1)
	{
		auto name = wstr_to_str(cd.args[0]);
		auto var = m_variables_map.find(name);
		if (var != m_variables_map.end())
		{
			auto pVar = var->second;
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
		else
		{
			PrintLine("Variable [%s] not found", name.c_str());
		}


	}
}

void CConsole::Dump()
{
	DumpCVars(this, 0);
}

void CConsole::DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter)
{
	for (auto& var : m_variables_map)
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

ICVar* CConsole::GetCVar(const char* name, const bool bCaseSensitive)
{
	std::string data = name;
	ICVar* pVar = nullptr;
	if (!bCaseSensitive)
	{
		std::transform(data.begin(), data.end(), data.begin(),
			[](unsigned char c) { return std::tolower(c); });
	}
	auto var = m_variables_map.find(name);
	if (var != m_variables_map.end())
	{
		pVar = var->second;
	}
	return pVar;
}

bool CConsole::Init()
{
	m_Font = new FreeTypeFont();
	m_Font->Init("arial.ttf", 16, line_height);
	m_Texture = new Texture();
	const char* texture_path = "console_background2.jpg";
	ICVar* background = GetCVar("console_background");
	r_anim_speed = CreateVariable("r_anim_speed", 0.1f, 0);

	if (background != nullptr)
		texture_path = background->GetString();
	m_Texture->load(texture_path);
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
	m_variables_map[sName] = var;
	return var;
}

ICVar* CConsole::CreateVariable(const char* sName, int iValue, int nFlags, const char* help)
{
	ICVar* var = new CCVar(strdup(sName), iValue, const_cast<char*>(help));
	if (var == nullptr) return var;
	m_variables_map[sName] = var;
	return var;

}

ICVar* CConsole::CreateVariable(const char* sName, float fValue, int nFlags, const char* help)
{
	ICVar* var = new CCVar(strdup(sName), fValue, const_cast<char*>(help));
	if (var == nullptr) return var;
	m_variables_map[sName] = var;
	return var;
}

void CConsole::handleCommandTextEnter(uint32_t ch)
{
	if (ch == 8)
	{
		if (command.size() > 0) command.pop_back();
	}
	else
	{
		if (iswgraph(ch) || (iswblank(ch) && ch != '\t'))
			command += ch;
	}
	fillCommandText();
}

bool CConsole::handleCommand(std::wstring command)
{
	bool result = false;
	auto cd = parseCommand(command);
	//cd.history = &history;

	auto cmd_it = m_Commands.find(cd.command);

	if (cmd_it != m_Commands.end())
		result = cmd_it->second.Command->execute(cd);
	else if (cd.command == L"close")
		isOpened = false;
	//history.push_back(str_to_wstr(getPrompt()) + command);
	return result;
}

CommandDesc CConsole::parseCommand(std::wstring& command)
{
	enum {COMMAND, ARGS, INCMD, INSPACE, INARGSPACE, INARG} state1 = INSPACE, state2;
	CommandDesc cd;
	int begin_cmd = 0, end_cmd = 0;
	int begin_args = 0, end_args = 0;
	std::wstring current_arg;
	
	for (int i = begin_cmd; i < command.size(); i++)
	{
		switch (state1)
		{
		case COMMAND:
			if (command[i] != L' ')
				cd.command += command[i];
			else
			{
				state1 = INARGSPACE;	
			}
			break;
		case ARGS:
			if (command[i] != L' ')
				current_arg += command[i];
			else
			{
				state1 = INARGSPACE;	
				cd.args.push_back(current_arg);
				current_arg.clear();
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
				current_arg += command[i];
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
{
	m_engine = GetIEngine();
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
	if (m_Texture) delete m_Texture;
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
	int pos = 0;
	command_text.clear();
	for (auto ch : command)
	{
		command_text.push_back(ch);
	}
	command_text.push_back('_');
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
		Text(" FPS: " + std::to_string(GetIEngine()->getIGame()->getFPS()) + "\n# ", glm::vec3(1.0, 0.3, 0.5), 1.0),
	};
}

void CConsole::printLine(int line)
{
	int i = 0;
	for (auto &element = cmd_buffer[line].begin(); element != cmd_buffer[line].end(); element++, i++)
	{
		printText(*element, line);
	}
}

void CConsole::printText(Text & element, int line)
{
	int curr_y = m_Font->GetYPos();
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
}

void CConsole::Help(const char *cmd)
{
	if (cmd != nullptr)
	{
		auto it = m_Commands.find(str_to_wstr(std::string(cmd)));
		if (it == m_Commands.end())
			return;
		cmd_buffer.push_back({ Text(std::string(cmd) + ": " + it->second.help + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
	}
	else
	{
		for (auto& cmd : m_Commands)
		{
			if (cmd.second.help.size() > 0)
				cmd_buffer.push_back({ Text(std::string(wstr_to_str(cmd.first)) + ": " + cmd.second.help + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
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
	if (type != CVAR_STRING)
		return;
	if (value.s != nullptr)
		delete[] value.s;
	value.s = strdup(s);
	type = CVAR_STRING;
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
