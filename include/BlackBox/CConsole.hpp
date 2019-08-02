#pragma once
#include <BlackBox/IConsole.hpp>
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Render/Texture.hpp>

#include <map>
#include <vector>
#include <string>

struct IFont;

class CCVar : public ICVar
{
public:
	CCVar(const char* name, int value, char *help) : name(name), value(value), type(CVAR_INT), help(help) {}
	CCVar(const char* name, const char *value, char *help) : name(name), value(value), type(CVAR_STRING), help(help) {}
	CCVar(const char* name, float value, char *help) : name(name), value(value), type(CVAR_FLOAT), help(help) {}
	CCVar() : name(""), value(0.0f), type(CVAR_STRING), help(nullptr) {}

	// Inherited via ICVar
	virtual void Release() override;
	virtual int GetIVal() override;
	virtual float GetFVal() override;
	virtual char* GetString() override;
	virtual void Set(const char* s) override;
	virtual void ForceSet(const char* s) override;
	virtual void Set(float f) override;
	virtual void Set(int i) override;
	virtual void Refresh() override;
	virtual void ClearFlags(int flags) override;
	virtual int GetFlags() override;
	virtual int SetFlags(int flags) override;
	virtual int GetType() override;
	virtual const char* GetName() override;
	virtual const char* GetHelp() override;

private:
	const char* name;
	union Value{
		int i;
		char* s;
		float f;
		Value(int i) : i(i) {}
		Value(const char * s) : s(const_cast<char*>(s)) {}
		Value(float f) : f(f) {}
	}value;
	int type;
	char* help;
};

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

class CConsole : public IConsole, public IInputEventListener, public ICVarDumpSink
{
	friend class SetCommand;
	friend class GetCommand;
	friend class DumpCommand;
	const int MESSAGE_BUFFER_SIZE = 1024 * 16;
public:
	CConsole();
	~CConsole();
	// Inherited via IConsole
	virtual bool Init() override;
	virtual void ShowConsole(bool show) override;
	virtual void SetImage(ITexture* pTexture) override;
	virtual void Update() override;
	virtual void Draw() override;
	void Animate(float deltatime, IRender* render);
	void CalcMetrics(int& end);
	virtual void AddCommand(const char* sName, IEditCommand* command, const char* help = "") override;
	virtual void ExecuteString(const char* command) override;
	virtual void ExecuteFile(const char* file) override;
	virtual bool OnInputEvent(sf::Event& event) override;
	void getHistoryElement();
	void completeCommand(std::vector<std::wstring>& completion);
	void setBuffer();
	bool handleEnterText();
	virtual void AddArgumentCompletion(const char* cmd, const char* arg, int n) override;
	virtual void Clear() override;
	virtual void Help(const char *cmd) override;
	virtual void PrintLine(const char* format, ...) override;
	virtual bool IsOpened() override;

	virtual ICVar* CreateVariable(const char* sName, const char* sValue, int nFlags, const char* help = "") override;

	virtual ICVar* CreateVariable(const char* sName, int iValue, int nFlags, const char* help = "") override;

	virtual ICVar* CreateVariable(const char* sName, float fValue, int nFlags, const char* help = "") override;

	virtual ICVar* GetCVar(const char* name, const bool bCaseSensitive = true) override;


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
	void addToCommandBuffer(std::vector<std::wstring>& completion);
	void addText(std::wstring& cmd);
	;
	void Set(CommandDesc& cd);
	void Get(CommandDesc& cd);
	void CConsole::Dump();
	void getBuffer();
	bool needShowCursor();
private:
	std::map<std::wstring, CommandInfo> m_Commands;
	std::map<std::string, std::ifstream> scripts;
	bool is_input = false;
	bool input_trigered = false;
	std::wstring command;
	std::string command_text;
	IFont* m_Font;
	bool isOpened = false;
	bool cmd_is_compete = false;
	IEngine* m_engine;
	Texture* m_Texture;

	float height;

	// for animate console
	bool animate = false;
	float gravity = 1;
	float speed = 400;
	float curr_speed = 1300;
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
	std::string message_buffer;

	std::string user = "root";
	std::string pc = "HackMan";
	std::string env = "BlackBox";
	std::string cd = "~"; //current directory
	//
	glm::vec3 promptColor = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 textColor = glm::vec3(1.0, 1.0, 0.0);

	std::map<std::string, ICVar*> m_variables_map;

	ICVarDumpSink* m_pCVarDumpCallback = nullptr;
	ICVar* r_anim_speed;
	float time = 0.0f;
	float cursor_tick = 0.0, cursor_tack = 0.0;
	bool cursor_tick_tack = true;
	int history_line = 0;


	// Inherited via IConsole


	// Inherited via IConsole
	virtual void DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter = 0) override;


	// Inherited via ICVarDumpSink
	virtual void OnElementFound(ICVar* pCVar) override;

};
