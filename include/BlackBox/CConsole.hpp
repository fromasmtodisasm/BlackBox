#pragma once
#include <BlackBox/IConsole.hpp>
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Render/Texture.hpp>

#include <map>
#include <vector>
#include <string>

struct IFont;

struct CommandInfo
{
	IEditCommand* Command;
	std::multimap<int, std::string> argsCompletion;
	std::string help;
};

struct Text
{
	std::string data;
	glm::vec3 color;
	float x, y;
	float scale;
	Text(std::string& data, glm::vec3& color, float scale)
		:
		data(data), color(color), scale(scale)
	{

	}
};

typedef std::vector<Text> CommandLine;

struct CommandDesc
{
	std::wstring command;
	std::vector<std::wstring> args;
	std::vector<std::wstring> *history;
	CommandDesc(){}
	CommandDesc(decltype(history) *history) : history(nullptr)
	{

	}
};

class CConsole : public IConsole, public IInputEventListener
{
public:
	CConsole();
	~CConsole();
	// Унаследовано через IConsole
	virtual void ShowConsole(bool show) override;
	virtual void SetImage(ITexture* pTexture) override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void AddCommand(const char* sName, IEditCommand* command, const char* help = "") override;
	virtual void ExecuteString(const char* command) override;
	virtual void ExecuteFile(const char* file) override;

	// Унаследовано через IInputEventListener
	virtual bool OnInputEvent(sf::Event& event) override;

	//void fillcmd

private:
private:
	CommandDesc parseCommand(std::wstring& command);
	bool handleCommand(std::wstring command);
	void handleCommandTextEnter(uint32_t ch);
	std::vector<std::wstring> autocomplete(std::wstring cmd);
	void doFile(std::ifstream& cfg);
	void fillCommandText();
	void setFont(IFont* font);
	CommandLine getPrompt();
	void printLine(int line);
	void printText(Text & element, int line);
	template<typename It>
	void printLineInternal(It iterator);
	void addToCommandBuffer(std::vector<std::wstring>& completion);;
private:
	std::map<std::wstring, CommandInfo> m_Commands;
	std::map<std::string, std::ifstream> scripts;
	bool is_input = false;
	bool input_trigered = false;
	std::wstring command;
	std::string command_text;
	IFont* m_Font;
	bool isShow = false;
	bool cmd_is_compete = false;
	IEngine* m_engine;
	Texture* m_Texture;

	float height;

	// for animate console
	bool animate = false;
	float gravity = 1;
	float speed = 90;
	float curr_speed = 100;
	float curr_height = 0;
	//
	int line_count = 0;
	int line_in_console = 0;
	int line_height = 18;
	int current_line = 0;
	int on_line = 0;
	std::vector<CommandLine> cmd_buffer;
	std::vector<std::wstring> history;
	std::string m_prompt;

	std::string user = "root";
	std::string pc = "HackMan";
	std::string env = "BlackBox";
	std::string cd = "~"; //current directory
	//
	glm::vec3 promptColor = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 textColor = glm::vec3(1.0, 1.0, 0.0);

	// Inherited via IConsole
	virtual void AddArgumentCompletion(const char* cmd, const char* arg, int n) override;

	// Inherited via IConsole
	virtual void Clear() override;

	// Inherited via IConsole
	virtual void Help(const char *cmd) override;
};
