#include <BlackBox/CConsole.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Utils.hpp>
#include <BlackBox/Render/IFont.hpp>

#include <string>
#include <fstream>
#include <glm/glm.hpp>

void CConsole::SetImage(ITexture* pTexture)
{
}

void CConsole::Update()
{
}

void CConsole::Draw()
{
	if (!isShow) return;
	std::string cmd_text = std::string(">") + command_text;
	m_Font->RenderText(
		cmd_text,
		0.f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
}

void CConsole::AddCommand(const char* sName, IEditCommand* command, const char* help)
{
	m_Commands[str_to_wstr(std::string(sName))] = command;
}

void CConsole::ExecuteString(const char* command)
{
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
	static std::vector<std::wstring> history;
	bool result = false;
	auto cd = parseCommand(command);
	cd.history = &history;

	auto cmd_it = m_Commands.find(cd.command);

	if (cmd_it != m_Commands.end())
		result = cmd_it->second->execute(cd);
	else if (cd.command == L"close")
		isShow = false;
	history.push_back(command);
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
}

void CConsole::ShowConsole(bool show)
{
	if (isShow != show)
	{
		isShow = show;
		if (show)
			input_trigered = true;
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
