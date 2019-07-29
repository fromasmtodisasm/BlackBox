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

#include <glm/glm.hpp>

class HelpCommand : IEditCommand 
{
public:
	HelpCommand();
private:
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		cd.history->clear();
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
	if (!isShow) return;
	auto deltatime = GetIEngine()->getIGame()->getDeltaTime();
	auto render = GetIEngine()->getIRender();
	float height = render->GetHeight();
	/*
	if (animate)
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
	*/
	int begin, end;
	auto prompt = getPrompt();
	render->DrawImage(0, 384, render->GetWidth(), 384, m_Texture->id, 0, 0, m_Texture->width, height, 0, 0, 0, 1.0);
	line_in_console = (int)(height / 2) / (int)line_height;
	int i = 0;
	int size = cmd_buffer.size();
	if (line_in_console > size)
	{
		i = 0;
		line_count = size;
		end = size;
	}
	else
	{
		i = size - line_in_console;
		line_count = line_in_console - 1;
		end = size - 1;
	}
	for (int line = 0; i < end; i++, line++)
	{
		m_Font->RenderText(
			cmd_buffer[i],
			0.f, height / 2 - line * line_height - line_height, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	}
	m_Font->RenderText(
		prompt + command_text,
		0.f, height / 2 - line_count * line_height - line_height, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
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
	if (!isShow)
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
				for (auto& cmd : completion)
				{
					command += cmd + L" ";
				}
			}
			return true;
		case sf::Keyboard::Enter:
			cmd_is_compete = true;
			line_count++;
			cmd_buffer.push_back(getPrompt() + wstr_to_str(command));
			//m_World->getActiveScene()->setPostProcessor(nullptr);
			return handleCommand(command);
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
	cd.history = &history;

	auto cmd_it = m_Commands.find(cd.command);

	if (cmd_it != m_Commands.end())
		result = cmd_it->second.Command->execute(cd);
	else if (cd.command == L"close")
		isShow = false;
	else if (cd.command == L"help")
		help(cd);
	history.push_back(str_to_wstr(getPrompt()) + command);
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
	m_Font = new FreeTypeFont();
	m_Font->Init("arial.ttf", 16, 18);
	m_engine = GetIEngine();
	m_Texture = new Texture();
	m_Texture->load("console_background2.jpg");
	//prompt = user + " #";
}
CConsole::~CConsole()
{
	if (m_Font) delete m_Font;
	if (m_Texture) delete m_Texture;
}

void CConsole::ShowConsole(bool show)
{
	if (isShow != show)
	{
		isShow = show;
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

std::string CConsole::getPrompt()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	return user + " " + std::string(std::ctime(&time)) + " # ";
}

void CConsole::help(CommandDesc& cd)
{
	if (cd.args.size() > 0)
	{
		auto it = m_Commands.find(cd.args[0]);
		if (it == m_Commands.end())
			return;
		//line_count++;
		cmd_buffer.push_back(wstr_to_str(cd.command) + ": " + it->second.help);

	}
}

void CConsole::AddArgumentCompletion(const char* cmd, const char* arg, int n)
{
}

void CConsole::Clear()
{
	cmd_buffer.clear();
}

HelpCommand::HelpCommand()
{
}
